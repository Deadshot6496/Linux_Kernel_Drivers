obj-m += myModule.o
 
 KDIR = /lib/modules/4.19.66-v7+/build
all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
