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

struct gpio_key
{
	int gpio;
	struct gpio_desc *gpiod;
	int flag;
	int irq;
};

static struct gpio_key *my_gpio_key;

//4. 中断处理函数
static irqreturn_t gpio_ker_isr(int irq,void *dev_id)
{
	struct gpio_key *gpio_key = dev_id;
	int val;
	val = gpiod_get_value(gpio_key->gpiod);
	printk("key %d %d",gpio_key->gpio,val);
	return IRQ_HANDLED;
}

static int gpio_key_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	int err;
	int count;
	int i;
	enum of_gpio_flags flag;
	unsigned int flags = GPIOF_IN;
	
	printk("key probe %s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	count = of_gpio_count(node);

	if(!count)
	{
		printk("there is not any gpio available %s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	}

	my_gpio_key = kzalloc(sizeof(struct gpio_key)*count,GFP_KERNEL);
	for(i = 0;i<count;i++)
	{
		my_gpio_key[i].gpio = of_get_gpio_flags(node,i,&flag);
		if(my_gpio_key[i].gpio < 0)
		{
			printk("%s %s line %d, of_get_gpio_flags fail\n", __FILE__, __FUNCTION__, __LINE__);
			return -1;
		}

		//1.从设备树获取gpio
		my_gpio_key[i].gpiod = gpio_to_desc(my_gpio_key[i].gpio);
		my_gpio_key[i].flag = flag & OF_GPIO_ACTIVE_LOW;

		if(flag & OF_GPIO_ACTIVE_LOW)
		{
			flags |= GPIOF_ACTIVE_LOW;	
		}

		err = devm_gpio_request_one(&pdev->dev,my_gpio_key[i].gpio,flags,NULL);
		//2.从gpio获取中断号
		my_gpio_key[i].irq = gpio_to_irq(my_gpio_key[i].gpio);
	}

	for(i = 0;i<count;i++)
	{
		//3.申请中断
		err = request_irq(my_gpio_key[i].irq,gpio_ker_isr,IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING,"gpio_key",&my_gpio_key[i]);
	}
	return 0;
	
}

static int gpio_key_remove(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	int count;
	int i;

	count = of_gpio_count(node);
	for(i = 0;i<count;i++)
	{
		free_irq(my_gpio_key[i].irq,&my_gpio_key[i]);
	}
	kfree(my_gpio_key);
	return 0;
}

static const struct of_device_id keys[] = {
	{.compatible = "key,6ull_key"},
	{},
};

static struct platform_driver gpio_keys_driver = {
	.probe = gpio_key_probe,
	.remove = gpio_key_remove,
	.driver = {
		.name = "gpio_key",
		.of_match_table = keys,
	}
};

static int __init my_key_init(void)
{
	int err;
	int i;
	printk("%s %s  line %d\n"__FILE__,__FUNCTION__,__LINE__);
	platform_driver_register(&gpio_keys_driver);
	return 0;

}

static void __exit my_key_exit(void)
{
	int i;
	printk("%s %s  line %d\n"__FILE__,__FUNCTION__,__LINE__);
	//iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
	//iounmap(GPIO5_DR);
	//iounmap(GPIO5_GDIR);
	//for(i = 0; i < LED_NUM; i++)
	//{
	//	device_destroy(led_class,MKDEV(major, i));
	//}
	//class_destroy(led_class);
	//unregister_chrdev(major,"led");
	platform_driver_unregister(&gpio_keys_driver);

}

module_init(my_key_init);
module_exit(my_key_exit);
MODULE_LICENSE("GPL");




