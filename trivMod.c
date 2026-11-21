
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>

static dev_t trivMod_dev;
static int units = 1;
static struct cdev trivMod_cdev;
static struct class *trivMod_class;
static struct device *trivMod_device;

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

    trivMod_class = class_create("trivMod");

    if (IS_ERR(trivMod_class)) {
       ret = PTR_ERR(trivMod_class);
       cdev_del(&trivMod_cdev);
       unregister_chrdev_region(trivMod_dev, units);
       return ret;
    }


    trivMod_device = device_create(trivMod_class, NULL, trivMod_dev, NULL, "trivMod");

    if (IS_ERR(trivMod_device)) {
        ret = PTR_ERR(trivMod_device);
        class_destroy(trivMod_class);
        cdev_del(&trivMod_cdev);
        unregister_chrdev_region(trivMod_dev, units);
        return ret;
    }



    printk(KERN_INFO "Hello World\n");
    return 0;
}




static void __exit trivMod_exit(void)
{
    device_destroy(trivMod_class, trivMod_dev);
    class_destroy(trivMod_class);

    cdev_del(&trivMod_cdev);
    unregister_chrdev_region(trivMod_dev,units);

    printk(KERN_INFO "Bye Bye World\n");
}


module_init(trivMod_init);
module_exit(trivMod_exit);

MODULE_LICENSE("GPL");
