#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>

int ns_init(void)
{
    struct task_struct *task;
    
    for_each_process(task){

        printk(KERN_INFO "command = [%s] pid = [%d] status = [%d]\n", task->comm, task->pid, task->__state); 
    }
    return 0;
}

void ns_exit(void)
{
    printk(KERN_INFO "LINEAR module was removed\n");
}

module_init(ns_init);
module_exit(ns_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LINEAR");
MODULE_AUTHOR("NS");
