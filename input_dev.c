#include <linux/module.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/spinlock.h>

static const struct of_device_id input_dev_demo_of_match[] = {
	{ .compatible = "100ask,input_dev_demo", },
	{ /* sentinel */ }
};

static struct input_dev *g_input_dev;
static int g_irq;

static irqreturn_t input_dev_demo_irq(int irq,void *dev_id)
{
		/* read data */

		/* report data*/
		// input_event(g_input_dev,EV_KEY,xx,0);
		// input_sync(g_input_dev);

		printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);

		return IRQ_HANDLED;
}
// alloc/set/register platform driver
static int input_dev_demo_probe(struct platform_device *pdev)
{
    
    struct device *dev = &pdev->dev;
	//struct resource *irq;
	int error;

	int gpio;
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	gpio = of_get_gpio(pdev->dev.of_node, 0);
    //get hardware info from dts
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    //alloc/set/register platform device
    g_input_dev = devm_input_allocate_device(&pdev->dev);

    g_input_dev->name = "input dev demo";
	g_input_dev->phys = "input_dev_demo";
	g_input_dev->dev.parent = dev;

	g_input_dev->id.bustype = BUS_HOST;
	g_input_dev->id.vendor = 0x0001;
	g_input_dev->id.product = 0x0001;
	g_input_dev->id.version = 0x0100;

	// set 1. which event type
	__set_bit(EV_KEY, g_input_dev->evbit);
	__set_bit(EV_ABS, g_input_dev->evbit);
    
	// set 2. which event
	__set_bit(BTN_TOUCH, g_input_dev->keybit);
	__set_bit(ABS_MT_SLOT,g_input_dev->absbit);
	__set_bit(ABS_MT_POSITION_X,g_input_dev->absbit);	
	__set_bit(ABS_MT_POSITION_Y,g_input_dev->absbit);

	//set parameter
	input_set_abs_params(g_input_dev, ABS_MT_POSITION_X, 0, 0xffff, 0, 0);
	input_set_abs_params(g_input_dev, ABS_MT_POSITION_Y, 0, 0xffff, 0, 0);

	//register input device
	error = input_register_device(g_input_dev);
	if (error) {
		dev_err(dev, "Failed to register input device\n");
		return error;
	}
    //hardware operation

	//irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	//g_irq = gpio_to_irq(gpio);
	// g_irq	= irq->start;

	g_irq	= gpio_to_irq(gpio);
	error = request_irq(g_irq, input_dev_demo_irq, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "input_dev_demo", NULL);
    return 0;
}

static int input_dev_demo_remove(struct platform_device *pdev)
{
	free_irq(g_irq,NULL);
	input_unregister_device(g_input_dev);
    return 0;
}

static struct platform_driver input_dev_demo_driver = {
	.probe = input_dev_demo_probe,
	.remove = input_dev_demo_remove,
	.driver = {
		.name = "input_dev_demo",
        .of_match_table = input_dev_demo_of_match,
	},
};

static int __init input_dev_demo_init(void)
{
	printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	return platform_driver_register(&input_dev_demo_driver);
}

static void __exit input_dev_demo_exit(void)
{
	printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	platform_driver_unregister(&input_dev_demo_driver);
}


module_init(input_dev_demo_init);
module_exit(input_dev_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yg <huazi_ss@163.com>");
MODULE_DESCRIPTION("input dev demo");

