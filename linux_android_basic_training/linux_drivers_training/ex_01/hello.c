/*****************************************************
** Filename: hello.c 
** Copyright (c) 2013 Wistron
** All rights reserved.
** 
** Description:
**      This is a simple kernel module.
** Author/Created :
**       Albert Yang, Apr 08'13
** Modification History:
**      Apr 10'13
**
****************************************************/


#include <linux/init.h>
#include <linux/module.h>

/**********************************************
***Description:
*** 		loading function
**********************************************/
static int hello_init(void)
{
	printk(KERN_ALERT "Hello, beautiful world!\n"); //The kenerl prints debugging information
	return 0;
}

/*******************************************
***Description:
***		Uninstall function
*********************************************/
static void hello_exit(void)
{
	printk( KERN_ALERT"Goodbye, cruel world!\n");
}


module_init(hello_init);  //assign the loading function
module_exit(hello_exit);  //assign the uninstall function


/*
*Statement and description of the module
*/
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Albert");
MODULE_DESCRIPTION("A simple kernel module");
MODULE_VERSION("1.0");



