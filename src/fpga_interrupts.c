#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/spinlock_types.h>
#include <linux/uaccess.h>
#include <linux/wait.h>

// https://gist.github.com/vrbadev/0850428601283986f27221f16ee5ceba

MODULE_AUTHOR("Joshua Estes");
MODULE_DESCRIPTION("FPGA Interrupt Driver");
MODULE_VERSION("0.1");

#define DEV_NAME "fpga_interrupts"

static irq_handler_t __isr(int irq, void *dev_id)
{
	printk(KERN_INFO DEV_NAME ": ISR\n");
	return (irq_handler_t)IRQ_HANDLED;
}

static int __test_int_driver_probe(struct platform_device *pdev)
{
	int irq_num = -1;

	irq_num = platform_get_irq(pdev, 0);
	printk(KERN_INFO DEV_NAME ": IRQ Number: %d\n", irq_num);
	return request_irq(irq_num, (irq_handler_t)__isr, IRQF_TRIGGER_NONE, DEV_NAME, NULL);
}

static int __test_int_driver_remove(struct platform_device *pdev)
{
	int irq_num = -1;

	irq_num = platform_get_irq(pdev, 0);
	printk(KERN_INFO DEV_NAME ": Removing IRQ Number: %d\n", irq_num);
	free_irq(irq_num, NULL);

	return 0;
}

static const struct of_device_id __test_int_driver_id[] = {
	{ .compatible = "altr,fpga_interrupts" },
	{}
};

static const struct platform_driver __test_int_driver = {
	.driver = {
		.name = DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(__test_int_driver_id),
	},
	.probe = __test_int_driver_probe,
	.remove = __test_int_driver_remove,
};

module_platform_driver(__test_int_driver);

MODULE_LICENSE("GPL");