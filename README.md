# small-goblin

Currently WIP, along with a LKM auto-compiler for ease of writing deployment infra across networks utilizing different kernel versions.

This is a LKM Rootkit leveraging system call hooks via kprobes and kallsyms_lookup_name for various versioning differences pre and post Linux Kernel 5.7 in order to backdoor the OpenSSH Daemon without touching disk via read and write interference.

Thank you to xcellerator, f0lg0, and nerelod for creating the resources, solutions, and inspiration for this project.
