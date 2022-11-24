#ifndef HEADER_HOOKS_MANAGER
#define HEADER_HOOKS_MANAGER

#include <linux/module.h>
#include <linux/kernel.h>

#ifdef PTREGS_SYSCALL_STUBS
// Define linux mkdir function call, for "pt-regs" versions of the kernel
static asmlinkage long (*lx_mkdir)(const struct pt_regs *);
asmlinkage int pn_mkdir(const struct pt_regs *regs);

#else
// Define linux mkdir function call, for non-"pt-regs" versions of the kernel
static asmlinkage long (*lx_mkdir)(const char __user *pathname, umode_t mode);
asmlinkage int pn_mkdir(const char __user *pathname, umode_t mode);

#endif

#endif