/**********************************************************************************************
*** Filename:
***			test.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		 A simple char device.
***
***	Author/Creat Date:
*** 		Albert Yang, Apri 16,13
***
***	Note:
***
************************************************************************************************/

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


#include "cmd.h"/*For the cmd in ioctl*/

static struct class *test_class; //create a test_class

/*
* Declare the device number, major, minor
*/
unsigned int major,minor;
dev_t devno;



int test_open(struct inode *node,struct file *filp)
{
	printk("This is test_open\n");
	return 0;
}
	
int test_close(struct inode *node,struct file *filp)
{
	printk("This is test_close\n");
	return 0;
}


/*****************************************************************************************
***	Description:		
***			read data from kernel	
***
***	Return Value:
***			If read sucessfully, return the bytes that have readed.
*******************************************************************************************/

ssize_t test_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	if(!copy_to_user(buf,"this is kernel data",count))
	{
		printk("<Read from kernel> buf is :%s\n",buf);
		return count;
	}
	else
	 {
		ret = -EFAULT;
		printk("Kernel test_read failed\n");
		return ret;
	}
}


/*****************************************************************************************
***	Description:		
***			write data to kernel	
***
***	Return Value:
***			If write sucessfully, return the bytes that have written.
*******************************************************************************************/
ssize_t test_write(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	char kbuf[20];
	if(!copy_from_user(kbuf,buf,count))
	{

		printk("<write to kernel> buf is :%s\n",buf);
		return count;
	}
	else 
	{
		ret = -EFAULT;
		return ret;
	}


	return 0;
}


/*****************************************************************************************
***	Description:		
***			Judge the cmd, and execute the actions.	
***
***	Return Value:
***			If find the cmd sucessfully, return 0, else return the error symbol.
*******************************************************************************************/
static int test_ioctl(struct inode *inodep, struct file *filep, unsigned int cmd, unsigned long arg)
{
	int ret;

	printk("Hello, this is ioctl function\n");
	switch(cmd)
	{
		case HELLO:
			printk("This is HELLO cmd\n");
			ret = 0;
			break;

		default:
			printk("error cmd\n");
			ret = -EINVAL;
			break;
					
	}

	return ret;
}



/*
*The struct file_operations
*/
struct cdev cdev;
struct file_operations test_fops={
	.open = test_open,
	.release = test_close,
	.write = test_write,
	.read = test_read,
	.unlocked_ioctl = test_ioctl,
};




/*
*Loading function
*/
static int __init test_init(void)
{

	int result;

	if(major)//if major is not equal zero, apply for a static device number
	{
		devno = MKDEV(major,minor);
		result = register_chrdev_region(devno,1,"test_drv");

	}
	else//otherwise, dynamically allocated by the kernel
	{
		result = alloc_chrdev_region(&devno,minor,1,"test_drv");
		major = MAJOR(devno);
		minor = MINOR(devno);
	}
	
	/*check whether register successfully or not*/
	if(result < 0)
	{
		printk("register devno errno!\n");
		result = -EBUSY;
		goto err0;
	}

	printk("major[%d],minor[%d]\n",major,minor);

	/*
	*init the cdev
	*/
	cdev_init(&cdev,&test_fops); 
	cdev.owner = THIS_MODULE;
	cdev.ops = &test_fops;

	/*Add cdev*/
	result = cdev_add(&cdev,devno,1);
	if(result < 0)
	{

		printk("cdev_add errno!\n");
		result = -ENODEV;
		goto err1;
	}

	/*
	*Creat class and device.
	*/
	test_class = class_create(THIS_MODULE,"test_class");
	if(IS_ERR(test_class))
	{
		printk("creat test_class failed!\n");
		return -1;
	}
	device_create(test_class,NULL,devno,NULL,"%s","test");


    printk("hello kernel!\n");



err0:
	return result;
err1:
	unregister_chrdev_region(devno,1);//cancel the wrong handleing
	return 0;
}


/*
*Unistall function
*/
static void __exit test_exit(void)
{
    printk("Goodbye kernel!\n");

	/*
	*delete the cdev and destroy the test_class and device that have been created.
	*/
	cdev_del(&cdev);
	device_destroy(test_class,devno);
	class_destroy(test_class);

	/*unregister char device*/
	unregister_chrdev_region(devno,1);
	
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Albert");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("A char device module");

