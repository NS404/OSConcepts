#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

void DFS(struct task_struct *task)
{
    struct task_struct *child;
    struct list_head *list;
    
    printk(KERN_INFO "command = [%s]  pid = [%d]  status = [%d]\n", task->comm, task->pid, task->__state);
    list_for_each(list, &task->children)
    {
        child = list_entry(list, struct task_struct, sibling);
        DFS(child);
    }
}

int ns_init(void)
{
    DFS(&init_task);
    return 0;
}

void ns_exit(void)
{
    printk(KERN_INFO "Module DFSLIST was removed\n");
}

module_init(ns_init);
module_exit(ns_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NS");
MODULE_DESCRIPTION("DFSLIST");
