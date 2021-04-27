#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/jiffies.h> /* jiffies */
#include <asm/system_misc.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/timer.h>
#include <linux/delay.h>
extern struct module __this_module;

#define BUFFER 10

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Nelson Leonardo Gonzalez Dantas");
MODULE_DESCRIPTION("IR Emitter Code");

//Defines functions
static int myIR_open(struct inode *inode, struct file *filp);
static int myIR_release(struct inode *inode, struct file *filp);
static ssize_t myIR_read(struct file *filp,
                            char *buf, size_t count, loff_t *f_pos);
static ssize_t myIR_write(struct file *filp,
                             const char *buf, size_t count, loff_t *f_pos);
static void myIR_exit(void);
static int myIR_init(void);
void color_changer(int color);

//Defines file operations
struct file_operations myIR_fops =
{
owner:
	THIS_MODULE,
read:
    myIR_read,
write:
    myIR_write,
open:
    myIR_open,
release:
    myIR_release
};

/* Declaration of the init and exit functions */
module_init(myIR_init);
module_exit(myIR_exit);

static int myIR_major = 61;
//buffer for message
static char *myIR_buffer;


//variables 
//Setup GPIOS
static unsigned int red = 67;
static unsigned int blue = 68;
static unsigned int green = 44;
static unsigned int purple = 26;
static unsigned int aqua = 46;
static unsigned int pink = 65;
static unsigned int yellow = 61;
static unsigned int orange = 27;

//Represents on or off
int HIGH = 1;
int LOW = 0;


static char *current_color;


//GPIO Pins will be enabled to send signal to Arduino, each pin represents a color
void start_lights(void){
	gpio_request(red, "sysfs");
	gpio_direction_output(red, 0);
	gpio_export(red, 0);

	gpio_request(blue, "sysfs");
	gpio_direction_output(blue, 0);
	gpio_export(blue, 0);

	gpio_request(green, "sysfs");
	gpio_direction_output(green, 0);
	gpio_export(green, 0);

	gpio_request(purple, "sysfs");
	gpio_direction_output(purple, 0);
	gpio_export(purple, 0);

	gpio_request(aqua, "sysfs");
	gpio_direction_output(aqua, 0);
	gpio_export(aqua, 0);

	gpio_request(pink, "sysfs");
	gpio_direction_output(pink, 0);
	gpio_export(pink, 0);

	gpio_request(yellow, "sysfs");
	gpio_direction_output(yellow, 0);
	gpio_export(yellow, 0);

	gpio_request(orange, "sysfs");
	gpio_direction_output(orange, 0);
	gpio_export(orange, 0);

}

static int myIR_init(void){
	int result;
	result = register_chrdev(myIR_major, "myIR", &myIR_fops);
	if (result < 0){
		//error on registering module
		return result;
	}
	myIR_buffer = kmalloc(BUFFER, GFP_KERNEL);

	if(!myIR_buffer){
		//not enough memory available
		result = -ENOMEM;
		myIR_exit();
		return result;
	}
	memset(myIR_buffer, 0, BUFFER);
	current_color = kmalloc(10, GFP_KERNEL);
	current_color = "blue";
	printk(KERN_INFO "Make sure Arduino and Flood Lights are On\n");
	start_lights();
	color_changer(1);
	return 0;
}


//Exits IR module
static void myIR_exit(void)
{

    /* Freeing the major number */
    unregister_chrdev(myIR_major, "myIR");

    /* Freeing buffer memory */
    if (myIR_buffer)
    {
        kfree(myIR_buffer);
    }
    // Freeing color string
    gpio_unexport(red);
    gpio_unexport(blue);
    gpio_unexport(green);
    gpio_unexport(purple);
    gpio_unexport(aqua);
    gpio_unexport(pink);
    gpio_unexport(yellow);
    gpio_unexport(orange);
}


static int myIR_open(struct inode *inode, struct file *filp){
	return 0;
}
static int myIR_release(struct inode *inode, struct file *filp){
	return 0;
}

//Values that will be printed when cat is requested
static ssize_t myIR_read(struct file *filp,
                            char *buf, size_t count, loff_t *f_pos){
	loff_t size = 0;
	ssize_t len = 0;
	char buf2[256];
	size += snprintf(buf2 + size, (256 - size), "Current color is: %s\n", current_color);
	len = (ssize_t)min((loff_t)size - *f_pos, (loff_t)count);
	if (len <= 0)
		return 0;
	if (*f_pos > 0){
		return 0;
	}
	if (copy_to_user(buf, buf2 + *f_pos, len)) // send requested bytes
		return -EFAULT;
    *f_pos += len;
    return len;
}


//Writing to /dev/myIR
static ssize_t myIR_write(struct file *filp,
                             const char *buf, size_t count, loff_t *f_pos){
	
	//extracts message from the user
	char tbuf[256];
	char color[10];
	int value;
	memset(tbuf,0,256);
	if (copy_from_user(tbuf, buf, 256)){
		return -EFAULT;
	}
	sscanf(tbuf, "%s", color);
	if (strcmp(current_color, color) != 0){
	if (strcmp(color, "red") == 0) value=0;
	else if (strcmp(color, "blue") == 0) value=1;
	else if (strcmp(color, "green") == 0) value=2;
	else if (strcmp(color, "purple") == 0) value=3;
	else if (strcmp(color, "aqua") == 0) value=4;
	else if (strcmp(color, "pink") == 0) value=5;
	else if (strcmp(color, "yellow") == 0) value=6;
	else if (strcmp(color, "orange") == 0) value=7;
	color_changer(value);
}
	return count;
}

//Sender of signal function
void color_changer(int color){
	switch(color){
		case 0:
		gpio_set_value(red, HIGH);
		udelay(500);
		gpio_set_value(red, LOW);
		current_color = "red";
		break;

		case 1:
		gpio_set_value(blue, HIGH);
		udelay(500);
		gpio_set_value(blue, LOW);
		current_color = "blue";
		break;

		case 2:
		gpio_set_value(green, HIGH);
		udelay(500);
		gpio_set_value(green, LOW);
		current_color = "green";
		break;

		case 3:
		gpio_set_value(purple, HIGH);
		udelay(500);
		gpio_set_value(purple, LOW);
		current_color = "purple";
		break;

		case 4:
		gpio_set_value(aqua, HIGH);
		udelay(500);
		gpio_set_value(aqua, LOW);
		current_color = "aqua";
		break;

		case 5:
		gpio_set_value(pink, HIGH);
		udelay(500);
		gpio_set_value(pink, LOW);
		current_color = "pink";
		break;

		case 6:
		gpio_set_value(yellow, HIGH);
		udelay(500);
		gpio_set_value(yellow, LOW);
		current_color = "yellow";
		break;

		case 7:
		gpio_set_value(orange, HIGH);
		udelay(500);
		gpio_set_value(orange, LOW);
		current_color = "orange";
		break;
}

}

