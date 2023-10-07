/* this kernel module creates a proc file named
 * proc/seconds that reports the number of elapsed
 * seconds since the kernel module was loaded
 * using jiffies and HZ values.
 * */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <asm/param.h>


#define BUFFERSIZE 128
#define PROC_NAME "seconds"
unsigned long jiffies_on_load = 0;


static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *ops);

static struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

static int proc_init(void)
{
    jiffies_on_load = jiffies;
    proc_create(PROC_NAME, 0, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}


static void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}


static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *ops)
{
    int rv = 0;
    char buffer[BUFFERSIZE];
    static int completed = 0;

    if(completed)
    {
        completed = 0;
        return 0;
    }
    completed = 1;
    
    unsigned long seconds_since_load = (jiffies - jiffies_on_load)/HZ;
        
    rv = sprintf(buffer, "Seconds since kernel was loaded: %lus\n", seconds_since_load);
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("seconds");
MODULE_AUTHOR("NS");
