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

MODULE_AUTHOR("Joshua Estes");
MODULE_DESCRIPTION("FPGA Interrupt Driver");
MODULE_VERSION("0.1");

irqreturn_t fpga_interrupt_handler(int irq, void *dev_id, struct pt_regs *regs)
{
    pr_info("Interrupt\n");
    return IRQ_HANDLED;
}

static int __init irq_ex_init(void)
{
    pr_info("Requesting IRQ LN 72\n");

    free_irq(72, NULL);
    return request_irq(72, fpga_interrupt_handler, IRQF_TRIGGER_RISING, "fpga_interrupt", NULL);
}

static void __exit irq_ex_exit(void)
{
    pr_info("Freeing IRQ LN 72\n");
    free_irq(72, NULL);
}

module_init(irq_ex_init);
module_exit(request_irq);

MODULE_LICENSE("GPL");