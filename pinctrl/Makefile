KERN_DIR = /home/book/Documents/100ask_imx6ull-qemu/linux-4.9.88

all:
	make -C $(KERN_DIR) M=`pwd` modules
	# 编译测试程序
	# $(CROSS_COMPILE)gcc -o led_test led_test.c
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m += virtual_pinctrl_client.o
obj-m += virtual_pinctrl_driver.o