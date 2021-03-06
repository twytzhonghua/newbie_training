/************************************************************************************
*** Filename:
***			gpio_get_value.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		 A simple char device driver that read some gpios' value
***
***	Author/Creat Date:
*** 		Albert Yang, Jun 20,13
***
***	Modification History:
***			Jul 4, 13
***
***	Note:
**
************************************************************************************************/
#include <linux/semaphore.h>/*For sema_init(),down(),up()*/
#include <linux/jiffies.h>/*For jiffies  HZ = 100 */
#include <linux/miscdevice.h>/*For struct miscdevice ...*/
#include <linux/module.h>/*For module specific items*/
#include <linux/types.h>/*for standard types (like size_t) */
#include <linux/kernel.h>/*For printk/panic/...*/
#include <linux/device.h>/*For struct class*/
#include <linux/string.h>/*For strlen...*/
#include <linux/errno.h> /*For the -ENODEV/...value*/
#include <linux/init.h>/*For  __init/__exit/....*/
#include <linux/fs.h>/*For struct file_operations*/
#include <linux/io.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <linux/cdev.h>/*For struct cdev/cdev_init/...*/
#include <asm/uaccess.h>/*For copy_to_user/ ...*/
#include <linux/errno.h>/*For error symbol*/
#include <linux/slab.h>  /*for kmalloc()*/
#include <linux/sched.h>  
#include <linux/delay.h>
#include <mach/mt_gpio.h>
#include <linux/ioport.h>       /* For io-port access */

#define DEV_SIZE 100
#define  MISC_NAME "chardrv_test"
static volatile short int *GPIO_DOUT13;//第10位对应GPIO218的输出
static volatile short int *GPIO_DOUT10;//第14位对应GPIO174输出
#define OSC_EN	   GPIO218
#define MSDC2_DAT2 GPIO174

/*
*declare the struct my_data, contains private data
* kbuf :the data kernel keeps.
*cur_size: the current size of the kbuf:
*sem:	the semaphore of contrl concurrency 
*/
struct my_data{
		
	char kbuf[DEV_SIZE];
	unsigned int cur_size;
	struct semaphore sem; 
};

struct my_data *devp ;//decalre a structure pointer, then use kmalloc() to allocate space for it 

int misc_open(struct inode *node,struct file *filp)
{

	filp->private_data = devp; // give the structure pointer to filp->private_data
	printk("This is misc_open\n");
	return 0;
}
	
int misc_close(struct inode *node,struct file *filp)
{
	printk("This is misc_close\n");
	return 0;
}


/*****************************************************************************************
***	Description:		
***			read data from kernel	
***
***	Return Value:
***			If read sucessfully, return the bytes that have readed.
*******************************************************************************************/

static ssize_t misc_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int ret,i;
	int gpio_value[4];
	struct my_data *dev = filp->private_data;
	
	printk("*********************this is misc_read**1111111111111111111111111111111111111*******\n");
	
	/*
	*check the size of *offset,if *offset out of the array, return;
	*/
	if(*offset > DEV_SIZE)
	{
		return count?-ENXIO:0;
	}
	if(*offset + count > DEV_SIZE)//if count is longer, Read data as more as possible.
	{
		count = DEV_SIZE - *offset;	
	}


	if(down_interruptible(&dev->sem)) 
		return -ERESTARTSYS;
	
	/*
	*Check read successfully or not
	*/
	if(!copy_to_user(buf,(void *)(dev->kbuf+*offset),count))
	{
		ret = count;
		dev->cur_size -= count; //change the current size
		*offset += count;//change *offset after has been readed
	
	}
	else
	 {
		ret = -EFAULT;
		printk("Kernel test_read failed\n");
	}

	gpio_value[0]  = mt_get_gpio_in(OSC_EN);
	gpio_value[1]  = mt_get_gpio_in(MSDC2_DAT2);
	gpio_value[2]  =((1<<10) & (*GPIO_DOUT13)) ? 1 : 0;
	gpio_value[3]  =((1<<14) & (*GPIO_DOUT10)) ? 1 : 0;

	for(i=0;i<4;i++)                
		printk("gpio_value[%d]=%d\n",i,gpio_value[i]);

	up(&dev->sem);                 
	
	printk("*****************the end of the misc_read function ***********************\n");
	return ret;
}


