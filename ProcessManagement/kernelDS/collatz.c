#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/slab.h>

struct collz
{
    int num;
    struct list_head list;
};
static LIST_HEAD(collz_seq);
static int start = 25;
module_param(start, int, 0);


int ns_init(void)
{
//    static int start = 25;
//    module_param(start, int, 0);

    struct collz *lg;
    lg = kmalloc(sizeof(*lg), GFP_KERNEL);
    lg->num = start;
    INIT_LIST_HEAD(&lg->list); 
    
    int i = start;
    while(i > 1)
    {
        struct collz *lg;
        lg = kmalloc(sizeof(*lg), GFP_KERNEL);
        lg->num = i;
        INIT_LIST_HEAD(&lg->list);
        list_add_tail(&lg->list, &collz_seq);

        i = (i % 2 == 0)? i/2 : 3*i+1;        
    }
        struct collz *one;
        one = kmalloc(sizeof(*one), GFP_KERNEL);
        one->num = 1;
        INIT_LIST_HEAD(&one->list);
        list_add_tail(&one->list, &collz_seq);

        struct collz *ptr;
        list_for_each_entry(ptr, &collz_seq, list)
        {
            printk("Collatz: %d\n", ptr->num);
        }
    return 0;
}

void ns_exit(void)
{   
    struct collz *ptr, *next;
    list_for_each_entry_safe(ptr, next, &collz_seq, list)
    {
        list_del(&ptr->list);
        kfree(ptr);
    }
    printk("Module collatz was removed\n");
}

module_init(ns_init);
module_exit(ns_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("COLLATZ");
MODULE_AUTHOR("NS");
