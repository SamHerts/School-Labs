#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/interrupt.h> 
#include <linux/string.h> 
#include <linux/slab.h> 
#include <linux/fs.h> 
#include <linux/cdev.h> 
#include <linux/device.h> 
#include <stdbool.h> 
#include <asm/io.h> 
#include <asm/uaccess.h> 
#include "../../../address_map_arm.h" 
#include "../../../interrupt_ID.h"

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Altera University Program"); 
MODULE_DESCRIPTION("DE1SoC Video Driver");

 static int device_open (struct inode *, struct file *);
 static int device_release (struct inode *, struct file *);
 static ssize_t device_read (struct file *, char *, size_t, loff_t *);
 static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

 #define SUCCESS 0 
 #define DEVICE_NAME "video"

 static dev_t dev_no = 0;
 static struct cdev *video_cdev = NULL;
 static struct class *video_class = NULL;
 #define MAX_SIZE 256
 static char video_msg[MAX_SIZE*2];

 static struct file_operations fops = {
	.owner = THIS_MODULE,
 	.open = device_open,
 	.release = device_release,
 	.read = device_read,
 	.write = device_write
 };

void * LW_virtual;
volatile int *pixel_ctrl_ptr, *char_ctrl_ptr;
int pixel_buffer, char_buffer, resolution_x, resolution_y;
int *temp, *char_temp;

void get_screen_specs(volatile int *); 
void clear_screen(void); 
void plot_pixel(int, int, short int); 
void plot_char(int, int, int); 
void plot_line(int, int, int, int, short int); 
void sync(void);
void erase(void);
void writeString(int, int, char[MAX_SIZE]);

 static int __init start_video(void)
 {
	int err = 0;

	if ((err = alloc_chrdev_region (&dev_no, 0, 1, DEVICE_NAME)) < 0) {
		 printk (KERN_ERR "video: alloc_chrdev_region() error %d\n", err);
		 return err;
	}
	 video_class = class_create (THIS_MODULE, DEVICE_NAME);

	 video_cdev = cdev_alloc ();
	 video_cdev->ops = &fops;
	 video_cdev->owner = THIS_MODULE;

	 if ((err = cdev_add (video_cdev, dev_no, 1)) < 0) {
		 printk (KERN_ERR "video: cdev_add() error %d\n", err);
		 return err;
	 }
	LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
 	if (LW_virtual == 0)
        printk (KERN_ERR "Error: ioremap_nocache returned NULL\n");
        pixel_ctrl_ptr = (unsigned int* ) (LW_virtual + PIXEL_BUF_CTRL_BASE);
	char_ctrl_ptr = (unsigned int*) (LW_virtual + CHAR_BUF_CTRL_BASE);
        get_screen_specs (pixel_ctrl_ptr);
	pixel_buffer = (int) ioremap_nocache (FPGA_ONCHIP_BASE, FPGA_ONCHIP_SPAN);
    	if (pixel_buffer == 0)
        	printk (KERN_ERR "Error: ioremap_nocache returned NULL\n");
	char_buffer = (int)ioremap_nocache(FPGA_CHAR_BASE, FPGA_CHAR_SPAN);
	if (char_buffer == 0)
		printk (KERN_ERR "Error: ioremap_nocache returned NULL\n");
	device_create (video_class, NULL, dev_no, NULL, DEVICE_NAME );
	return 0;

	clear_screen();
 }

static void __exit stop_video(void)
 {
	iounmap(LW_virtual);
	iounmap((void *)pixel_buffer);
	iounmap((void *)pixel_ctrl_ptr);
	iounmap((void *)char_ctrl_ptr);
	iounmap((void *)char_buffer);

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
	 sprintf(video_msg, "%d,%d\n", resolution_x, resolution_y);

	bytes = strlen (video_msg) - ( *offset);
	bytes = bytes > length ? length : bytes;
	 if (bytes)
		 (void) copy_to_user (buffer, &video_msg[*offset], bytes);
	 *offset = bytes;
	 return bytes;
 }
 static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
 {
	 size_t bytes;
	 bytes = length;
	 char command[MAX_SIZE], string[MAX_SIZE]="                    ";
	 int x, x0, x1, y, y0, y1; 
	 unsigned int color;
	 if (bytes > MAX_SIZE - 1)
		 bytes = MAX_SIZE - 1;
    (void) copy_from_user (video_msg, buffer, bytes);
     video_msg[bytes] = '\0';
    if (strstr(video_msg, "clear") != NULL) { //Clear Command
        clear_screen();
    }
    if (strstr(video_msg, "pixel") != NULL) { //Plot Command
	sscanf(video_msg, "%s %d,%d %x",command,&x,&y,&color);
	if (x >= 0 && x <= resolution_x && y >= 0 && y <= resolution_y)
		plot_pixel(x, y, color);
    }
    if (strstr(video_msg, "line") != NULL) { //Plot Line Command
        sscanf(video_msg, "%s %d,%d %d,%d %x",command,&x0,&y0,&x1,&y1,&color);
	if (x0 >= 0 && x0 <= resolution_x && y0 >= 0 && y0 <= resolution_y)
		plot_line(x0, x1, y0, y1, color);
    }
	if (strstr(video_msg, "sync") != NULL) {
		sync();
	}
	if (strstr(video_msg, "erase") != NULL) {
		erase();
	}
	if (strstr(video_msg, "text") != NULL) {
		sscanf(video_msg, "%s %d,%d %s\n", command,&x,&y, string);
		writeString(x, y, string);}
	 return bytes;
 }
void get_screen_specs(volatile int *pixel_ctrl_ptr){
    resolution_x = pixel_ctrl_ptr[2] & 0x0000FFFF;
    resolution_y = pixel_ctrl_ptr[2] >> 16;
}
void clear_screen(){
	int i, j;
	for (j = 0; j < resolution_y; j++) {
		for (i = 0; i < resolution_x; i++) {
			plot_pixel(i, j, 0x0);
		}
	}
}
void plot_pixel(int x, int y, short int color){
    temp = (int *)((uintptr_t)pixel_buffer | (y << 10 | x << 1));
    *temp = color;
}
void plot_line(int x0, int x1, int y0, int y1, short int color){
    
	bool is_steep = (abs(y1 - y0) > abs(x1 - x0));
	if (is_steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = -(deltax / 2);
	int y = y0;
	int x;
	int y_step;
	if (y0 < y1) {
		y_step = 1;
	}
	else { y_step = -1; }
	for (x = x0; x < x1; x++) {
		is_steep ? plot_pixel(y, x, color) : plot_pixel(x, y, color);
		error += deltay;
		if (error >= 0) {
			y += y_step;
			error -= deltax;
		}
	}
}
void sync() {
        pixel_ctrl_ptr[0] = 0x1;
	while((*(pixel_ctrl_ptr + 3) & 0x000000001) == 1){

	}
}
void erase(){
	*char_ctrl_ptr = *char_ctrl_ptr + 0x000010000;
}
void plot_char(int x, int y, int char_val){
	char_temp = (int *)((uintptr_t)char_buffer | (y << 7 | x));
   		 *char_temp = char_val;
}
void writeString(int x, int y, char buffer[MAX_SIZE]){
	int size = 20;
	int i = 0;
	while(i < size){
		plot_char(x+i,y,(int)buffer[i]);
		i++;
	} 
}

 MODULE_LICENSE("GPL");
 module_init (start_video);
 module_exit (stop_video);
