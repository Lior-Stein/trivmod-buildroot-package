
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>

static dev_t trivMod_dev;
static int units = 1;


static struct trivMod_priv {
    struct cdev cdev;
    struct class *cl;
    struct device *device;
    int opens;
    int closes;
} trivMod_priv;



static int trivMod_open(struct inode *ino, struct file *filp)
{
    struct trivMod_priv *priv;

    priv = container_of(ino->i_cdev, struct trivMod_priv, cdev);
    filp->private_data = priv;
    priv->opens++;

    printk(KERN_INFO "trivMod: open count = %d\n", priv->opens);

    return 0;
}


static int trivMod_release(struct inode *ino, struct file *filp)
{
    struct trivMod_priv *priv = filp->private_data;

    if (priv != NULL) {
        priv->closes++;
        printk(KERN_INFO "trivMod: release count = %d\n", priv->closes);
        filp->private_data = NULL;
    }

    return 0;
}



static const struct file_operations trivMod_fops = {
    .owner = THIS_MODULE,
    .open = trivMod_open,
    .release = trivMod_release,
};


static int __init trivMod_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&trivMod_dev, 0, units, "trivMod");
    if (ret < 0)
        return ret;

    cdev_init(&trivMod_priv.cdev, &trivMod_fops);

    ret = cdev_add(&trivMod_priv.cdev, trivMod_dev, units);
    if (ret < 0) {
        unregister_chrdev_region(trivMod_dev, units);
        return ret;
    }

    trivMod_priv.cl = class_create("trivMod");

    if (IS_ERR(trivMod_priv.cl)) {
       ret = PTR_ERR(trivMod_priv.cl);
       cdev_del(&trivMod_priv.cdev);
       unregister_chrdev_region(trivMod_dev, units);
       return ret;
    }


    trivMod_priv.device = device_create(trivMod_priv.cl, NULL, trivMod_dev, NULL, "trivMod");

    if (IS_ERR(trivMod_priv.device)) {
        ret = PTR_ERR(trivMod_priv.device);
        class_destroy(trivMod_priv.cl);
        cdev_del(&trivMod_priv.cdev);
        unregister_chrdev_region(trivMod_dev, units);
        return ret;
    }

    trivMod_priv.opens = 0;
    trivMod_priv.closes = 0;



    printk(KERN_INFO "Hello World\n");
    return 0;
}




static void __exit trivMod_exit(void)
{
    device_destroy(trivMod_priv.cl, trivMod_dev);
    class_destroy(trivMod_priv.cl);

    cdev_del(&trivMod_priv.cdev);
    unregister_chrdev_region(trivMod_dev,units);

    printk(KERN_INFO "Bye Bye World\n");
}


module_init(trivMod_init);
module_exit(trivMod_exit);

MODULE_LICENSE("GPL");
