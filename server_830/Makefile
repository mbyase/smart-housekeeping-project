#module templet
ifeq ($(KERNELRELEASE),)
KERNELDIR ?= /home/farsight/duanxueqi/linux-3.14-fs4412
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	cp -af *.ko /tftpboot
	arm-linux-gcc fs4412_led_app.c -o app
	cp -af app  /tftpboot
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules clean
	#rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module* modules* a.out
   
else
    obj-m := fs4412_led_drv.o
endif



