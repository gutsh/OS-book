#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/param.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
	.read = proc_read,
};

unsigned long start;

int proc_init(void)
{
    start=jiffies;
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "Module has been successfully loaded\n");
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "Module has been successfully removed\n");
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed) {
	completed = 0;
	return 0;
    }
    completed = 1;
    rv=sprintf(buffer, "%lu", (jiffies-start)/HZ);
    buffer[rv]='\n';
    raw_copy_to_user(usr_buf, buffer, ++rv);
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds module");
MODULE_AUTHOR("Gutsh");

