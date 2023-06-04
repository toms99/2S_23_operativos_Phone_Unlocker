all: build_Client build_Server

build_Server:
	gcc -o ./out/server ./server/server.c -ljansson

run_Server: build_Server
	./out/server

build_Client:
	gcc client/client.c -o out/client -ljansson -lc
	
run_Client: build_Client
	./out/client 123456789 2

obj-m += driver/arduino_driver.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
CC = x86_64-linux-gnu-gcc

build_driver: clean_driver 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CC=$(CC)

add_driver_to_kernel: build_driver remove_driver_from_kernel
	sudo insmod driver/arduino_driver.ko
	$(MAKE) -C $(KDIR) M=$(PWD) clean

first_add_driver_to_kernel: build_driver
	sudo insmod driver/arduino_driver.ko
	$(MAKE) -C $(KDIR) M=$(PWD) clean

remove_driver_from_kernel:
	sudo rmmod arduino_driver

clean_driver:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
