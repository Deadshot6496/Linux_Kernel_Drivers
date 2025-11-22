obj-m += myModule.o
 
 KDIR = /lib/modules/6.12.47+rpt-rpi-v8/build
all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
