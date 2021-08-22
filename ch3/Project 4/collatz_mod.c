#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/list.h>
#include <linux/moduleparam.h>

static LIST_HEAD(num_list);
static int start=25;
module_param(start,int,0);

struct val_t {
	int num;
	struct list_head list;
} *val;

int m_init(void)
{
    printk(KERN_INFO "Loading kernel module\n");
    while(start!=1){
	val=kmalloc(sizeof(struct val_t), GFP_KERNEL);
	if(start%2==0) start/=2;
	else start=start*3+1;
	val->num=start;
	INIT_LIST_HEAD(&val->list);
	list_add_tail(&val->list,&num_list);
    }
    list_for_each_entry(val,&num_list,list){
	    printk(KERN_INFO "%i\n",val->num);
    }
    return 0;
}

void m_exit(void)
{
    struct val_t *next;
    printk(KERN_INFO "Removing kernel module\n");
    list_for_each_entry_safe(val,next,&num_list,list){
	    list_del(&val->list);
	    kfree(val);
    }
    if(list_empty(&num_list)) printk(KERN_INFO "Everything is fine\n");
    else printk(KERN_INFO "Something went wrong\n");
}

module_init(m_init);
module_exit(m_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Collatz module");
MODULE_AUTHOR("Gutsh");

