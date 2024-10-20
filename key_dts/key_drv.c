#include <linux/module.h>
#include <linux/poll.h>

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
	struct timer_list key_timer;
	struct tasklet_struct key_tasklet;
};

static struct gpio_key *my_gpio_key;

//异步通知
static struct fasync_struct *button_fasync;

//主设备号
static int major = 0;
static struct class *gpio_key_class;

//环形缓冲begin
#define BUF_LEN 128
static int g_keys[BUF_LEN];
static int r,w;
#define NEXT_POS(x) ((x+1)%BUF_LEN)
static int is_key_buf_empty(void)
{
	return (r == w);
}

static int is_key_buf_full(void)
{
	return (r == NEXT_POS(w));
}

static void put_key(int key)
{
	if(!is_key_buf_full())
	{
		g_keys[w] = key;
		w= NEXT_POS(w);
	}
}


static int get_key(void)
{
		int key = 0;
		if(!is_key_buf_empty())
		{
			key = g_keys[r];
			r = NEXT_POS(r);
		}
		return key;

}
//环形缓冲end

static DECLARE_WAIT_QUEUE_HEAD(gpio_key_wait);

static void key_timer_expire(unsigned long data)
{
	struct gpio_key *gpio_key = data;
	int val;
	int key;
	val = gpiod_get_value(gpio_key->gpiod);

	printk("key_timer_expire key %d %d\n",gpio_key->gpio,val);
	key = (gpio_key->gpio << 8) | val;
	put_key(key);
	wake_up_interruptible(&gpio_key_wait);
	kill_fasync(&button_fasync,SIGIO,POLL_IN);
}

static void key_tasklet_func(unsigned long data)
{
	struct gpio_key *gpio_key = data;
	int val;
	int key;

	val = gpiod_get_value(gpio_key->gpiod);

	printk("tasklet key %d %d\n",gpio_key->gpio,val);
}

//实现对应的结构体内的函数
static ssize_t gpio_key_drv_read(struct file *file,char __user *buf,size_t size,loff_t *offset)
{
	int err;
	int key;

	if(is_key_buf_empty() && file->f_flags & O_NONBLOCK)
	{
		return -EAGAIN;
	}

	wait_event_interruptible(gpio_key_wait, !is_key_buf_empty());
	key = get_key();
	err = copy_to_user(buf,&key,4);
	return 4;
}

static unsigned int gpio_key_drv_poll(struct file *file,poll_table * wait)
{
	printk("%s %s  line %d\n"__FILE__,__FUNCTION__,__LINE__);
	poll_wait(file,&gpio_key_wait,wait);
	return is_key_buf_empty() ? 0 :(POLLIN | POLLRDNORM);
}

static int gpio_key_drv_fasync(int fd,struct file *file,int on)
{
	if(fasync_helper(fd,file,on,&button_fasync) >= 0)
	{
		printk("fasync_helper ok\n");
		return 0;
	}
	else
	{
		printk("fasync_helper error\n");
		return -EIO;
	}
}

static struct file_operations gpio_key_ops = {
 .owner = THIS_MODULE,
 .read = gpio_key_drv_read,
 .poll = gpio_key_drv_poll,
 .fasync = gpio_key_drv_fasync,
};


//4. 中断处理函数
static irqreturn_t gpio_ker_isr(int irq,void *dev_id)
{
	struct gpio_key *gpio_key = dev_id;
	printk("gpio_key_isr key %d irq happened\n",gpio_key->gpio);
	tasklet_schedule(&gpio_key->key_tasklet);
	mod_timer(&gpio_key->key_timer,jiffies + HZ/50);

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

		//定时器
		setup_timer(&my_gpio_key[i].key_timer,key_timer_expire,&my_gpio_key[i]);
		my_gpio_key[i].key_timer.expires = ~0;
		add_timer(&my_gpio_key[i].key_timer);

		//tasklet
		tasklet_init(&my_gpio_key[i].key_tasklet,key_tasklet_func,&my_gpio_key[i]);
		
	}

	for(i = 0;i<count;i++)
	{
		//3.申请中断
		err = request_irq(my_gpio_key[i].irq,gpio_ker_isr,IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING,"gpio_key",&my_gpio_key[i]);
	}

	//注册fops
	major = register_chrdev(0,"my_gpio_key_class",&gpio_key_ops);

	gpio_key_class = class_create(THIS_MODULE,"my_gpio_key_class");
	if(IS_ERR(gpio_key_class))
	{
		//printk("%s %s line %d, of_get_gpio_flags fail\n", __FILE__, __FUNCTION__, __LINE__);
		//unregister_chrdev(major,my_gpio_key_class");
		
		unregister_chrdev(major,"my_gpio_key_class");
		return PTR_ERR(gpio_key_class);
		
	}

	device_create(gpio_key_class,NULL,MKDEV(major,0),NULL,"my_gpio_key");
	return 0;
	
}

static int gpio_key_remove(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	int count;
	int i;

	device_destroy(gpio_key_class,MKDEV(major, 0));
	class_destroy(gpio_key_class);
	unregister_chrdev(major,"my_gpio_key_class");

	count = of_gpio_count(node);
	for(i = 0;i<count;i++)
	{
		free_irq(my_gpio_key[i].irq,&my_gpio_key[i]);
		del_timer(&my_gpio_key[i].key_timer);
		tasklet_kill(&my_gpio_key[i].key_tasklet);
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




