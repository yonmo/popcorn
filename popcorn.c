#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yonmo");
MODULE_DESCRIPTION("Popcorn LKM");
MODULE_VERSION("0.01");

#include "headers/ftrace_helper.h"

#include "src/credentials.c"
#include "src/hooks.c"

#if !defined(CONFIG_X86_64) || (LINUX_VERSION_CODE < KERNEL_VERSION(4,17,0))
#define VERSION_NOT_SUPPORTED
#endif

static void __exit popcorn_exit(void) {
    printk(KERN_INFO "Kernels get popped eventually...\n");
    fh_remove_hooks(kernel_hooks, ARRAY_SIZE(kernel_hooks));
}

static int __init popcorn_init(void) {
    int err;
    #ifdef VERSION_NOT_SUPPORTED
        printk(KERN_INFO "Kernels left unpopped...\n");
        return -1;
    #endif
    err = fh_install_hooks(kernel_hooks, ARRAY_SIZE(kernel_hooks));
    if (err)
        return err;
    printk(KERN_INFO "Popcorns have kernels!\n");
	return 0;
}

module_init(popcorn_init);
module_exit(popcorn_exit);