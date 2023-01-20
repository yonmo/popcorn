#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yonmo");
MODULE_DESCRIPTION("Popcorn LKM");
MODULE_VERSION("0.01");

#if !defined(CONFIG_X86_64) || (LINUX_VERSION_CODE < KERNEL_VERSION(4,17,0))
#define VERSION_NOT_SUPPORTED
#endif

static void __exit popcorn_exit(void);

static int __init popcorn_init(void);

static int setup(void);

static int teardown(void);