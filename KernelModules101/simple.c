#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <linux/jiffies.h>
#include <asm/param.h>

/* This function is called when the module is loaded. */
static int simple_init(void)
{
    printk(KERN_INFO "Loading Module\n");
    printk(KERN_INFO "Golden Ratio: %lu\n", GOLDEN_RATIO_PRIME);    
    printk(KERN_INFO "tick rate: %lu\n", HZ);
    return 0;
}

/* This function is called when the module is removed. */
static void simple_exit(void) {
    printk(KERN_INFO "GCD of 3300 and 24: %lu", gcd(3300, 24)); 
    printk(KERN_INFO "No of interrupts since boot: %lu\n", jiffies);
    printk(KERN_INFO "Removing Module\n");

}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
