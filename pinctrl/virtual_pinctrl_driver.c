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
#include "core.h"

static struct pinctrl_dev *g_pinctrl_dev;

static const struct pinctrl_pin_desc virtual_pinctrl_pins[] = {
    {0,"pin0",NULL},
    {1,"pin1",NULL},
    {2,"pin2",NULL},
    {3,"pin3",NULL},
};

static unsigned long g_configs[4];

static const char *function_names[] = {"gpio","i2c","uart"};

struct virtual_function_desc  {
    const char *func_name;
    const char **groups;
    int num_groups;
};

static const char * func0_groups[] = {"pin0","pin1","pin2","pin3"};
static const char * func1_groups[] = {"pin0","pin1"};
static const char * func2_groups[] = {"pin2","pin3"};
static const struct virtual_function_desc g_virtual_function_descs[] = {
    {"gpio",func0_groups,4},
    {"i2c",func1_groups,2},
    {"uart",func2_groups,2},
};

static int virtual_pinctrl_get_groups_count(struct pinctrl_dev *pctl)
{
    return pctl->desc->npins;
}

static const char *virtual_pinctrl_get_group_name (struct pinctrl_dev *pctl,
				       unsigned selector){
    return pctl->desc->pins[selector].name;
}

static int virtual_pinctrl_get_group_pins(struct pinctrl_dev *pctl, unsigned selector, const unsigned **pins, unsigned *num_pins)
{

    if(selector >= pctl->desc->npins) {
        printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
        return -EINVAL;
    }
    *pins = &pctl->desc->pins[selector].number;
    *num_pins = 1;
    return 0;
}

static int virtual_pinctrl_dt_node_to_map(struct pinctrl_dev *pctl, struct device_node *np, struct pinctrl_map **map, unsigned *num_maps)
{
    //确定引脚个数
    int pin_nums = 0;
    int i;
    const char *pin;
    const char *func;
    unsigned int config;
    unsigned long *configs;

    struct pinctrl_map *pinctrl_map;
    //
    while(1)
    {
        if(of_property_read_string_index(np, "groups", pin_nums, &pin) == 0)
        {
            pin_nums++;
        }
        else
        {
            break;
        }
        //存入pinctrl_map

    }
    //存入pinctrl_map
    pinctrl_map = kmalloc(sizeof(struct pinctrl_map) * pin_nums * 2,  GFP_KERNEL);
    if(!pinctrl_map)
    {
        printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
        return -ENOMEM;
    }

    for(i = 0; i < pin_nums; i++)
    {
        //get pin/func/config
        of_property_read_string_index(np, "groups", i, &pin); 
        of_property_read_string_index(np, "functions", i, &func);

        of_property_read_u32_index(np, "configs", i, &config);

        configs = kzalloc(sizeof(configs), GFP_KERNEL);
        if(!configs)
        {
            printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
            return -ENOMEM;
        }
        

        pinctrl_map[i*2].type = PIN_MAP_TYPE_MUX_GROUP;
        pinctrl_map[i*2].data.mux.group = pin;
        pinctrl_map[i*2].data.mux.function = func;

        pinctrl_map[i*2+1].type = PIN_MAP_TYPE_CONFIGS_PIN;
        pinctrl_map[i*2+1].data.configs.group_or_pin = pin;
        pinctrl_map[i*2+1].data.configs.configs = configs;
        configs[0] = config;
        pinctrl_map[i*2+1].data.configs.num_configs = 1;
        
    }

    *map = pinctrl_map;
    *num_maps = pin_nums * 2;
    return 0;
    
    
}

static void virtual_pinctrl_dt_free_map(struct pinctrl_dev *pctl, struct pinctrl_map *map, unsigned num_maps)
{
    while(num_maps--)
    {
        if(map->type == PIN_MAP_TYPE_CONFIGS_GROUP)
        {
            kfree(map->data.configs.configs);
        }
        kfree(map);
        map++;
    }
}

static void virtual_pinctrl_pin_dbg_show(struct pinctrl_dev *pctl, struct seq_file *s, unsigned offset)
{
    seq_printf(s, "%s\n", dev_name(pctl->dev));
}

const struct pinctrl_ops pinctrl_ops = {
    .get_groups_count = virtual_pinctrl_get_groups_count,
    .get_group_name = virtual_pinctrl_get_group_name,
    .get_group_pins = virtual_pinctrl_get_group_pins,
    .pin_dbg_show = virtual_pinctrl_pin_dbg_show,
    .dt_node_to_map = virtual_pinctrl_dt_node_to_map,
    .dt_free_map = virtual_pinctrl_dt_free_map,
};

static const struct of_device_id virtual_pinctrl_of_match[] = {
    { .compatible = "virtual_pinctrl", },
    { },
};

