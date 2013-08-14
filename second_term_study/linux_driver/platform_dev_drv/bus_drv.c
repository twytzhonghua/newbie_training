#include <linux/module.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/spinlock.h>

static int __devinit albert_probe(struct platform_device *op)
{
	printk("hello, find albert_driver\n");	
	return 0;
}

static int __devexit albert_remove(struct platform_device *op)
{
	printk("goodbye, remove albert_driver\n");	
	return 0;
}

/* Structure for a device driver */
static struct platform_driver albert_driver = {
	.probe		= albert_probe,
	.remove		= albert_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name ="albert_driver",

	},
};

static int __init mytest_init(void)
{
	return platform_driver_register(&albert_driver);
}

static void __exit mytest_exit(void)
{
	platform_driver_unregister(&albert_driver);
}

module_init(mytest_init);
module_exit(mytest_exit);

MODULE_LICENSE("GPL");