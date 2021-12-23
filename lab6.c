#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#include <linux/slab.h>
#include <linux/ktime.h>


MODULE_AUTHOR("Maksym Shchotkin <maksym.shchotkin@gmail.com>");
MODULE_DESCRIPTION("Lab6");
MODULE_LICENSE("Dual BSD/GPL");

struct list_head_ {
	struct list_head_ *next;
	ktime_t time;
	};

static int num = 1;
static struct list_head_ *head;

module_param(num, uint, 0444);
MODULE_PARM_DESC(num, "'hello world' will be printed this amount of times");

uint i;
static int __init hello_init(void)
{
	struct list_head_ *ld1, *ld2;
	head = kmalloc(sizeof(struct list_head_ *), GFP_KERNEL);
	ld1 = head;
	
	if (num == 0) {
		pr_warn("num = 0");
		}
	else if (num >= 5 && num <= 10) {
		pr_warn("5 < num < 10");
		}
	else if (num > 10) {
		BUG_ON(num > 10);
		pr_warn("10 < num");
		return -EINVAL;
		}

	for (i = 0; i < num; i++){
		ld1->next = kmalloc(sizeof(struct list_head_), GFP_KERNEL);
		ld1->time = ktime_get();
		if (i == 7){
			ld1 = NULL;
			}
			
		BUG_ON(!ld1);
		pr_info("hello world\n");
		ld2 = ld1;
		ld1 = ld1->next;
	}
	
	if (num != 0) {
		kfree(ld2->next);
		ld2->next = NULL;
	}
	
	return 0;
}

static void __exit hello_exit(void)
{
	struct list_head_ *var;
	while (num != 0 && head != NULL) {
		var = head;
		pr_info("Time - %lld", var->time);
		head = var->next;
		kfree(var);
	}
pr_info("\n");
}

module_init(hello_init);
module_exit(hello_exit);
