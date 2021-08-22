#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

static int task_init(void)
{
	#include <linux/sched.h>

	int cou = 0;
	struct task_struct *task;
	printk(KERN_INFO "Here it comes!\n");
	for_each_process(task){
		printk(KERN_INFO "command=[%s] pid=[%i] state=[%li]\n",task->comm,task->pid,task->state);
		cou++;
	}
	printk(KERN_INFO "There are %i processes currently executing\n",cou);
	return 0;
}

static void task_exit(void) 
{
        printk( KERN_INFO "Bye...\n");
}

module_init( task_init );
module_exit( task_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("All processes stat");
MODULE_AUTHOR("Gutsh");

