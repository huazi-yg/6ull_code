#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/platform_device.h>
#include <asm/io.h>

#include "led_opr.h"
#include "led_drv.h"
#include "led_resource.h"

static struct led_resource *led_resource_s;
static int g_ledpins[100];
static int g_ledcnt = 0;

static int board_demo_led_init(int which);
static int board_demo_led_ctl(int which, char status);





static int chip_demo_gpio_probe(struct platform_device * pdev)
{
	struct resource *res;
	int i = 0;
	while (1) 
	{
		res = platform_get_resource(pdev,IORESOURCE_IRQ,i++);
		if(!res)
		{
			break;
		}

		g_ledpins[g_ledcnt] = res->start;
		led_class_create_device(g_ledcnt);
		g_ledcnt++;
	}
	return 0;
}

static int chip_demo_gpio_remove(struct platform_device * pdev)
{
	struct resource *res;
	int i = 0;
	while (1) 
	{
		res = platform_get_resource(pdev,IORESOURCE_IRQ,i++);
		if(!res)
		{
			break;
		}

		led_class_destroy_device(i);
		i++;
		g_ledcnt--;
	}
	return 0;
}


static struct platform_driver chip_demo_gpio_driver  = {
	.probe = chip_demo_gpio_probe,
	.remove = chip_demo_gpio_remove,
	.driver = {
		.name = "led_driver",
	},
};

static struct led_operations board_demo_led_opr = {
	.num = 1,
	.init = board_demo_led_init,
	.ctl = board_demo_led_ctl,
};

struct led_operations *get_board_led_opr(void)
{
	return &board_demo_led_opr;
}

static int __init chip_demo_gpio_drv_init(void)
{
	int err;

	err = platform_driver_register(&chip_demo_gpio_driver);

	register_led_operations(&board_demo_led_opr);
	return 0;
	
}

static void __exit chip_demo_gpio_drv_exit(void)
{
    platform_driver_unregister(&chip_demo_gpio_driver);
}


static int board_demo_led_init(int which)
{

	switch (GROUP(g_ledpins[which])) {
		case 0:
			printk("init pin of group %d ...\n",which);
		break;
		case 1:
			printk("init pin of group %d ...\n",which);
		break;
		case 2:
			printk("init pin of group %d ...\n",which);
		break;
		case 3:
			printk("init pin of group %d ...\n",which);
		break;
		default:
			printk("init pin of group %d ...\n",which);
		break;
	
	}
	return 0;
}

static int board_demo_led_ctl(int which, char status)
{
	printk("led ctl\n");
	switch (GROUP(g_ledpins[which])) {
		case 0:
			printk("set pin of group 0 ...\n");
		break;
		case 1:
			printk("set pin of group 1 ...\n");
		break;
		case 2:
			printk("set pin of group 2 ...\n");
		break;
		case 3:
			printk("set pin of group 3 ...\n");
		break;
		default:
			printk("there is no led handware!\n");
		break;
	
	}

	return 0;
}




module_init(chip_demo_gpio_drv_init);
module_exit(chip_demo_gpio_drv_exit);

MODULE_LICENSE("GPL");
