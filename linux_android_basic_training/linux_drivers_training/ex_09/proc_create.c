/************************************************************************************
*** Filename:
***			proc_create.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		 A simple char device driver create node under /proc
***
***	Author/Creat Date:
*** 		Albert Yang, Jun 24,13
***
***	Modification History:
***			
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
#include <linux/cdev.h>/*For struct cdev/cdev_init/...*/
#include <asm/uaccess.h>/*For copy_to_user/ ...*/
#include <linux/errno.h>/*For error symbol*/
#include <linux/slab.h>  /*for kmalloc()*/
#include <linux/sched.h>  
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>

#define DEV_SIZE 100


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
struct proc_dir_entry *proc_parent = NULL; 


int proc_open(struct inode *node,struct file *filp)
{

	filp->private_data = devp; // give the structure pointer to filp->private_data
	printk("This is proc_open\n");
	return 0;
}
	
int proc_close(struct inode *node,struct file *filp)
{
	printk("This is proc_close\n");
	return 0;
}


/*****************************************************************************************
***	Description:		
***			read data from kernel	
***
***	Return Value:
***			If read sucessfully, return the bytes that have readed.
*******************************************************************************************/

static ssize_t proc_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	struct my_data *dev = filp->private_data;

	/*同步访问*/  
	if(down_interruptible(&(dev->sem))) {  
	        return -ERESTARTSYS;          
	}    
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
	if(dev->cur_size == 0)//check the current size of the array.
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
		
	up(&(dev->sem));
	
	return ret;
}


/*****************************************************************************************
***	Description:		
***			write data to kernel	
***
***	Return Value:
***			If write sucessfully, return the bytes that have written.
*******************************************************************************************/
static ssize_t proc_write(struct file *filp,const char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	

	struct my_data *dev = filp->private_data;
	
	if(down_interruptible(&(dev->sem))) {  
	        return -ERESTARTSYS;          
	}  

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
	
	up(&(dev->sem));

	return ret;
}






/*****************************************************************************************
***	Description:	file location function	
*******************************************************************************************/
loff_t proc_llseek(struct file *filp, loff_t offset, int whence)
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
struct file_operations proc_fops={
	.open = proc_open,
	.release = proc_close,
	.read = proc_read,
	.write = proc_write,
	.llseek = proc_llseek,
};


static int  create_proc(void)
{
     proc_parent= proc_mkdir("albert", NULL);//create albert diretory under /proc 
 
     if (proc_parent) {
         proc_create("my_proc", S_IRUGO, proc_parent,&proc_fops);//create device node under /proc/albert
         return (1);
     }
     return (0);
 }
 

static void remove_proc(void)
{
	remove_proc_entry("my_proc", NULL);
	remove_proc_entry("albert", NULL);
}


/*
*Loading function
*/
static int __init proc_init(void)
{

	devp = kmalloc(sizeof(struct my_data),GFP_KERNEL);//allocate space for devp
	if(!devp)
	{
		printk("kmalloc failed!\n");
		return -ENOMEM;
	}

	memset(devp, 0, sizeof(struct my_data)); 
	/*
	*create device under proc
	*/
	if(!create_proc())
	{
		printk("Create proc device failed!!!\n");
	}
	else
	{
		printk("Create proc device successfully!\n");
	}
	
	
	sema_init(&devp->sem,1); 
	
	printk("Hello kernel!\n");
	return 0;
}


/*
*Unistall function
*/
static void __exit proc_exit(void)
{
	printk("Goodbye kernel!\n");

	if(devp != NULL)	
	{
		kfree(devp);//delete structure pointer
		devp = NULL;
	}
		
	remove_proc();


}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Albert");
MODULE_VERSION("1.1");
MODULE_DESCRIPTION("A simple driver create node under /proc");












