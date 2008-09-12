#ifndef _ASM_GENERIC_BUG_H
#define _ASM_GENERIC_BUG_H

#include <linux/compiler.h>
#include <linux/config.h>

#ifndef HAVE_ARCH_BUG
#define BUG() do { \
	printk("kernel BUG at %s:%d!\n", __FILE__, __LINE__); \
	panic("BUG!"); \
} while (0)
#endif

#ifndef HAVE_ARCH_PAGE_BUG
#define PAGE_BUG(page) do { \
	printk("page BUG for page at %p\n", page); \
	BUG(); \
} while (0)
#endif

#ifndef HAVE_ARCH_BUG_ON
#define BUG_ON(condition) do { if (unlikely((condition)!=0)) BUG(); } while(0)
#endif

#ifndef HAVE_ARCH_WARN_ON
#define WARN_ON(condition) do { \
	if (unlikely((condition)!=0)) { \
		printk("%s/%d: BUG in %s at %s:%d\n", current->comm, current->pid,__FUNCTION__, __FILE__, __LINE__); \
		dump_stack(); \
	} \
} while (0)
#endif

#ifdef CONFIG_PREEMPT_RT
# define WARN_ON_RT(condition) WARN_ON(condition)
#else
# define WARN_ON_RT(condition) do { } while (0)
#endif

#endif
