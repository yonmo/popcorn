obj-m += popcorn.o

HEADERS= /lib/modules/$(shell uname -r)/build

modules:
	make -C $(HEADERS) M=$(PWD) modules

clean:
	make -C $(HEADERS) M=$(PWD) clean

test:
	sudo dmesg -C
	sudo insmod popcorn.ko
	sudo lsmod | grep popcorn
	sudo rmmod popcorn.ko
	dmesg

install:
	sudo dmesg -C
	sudo insmod popcorn.ko

purge:
	sudo rmmod popcorn.ko
	dmesg
