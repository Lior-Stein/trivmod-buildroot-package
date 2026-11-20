
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

static dev_t trivMod_dev;


static int __init trivMod_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&trivMod_dev, 1, 1, "trivMod");

    if (ret < 0)
        return ret;

    printk(KERN_INFO "Hello World\n");
    return 0;
}



static void __exit trivMod_exit(void)
{
    unregister_chrdev_region(trivMod_dev, 1);

    printk(KERN_INFO "Bye Bye World\n");
}


module_init(trivMod_init);
module_exit(trivMod_exit);

MODULE_LICENSE("GPL");
