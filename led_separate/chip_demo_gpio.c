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
#include "led_opr.h"
#include "led_resource.h"

#include <asm/io.h>

static volatile unsigned int *CCM_CCGR1;
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
static volatile unsigned int *GPIO5_GDIR;
static volatile unsigned int *GPIO5_DR;

static struct led_resource *led_resource_s;
static int board_demo_led_init(int which)
{
	if(!led_resource_s)
	{
		led_resource_s = get_led_resource();
	}
	switch (GROUP(led_resource_s->pin)) {
		case 0:
			printk("led init group %d pin %d \n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
		break;
		case 1:
			printk("led init group %d pin %d \n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
		break;
		case 2:
			printk("led init group %d pin %d \n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
		break;
		case 3:
			printk("led init group %d pin %d \n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
		break;
		default:
			printk("led init group %d pin %d \n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
		break;
	
	}
	return 0;
}

static int board_demo_led_ctl(int which, char status)
{
	printk("led ctl");
	switch (GROUP(led_resource_s->pin)) {
		case 0:
			if(status)
			{
				printk("led set group %d pin %d on\n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
			}
			else {
				printk("led set group %d pin %d off\n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
			}
		break;
		case 1:
			if(status)
			{
				printk("led set group %d pin %d on\n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
			}
			else {
				printk("led set group %d pin %d off\n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
			}
		break;
		case 2:
			if(status)
			{
				printk("led set group %d pin %d on\n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
			}
			else {
				printk("led set group %d pin %d off\n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
			}
		break;
		case 3:
			if(status)
			{
				printk("led set group %d pin %d on\n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
			}
			else {
				printk("led set group %d pin %d off\n",GROUP(led_resource_s->pin),PIN(led_resource_s->pin));
			}
		break;
		default:
			printk("there is no led handware!\n");
		break;
	
	}

	return 0;
}

static struct led_operations board_demo_led_opr = {
	.num = 1,
	.init = board_demo_led_init,
	.ctl = board_demo_led_ctl,
};

struct led_operations *get_board_led_opr(void)
{
	return &board_demo_led_opr;
}
