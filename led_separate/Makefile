KERN_DIR = /home/y/Documents/100ask_imx6ull-sdk/Linux-4.9.88

all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(CROSS_COMPILE)gcc -o led_test led_test.c
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f led_test

led-y := led_drv.o chip_demo_gpio.o board_a_led.o
obj-m += led.o
