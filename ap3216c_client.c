#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/mod_devicetable.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>
#include <linux/property.h>
#include <linux/acpi.h>
#include <linux/i2c.h>
//#include <linux/nvmem_provider.h>
#include <linux/regmap.h>
#include <linux/pm_runtime.h>
#include <linux/gpio/consumer.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

static struct i2c_client *ap3216c_client;

#if 0
static int __init i2c_client_ap3216c_init(void)
{
	struct i2c_adapter *adapter;
	struct i2c_board_info info = {
		I2C_BOARD_INFO("ap3216c", 0x1e);
	};
	adapter = i2c_get_adapter(0);
	ap3216c_client = i2c_new_device(adapter,&info);
	i2c_put_adapter(adapter);
	printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	return 0;
}
#else

static const unsigned short normal_i2c[] = {0x1e,I2C_CLIENT_END};

static int __init i2c_client_ap3216c_init(void)
{
	struct i2c_adapter *adapter;
	struct i2c_board_info i2c_info;
	
	memset(&i2c_info,0,sizeof(struct i2c_board_info));
	strscpy(i2c_info.type,"ap3216c",sizeof(i2c_info.type));
	adapter = i2c_get_adapter(0);
	ap3216c_client = i2c_new_probed_device(adapter,&i2c_info, normal_i2c, NULL);
	i2c_put_adapter(adapter); 
	printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	return 0;
}

#endif

static void __exit i2c_client_ap3216c_exit(void)
{
	i2c_unregister_device(ap3216c_client);
}

module_init(i2c_client_ap3216c_init);
module_exit(i2c_client_ap3216c_exit);

MODULE_AUTHOR("yin guang");
MODULE_LICENSE("GPL");