#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/list.h>

static void dfs_tree(struct task_struct *root)
{
	struct task_struct *task;
	struct list_head *list;
	printk(KERN_INFO "command=[%s] pid=[%i] state=[%li]\n",root->comm,root->pid,root->state);
	list_for_each(list, &root->children){
		if (list_empty(list)) return;
		task = list_entry(list, struct task_struct, sibling);
		dfs_tree(task);
	}
}

static int task_init(void)
{
	printk(KERN_INFO "Here it comes!\n");
	dfs_tree(&init_task);
	return 0;
}

static void task_exit(void) 
{
        printk( KERN_INFO "Bye...\n");
}

module_init( task_init );
module_exit( task_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("All processes stat using DFS");
MODULE_AUTHOR("Gutsh");

