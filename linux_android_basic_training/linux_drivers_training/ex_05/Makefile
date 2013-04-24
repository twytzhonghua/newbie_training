obj-m := concur_race.o

KERNELDIR ?= /usr/src/linux-headers-2.6.38-8-generic/
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules    


clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf .tmp_versions
