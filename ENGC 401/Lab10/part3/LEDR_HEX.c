#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include "../../Includes/address_map_arm.h"

#define SUCCESS 0
#define LEDR_DEVICE_NAME "LEDR"
#define HEX_DEVICE_NAME "HEX"

#define MAX_SIZE 256 // assume that no message longer than this will be used

//Switch character device drivers
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);

static ssize_t led_write(struct file *, const char *, size_t, loff_t *);
static ssize_t hex_write(struct file *, const char *, size_t, loff_t *);

int SetDigit(int);
void setDisplay(int);

void * LW_virtual;
volatile int *LEDR_ptr;
volatile int *HEX1_ptr;
volatile int *HEX2_ptr;

static dev_t led_dev_no = 0;
static struct cdev * led_cdev = NULL;
static struct class * led_class = NULL;
static char led_msg[MAX_SIZE]; // the string that can be read

static dev_t hex_dev_no = 1;
static struct cdev * hex_cdev = NULL;
static struct class * hex_class = NULL;
static char hex_msg[MAX_SIZE]; // the string that can be read


static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_release,
	.write = led_write
};

static struct file_operations hex_fops = {
        .owner = THIS_MODULE,
        .open = device_open,
        .release = device_release,
	.write = hex_write
};

static int __init start_ledr_hex(void)
{
	int err = 0;
	LW_virtual = ioremap_nocache(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
	LEDR_ptr = LW_virtual + LEDR_BASE;
	HEX1_ptr = LW_virtual + HEX3_HEX0_BASE;
	HEX2_ptr = LW_virtual + HEX5_HEX4_BASE;

	/* Get a device number. Get one minor number (0) */
	if ((err = alloc_chrdev_region(&led_dev_no, 0, 1, LEDR_DEVICE_NAME)) < 0) {
		printk(KERN_ERR "LED: alloc_led_region() error %d\n", err);
		return err;
	}

	led_class = class_create(THIS_MODULE, LEDR_DEVICE_NAME);
	led_cdev = cdev_alloc ();
	led_cdev->ops = &led_fops;
	led_cdev->owner = THIS_MODULE;

	if ((err = cdev_add(led_cdev, led_dev_no, 1)) < 0) {
                printk (KERN_ERR "led: cdev_add() error %d\n", err);
		return err;
        }
	device_create(led_class, NULL, led_dev_no, NULL, LEDR_DEVICE_NAME);


	if ((err = alloc_chrdev_region(&hex_dev_no, 0, 1, HEX_DEVICE_NAME)) < 0) {
		printk(KERN_ERR "hex: alloc_hex_region() error %d\n", err);
		return err;
	}

	hex_class = class_create(THIS_MODULE, HEX_DEVICE_NAME);
	hex_cdev = cdev_alloc ();
	hex_cdev->ops = &hex_fops;
	hex_cdev->owner = THIS_MODULE;

	if ((err = cdev_add(hex_cdev, hex_dev_no, 1)) < 0){
		printk(KERN_ERR "hex: cdev_add() error %d\n", err);
		return err;
	}

	device_create(hex_class, NULL, hex_dev_no, NULL, HEX_DEVICE_NAME);

	return 0;
}

static void __exit stop_ledr_hex(void)
{
	*LEDR_ptr = 0;
	*HEX1_ptr = 0;
	*HEX2_ptr = 0;
	device_destroy(led_class, led_dev_no);
	device_destroy(hex_class, hex_dev_no);

	cdev_del(led_cdev);
	cdev_del(hex_cdev);

	class_destroy(led_class);
	class_destroy(hex_class);

	unregister_chrdev_region(led_dev_no, 1);
	unregister_chrdev_region(hex_dev_no, 1);
}
/* Called when a process opens chardev */
static int device_open(struct inode *inode, struct file *file)
{
	return SUCCESS;
}
/* Called when a process closes chardev */
static int device_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t led_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
	size_t bytes;
	bytes = length;
	if (bytes > MAX_SIZE - 1) // can copy all at once, or not?
		bytes = MAX_SIZE - 1;
	(void) copy_from_user (led_msg, buffer, bytes);
	// Note: we do NOT update *offset; we keep the last MAX_SIZE or fewer bytes
	led_msg[bytes] = '\0'; // NULL terminate

	//this is where we update the LEDs

	if(!kstrtol(led_msg, 10, &(*LEDR_ptr)));

	return bytes;
}

static ssize_t hex_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
	size_t bytes;
	int val;
	bytes = length;
	if (bytes > MAX_SIZE - 1) // can copy all at once, or not?
		bytes = MAX_SIZE - 1;
	(void) copy_from_user (hex_msg, buffer, bytes);
	hex_msg[bytes] = '\0'; // NULL terminate
	// Note: we do NOT update *offset; we keep the last MAX_SIZE or fewer bytes

	if(!kstrtol(hex_msg, 10, &val))
		setDisplay(val);
	return bytes;
}

int setDigit(int val)
{
	int segment[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	return segment[val];
}

void setDisplay(int val)
{
	int DDHex, SSHex, MMHex;

	DDHex = (setDigit((val/10)%10) << 8) + setDigit(val%10); // Set the first and second digit of each value
	val /= 100;
	SSHex = (setDigit((val/10)%10) << 8) + setDigit(val%10); // Splits the timing value into tens place and hundreds place, converts, and shifts accordingly
	val /= 100;
	MMHex = (setDigit((val/10)%10) << 8) + setDigit(val%10);
	*HEX1_ptr = (SSHex <<  16) + DDHex;
	*HEX2_ptr = MMHex;
}

MODULE_LICENSE("GPL");
module_init(start_ledr_hex);
module_exit(stop_ledr_hex);
