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
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>

#define DRIVER_NAME "gpio_key_driver"

struct gpio_key{
	char name[100];
	int irq;
	int cnt;
} ;

static struct gpio_key gpio_keys_100ask[100];

static irqreturn_t gpio_key_isr(int irq, void *dev_id)
{
    struct gpio_key *gpio_key = dev_id;
    gpio_key->cnt++;
    printk("GPIO key %s pressed, count=%d\n", gpio_key->name, gpio_key->cnt);
    return IRQ_HANDLED;
}

static int gpio_key_probe(struct platform_device *pdev)
{
    int err;
    int i =0;
    int irq;

    printk("gpio_key_probe of node: %s\n", pdev->dev.of_node->name);

    while (1)
    {
        irq = platform_get_irq(pdev, i);
        if (irq < 0)
        {
            printk("irq %d not found\n", i);
            break;
        }
        gpio_keys_100ask[i].irq = irq;
        sprintf(gpio_keys_100ask[i].name, "gpio_key_%d", i);

        err = devm_request_irq(&pdev->dev, gpio_keys_100ask[i].irq, gpio_key_isr, 
                              IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING, 
                              gpio_keys_100ask[i].name, &gpio_keys_100ask[i]);
        if (err) {
            dev_err(&pdev->dev, "Failed to request IRQ %d: %d\n", 
                   gpio_keys_100ask[i].irq, err);
            continue;
        }
        printk("Registered IRQ %d for %s\n", gpio_keys_100ask[i].irq, gpio_keys_100ask[i].name);
        i++;
    }
    printk("gpio_key_probe success\n");
    return 0;
}
static int gpio_key_remove(struct platform_device *pdev)
{
    printk("gpio_key_remove\n");
    return 0;
}

static const struct of_device_id gpio_key_of_match[] = {
    { .compatible = "gpio_key" },
    { },
};

static struct platform_driver gpio_key_driver = {
    .probe = gpio_key_probe,
    .remove = gpio_key_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = gpio_key_of_match,
    },
};
static int __init gpio_key_init(void)
{
    int err;
    printk("gpio_key_init\n");
    err = platform_driver_register(&gpio_key_driver);

    return err;
}

static void __exit gpio_key_exit(void)
{
    printk("gpio_key_exit\n");
    platform_driver_unregister(&gpio_key_driver);
}

module_init(gpio_key_init);
module_exit(gpio_key_exit);

MODULE_LICENSE("GPL");