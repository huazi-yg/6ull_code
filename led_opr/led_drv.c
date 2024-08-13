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
#include <asm/io.h>

#include "led_opr.h"

#define LED_NUM 2
//1.确定主设备号
static int major = 0;
static struct class *led_class;
struct led_operations *p_led_opr;

#define MIN(a,b) (a < b ? a : b)

//0x02290014
volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
//020A_C000
volatile unsigned int *GPIO5_DR;
//020A_C000 + 4h offset
volatile unsigned int *GPIO5_GDIR;

static ssize_t led_read(struct file *file,char __user *buf,size_t size,loff_t *offset)
{
	return 0;
}

static ssize_t led_write(struct file *file,const char __user *buf,size_t size,loff_t *offset)
{
        int err;
	char status;
	struct inode *inode = file_inode(file);
	int minor = iminor(inode);

	int ret = copy_from_user(&status,buf,1);
        //根据次设备号控制led
	p_led_opr->ctl(minor,status);

	return 1;
}

static int led_open(struct inode *node, struct file *file)
{
	// printk("%s %s  line %d\n"__FILE__,__FUNCTION__,__LINE__);
	// return 0;
	//*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0x0F;
	//*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x05;
	//*GPIO5_GDIR |=0x08;
	int minor = iminor(node);

	p_led_opr->init(minor);
	return 0;//必须要返回值，不然会报错
}

static int led_close(struct inode *node, struct file *file)
{
	// printk("%s %s  line %d\n"__FILE__,__FUNCTION__,__LINE__);
	 return 0;
}

//定义自己的file_operations结构体

static struct file_operations led = {
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_close,
	.write = led_write,
	.read = led_read,
};

static int __init led_init(void)
{
	int err;
	int i;
	printk("%s %s  line %d\n"__FILE__,__FUNCTION__,__LINE__);
	major = register_chrdev(0,"led",&led);//注册字符设备
	
//	IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x02290000+0x14,4);
//	GPIO5_GDIR = ioremap(0x020ac000+0x04,4);
//	GPIO5_DR = ioremap(0x020AC000,4);

	led_class = class_create(THIS_MODULE, "led_class");
	err = PTR_ERR(led_class);
	if(IS_ERR(led_class))
		{
			printk("%s %s  line %d\n"__FILE__,__FUNCTION__,__LINE__);
			unregister_chrdev(major,"led");
			return -1;
		}
	for (i = 0; i < LED_NUM ; i++)
	{
		device_create(led_class,NULL,MKDEV(major,i),NULL,"led");//dev/hello
	}
	p_led_opr = get_board_led_opr();
	return 0;

}

static void __exit led_exit(void)
{
	int i;
	printk("%s %s  line %d\n"__FILE__,__FUNCTION__,__LINE__);
	//iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
	//iounmap(GPIO5_DR);
	//iounmap(GPIO5_GDIR);
	for(i = 0; i < LED_NUM; i++)
	{
		device_destroy(led_class,MKDEV(major, i));
	}
	class_destroy(led_class);
	unregister_chrdev(major,"led");

}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");