static int virtual_pinctrl_get_functions_count(struct pinctrl_dev *pctl)
{
    return ARRAY_SIZE(g_virtual_function_descs);
}
static const char *virtual_pinctrl_get_function_name (struct pinctrl_dev *pctldev,
					  unsigned selector)
{
    if(selector >= ARRAY_SIZE(function_names)) {
        printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
        return NULL;
    }
    return g_virtual_function_descs[selector].func_name;;
}

static int virtual_pinctrl_get_function_groups(struct pinctrl_dev *pctl, unsigned selector, const char * const **groups, unsigned * const num_groups)
{
    *groups = g_virtual_function_descs[selector].groups;
    *num_groups = g_virtual_function_descs[selector].num_groups;
    return 0;
}

static int virtual_pinctrl_set_mux(struct pinctrl_dev *pctl, unsigned selector, unsigned group)
{
    printk("Set %s as %s\n",pctl->desc->pins[group].name,g_virtual_function_descs[selector].func_name);
    return 0;
}


static const struct pinmux_ops virtual_pinmux_ops = {
    .get_functions_count = virtual_pinctrl_get_functions_count,
    .get_function_name = virtual_pinctrl_get_function_name,
    .get_function_groups = virtual_pinctrl_get_function_groups,
    .set_mux = virtual_pinctrl_set_mux,
};

static int virtual_pinctrl_pin_config_get(struct pinctrl_dev *pctl, unsigned pin, unsigned long *config)
{
    *config = g_configs[pin];
    return 0;

}

static int virtual_pinctrl_pin_config_set(struct pinctrl_dev *pctl, unsigned pin, unsigned long * configs,unsigned number_configs)
{
    if(number_configs != 1) 
    {
        printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
        return -EINVAL;
    }
    g_configs[pin] = *configs;
    printk("config %s as 0x%lx\n",pctl->desc->pins[pin].name,*configs);
    return 0;
}   

static void virtual_pinctrl_pin_config_dbg_show(struct pinctrl_dev *pctl, struct seq_file *s, unsigned pin_id)
{
    seq_printf(s, "0x%lx\n", g_configs[pin_id]); 
}

static void virtual_pinctrl_pin_config_group_dbg_show(struct pinctrl_dev *pctl, struct seq_file *s, unsigned pin_id)
{
    seq_printf(s, "0x%lx\n", g_configs[pin_id]); 
}   




static const struct pinconf_ops virtual_pinconf_ops = {
    .pin_config_get = virtual_pinctrl_pin_config_get,
    .pin_config_set = virtual_pinctrl_pin_config_set,
    .pin_config_dbg_show = virtual_pinctrl_pin_config_dbg_show,
    .pin_config_group_dbg_show = virtual_pinctrl_pin_config_group_dbg_show,
};

static int virtual_pinctrl_probe(struct platform_device *pdev)
{
    
    struct pinctrl_desc *pinctrl_desc;
    
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);

    //分配pinctrl_desc
    pinctrl_desc = devm_kzalloc(&pdev->dev, sizeof(*pinctrl_desc), GFP_KERNEL);
    if (!pinctrl_desc) {
        printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
        return -ENOMEM;
    }

    //设置pinctrl_desc

    pinctrl_desc->name = dev_name(&pdev->dev);
    pinctrl_desc->owner = THIS_MODULE;
    //设置pinctrl_desc的pins and groups
    pinctrl_desc->pins = virtual_pinctrl_pins;
    pinctrl_desc->npins = ARRAY_SIZE(virtual_pinctrl_pins);

    pinctrl_desc->pctlops = &pinctrl_ops;
    //设置pinctrl_desc的 pin mux
    pinctrl_desc->pmxops = &virtual_pinmux_ops;
    //设置pinctrl_desc的 pin config
    pinctrl_desc->confops = &virtual_pinconf_ops;

    //注册pinctrl_desc

    g_pinctrl_dev = devm_pinctrl_register(&pdev->dev, pinctrl_desc, NULL);
    

    return 0;
}
static int virtual_pinctrl_remove(struct platform_device *pdev)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}


static struct platform_driver virtual_pinctrl_driver = {
    .probe = virtual_pinctrl_probe,
    .remove = virtual_pinctrl_remove,
    .driver = {
        .name = "virtual_pinctrl",
        .of_match_table = virtual_pinctrl_of_match,
    },
};
//1. 入口函数
static int __init virtual_pinctrl_init(void)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    return platform_driver_register(&virtual_pinctrl_driver);
}
//2. 出口函数
static void __exit virtual_pinctrl_exit(void)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    platform_driver_unregister(&virtual_pinctrl_driver);
}

module_init(virtual_pinctrl_init);
module_exit(virtual_pinctrl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yg <huazi_ss@163.com>");
MODULE_DESCRIPTION("virtual pinctrl driver");


