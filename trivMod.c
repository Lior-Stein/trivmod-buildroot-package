
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include "trivMod_ioctl.h"

#define BUFFER_SIZE 4096

static dev_t trivMod_dev;
static int units = 1;


static struct trivMod_priv {
    struct cdev cdev;
    struct class *cl;
    struct device *device;
    struct trivMod_stats stats;

    char buffer[BUFFER_SIZE];
    size_t head;
    size_t tail;
} trivMod_priv;



static int trivMod_open(struct inode *ino, struct file *filp)
{
    struct trivMod_priv *priv;

    priv = container_of(ino->i_cdev, struct trivMod_priv, cdev);
    filp->private_data = priv;
    priv->stats.opens++;

    printk(KERN_INFO "trivMod: open count = %llu\n", priv->stats.opens);

    return 0;
}


static int trivMod_release(struct inode *ino, struct file *filp)
{
    struct trivMod_priv *priv = filp->private_data;

    if (priv != NULL) {
        priv->stats.closes++;
        printk(KERN_INFO "trivMod: release count = %llu\n", priv->stats.closes);
        filp->private_data = NULL;
    }

    return 0;
}


static ssize_t trivMod_write(struct file *filp,
                             const char __user *buff,
                             size_t count, loff_t *offp)
{
    struct trivMod_priv *priv = filp->private_data;
    size_t head = priv->head;
    size_t space;
    size_t first;

    if (count == 0)
        return 0;

    space = (priv->tail + BUFFER_SIZE - head - 1) % BUFFER_SIZE;

    if (space == 0)
        return -ENOSPC;

    count = min(count, space);

    first = min(count, BUFFER_SIZE - head);

    if (copy_from_user(priv->buffer + head, buff, first))
        return -EFAULT;

    if (count > first) {
        if (copy_from_user(priv->buffer, buff + first, count - first))
            return -EFAULT;
    }

    priv->head = (head + count) % BUFFER_SIZE;
   

    priv->stats.bytes_written += count;

    return count;
}


static ssize_t trivMod_read(struct file *filp, char __user *buf,
                            size_t count, loff_t *offp)
{
    struct trivMod_priv *priv = filp->private_data;
    size_t available;
    size_t first;

    available = (priv->head + BUFFER_SIZE - priv->tail) % BUFFER_SIZE;
    count = min(count, available);
    if (count == 0)
        return 0;

    first = min(count, BUFFER_SIZE - priv->tail);

    if (copy_to_user(buf, priv->buffer + priv->tail, first))
        return -EFAULT;

    if (count > first) {
        if (copy_to_user(buf + first, priv->buffer, count - first))
            return -EFAULT;
    }

    priv->tail = (priv->tail + count) % BUFFER_SIZE;
    

    priv->stats.bytes_read += count;

    return count;
}

static long trivMod_ioctl(struct file *filp, unsigned int cmd,
                          unsigned long arg)
{
    struct trivMod_priv *priv = filp->private_data;

    switch (cmd) {
    case TRIVMOD_STATSGET:
        if (copy_to_user((void __user *)arg, &priv->stats,
                         sizeof(priv->stats)))
            return -EFAULT;
        return 0;

    case TRIVMOD_STATSCLR:
        priv->stats = (struct trivMod_stats){0};
        return 0;

    default:
        return -EINVAL;
    }
}

static const struct file_operations trivMod_fops = {
    .owner = THIS_MODULE,
    .open = trivMod_open,
    .release = trivMod_release,
    .read    = trivMod_read,
    .write   = trivMod_write,
    .unlocked_ioctl = trivMod_ioctl,
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
        goto err_unregister_region;
    }

    trivMod_priv.cl = class_create("trivMod");

    if (IS_ERR(trivMod_priv.cl)) {
       ret = PTR_ERR(trivMod_priv.cl);
       goto err_del_cdev;
    }


    trivMod_priv.device = device_create(trivMod_priv.cl, NULL, trivMod_dev, NULL, "trivMod");

    if (IS_ERR(trivMod_priv.device)) {
        ret = PTR_ERR(trivMod_priv.device);
        goto err_destroy_class;
    }

    trivMod_priv.head = 0;
    trivMod_priv.tail = 0;



    printk(KERN_INFO "Hello World\n");
    return 0;

    err_destroy_class:
    class_destroy(trivMod_priv.cl);
    err_del_cdev:
        cdev_del(&trivMod_priv.cdev);
    err_unregister_region:
        unregister_chrdev_region(trivMod_dev, units);
        return ret;
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
