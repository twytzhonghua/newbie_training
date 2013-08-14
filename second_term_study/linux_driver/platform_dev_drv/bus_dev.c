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






static struct platform_device albert_dev = {
    .name         = "albert_driver",
    .id       = -1,

/*    .dev = { 
    	.release = led_release, 
	},
*/
};



static int my_dev_init(void)
{
	platform_device_register(&albert_dev);
	return 0;
}

static void my_dev_exit(void)
{
	platform_device_unregister(&albert_dev);
}

module_init(my_dev_init);
module_exit(my_dev_exit);

MODULE_LICENSE("GPL");





