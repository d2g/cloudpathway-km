KDIR := /lib/modules/$(shell uname -r)/build
#KDIR := /usr/src/rpi/linux-rpi-3.10.y
#CCPREFIX := /usr/src/rpi/tools-master/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-
PWD := $(shell pwd)

obj-m += main.o
main-objs := ./src/main.o ./src/capture.o ./src/communicate.o ./src/broadcast.o ./src/datastore.o

default:	
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
rpi:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CCPREFIX) -C $(KDIR) SUBDIRS=$(PWD) modules