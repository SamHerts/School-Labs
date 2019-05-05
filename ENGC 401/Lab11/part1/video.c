#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include "../../Includes/address_map_arm.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Altera University Program");
MODULE_DESCRIPTION("DE1SoC Video Driver");

// Declare global variables needed to use the pixel buffer

#define SUCCESS 0
#define DEVICE_NAME "video"
#define MAX_SIZE 256 // assume that no message longer than this will be used

void *LW_virtual; // used to access FPGA light-weight bridge
volatile int * pixel_ctrl_ptr; // virtual address of pixel buffer controller
int pixel_buffer; // used for virtual address of pixel buffer
int resolution_x, resolution_y; // VGA screen size

 // Declare variables and prototypes needed for a character device driver
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

void plot_pixel(int, int, short int);
void clear_screen(void);
void get_screen_specs(volatile int*);


static dev_t dev_no = 0;
static struct cdev * video_cdev = NULL;
static struct class * video_class = NULL;
static char video_msg[MAX_SIZE]; // the string that can be read or written


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_release,
	.read = device_read,
	.write = device_write
};

/* Code to initialize the video driver */
static int __init start_video(void)
{
	int err = 0;

	// initialize the dev_t, cdev, and class data structures
	/* Get a device number. Get one minor number (0) */
	if ((err = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME)) < 0) {
		printk(KERN_ERR "VIDEO: alloc_chrdev_region() error %d\n", err);
		return err;
	}

	video_class = class_create(THIS_MODULE, DEVICE_NAME);
	video_cdev = cdev_alloc();
	video_cdev->ops = &fops;
	video_cdev->owner = THIS_MODULE;

	// Add the character device to the kernel
	if ((err = cdev_add(video_cdev, dev_no, 1)) < 0) {
		printk(KERN_ERR "VIDEO: cdev_add() error %d\n", err);
		return err;
	}

	// generate a virtual address for the FPGA lightweight bridge
	LW_virtual = ioremap_nocache(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
	if (LW_virtual == 0)
	{
		printk(KERN_ERR "Error: ioremap_nocache returned NULL\n");
	}
	// Create virtual memory access to the pixel buffer controller
	pixel_ctrl_ptr = (unsigned int *)(LW_virtual + PIXEL_BUF_CTRL_BASE);
	get_screen_specs(pixel_ctrl_ptr); // determine X, Y screen size
	// Create virtual memory access to the pixel buffer
	pixel_buffer = (int)ioremap_nocache(FPGA_ONCHIP_BASE, FPGA_ONCHIP_SPAN);
	if (pixel_buffer == 0)
	{
		printk(KERN_ERR "Error: ioremap_nocache returned NULL\n");
	}
	/* Erase the pixel buffer */
	device_create(video_class, NULL, dev_no, NULL, DEVICE_NAME);

	clear_screen();
	return 0;
}

void get_screen_specs(volatile int * pixel_ctrl_ptr)
{
	resolution_x = *(pixel_ctrl_ptr + 8) & 0x0000FFFF; //collect the first 16 bits from resolution register
	resolution_y = *(pixel_ctrl_ptr + 8) >> 16; //collect second 16 bits from resolution register
}
void clear_screen()
{
	int i,j;
	for( i = 0; i < resolution_x; i++)
	{
		for (j = 0; j < resolution_y; j++)
		{
			plot_pixel(i, j, 0);
		}
	}
}

void plot_pixel(int x, int y, short int color)
{
	int *location;//temporary pointer to pixel location
	location = (int *)((uintptr_t)pixel_buffer + (y << 10 | x << 1));//set location and shift x and y into position
	*location = color;//set color value at address of X and Y
}

static void __exit stop_video(void)
{
	/* unmap the physical-to-virtual mappings */
	iounmap(LW_virtual);
	iounmap((void *)pixel_buffer);
	iounmap(pixel_ctrl_ptr);
	/* Remove the device from the kernel */

	device_destroy(video_class, dev_no);
	cdev_del(video_cdev);
	class_destroy(video_class);
	unregister_chrdev_region(dev_no, 1);
}

static int device_open(struct inode *inode, struct file *file)
{
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	size_t bytes;
	//sprintf ( storage buffer, string, string....etc)
	sprintf(video_msg, "%d,%d\n", resolution_x, resolution_y);

	bytes = strlen(video_msg) - (*offset); // how many bytes not yet sent?
	bytes = bytes > length ? length : bytes; // too much to send at once?
	if (bytes)
	{
		(void)copy_to_user(buffer, &video_msg[*offset], bytes);
	}
	*offset = bytes; // keep track of number of bytes sent to the user
	return bytes;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
	size_t bytes;
	char *command = kmalloc(length+1, GFP_KERNEL);
	bytes = length;

	if (bytes > MAX_SIZE - 1) // can copy all at once, or not?
		bytes = MAX_SIZE - 1;
	(void)copy_from_user(video_msg, buffer, bytes);
	video_msg[bytes] = '\0'; // NULL terminate
	// Note: we do NOT update *offset; we keep the last MAX_SIZE or fewer bytes

	//COMMAND: clear
	if (strstr(video_msg, "clear") != NULL)//if you can find the string clear in command, then
	{
		clear_screen(); //clear the screen
	}
	//COMMAND: pixel
	if(strstr(video_msg, "pixel") != NULL)//if you can find the string pixel in command, then
	{
		int x = 0, y = 0, color = 0;

		strcpy(command, video_msg);

		//strsep(&command, " ,"); //throw away the command portion
		//kstrtol( strsep(&command, " ,"), 10, x);//convert the x char into integer
		//kstrtol( strsep(&command, " ,"), 10, y);//convert the y char into integer
		//kstrtol( strsep(&command, " ,"), 16, color);//convert the color into integer

 		strsep(&command, " ,"); //throw away the command portion
                x = simple_strtol( strsep(&command, " ,"),NULL, 10);//convert the x char into integer
                y = simple_strtol( strsep(&command, " ,"),NULL, 10);//convert the y char into integer
                color = simple_strtol( strsep(&command, " ,"), NULL,16);//convert the color into integer
		if ((x >= 0 && x <= resolution_x) && (y >= 0 && y <= resolution_y))//ensure given coordinates are in range
		{
			plot_pixel(x, y, color);//print pixel
		}
	}
	return bytes;
}

MODULE_LICENSE("GPL");
module_init(start_video);
module_exit(stop_video);
