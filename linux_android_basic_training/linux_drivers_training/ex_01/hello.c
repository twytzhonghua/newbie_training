/**********************************
** Filename: hello.c 
** Copyright (c) 2006 Wistron
** All rights reserved.
** 
** Description:
**      This is a simple kernel module.
** Author/Created :
**       Albert Yang, Apr 08'13
** Modification History:
**      Apr 09'13
**
**********************************/


#include <linux/init.h>
#include <linux/module.h>

/*
*Statement of module's permission 
*/
MODULE_LICENSE("Dual BSD/GPL");

/*********************
***Description:
*** 		loading function
*********************/
static int hello_init(void)
{
	printk(KERN_ALERT "Hello, beautiful world!\n"); //The kenerl prints debugging information
	return 0;
}

/********************************
***Description:
***		Uninstall function
***********************************/
static void hello_exit(void)
{
	printk( KERN_ALERT"Goodbye, cruel world!\n");
}


module_init(hello_init);  //指定加载函数
module_exit(hello_exit);  //指定卸载函数


/*
*Statement and description of the module
*/
MODULE_AUTHOR("Albert");
MODULE_DESCRIPTION("A simple kernel module");
MODULE_VERSION("1.0");



