#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>

/* Kernel character device driver /dev/chardev. */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
#define SUCCESS 0
#define DEVICE_NAME "chardev"

static dev_t dev_no = 0;
static struct cdev *chardev_cdev = NULL;
static struct class *chardev_class = NULL;
#define MAX_SIZE 256 // assume that no message longer than this will be used
static char chardev_msg[MAX_SIZE]; // the string that can be read or written

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_release,
	.read = device_read,
	.write = device_write
};

static int __init start_chardev(void)
{
	int err = 0;
	/* Get a device number. Get one minor number (0) */
	if ((err = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME)) < 0) {
		printk(KERN_ERR "chardev: alloc_chrdev_region() error %d\n", err);
		return err;
	}
	chardev_class = class_create(THIS_MODULE, DEVICE_NAME);
	// Allocate and initialize the char device
	chardev_cdev = cdev_alloc();
	chardev_cdev->ops = &fops;
	chardev_cdev->owner = THIS_MODULE;
	// Add the character device to the kernel
	if ((err = cdev_add(chardev_cdev, dev_no, 1)) < 0) {
		printk(KERN_ERR "chardev: cdev_add() error %d\n", err);
		return err;
	}
	device_create(chardev_class, NULL, dev_no, NULL, DEVICE_NAME);
	strcpy(chardev_msg, "Hello from chardev\n");
	return 0;
}

static void __exit stop_chardev(void)
{
	device_destroy(chardev_class, dev_no);
	cdev_del(chardev_cdev);
	class_destroy(chardev_class);
	unregister_chrdev_region(dev_no, 1);
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
/* Called when a process reads from chardev. Provides character data from
* chardev_msg. Returns, and sets *offset to, the number of bytes read. */
static ssize_t device_read(struct file *filp, char *buffer, size_t length,
	loff_t *offset)
{
	size_t bytes;
	bytes = strlen(chardev_msg) - (*offset); // how many bytes not yet sent?
	bytes = bytes > length ? length : bytes; // too much to send at once?
	if (bytes)
		(void)copy_to_user(buffer, &chardev_msg[*offset], bytes);
	*offset = bytes; // keep track of number of bytes sent to the user
	return bytes;
}
/* Called when a process writes to chardev. Stores the data received into
* chardev_msg, and returns the number of bytes stored. */
static ssize_t device_write(struct file *filp, const char *buffer, size_t
	length, loff_t *offset)
{
	size_t bytes;
	bytes = length;

	if (bytes > MAX_SIZE - 1) // can copy all at once, or not?
		bytes = MAX_SIZE - 1;
	(void) copy_from_user (chardev_msg, buffer, bytes);
	chardev_msg[bytes] = '\0'; // NULL terminate
	// Note: we do NOT update *offset; we keep the last MAX_SIZE or fewer bytes
	return bytes;
}

MODULE_LICENSE("GPL");
module_init(start_chardev);
module_exit(stop_chardev);
