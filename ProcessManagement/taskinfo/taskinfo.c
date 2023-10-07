#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>

#define PROC_NAME "pid"
#define BUFFER_SIZE 128

static long l_pid;

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops =
{
    .proc_read = proc_read,
    .proc_write = proc_write,
};

int proc_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;    
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    struct task_struct *tsk = NULL;

    if (completed)
    {
        completed = 0;
        return 0;
    }
    tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);
    if (tsk == NULL)
    {
        printk(KERN_INFO "Invalid pid\n");
        return -1;
    }

    completed = 1;
    
    rv = sprintf(buffer, "command = [%s] pid = [%ld] state = [%d]\n", tsk->comm, l_pid, tsk->__state);    
    if(copy_to_user(usr_buf, buffer, rv))
        rv = -1;
        

    return rv;
}

ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
    char *k_mem;
    
    //allocate kernel memory
    k_mem = kmalloc(count, GFP_KERNEL);
    
    //copies user space usr_buf to kernel memory
    if (copy_from_user(k_mem, usr_buf, count))
    {
        printk (KERN_INFO "Error copying from user\n");
        return -1;
    }
    
    sscanf(k_mem, "%ld", &l_pid);
    printk(KERN_INFO "pid : %ld\n", l_pid);
    //return kernel memory
    kfree(k_mem);
    return count;
}
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SSG");
