all: build_Client build_Server

# Server ----------------------------------------------------------

build_Server: build_lib
	gcc -o ./out/server ./server/server.c -ljansson -L./server/lib/ -larduino_lib -I./server/lib

run_Server: build_Server
	sudo ./out/server

# Client -----------------------------------------------------------

build_Client:
	gcc client/client.c -o out/client -ljansson -lc
	
run_Client: build_Client
	./out/client
#	sudo ./out/client 123456789 2

# Driver ------------------------------------------------------------

obj-m += driver/SO_driver.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

build_driver: clean_driver 
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

add_driver_to_kernel: build_driver remove_driver_from_kernel
	sudo insmod driver/SO_driver.ko
	$(MAKE) -C $(KDIR) M=$(PWD) clean

first_add_driver_to_kernel: build_driver
	sudo rmmod ftdi_sio
	sudo insmod driver/SO_driver.ko
	$(MAKE) -C $(KDIR) M=$(PWD) clean

remove_driver_from_kernel:
	sudo rmmod SO_driver

clean_driver:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean


# Lib ------------------------------------------------------------

build_lib:
	gcc -c library/arduino_lib.c -o library/arduino_lib.o
	ar rcs library/libarduino_lib.a library/arduino_lib.o
	cp library/libarduino_lib.a server/lib
	cp library/arduino_lib.h server/lib