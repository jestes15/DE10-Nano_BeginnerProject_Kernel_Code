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
// Memory Mapping: https://linux-kernel-labs.github.io/refs/heads/master/labs/memory_mapping.html
// Interrupts:
// https://linux-kernel-labs.github.io/refs/heads/master/lectures/interrupts.html#interrupt-handling-in-linux

MODULE_AUTHOR("Joshua Estes");
MODULE_DESCRIPTION("FPGA Interrupt Driver");
MODULE_VERSION("0.1");

#define DEVNAME "fpga_interrupts"

static uint32_t irq_status;

static ssize_t irq_status_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t irq_status_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t len);

static DEFINE_SEMAPHORE(interrupt_mutex);
static DEFINE_SPINLOCK(interrupt_flag_lock);
static DEVICE_ATTR_RW(irq_status);

static ssize_t irq_status_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    spin_lock(&interrupt_flag_lock);
    buf[0] = irq_status;
    buf[1] = '\n';
    buf[2] = '\0';
    spin_unlock(&interrupt_flag_lock);

    return 3;
}

static ssize_t irq_status_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t len)
{
    return -EROFS;
}

static irq_handler_t __fpga_interrupts_driver_isr(int irq, void *kobj)
{
    sysfs_notify((struct kobject *)kobj, NULL, "irq_status");

    printk(KERN_INFO "%s: IRQ %d triggered!\n", DEVNAME, irq);

    return (irq_handler_t)IRQ_HANDLED;
}

static int __fpga_interrupts_driver_probe(struct platform_device *pdev)
{
    int ret;
    int irq_num;

    ret = device_create_file(&pdev->dev, &dev_attr_irq_status);
    if (ret)
    {
        printk(KERN_ALERT "%s: Error %d when executing device_create_file in function %s!\n", DEVNAME, ret,
               __FUNCTION__);
        return ret;
    }

    irq_num = platform_get_irq_optional(pdev, 0);
    printk(KERN_INFO "%s: IRQ %d about to be registered!\n", DEVNAME, irq_num);

    ret = request_irq(irq_num, (irq_handler_t)__fpga_interrupts_driver_isr, 0, DEVNAME, &pdev->dev.kobj);

    if (ret < 0)
    {
        printk(KERN_ALERT "%s: Error %d when executing request_irq (irq_num=%d) "
                          "in function %s!\n",
               DEVNAME, ret, irq_num, __FUNCTION__);
        return ret;
    }

    return 0;
}

static int __fpga_interrupts_driver_remove(struct platform_device *pdev)
{
    int irq_num = platform_get_irq_optional(pdev, 0);

    if (irq_num < 0)
    {
        printk(KERN_INFO "%s: IRQ %d about to be freed!\n", DEVNAME, irq_num);
        free_irq(irq_num, &pdev->dev.kobj);
    }

    device_remove_file(&pdev->dev, &dev_attr_irq_status);

    return 0;
}

static const struct of_device_id __fpga_interrupts_driver_id[] = {{.compatible = "altr,fpga_interrupts"}, {}};

static struct platform_driver __fpga_interrupts_driver = {
    .driver =
        {
            .name = DEVNAME,
            .owner = THIS_MODULE,
            .of_match_table = of_match_ptr(__fpga_interrupts_driver_id),
        },
    .probe = __fpga_interrupts_driver_probe,
    .remove = __fpga_interrupts_driver_remove};

module_platform_driver(__fpga_interrupts_driver);

MODULE_LICENSE("GPL");