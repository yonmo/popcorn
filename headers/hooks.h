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


#else
// Define linux mkdir function call, for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_mkdir)(const char __user *pathname, umode_t mode);
asmlinkage int pn_mkdir(const char __user *pathname, umode_t mode);
// Define linux kill function call, for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_kill)(pid_t pid, int sig);
asmlinkage int pn_kill(pid_t pid, int sig);
// Define linux rmdir function call, for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_rmdir)(const char __user *pathname)
asmlinkage int pn_rmdir(const char __user *pathname);

#endif

#endif