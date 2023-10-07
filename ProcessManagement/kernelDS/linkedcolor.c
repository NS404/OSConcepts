#include <linux/list.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

struct color {
    int red;
    int green;
    int blue;
    struct list_head list;
};

static LIST_HEAD(color_list);

int ns_init(void)
{    
//    static LIST_HEAD(color_list);
    
    struct color *violet;
    struct color *pink;
    struct color *orange;
    struct color *navy;    

    violet = kmalloc(sizeof(*violet), GFP_KERNEL);
    violet->red = 138;
    violet->blue = 43;
    violet->green = 226;
    INIT_LIST_HEAD(&violet->list);
    
    pink = kmalloc(sizeof(*pink), GFP_KERNEL);
    pink->red = 255;
    pink->blue = 164;
    pink->green = 63;
    INIT_LIST_HEAD(&pink->list);
    
    orange = kmalloc(sizeof(*orange), GFP_KERNEL);
    orange->red = 248;
    orange->blue = 3;
    orange->green =111;
    INIT_LIST_HEAD(&orange->list);
        
    navy = kmalloc(sizeof(*navy), GFP_KERNEL);
    navy->red = 0;
    navy->blue = 90;
    navy->green = 66;
    INIT_LIST_HEAD(&navy->list);
    
    list_add_tail(&violet->list, &color_list);
    list_add_tail(&pink->list, &color_list);
    list_add_tail(&orange->list, &color_list);
    list_add_tail(&navy->list, &color_list);

    struct color *ptr;
    //traversing the linked list
    list_for_each_entry(ptr, &color_list, list){
        printk("RGB(%d,%d,%d)\n",ptr->red, ptr->green, ptr->blue);
    }
    return 0;
}

void ns_exit(void)
{
    struct color *ptr, *next;
    list_for_each_entry_safe(ptr, next, &color_list, list){
        list_del(&ptr->list);
        kfree(ptr);
    }
    printk(KERN_INFO "module linkedColor was removed\n");
}

module_init(ns_init);
module_exit(ns_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("linkedColor");
MODULE_AUTHOR("ns");
