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
#include <linux/pwm.h>
extern struct module __this_module;

#define BUFFER 10

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Nelson Leonardo Gonzalez Dantas");
MODULE_DESCRIPTION("IR Emitter Code");


//entries to proc


static int myIR_open(struct inode *inode, struct file *filp);
static int myIR_release(struct inode *inode, struct file *filp);
static ssize_t myIR_read(struct file *filp,
                            char *buf, size_t count, loff_t *f_pos);
static ssize_t myIR_write(struct file *filp,
                             const char *buf, size_t count, loff_t *f_pos);
static void myIR_exit(void);
static int myIR_init(void);
void color_changer(int color);


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

//initializes timer module

//variables 
//Setup GPIOS

//VDD = 5
int HIGH = 1;
int LOW = 0;
//GPIO

//color switch

//colors

int RED[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1};
int BLUE[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1};
struct pwm_device * pwm7;


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
	pwm7 = kmalloc(sizeof(pwm7), GFP_KERNEL);
	pwm7 = pwm_request(8, "pwm7");
	memset(myIR_buffer, 0, BUFFER);
	pwm_config(pwm7, 14000, 26163);
	return 0;
}


//exits timer module
static void myIR_exit(void)
{

    /* Freeing the major number */
    unregister_chrdev(myIR_major, "myIR");

    /* Freeing buffer memory */
    if (myIR_buffer)
    {
        kfree(myIR_buffer);
    }

}


static int myIR_open(struct inode *inode, struct file *filp){
	return 0;
}
static int myIR_release(struct inode *inode, struct file *filp){
	return 0;
}

static ssize_t myIR_read(struct file *filp,
                            char *buf, size_t count, loff_t *f_pos){
	loff_t size = 0;
	ssize_t len = 0;
	char buf2[256]; 
	char numBuf[2] = "";
	char *current_mode = "Normal";
	char *red_light = "Off";
	char *yellow_light = "Off";
	char *green_light = "Off";
	char *ped = "No ";
	sprintf(numBuf,"%d",3);

	size += snprintf(buf2 + size, (256 - size), "Mode: %s\nCurrent cycle rate: %s\nCurrent Status:\nRed: %s\nYellow: %s\nGreen: %s\nPedestrian? %s\n", 
		current_mode, numBuf, red_light, yellow_light, green_light, ped);
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
	printk(KERN_ALERT "called \n");
	if (strcmp(color, "red") == 0) value=1;
	else if (strcmp(color, "blue") == 0) value=2;
	printk(KERN_ALERT "here %s %d\n", color, value);
	color_changer(value);
	return count;
}

void color_changer(int color){
	printk(KERN_ALERT "here");
	int *color_command;
	int bitl;
	int i;

	switch(color){
	case 1:
	color_command = RED;
	break;
	case 2:
	color_command = BLUE;
	break;
}
	printk(KERN_ALERT "here number is %d\n", color_command[1]);
	local_irq_disable();
	pwm_enable(pwm7);
	usleep_range(9000, 9000);
	pwm_config(pwm7, 0, 26163);
	usleep_range(4300,4300);
	for(i = 0; i< 32; i++){
		bitl = color_command[i];
		pwm_config(pwm7, 14000, 26163);
		usleep_range(560, 560);
		pwm_config(pwm7, 0, 26163);
		if (bitl == 1){
		usleep_range(1680, 1680);
	}
	else{
		usleep_range(560, 560);
	}
	}
	pwm_config(pwm7, 14000, 26163);
	usleep_range(560, 560);
	pwm_config(pwm7, 0, 26163);
	usleep_range(560, 560);
	pwm_disable(pwm7);
	pwm_config(pwm7, 14000, 26163);
	local_irq_enable();
}

//send SIGIO signal back to kernel
