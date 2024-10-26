#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <linux/spinlock_types.h>
#include <linux/uaccess.h>
#include <linux/irq.h>
#include <linux/gpio.h>

MODULE_AUTHOR("Joshua Estes");
MODULE_DESCRIPTION("FPGA Interrupt Driver");
MODULE_VERSION("0.1");

irqreturn_t irq_handler(int irq) {}

static int __init hello_2_init(void)
{
	pr_info("Hello, world 2\n");
	return 0;
}

static void __exit hello_2_exit(void)
{
	pr_info("Goodbye, world 2\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");