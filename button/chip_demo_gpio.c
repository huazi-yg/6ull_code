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
#include <linux/of.h>

#include "button_drv.h"

static int g_ledpins[100];
static int g_ledcnt = 0;

static int board_demo_button_read(int which);
static int board_demo_button_init(int which);
static int chip_demo_gpio_probe(struct platform_device * pdev);
static int chip_demo_gpio_remove(struct platform_device * pdev);
static void __exit chip_demo_gpio_drv_exit(void);
static int __init chip_demo_gpio_drv_init(void);

static const struct of_device_id buttondriver[] = {
	{ .compatible = "button,6ull"},//在设备树中匹配
	{ },
};

static struct platform_driver chip_demo_gpio_driver  = {
	.probe = chip_demo_gpio_probe,
	.remove = chip_demo_gpio_remove,
	.driver = {
		.name = "led_driver",
		.of_match_table = buttondriver,
	},
	
};

static int chip_demo_gpio_probe(struct platform_device * pdev)
{
	// struct device_node *np;
	// int err = 0;
	// int led_pin;

	// np = pdev->dev.of_node;

	// if(!np)
	// 	return -1;
	// err = of_property_read_u32(np,"pin",&led_pin);

	// g_ledpins[g_ledcnt] = led_pin;
	// led_class_create_device(g_ledcnt);
	// g_ledcnt++;

	return 0;
	
}

static int chip_demo_gpio_remove(struct platform_device * pdev)
{
	// struct device_node *np;
	// int err = 0;
	// int led_pin;
	// int i;

	// np = pdev->dev.of_node;
	// if(!np)
	// {
	// 	return -1;
	// }
	// err = of_property_read_u32(np,"pin",&led_pin);

	// for(i = 0;i<g_ledcnt;i++)
	// {
	// 	if(g_ledpins[i] == led_pin)
	// 	{
	// 		led_class_destroy_device(i);
	// 		g_ledpins[i] = -1;
	// 		break;
	// 	}
	// }

	// for(i = 0;i<g_ledcnt;i++)
	// {
	// 	if(g_ledpins[i] != -1)
	// 	{
	// 		led_class_destroy_device(i);
	// 		break;
	// 	}
	// }

	// if(i == g_ledcnt)
	// 	g_ledcnt = 0;
	return 0;
	
	
}

static struct button_operations board_button_opr = {
	.count = 1,
	.init = board_demo_button_init,
	.read = board_demo_button_read,
};

static int __init chip_demo_gpio_drv_init(void)
{
	register_button_operations(&board_button_opr);
	return 0;
}

static void __exit chip_demo_gpio_drv_exit(void)
{
    unregister_button_operations(&board_button_opr);
}

static int board_demo_button_init(int which)
{
	printk("%s %s %d, init gpio for button %d\n", __FILE__, __FUNCTION__, __LINE__, which);
}

static int board_demo_button_read(int which)
{
	printk("%s %s %d, read gpio for button %d\n", __FILE__, __FUNCTION__, __LINE__, which);
	return 1;
}

module_init(chip_demo_gpio_drv_init);
module_exit(chip_demo_gpio_drv_exit);
MODULE_LICENSE("GPL");
