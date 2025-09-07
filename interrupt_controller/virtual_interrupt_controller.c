
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/irqdesc.h>
#include <linux/irqdomain.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/random.h>
#include <linux/irqchip/chained_irq.h>

#define DRIVER_NAME "virtual-interrupt-controller"
#define VINTC_BASE 32
#define VINTC_COUNT 4

static struct irq_domain *virtual_intc_domain;

static int virtual_intc_get_hwirq(void)
{
	return get_random_int() & 0x3;
}

static void virtual_intc_irq_ack(struct irq_data *data)
{
	pr_debug("VINTC: ack irq %d\n", data->irq);
}

static void virtual_intc_irq_mask(struct irq_data *data)
{
	pr_debug("VINTC: mask irq %d\n", data->irq);
}

static void virtual_intc_irq_mask_ack(struct irq_data *data)
{
	pr_debug("VINTC: mask_ack irq %d\n", data->irq);
}

static void virtual_intc_irq_unmask(struct irq_data *data)
{
	pr_debug("VINTC: unmask irq %d\n", data->irq);
}

static void virtual_intc_irq_eoi(struct irq_data *data)
{
	pr_debug("VINTC: eoi irq %d\n", data->irq);
}

static struct irq_chip virtual_intc_chip = {
	.name		= "VINTC",
	.irq_ack	= virtual_intc_irq_ack,
	.irq_mask	= virtual_intc_irq_mask,
    .irq_mask_ack   = virtual_intc_irq_mask_ack,
	.irq_unmask	    = virtual_intc_irq_unmask,
    .irq_eoi     = virtual_intc_irq_eoi,
};

static int vintc_domain_map(struct irq_domain *d, unsigned int vintc,
			irq_hw_number_t hw)
{
	irq_set_chip_and_handler(vintc, &virtual_intc_chip, handle_level_irq);
	irq_set_chip_data(vintc, d->host_data);
	return 0;
}

static const struct irq_domain_ops vintc_domain_ops = {
	.map = vintc_domain_map,
	.xlate = irq_domain_xlate_twocell,
};

static void virtual_intc_irq_handler(struct irq_desc *desc)
{
	struct irq_chip *chip = irq_desc_get_chip(desc);
	unsigned int virq;
/* 它的功能时分辨是哪一个hwirq, 调用对应的irq_desc[].handle_irq */
	int hwirq;

	chained_irq_enter(chip, desc);

	/* a. 分辨中断 */
	hwirq = virtual_intc_get_hwirq();
	virq = irq_find_mapping(virtual_intc_domain, hwirq);
	if (virq)
		generic_handle_irq(virq);

	chained_irq_exit(chip, desc);

}

static int virtual_intc_probe(struct platform_device *pdev)
{
    struct device_node *np = pdev->dev.of_node;

    int irq_to_parent;
    int irq_base;

    //1. 分配/设置/注册irq_domain
    irq_to_parent = platform_get_irq(pdev, 0);
    printk("irq_to_parent = %d\n", irq_to_parent);

    //2. 设置irq_desc
    irq_set_chained_handler_and_data(irq_to_parent, virtual_intc_irq_handler, NULL);

    //3. 申请中断
    irq_base = irq_alloc_descs(-1,0,4,numa_node_id());
    printk("virtual_intc_probe irq_base = %d\n", irq_base);

    virtual_intc_domain =  irq_domain_add_legacy(np, VINTC_COUNT, irq_base, 0,
				       &vintc_domain_ops, NULL);

    return 0;

}

static int virtual_intc_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id virtual_intc_of_match[] = {
	{ .compatible = "huazi,virtual-irq-controller" },
	{},
};

static struct platform_driver virtual_intc_driver = {
	.probe = virtual_intc_probe,
	.remove = virtual_intc_remove,
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = virtual_intc_of_match,
	},
};


/* 1. 入口函数 */
static int __init virtual_intc_init(void)
{	
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* 1.1 注册一个platform_driver */
	return platform_driver_register(&virtual_intc_driver);
}

/**
 * @description	: 出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit virtual_intc_exit(void)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* 1.2 注销platform_driver */
	platform_driver_unregister(&virtual_intc_driver);
}

module_init(virtual_intc_init);
module_exit(virtual_intc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yin guang");
MODULE_DESCRIPTION("Virtual Interrupt Controller Driver");
MODULE_VERSION("1.0");