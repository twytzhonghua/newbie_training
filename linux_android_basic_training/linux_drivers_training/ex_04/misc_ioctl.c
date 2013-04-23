/**********************************************************************************************
*** Filename:
***			misc_test.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		 A simple char device.
***
***	Author/Creat Date:
*** 		Albert Yang, Apri 19,13
***
***	Modification History:
***			Apri 22, 13
***	Note:
***
************************************************************************************************/
#include <linux/miscdevice.h>/*For struct misdevice,mis_register,mis_deregister()*/
#include <linux/module.h>/*For module specific items*/
#include <linux/types.h>/*for standard types (like size_t) */
#include <linux/kernel.h>/*For printk/panic/...*/
#include <linux/device.h>/*For struct class*/
#include <linux/string.h>/*For strlen...*/
#include <linux/errno.h> /*For the -ENODEV/...value*/
#include <linux/init.h>/*For  __init/__exit/....*/
#include <linux/fs.h>/*For struct file_operations*/
#include <linux/cdev.h>/*For struct cdev/cdev_init/...*/
#include <asm/uaccess.h>/*For copy_to_user/ ...*/
#include <linux/errno.h>
#include <linux/slab.h>  /*for kmalloc()*/
#include <asm/ioctl.h>  /*for some defines of ioctl*/

#include "cmd.h"  /*for cmd in misc_ioctl)*/
#define DEV_SIZE 100
#define  MISC_NAME "misc_ioctl"

/*
*declare the struct my_data, contains private data
* kbuf :the data kernel keeps.
*cur_size: the current size of the kbuf:
*/
struct my_data{
		
	char kbuf[DEV_SIZE];
	unsigned int cur_size;
	
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

ssize_t misc_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	struct my_data *dev = filp->private_data;
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
	if(dev->cur_size<0)//check the current size of the array.
	{
		printk("cur_size is error\n");
		return -1;
	}

	
	/*
	*Check read successfully or not
	*/
	if(!copy_to_user(buf,(void *)(dev->kbuf+*offset),count))
	{
		ret = count;
		dev->cur_size -= count; //change the current size
		*offset += count;//change *offset after has been readed
	
		printk("<Read form kernel> buf is :%s\n",buf);
	}
	else
	 {
		ret = -EFAULT;
		printk("Kernel test_read failed\n");
	}

		return ret;
}


/*****************************************************************************************
***	Description:		
***			write data to kernel	
***
***	Return Value:
***			If write sucessfully, return the bytes that have written.
*******************************************************************************************/
ssize_t misc_write(struct file *filp, char __user *buf, size_t count, loff_t *offset)
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
	*Check write successfully or not
	*/
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


	return ret;
}


/*****************************************************************************************
***	Description:		
***			switch the cmd, and execute it
***
***	Return Value:
***			If there is no error, return 0.
***
***	Note:
***		After kernel linux-2.6.36,It didn't use not ioctl any longer.
***			The function prototype ：int (*unlocked_ioctl)(struct file *filp, unsigned int cmd, unsigned long arg)
***					the old ioctl:	 int (*ioctl)(struct inode *inodep, struct file *filp, unsigned int cmd, unsigned long arg)		
*******************************************************************************************/

static int misc_ioctl( struct file *filp,unsigned int cmd, unsigned long arg)
{

	
	struct my_data *dev = filp->private_data;
	switch(cmd)
	{
	
		case WRITE_IN:
		
			printk("This is WRITE cmd\n");
			/*
			*Check write successfully or not
			*/
			if(!copy_from_user(dev->kbuf,(void *)arg,50))
			{
				dev->cur_size += 50;
				printk("<write to kernel> buf is :%s\n",dev->kbuf);
			}
			else 
			{
				return -EFAULT;
			}
				
			break;
		
		case CLEAR:
			memset(dev->kbuf,0,DEV_SIZE);
			printk("This is clear cmd!\n");
			break;

		default:
			printk("Error cmd\n");

			break;
	}


	return 0;
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

	filp->f_pos = new_pos;
	return  new_pos;
}




/*
*The struct file_operations
*/
struct file_operations misc_fops={
	.open = misc_open,
	.release = misc_close,
	.write = misc_write,
	.read = misc_read,
	.llseek = misc_llseek,
	.unlocked_ioctl = misc_ioctl,
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

	ret = misc_register(&misc_dev);
	if(ret)
	{
		printk("misc_register error\n");
		return ret;
	
	}
	
	printk("Hello kernel!\n");
	return 0;
}


/*
*Unistall function
*/
static void __exit misc_exit(void)
{
    printk("Goodbye kernel!\n");
	
	kfree(devp);//delete structure pointer
	misc_deregister(&misc_dev);	
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Albert");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("A simple driver, contains ioctl mis_register......");

