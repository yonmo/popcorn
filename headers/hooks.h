#ifndef HEADER_HOOKS_MANAGER
#define HEADER_HOOKS_MANAGER

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>

#ifdef PTREGS_SYSCALL_STUBS
// Define linux mkdir function call, for "pt-regs" versions of the kernel
static asmlinkage long (*lx_mkdir)(const struct pt_regs *);
asmlinkage int pn_mkdir(const struct pt_regs *regs);
// Define linux kill function call, for "pt-regs" versions of the kernel
static asmlinkage long (*lx_kill)(const struct pt_regs *);
asmlinkage int pn_kill(const struct pt_regs *regs);
// Define linux rmdir function call, for "pt-regs" versions of the kernel
static asmlinkage long (*lx_rmdir)(const struct pt_regs *);
asmlinkage int pn_rmdir(const struct pt_regs *regs);
// Define linux reboot function call, for "pt-regs" versions of the kernel
static asmlinkage long (*lx_reboot)(const struct pt_regs *);
asmlinkage int pn_reboot(const struct pt_regs *regs);
// Define linux unlinkat function call for "pt-regs" versions of the kernel
static asmlinkage long (*lx_unlinkat)(const struct pt_regs *);
asmlinkage int pn_unlinkat(const struct pt_regs *regs);
// Define linux getdents64 function call for "pt-regs" versions of the kernel
static asmlinkage long (*lx_getdents64)(const struct pt_regs *);
asmlinkage int pn_getdents64(const struct pt_regs *regs);
// Define linux openat function call for "pt-regs" versions of the kernel
static asmlinkage long (*lx_openat)(const struct pt_regs *);
asmlinkage int pn_openat(const struct pt_regs *regs);
// Define linux read function call for "pt-regs" versions of the kernel
static asmlinkage long (*lx_read)(const struct pt_regs *);
asmlinkage int pn_read(const struct pt_regs *regs);



#else
// Define linux mkdir function call, for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_mkdir)(const char __user *pathname, umode_t mode);
asmlinkage int pn_mkdir(const char __user *pathname, umode_t mode);
// Define linux kill function call, for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_kill)(pid_t pid, int sig);
asmlinkage int pn_kill(pid_t pid, int sig);
// Define linux rmdir function call, for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_rmdir)(const char __user *pathname);
asmlinkage int pn_rmdir(const char __user *pathname);
// Define linux reboot function call for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_reboot)(int magic1, int magic2, unsigned int cmd, void __user *arg);
asmlinkage int pn_reboot(int magic1, int magic2, unsigned int cmd, void __user *arg);
// Define linux unlinkat function call for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_unlinkat)(int dfd, const char __user * pathname, int flag);
asmlinkage int pn_unlinkat(int dfd, const char __user *pathname, int flag);
// Define linux getdents64 function call for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_getdents64)(unsigned int fd, struct linux_dirent64 __user *dirent, unsigned int count);
asmlinkage int pn_getdents64(unsigned int fd, struct linux_dirent64 __user *dirent, unsigned int count);
// Define linux openat function call for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_openat)(int dfd, const char __user *filename, int flags, umode_t mode);
asmlinkage int pn_openat(int dfd, const char __user *filename, int flags, umode_t mode);
// Define linux read function call for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_read)(unsigned int fd, char __user *buf, size_t count);
asmlinkage int pn_read(unsigned int fd, char __user *buf, size_t count);

#endif

#endif