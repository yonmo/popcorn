# Popcorn
Linux Kernel Module Rootkit, leveraging hooks to functions involved in service configurations 

This is a LKM Rootkit leveraging system call hooks via kprobes and kallsyms_lookup_name for various versioning differences pre and post Linux Kernel 5.7 in order to backdoor the OpenSSH Daemon without touching disk via read and write interference.
In the future I hope to implement inline function hooking and eBPF detection bypasses, to create a high stealth service backdoor.

IMPORTANT: This code is for educational purposes only. I am not endorsing its usage for the violatation of local/federal/global expectations and laws, copyright/patent requirements, or terms of service/end license agreements for any company.

Thank you to xcellerator, f0lg0, and nerelod for creating the resources, solutions, and inspiration for this project.
