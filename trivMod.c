
#include <linux/module.h>
#include <linux/kernel.h>

static int __init trivMod_init(void)
{
    printk(KERN_INFO "Hello World\n");
    return 0;
}

static void __exit trivMod_exit(void)
{
    printk(KERN_INFO "Bye Bye World\n");
}

module_init(trivMod_init);
module_exit(trivMod_exit);

MODULE_LICENSE("GPL");
