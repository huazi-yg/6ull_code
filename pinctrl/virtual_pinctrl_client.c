
#include <linux/err.h>
#include <linux/io.h>
#include <linux/init.h>
#include <linux/mfd/syscon.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>

#include <linux/pinctrl/machine.h>
#include <linux/pinctrl/pinconf.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/pinmux.h>
#include <linux/slab.h>
#include <linux/regmap.h>

#include <linux/module.h>

static const struct of_device_id virtual_pinctrl_client_of_match[] = {
    { .compatible = "virtual_pinctrl_client", },
    { },
};

static int virtual_pinctrl_client_probe(struct platform_device *pdev)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

static int virtual_pinctrl_client_remove(struct platform_device *pdev)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

static struct platform_driver virtual_pinctrl_client_driver = {
    .probe = virtual_pinctrl_client_probe,
    .remove = virtual_pinctrl_client_remove,
    .driver = {
        .name = "virtual_pinctrl_client",
        .of_match_table = virtual_pinctrl_client_of_match,
    },
};

static int __init virtual_pinctrl_client_init(void)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    return platform_driver_register(&virtual_pinctrl_client_driver);
}

static void __exit virtual_pinctrl_client_exit(void)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    platform_driver_unregister(&virtual_pinctrl_client_driver);
}

module_init(virtual_pinctrl_client_init);
module_exit(virtual_pinctrl_client_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yg <huazi_ss@163.com>");
MODULE_DESCRIPTION("virtual pinctrl client");   