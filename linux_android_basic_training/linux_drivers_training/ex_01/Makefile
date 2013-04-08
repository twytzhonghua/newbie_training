ifneq ($(KERNELRELEASE),)

obj-m := hello.o


else
 
KERNELDIR ?= /usr/src/linux-headers-2.6.38-8-generic/
PWD := $(shell pwd)


default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* Module.* modules.order 

endif