/*****************************************************************************************
***	Description:		
***			write data to kernel	
***
***	Return Value:
***			If write sucessfully, return the bytes that have written.
*******************************************************************************************/
static ssize_t misc_write(struct file *filp,const char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	

	struct my_data *dev = filp->private_data;

	/*
	*check the size of *offset,if writing off the end of the array, return;
	*/
	if(*offset > DEV_SIZE)
	{
		return count?-ENXIO:0;
	}
	if(*offset + count > DEV_SIZE)//if there is no enough space to write all the data, return
	{
		printk("offset + count > DEV_SIZE,error\n");
		return -1;
	}
/*
	*Get the semaphore
	*/
	if(down_interruptible(&dev->sem)) 
		return -ERESTARTSYS;
	
	/*
	*Check write successfully or not
	*/
	printk("before writing\n");
	if(!copy_from_user(dev->kbuf+*offset,(void *)buf,count))
	{
		ret = count;
		dev->cur_size += count; //hange the current size
		*offset += count; // change the *offset
		printk("<write to kernel> buf is :%s\n",buf);
	}
	else 
	{
		ret = -EFAULT;
	}
	
	
	up(&dev->sem);

	return ret;
}






/*****************************************************************************************
***	Description:	file location function	
*******************************************************************************************/
loff_t misc_llseek(struct file *filp, loff_t offset, int whence)
{
	/*
	*declare the new_pos, old_pos
	*/
	loff_t new_pos;
	loff_t old_pos = filp->f_pos;

	switch(whence)
	{
		case SEEK_SET:
		new_pos = offset;
		break;

		case SEEK_CUR:
		new_pos = old_pos + offset;
		break;

		case SEEK_END:
		new_pos = DEV_SIZE + offset;
		break;

		default:
		printk("Wrong offset!\n");
		return -EINVAL;
	}

	/*
	*check the new_pos is legal or not
	*/
	if(new_pos < 0 || new_pos > DEV_SIZE)
	{
		printk("F_pos failed!\n");
		return -EINVAL;
	}

	/*
	*update the offset
	*/
	filp->f_pos = new_pos;
	return  new_pos;
}




/*
*The struct file_operations
*/
struct file_operations misc_fops={
	.open = misc_open,
	.release = misc_close,
	.read = misc_read,
	.write = misc_write,
	.llseek = misc_llseek,
};

/*
*fill the content of struct miscdevice
*/
static struct miscdevice misc_dev=
{
	.minor = MISC_DYNAMIC_MINOR,//Dynamically allocate minor device
	.name = MISC_NAME,
	.fops = &misc_fops,
};

/*
*Loading function
*/
static int __init misc_init(void)
{

	int ret;
	devp = kmalloc(sizeof(struct my_data),GFP_KERNEL);//allocate space for devp
	if(!devp)
	{
		printk("kmalloc failed!\n");
		return -ENOMEM;
	}

	memset(devp,0,sizeof(struct my_data));
	/*
	*register the miscdevice
	*/
	ret = misc_register(&misc_dev);
	if(ret)
	{
		printk("misc_register error\n");
		return ret;
	
	}
	
	/*
	*initialize the semaphore
	*/
	sema_init(&devp->sem,1); 	
	
	GPIO_DOUT13 = ioremap(0x100058D0, 4);
	GPIO_DOUT10 = ioremap(0x100058A0, 4);
	
	printk("Hello kernel!\n");
	return 0;
}


/*
*Unistall function
*/
static void __exit misc_exit(void)
{
    printk("Goodbye kernel!\n");

	if(devp != NULL)	
	{
		kfree(devp);//delete structure pointer
		devp = NULL;
	}
	misc_deregister(&misc_dev);	
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Albert");
MODULE_VERSION("1.1");
MODULE_DESCRIPTION("A simple driver about get current time and count the cost time");












