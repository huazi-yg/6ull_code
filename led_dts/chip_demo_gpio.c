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
#include <linux/gpio/consumer.h>

#include "led_opr.h"
#include "led_drv.h"
#include "led_resource.h"

static struct led_resource *led_resource_s;
static int g_ledpins[100];
static int g_ledcnt = 0;
static struct gpio_desc *led_gpio;

static int board_demo_led_init(int which);
static int board_demo_led_ctl(int which, char status);





static int chip_demo_gpio_probe(struct platform_device * pdev)
{
	struct device_node *np;
	int err = 0;
	led_gpio = gpiod_get(&pdev->dev,"led",0);
	if (IS_ERR(led_gpio)) {
	 dev_err(&pdev->dev, "Failed to get GPIO for led\n");
	 return PTR_ERR(led_gpio);
 	}

	led_class_create_device(g_ledcnt);
	g_ledcnt++;

	return 0;
	
}

static int chip_demo_gpio_remove(struct platform_device * pdev)
{
	struct device_node *np;
	int led_pin;
	int i;

	np = pdev->dev.of_node;
	if(!np)
	{
		return -1;
	}

	for(i = 0;i<g_ledcnt;i++)
	{
		if(g_ledpins[i] == led_pin)
		{
			led_class_destroy_device(i);
			g_ledpins[i] = -1;
			break;
		}
	}

	for(i = 0;i<g_ledcnt;i++)
	{
		if(g_ledpins[i] != -1)
		{
			led_class_destroy_device(i);
			break;
		}
	}

	if(i == g_ledcnt)
		g_ledcnt = 0;
	gpiod_put(led_gpio);
	return 0;
	
	
}

//注册platform driver
static const struct of_device_id leddriver[] = {
	{ .compatible = "led1,6ull_led"},
	{ },
};

static struct platform_driver chip_demo_gpio_driver  = {
	.probe = chip_demo_gpio_probe,
	.remove = chip_demo_gpio_remove,
	.driver = {
		.name = "led1,6ull_led",
		.of_match_table = leddriver,
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
	printk("init pin of group %d ...\n",which);
	gpiod_direction_output(led_gpio,0);
	return 0;
	

}

static int board_demo_led_ctl(int which, char status)
{
	printk("set pin of group %d ...\n",which);
	gpiod_set_value(led_gpio, status);
	return 1;
}




module_init(chip_demo_gpio_drv_init);
module_exit(chip_demo_gpio_drv_exit);

MODULE_LICENSE("GPL");
