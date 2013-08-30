#include <linux/input.h> 
#include <linux/timer.h> 
#include <linux/module.h> 
#include <linux/init.h>
#include <asm/irq.h> 
#include <linux/irq.h> 
#include <asm-arm/irq.h> 
#include <asm/io.h>
#include <asm/gpio.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <asm/arch/regs-gpio.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/hardware.h>

/*
#include <mach/hardware.h>
#include <plat/regs-serial.h>
#include <mach/regs-gpio.h>
#include <linux/input.h>
*/
static struct input_dev *button_dev;
static struct timer_list buttons_timer;
static struct pin_desc *ppin_desc;


struct pin_desc{
	int irq;
	char *name;
	unsigned int pin;
	unsigned int val;	
};


static struct pin_desc pin_desc[3] = {
	{IRQ_EINT11,	"KEY_ENTER",	S3C2410_GPG3,	KEY_ENTER},
	{IRQ_EINT0,		"KEY_L",		S3C2410_GPF0,	KEY_L},
	{IRQ_EINT2,		"KEY_S",		S3C2410_GPF2,	KEY_S},
	
	
};


static irqreturn_t button_interrupt(int irq, void *dummy) 
{ 
	
	ppin_desc = (struct pin_desc *)dummy;
	mod_timer(&buttons_timer, jiffies+HZ/100);
	return IRQ_HANDLED; 
}


static void buttons_timer_function(unsigned long data)
{
	int val_temp = 0;
	
	if(!ppin_desc)
		return ;

	val_temp = s3c2410_gpio_getpin(ppin_desc->pin);
	if(0 == val_temp)
	{
		input_event(button_dev, EV_KEY, ppin_desc->val, 1);
		input_sync(button_dev);
	}
	else{

		input_event(button_dev, EV_KEY, ppin_desc->val,0);
		input_sync(button_dev);
	}

}



static int __init button_init(void) 
{ 
	int error,i;
	
	for(i=0;i<3;i++)
	{
		if (request_irq(pin_desc[i].irq, button_interrupt, IRQT_BOTHEDGE,
			pin_desc[i].name, &pin_desc[i]))
		{ 
		 
			printk(KERN_ERR "button.c: Can't allocate irq %d\n", pin_desc[i].irq);
			return -EBUSY; 
		}
	}
  
	button_dev = input_allocate_device(); //分配一个input_dev 结构
	if (!button_dev) 
	{ 
		printk(KERN_ERR "button.c: Not enough memory\n");
		error = -ENOMEM;
		goto err_free_irq; 
	}

	set_bit(EV_KEY, button_dev->evbit);//能产生按键类事件
	set_bit(EV_REP, button_dev->evbit);//能产生重复类事件
	
	set_bit(KEY_L, button_dev->keybit);//按键中的"l"
	set_bit(KEY_S, button_dev->keybit);//按键中的"s"
	set_bit(KEY_ENTER, button_dev->keybit);//"Enter"
		
	error = input_register_device(button_dev);//注册input_dev
	if (error) 
	{ 
		printk(KERN_ERR "button.c: Failed to register device\n");
		goto err_free_dev; 
	}

	init_timer(&buttons_timer);
	buttons_timer.function = buttons_timer_function;
	add_timer(&buttons_timer);
	
	
	return 0;
	
err_free_dev:
	input_free_device(button_dev);
	
err_free_irq:
	for(i=0;i<3;i++){
			free_irq(pin_desc[i].irq, &pin_desc[i]); 
		}

	return error; 
}




static void __exit button_exit(void) 
{ 
	char i;
	input_unregister_device(button_dev); 
	del_timer(&buttons_timer);
	for(i=0;i<3;i++){
		free_irq(pin_desc[i].irq, &pin_desc[i]); 
	}
	input_free_device(button_dev);	
}


module_init(button_init); 
module_exit(button_exit);

MODULE_LICENSE("GPL");
