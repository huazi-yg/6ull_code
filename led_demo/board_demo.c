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

#include <asm/io.h>

static volatile unsigned int *CCM_CCGR1;
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
static volatile unsigned int *GPIO5_GDIR;
static volatile unsigned int *GPIO5_DR;

static int board_demo_led_init(int which)
{
	printk("led int");
	unsigned int val;
	if(which == 0){
		if(!CCM_CCGR1)
		{
			CCM_CCGR1 = ioremap(0x20c406c,4);
			IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290014,4);
			GPIO5_GDIR = ioremap(0x020AC000 + 0x04,4);
			GPIO5_DR = ioremap(0x020AC000,4);
		}
		//使能
		*CCM_CCGR1 |= (3<<30);

		val = *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
		val &= ~(0xf);
		val |= (5);
		*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = val;

		*GPIO5_GDIR |= (1<<3);
	}
	else {
	
	}
	return 0;
}

static int board_demo_led_ctl(int which, char status)
{
	printk("led ctl");
	if(which == 0){
		if(status)
		{
			*GPIO5_DR &= ~(1<<3);
		}
		else {
			*GPIO5_DR |= (1<<3);
		}
	}
	else {
	
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
