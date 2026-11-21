
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static dev_t trivMod_dev;
static int units = 1;
static struct cdev trivMod_cdev;

static const struct file_operations trivMod_fops = {
    .owner = THIS_MODULE,
};


static int __init trivMod_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&trivMod_dev, 0, units, "trivMod");
    if (ret < 0)
        return ret;

    cdev_init(&trivMod_cdev, &trivMod_fops);

    ret = cdev_add(&trivMod_cdev, trivMod_dev, units);
    if (ret < 0) {
        unregister_chrdev_region(trivMod_dev, units);
        return ret;
    }

    printk(KERN_INFO "Hello World\n");
    return 0;
}




static void __exit trivMod_exit(void)
{
    cdev_del(&trivMod_cdev);
    unregister_chrdev_region(trivMod_dev,units);

    printk(KERN_INFO "Bye Bye World\n");
}


module_init(trivMod_init);
module_exit(trivMod_exit);

MODULE_LICENSE("GPL");
