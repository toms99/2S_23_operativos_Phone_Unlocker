obj-m += arduino_driver.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
CC = x86_64-linux-gnu-gcc

build_driver: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CC=$(CC)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules CC=$(CC)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean