
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

#include "led_resource.h"

static void led_dev_release(struct device *pdev)
{

}
static struct resource led_resource[] = {
    {
    .start = GROUP_PIN(3, 1),
    .flags = IORESOURCE_IRQ,
    .name = "led1_pin"
    },
    {
    .start = GROUP_PIN(5, 1),
    .flags = IORESOURCE_IRQ,
    .name = "led2_pin"
    }
};

static struct platform_device board_a_led_dev = {
    .name = "led_dev",
    .num_resources = ARRAY_SIZE(led_resource),
    .resource = led_resource,
    .dev = {
        .release = led_dev_release,
    },
};


//设备初始化
static int __init led_dev_init(void)
{
    int err;
    err = platform_device_register(&board_a_led_dev);
    return 0;
}

static void __exit led_dev_exit(void)
{
platform_device_unregister(&board_a_led_dev);
}

module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");

