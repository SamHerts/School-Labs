#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include "../../Includes/address_map_arm.h"



//Switch character device drivers
static int sw_device_open(struct inode *, struct file *);
static int sw_device_release(struct inode *, struct file *);

//static int key_device_open(struct inode *, struct file *);
//static int key_device_release(struct inode *, struct file *);

static ssize_t sw_device_read(struct file *, char *, size_t, loff_t *);
static ssize_t key_device_read(struct file *, char *, size_t, loff_t *);

#define SUCCESS 0
#define SW_DEVICE_NAME "SW"
#define KEY_DEVICE_NAME "KEY"

#define MAX_SIZE 256 // assume that no message longer than this will be used

void * LW_virtual;
volatile int *SW_ptr;
volatile int *KEY_ptr;

#define TEST_SW_BIT \
	((*SW_ptr) & (0x200) ? '1' : '0'),\
	((*SW_ptr) & (0x100) ? '1' : '0'),\
	((*SW_ptr) & (0x080) ? '1' : '0'),\
	((*SW_ptr) & (0x040) ? '1' : '0'),\
	((*SW_ptr) & (0x020) ? '1' : '0'),\
	((*SW_ptr) & (0x010) ? '1' : '0'),\
	((*SW_ptr) & (0x008) ? '1' : '0'),\
	((*SW_ptr) & (0x004) ? '1' : '0'),\
	((*SW_ptr) & (0x002) ? '1' : '0'),\
	((*SW_ptr) & (0x001) ? '1' : '0')

#define TEST_KEY_BIT \
	((*KEY_ptr) & (0x08) ? '1' : '0'),\
	((*KEY_ptr) & (0x04) ? '1' : '0'),\
	((*KEY_ptr) & (0x02) ? '1' : '0'),\
	((*KEY_ptr) & (0x01) ? '1' : '0')

static dev_t sw_dev_no = 0;
static struct cdev * sw_cdev = NULL;
static struct class * sw_class = NULL;
static char sw_msg[MAX_SIZE]; // the string that can be read

static dev_t key_dev_no = 1;
static struct cdev * key_cdev = NULL;
static struct class * key_class = NULL;
static char key_msg[MAX_SIZE]; // the string that can be read


static struct file_operations sw_fops = {
	.owner = THIS_MODULE,
	.open = sw_device_open,
	.release = sw_device_release,
	.read = sw_device_read,
};

static struct file_operations key_fops = {
        .owner = THIS_MODULE,
        .open = sw_device_open,
        .release = sw_device_release,
	.read = key_device_read,
};

static int __init start_key_sw(void)
{
	int err = 0;
	LW_virtual = ioremap_nocache(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
	SW_ptr = LW_virtual + SW_BASE;
	KEY_ptr = LW_virtual + KEY_BASE+0xC; // add 0xC for EdgeCapture register, not data register


	/* Get a device number. Get one minor number (0) */
	if ((err = alloc_chrdev_region(&sw_dev_no, 0, 1, SW_DEVICE_NAME)) < 0) {
		printk(KERN_ERR "sw: alloc_sw_region() error %d\n", err);
		return err;
	}

	sw_class = class_create(THIS_MODULE, SW_DEVICE_NAME);
	sw_cdev = cdev_alloc ();
	sw_cdev->ops = &sw_fops;
	sw_cdev->owner = THIS_MODULE;

	if ((err = cdev_add(sw_cdev, sw_dev_no, 1)) < 0) {
                printk (KERN_ERR "sw: cdev_add() error %d\n", err);
		return err;
        }
	device_create(sw_class, NULL, sw_dev_no, NULL, SW_DEVICE_NAME);


	if ((err = alloc_chrdev_region(&key_dev_no, 0, 1, KEY_DEVICE_NAME)) < 0) {
		printk(KERN_ERR "key: alloc_sw_region() error %d\n", err);
		return err;
	}

	key_class = class_create(THIS_MODULE, KEY_DEVICE_NAME);
	key_cdev = cdev_alloc ();
	key_cdev->ops = &key_fops;
	key_cdev->owner = THIS_MODULE;

	if ((err = cdev_add(key_cdev, key_dev_no, 1)) < 0){
		printk(KERN_ERR "key: cdev_add() error %d\n", err);
		return err;
	}

	device_create(key_class, NULL, key_dev_no, NULL, KEY_DEVICE_NAME);

	return 0;
}

static void __exit stop_key_sw(void)
{
	device_destroy(sw_class, sw_dev_no);
	device_destroy(key_class, key_dev_no);

	cdev_del(sw_cdev);
	cdev_del(key_cdev);

	class_destroy(sw_class);
	class_destroy(key_class);

	unregister_chrdev_region(sw_dev_no, 1);
	unregister_chrdev_region(key_dev_no, 1);
}
/* Called when a process opens chardev */
static int sw_device_open(struct inode *inode, struct file *file)
{
	return SUCCESS;
}
/* Called when a process closes chardev */
static int sw_device_release(struct inode *inode, struct file *file)
{
	*KEY_ptr = 0xFF;
	return 0;
}
/*
static int key_device_open(struct inode *inode, struct file *file)
{
	return SUCCESS;
}

static int key_device_release(struct inode *inode, struct file *file)
{
        *KEY_ptr = 0xFF; //attempt to write to the edgecapture register, clearing it of data
	return 0;
}
*/
/* Called when a process reads from chardev. Provides character data from
* chardev_msg. Returns, and sets *offset to, the number of bytes read. */
static ssize_t sw_device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	size_t bytes;
	//sprintf ( storage buffer, string, string....etc)
	sprintf(sw_msg,"%c%c%c%c%c%c%c%c%c%c", TEST_SW_BIT);

	bytes = strlen(sw_msg) - (*offset); // how many bytes not yet sent?
	bytes = bytes > length ? length : bytes; // too much to send at once?
	if (bytes)
		(void) copy_to_user(buffer, &sw_msg[*offset], bytes);
	* offset = bytes; // keep track of number of bytes sent to the user
	return bytes;
}

static ssize_t key_device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	size_t bytes;
	sprintf(key_msg, "%c%c%c%c", TEST_KEY_BIT);

	bytes = strlen(key_msg) - (*offset);
	bytes = bytes > length ? length : bytes;
	if(bytes)
		(void) copy_to_user(buffer, &key_msg[*offset], bytes);
	* offset = bytes;
	return bytes;
}


MODULE_LICENSE("GPL");
module_init(start_key_sw);
module_exit(stop_key_sw);
