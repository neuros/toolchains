#ifndef __LINUX_SPINLOCK_H
#define __LINUX_SPINLOCK_H

/*
 * include/linux/spinlock.h - generic locking declarations
 */

#include <linux/config.h>
#include <linux/preempt.h>
#include <linux/linkage.h>
#include <linux/compiler.h>
#include <linux/thread_info.h>
#include <linux/kernel.h>
#include <linux/stringify.h>

#include <linux/rt_lock.h>

#include <asm/processor.h>	/* for cpu relax */
#include <asm/system.h>

/*
 * Must define these before including other files, inline functions need them
 */
#define LOCK_SECTION_NAME                       \
        ".text.lock." __stringify(KBUILD_BASENAME)

#define LOCK_SECTION_START(extra)               \
        ".subsection 1\n\t"                     \
        extra                                   \
        ".ifndef " LOCK_SECTION_NAME "\n\t"     \
        LOCK_SECTION_NAME ":\n\t"               \
        ".endif\n"

#define LOCK_SECTION_END                        \
        ".previous\n\t"

#define __lockfunc fastcall __attribute__((section(".spinlock.text")))

/*
 * If CONFIG_SMP is set, pull in the _raw_* definitions
 */
#ifdef CONFIG_SMP
#include <asm/spinlock.h>

int __lockfunc _raw_spin_trylock(raw_spinlock_t *lock);
int __lockfunc _raw_read_trylock(raw_rwlock_t *lock);
int __lockfunc _raw_write_trylock(raw_rwlock_t *lock);

void __lockfunc _raw_spin_lock(raw_spinlock_t *lock)	__acquires(raw_spinlock_t);
void __lockfunc _raw_read_lock(raw_rwlock_t *lock)	__acquires(raw_rwlock_t);
void __lockfunc _raw_write_lock(raw_rwlock_t *lock)	__acquires(raw_rwlock_t);

void __lockfunc _raw_spin_unlock(raw_spinlock_t *lock)	__releases(raw_spinlock_t);
void __lockfunc _raw_read_unlock(raw_rwlock_t *lock)	__releases(raw_rwlock_t);
void __lockfunc _raw_write_unlock(raw_rwlock_t *lock)	__releases(raw_rwlock_t);

unsigned long __lockfunc _raw_spin_lock_irqsave(raw_spinlock_t *lock)	__acquires(raw_spinlock_t);
unsigned long __lockfunc _raw_read_lock_irqsave(raw_rwlock_t *lock)	__acquires(raw_rwlock_t);
unsigned long __lockfunc _raw_write_lock_irqsave(raw_rwlock_t *lock)	__acquires(raw_rwlock_t);

void __lockfunc _raw_spin_lock_irq(raw_spinlock_t *lock)	__acquires(raw_spinlock_t);
void __lockfunc _raw_spin_lock_bh(raw_spinlock_t *lock)		__acquires(raw_spinlock_t);
void __lockfunc _raw_read_lock_irq(raw_rwlock_t *lock)		__acquires(raw_rwlock_t);
void __lockfunc _raw_read_lock_bh(raw_rwlock_t *lock)		__acquires(raw_rwlock_t);
void __lockfunc _raw_write_lock_irq(raw_rwlock_t *lock)		__acquires(raw_rwlock_t);
void __lockfunc _raw_write_lock_bh(raw_rwlock_t *lock)		__acquires(raw_rwlock_t);

void __lockfunc _raw_spin_unlock_irqrestore(raw_spinlock_t *lock, unsigned long flags)	__releases(raw_spinlock_t);
void __lockfunc _raw_spin_unlock_irq(raw_spinlock_t *lock)				__releases(raw_spinlock_t);
void __lockfunc _raw_spin_unlock_bh(raw_spinlock_t *lock)				__releases(raw_spinlock_t);
void __lockfunc _raw_read_unlock_irqrestore(raw_rwlock_t *lock, unsigned long flags)	__releases(raw_rwlock_t);
void __lockfunc _raw_read_unlock_irq(raw_rwlock_t *lock)				__releases(raw_rwlock_t);
void __lockfunc _raw_read_unlock_bh(raw_rwlock_t *lock)					__releases(raw_rwlock_t);
void __lockfunc _raw_write_unlock_irqrestore(raw_rwlock_t *lock, unsigned long flags)	__releases(raw_rwlock_t);
void __lockfunc _raw_write_unlock_irq(raw_rwlock_t *lock)				__releases(raw_rwlock_t);
void __lockfunc _raw_write_unlock_bh(raw_rwlock_t *lock)				__releases(raw_rwlock_t);

int __lockfunc _raw_spin_trylock_bh(raw_spinlock_t *lock);
int __lockfunc _raw_spin_trylock_irq(raw_spinlock_t *lock);
int __lockfunc _raw_spin_trylock_irqsave(raw_spinlock_t *lock, unsigned long *flags);
int __lockfunc generic_raw_read_trylock(raw_rwlock_t *lock);
int in_lock_functions(unsigned long addr);

#else

#define in_lock_functions(ADDR) 0

#if !defined(CONFIG_PREEMPT) && !defined(CONFIG_DEBUG_SPINLOCK)
# define _atomic_dec_and_raw_spin_lock(atomic,lock) atomic_dec_and_test(atomic)
# define ATOMIC_DEC_AND_LOCK
#endif

#ifdef CONFIG_DEBUG_SPINLOCK
 
#define SPINLOCK_MAGIC	0x1D244B3C
typedef struct {
	unsigned long magic;
	volatile unsigned long lock;
	volatile unsigned int babble;
	const char *module;
	char *owner;
	int oline;
} spinlock_t;
#define SPIN_LOCK_UNLOCKED (spinlock_t) { SPINLOCK_MAGIC, 0, 10, __FILE__ , NULL, 0}

#define spin_lock_init(x) \
	do { \
		(x)->magic = SPINLOCK_MAGIC; \
		(x)->lock = 0; \
		(x)->babble = 5; \
		(x)->module = __FILE__; \
		(x)->owner = NULL; \
		(x)->oline = 0; \
	} while (0)

#define CHECK_LOCK(x) \
	do { \
	 	if ((x)->magic != SPINLOCK_MAGIC) { \
			printk(KERN_ERR "%s:%d: spin_is_locked on uninitialized spinlock %p.\n", \
					__FILE__, __LINE__, (x)); \
		} \
	} while(0)

#define __raw_spin_lock(x)		\
	do { \
	 	CHECK_LOCK(x); \
		if ((x)->lock&&(x)->babble) { \
			(x)->babble--; \
			printk("%s:%d: spin_lock(%s:%p) already locked by %s/%d\n", \
					__FILE__,__LINE__, (x)->module, \
					(x), (x)->owner, (x)->oline); \
		} \
		(x)->lock = 1; \
		(x)->owner = __FILE__; \
		(x)->oline = __LINE__; \
	} while (0)

/* without debugging, spin_is_locked on UP always says
 * FALSE. --> printk if already locked. */
#define spin_is_locked(x) \
	({ \
	 	CHECK_LOCK(x); \
		if ((x)->lock&&(x)->babble) { \
			(x)->babble--; \
			printk("%s:%d: spin_is_locked(%s:%p) already locked by %s/%d\n", \
					__FILE__,__LINE__, (x)->module, \
					(x), (x)->owner, (x)->oline); \
		} \
		0; \
	})

/* without debugging, spin_trylock on UP always says
 * TRUE. --> printk if already locked. */
#define __raw_spin_trylock(x) \
	({ \
	 	CHECK_LOCK(x); \
		if ((x)->lock&&(x)->babble) { \
			(x)->babble--; \
			printk("%s:%d: spin_trylock(%s:%p) already locked by %s/%d\n", \
					__FILE__,__LINE__, (x)->module, \
					(x), (x)->owner, (x)->oline); \
		} \
		(x)->lock = 1; \
		(x)->owner = __FILE__; \
		(x)->oline = __LINE__; \
		1; \
	})

#define spin_unlock_wait(x)	\
	do { \
	 	CHECK_LOCK(x); \
		if ((x)->lock&&(x)->babble) { \
			(x)->babble--; \
			printk("%s:%d: spin_unlock_wait(%s:%p) owned by %s/%d\n", \
					__FILE__,__LINE__, (x)->module, (x), \
					(x)->owner, (x)->oline); \
		}\
	} while (0)

#define __raw_spin_unlock(x) \
	do { \
	 	CHECK_LOCK(x); \
		if (!(x)->lock&&(x)->babble) { \
			(x)->babble--; \
			printk("%s:%d: spin_unlock(%s:%p) not locked\n", \
					__FILE__,__LINE__, (x)->module, (x));\
		} \
		(x)->lock = 0; \
	} while (0)
#else

/*
 * If CONFIG_SMP is unset, declare the _raw_* definitions as nops
 */
#define __raw_spin_lock_init(lock)	do { (void)(lock); } while(0)
#define __raw_spin_lock(lock)		do { (void)(lock); } while(0)
#define __raw_spin_is_locked(lock)	((void)(lock), 0)
#define __raw_spin_trylock(lock)	(((void)(lock), 1))
#define __raw_spin_unlock_wait(lock)	(void)(lock)
#define __raw_spin_unlock(lock) 	do { (void)(lock); } while(0)
#endif /* CONFIG_DEBUG_SPINLOCK */

/* RW spinlocks: No debug version */

// typedef struct { } raw_rwlock_t;
// #define __RAW_RW_LOCK_UNLOCKED { }
// #define RAW_RW_LOCK_UNLOCKED (raw_rwlock_t) __RAW_RW_LOCK_UNLOCKED

#define __raw_rwlock_init(lock)		do { (void)(lock); } while(0)
#define __raw_read_lock(lock)		do { (void)(lock); } while(0)
#define __raw_read_unlock(lock)		do { (void)(lock); } while(0)
#define __raw_write_lock(lock)		do { (void)(lock); } while(0)
#define __raw_write_unlock(lock)	do { (void)(lock); } while(0)
#define __raw_read_can_lock(lock)	(((void)(lock), 1))
#define __raw_write_can_lock(lock)	(((void)(lock), 1))
#define __raw_read_trylock(lock)	({ (void)(lock); (1); })
#define __raw_write_trylock(lock)	({ (void)(lock); (1); })

#define _raw_spin_trylock(lock)	({preempt_disable(); __raw_spin_trylock(lock) ? \
				1 : ({preempt_enable(); 0;});})

#define _raw_read_trylock(lock)	({preempt_disable();__raw_read_trylock(lock) ? \
				1 : ({preempt_enable(); 0;});})

#define _raw_write_trylock(lock)	({preempt_disable(); __raw_write_trylock(lock) ? \
				1 : ({preempt_enable(); 0;});})

#define _raw_spin_trylock_bh(lock)	({preempt_disable(); local_bh_disable(); \
				__raw_spin_trylock(lock) ? \
				1 : ({preempt_enable(); local_bh_enable(); 0;});})

#define _raw_spin_trylock_irq(lock) \
({ \
	local_irq_disable(); preempt_disable(); \
	__raw_spin_trylock(lock) ? \
	1 : ({ preempt_enable_no_resched(); local_irq_enable(); preempt_check_resched(); 0; }); \
})

#define _raw_spin_trylock_irqsave(lock, flags) \
({ \
	local_irq_save(*flags); preempt_disable(); \
	__raw_spin_trylock(lock) ? \
	1 : ({ preempt_enable_no_resched(); local_irq_restore(*flags); preempt_check_resched(); 0;}); \
})

#define _raw_spin_lock(lock)	\
do { \
	preempt_disable(); \
	__raw_spin_lock(lock); \
	__acquire(lock); \
} while(0)

#define _raw_write_lock(lock) \
do { \
	preempt_disable(); \
	__raw_write_lock(lock); \
	__acquire(lock); \
} while(0)
 
#define _raw_read_lock(lock)	\
do { \
	preempt_disable(); \
	__raw_read_lock(lock); \
	__acquire(lock); \
} while(0)

#define _raw_spin_unlock(lock) \
do { \
	__raw_spin_unlock(lock); \
	preempt_enable(); \
	__release(lock); \
} while (0)

#define _raw_write_unlock(lock) \
do { \
	__raw_write_unlock(lock); \
	preempt_enable(); \
	__release(lock); \
} while(0)

#define _raw_read_unlock(lock) \
do { \
	__raw_read_unlock(lock); \
	preempt_enable(); \
	__release(lock); \
} while(0)

#define _raw_spin_lock_irqsave(lock) \
({	unsigned long __flags; \
	local_irq_save(__flags); \
	preempt_disable(); \
	__raw_spin_lock(lock); \
	__acquire(lock); \
	__flags; \
})

#define _raw_spin_lock_irq(lock) \
do { \
	local_irq_disable(); \
	preempt_disable(); \
	__raw_spin_lock(lock); \
	__acquire(lock); \
} while (0)

#define _raw_spin_lock_bh(lock) \
do { \
	local_bh_disable(); \
	preempt_disable(); \
	__raw_spin_lock(lock); \
	__acquire(lock); \
} while (0)

#define _raw_read_lock_irqsave(lock) \
({	unsigned long __flags; \
	local_irq_save(__flags); \
	preempt_disable(); \
	__raw_read_lock(lock); \
	__acquire(lock); \
	__flags; \
})

#define _raw_read_lock_irq(lock) \
do { \
	local_irq_disable(); \
	preempt_disable(); \
	__raw_read_lock(lock); \
	__acquire(lock); \
} while (0)

#define _raw_read_lock_bh(lock) \
do { \
	local_bh_disable(); \
	preempt_disable(); \
	__raw_read_lock(lock); \
	__acquire(lock); \
} while (0)

#define _raw_write_lock_irqsave(lock) \
({	unsigned long __flags; \
	local_irq_save(__flags); \
	preempt_disable(); \
	__raw_write_lock(lock); \
	__acquire(lock); \
	__flags; \
})

#define _raw_write_lock_irq(lock) \
do { \
	local_irq_disable(); \
	preempt_disable(); \
	__raw_write_lock(lock); \
	__acquire(lock); \
} while (0)

#define _raw_write_lock_bh(lock) \
do { \
	local_bh_disable(); \
	preempt_disable(); \
	__raw_write_lock(lock); \
	__acquire(lock); \
} while (0)

#define _raw_spin_unlock_irqrestore(lock, flags) \
do { \
	__raw_spin_unlock(lock); \
	preempt_enable_no_resched(); \
	local_irq_restore(flags); \
	preempt_check_resched(); \
	__release(lock); \
} while (0)

#define _raw_spin_unlock_irq(lock) \
do { \
	__raw_spin_unlock(lock); \
	preempt_enable_no_resched(); \
	local_irq_enable(); \
	preempt_check_resched(); \
	__release(lock); \
} while (0)

#define _raw_spin_unlock_bh(lock) \
do { \
	__raw_spin_unlock(lock); \
	preempt_enable(); \
	local_bh_enable(); \
	__release(lock); \
} while (0)

#define _raw_write_unlock_bh(lock) \
do { \
	__raw_write_unlock(lock); \
	preempt_enable(); \
	local_bh_enable(); \
	__release(lock); \
} while (0)

#define _raw_read_unlock_irqrestore(lock, flags) \
do { \
	__raw_read_unlock(lock); \
	preempt_enable_no_resched(); \
	local_irq_restore(flags); \
	preempt_check_resched(); \
	__release(lock); \
} while (0)

#define _raw_write_unlock_irqrestore(lock, flags) \
do { \
	__raw_write_unlock(lock); \
	preempt_enable_no_resched(); \
	local_irq_restore(flags); \
	preempt_check_resched(); \
	__release(lock); \
} while (0)

#define _raw_read_unlock_irq(lock)	\
do { \
	__raw_read_unlock(lock);\
	preempt_enable_no_resched(); \
	local_irq_enable();	\
	preempt_check_resched(); \
	__release(lock); \
} while (0)

#define _raw_read_unlock_bh(lock)	\
do { \
	__raw_read_unlock(lock);\
	local_bh_enable();	\
	preempt_enable();	\
	__release(lock); \
} while (0)

#define _raw_write_unlock_irq(lock)	\
do { \
	__raw_write_unlock(lock);\
	preempt_enable_no_resched(); \
	local_irq_enable();	\
	preempt_check_resched(); \
	__release(lock); \
} while (0)

#endif /* !SMP */

extern int __bad_spinlock_type(void);

/*
 * The following ones are only implemented on PREEMPT_RT, but
 * the type selection macros need the prototypes even though the
 * functions never get called (hence, linked):
 */
extern void _spin_lock(spinlock_t *lock);
extern void _spin_lock_bh(spinlock_t *lock);
extern void _spin_lock_irq(spinlock_t *lock);
extern unsigned long _spin_lock_irqsave(spinlock_t *lock);
extern void _spin_unlock(spinlock_t *lock);
extern void _spin_unlock_wait(spinlock_t *lock);
extern void _spin_unlock_bh(spinlock_t *lock);
extern void _spin_unlock_irq(spinlock_t *lock);
extern void _spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags);
extern int _spin_trylock(spinlock_t *lock);
extern int _spin_trylock_bh(spinlock_t *lock);
extern int _spin_trylock_irq(spinlock_t *lock);
extern int _spin_trylock_irqsave(spinlock_t *lock, unsigned long *flags);
extern int _spin_can_lock(spinlock_t *lock);
extern int _spin_is_locked(spinlock_t *lock);
extern int atomic_dec_and_spin_lock(atomic_t *atomic, spinlock_t *lock);
extern void _spin_lock_init(spinlock_t *lock, char *name, char *file, int line);

#define TYPE_EQUAL(lock, type) \
		__builtin_types_compatible_p(typeof(lock), type *)

#define PICK_OP(type, optype, op, lock)				\
do {								\
	if (TYPE_EQUAL((lock), type))				\
		_raw_##optype##op((type *)(lock));		\
	else if (TYPE_EQUAL(lock, spinlock_t))			\
		_spin##op((spinlock_t *)(lock));		\
	else __bad_spinlock_type();				\
} while (0)

#define PICK_OP_RET(type, optype, op, lock...)			\
({								\
	int __ret;						\
								\
	if (TYPE_EQUAL((lock), type))	  			\
		__ret = _raw_##optype##op((type *)(lock));	\
	else if (TYPE_EQUAL(lock, spinlock_t))			\
		__ret = _spin##op((spinlock_t *)(lock));	\
	else __ret = __bad_spinlock_type();			\
								\
	__ret;							\
})

#define PICK_OP2(type, optype, op, lock, flags)			\
do {								\
	if (TYPE_EQUAL((lock), type))				\
		_raw_##optype##op((type *)(lock), flags);	\
	else if (TYPE_EQUAL(lock, spinlock_t))			\
		_spin##op((spinlock_t *)(lock), flags);		\
	else __bad_spinlock_type();				\
} while (0)

#define PICK_OP2_RET(type, optype, op, lock, flags)		\
({								\
	int __ret;						\
								\
	if (TYPE_EQUAL((lock), type))				\
		__ret = _raw_##optype##op((type *)(lock), flags);\
	else if (TYPE_EQUAL(lock, spinlock_t))			\
		__ret = _spin##op((spinlock_t *)(lock), flags);	\
	else __bad_spinlock_type();				\
								\
	__ret;							\
})


extern int _read_trylock(rwlock_t *rwlock);
extern int _write_trylock(rwlock_t *rwlock);
extern int _read_can_lock(rwlock_t *rwlock);
extern int _write_can_lock(rwlock_t *rwlock);
extern void _write_lock(rwlock_t *rwlock);
extern void _read_lock(rwlock_t *rwlock);
extern void _write_unlock(rwlock_t *rwlock);
extern void _read_unlock(rwlock_t *rwlock);
extern unsigned long _write_lock_irqsave(rwlock_t *rwlock);
extern unsigned long _read_lock_irqsave(rwlock_t *rwlock);
extern void _write_lock_irq(rwlock_t *rwlock);
extern void _read_lock_irq(rwlock_t *rwlock);
extern void _write_lock_bh(rwlock_t *rwlock);
extern void _read_lock_bh(rwlock_t *rwlock);
extern void _write_unlock_irq(rwlock_t *rwlock);
extern void _read_unlock_irq(rwlock_t *rwlock);
extern void _write_unlock_bh(rwlock_t *rwlock);
extern void _read_unlock_bh(rwlock_t *rwlock);
extern void _write_unlock_irqrestore(rwlock_t *rwlock, unsigned long flags);
extern void _read_unlock_irqrestore(rwlock_t *rwlock, unsigned long flags);
extern void _rwlock_init(rwlock_t *rwlock, char *name, char *file, int line);
extern int _rwlock_is_locked(rwlock_t *rwlock);

#define __PICK_RW_OP(type, optype, op, lock)				\
do {									\
	if (TYPE_EQUAL((lock), type))					\
		_raw_##optype##op((type *)(lock));			\
	else if (TYPE_EQUAL(lock, rwlock_t))				\
		##op((rwlock_t *)(lock));			\
	else __bad_spinlock_type();					\
} while (0)

#define PICK_RW_OP(type, optype, op, lock)				\
do {									\
	if (TYPE_EQUAL((lock), type))					\
		_raw_##optype##op((type *)(lock));			\
	else if (TYPE_EQUAL(lock, rwlock_t))				\
		_##optype##op((rwlock_t *)(lock));			\
	else __bad_spinlock_type();					\
} while (0)

#define __PICK_RW_OP_RET(type, optype, op, lock...)			\
({									\
	int __ret;							\
									\
	if (TYPE_EQUAL((lock), type))	  				\
		__ret = _raw_##optype##op((type *)(lock));		\
	else if (TYPE_EQUAL(lock, rwlock_t))				\
		__ret = _##optype##op((rwlock_t *)(lock));		\
	else __ret = __bad_spinlock_type();				\
									\
	__ret;								\
})

#define PICK_RW_OP_RET(type, optype, op, lock...)			\
({									\
	int __ret;							\
									\
	if (TYPE_EQUAL((lock), type))	  				\
		__ret = _raw_##optype##op((type *)(lock));		\
	else if (TYPE_EQUAL(lock, rwlock_t))				\
		__ret = _##optype##op((rwlock_t *)(lock));	\
	else __ret = __bad_spinlock_type();				\
									\
	__ret;								\
})

#define PICK_RW_OP2(type, optype, op, lock, flags)			\
do {									\
	if (TYPE_EQUAL((lock), type))					\
		_raw_##optype##op((type *)(lock), flags);		\
	else if (TYPE_EQUAL(lock, rwlock_t))				\
		_##optype##op((rwlock_t *)(lock), flags);	\
	else __bad_spinlock_type();					\
} while (0)

#define _raw_spin_lock_init __raw_spin_lock_init

#define PICK_OP_INIT(type, optype, op, lock)				\
do {									\
	if (TYPE_EQUAL((lock), type))					\
		_raw_##optype##op((type *)(lock));			\
	else if (TYPE_EQUAL(lock, spinlock_t))				\
		_spin##op((spinlock_t *)(lock), #lock, __FILE__, __LINE__); \
	else __bad_spinlock_type();					\
} while (0)


#define spin_lock_init(lock) \
		PICK_OP_INIT(raw_spinlock_t, spin, _lock_init, lock)

#define _raw_rwlock_init __raw_rwlock_init

#define __PICK_RW_OP_INIT(type, optype, op, lock)			\
do {									\
	if (TYPE_EQUAL((lock), type))					\
		_raw_##optype##op((type *)(lock));			\
	else if (TYPE_EQUAL(lock, rwlock_t))				\
		_##optype##op((rwlock_t *)(lock), #lock, __FILE__, __LINE__);\
	else __bad_spinlock_type();					\
} while (0)


#define rwlock_init(lock) \
		__PICK_RW_OP_INIT(raw_rwlock_t, rwlock, _init, lock)

#define _raw_spin_is_locked __raw_spin_is_locked

#define spin_is_locked(lock) \
		PICK_OP_RET(raw_spinlock_t, spin, _is_locked, lock)

#define _raw_rwlock_is_locked __raw_rwlock_is_locked

#define rwlock_is_locked(lock) \
		__PICK_RW_OP_RET(raw_rwlock_t, rwlock, _is_locked, lock)

#define _raw_spin_unlock_wait __raw_spin_unlock_wait

#define spin_unlock_wait(lock) \
		PICK_OP(raw_spinlock_t, spin, _unlock_wait, lock)
/*
 * Define the various spin_lock and rw_lock methods.  Note we define these
 * regardless of whether CONFIG_SMP or CONFIG_PREEMPT are set. The various
 * methods are defined as nops in the case they are not required.
 */
// #define spin_trylock(lock)	_spin_trylock(lock)
#define spin_trylock(lock)	__cond_lock(PICK_OP_RET(raw_spinlock_t, spin, _trylock, lock))

//#define read_trylock(lock)	_read_trylock(lock)
#define read_trylock(lock)	__cond_lock(PICK_RW_OP_RET(raw_rwlock_t, read, _trylock, lock))

//#define write_trylock(lock)	_write_trylock(lock)
#define write_trylock(lock)	__cond_lock(PICK_RW_OP_RET(raw_rwlock_t, write, _trylock, lock))

#define _raw_spin_can_lock __raw_spin_can_lock
#define _raw_read_can_lock __raw_read_can_lock
#define _raw_write_can_lock __raw_write_can_lock

#define spin_can_lock(lock)	__cond_lock(PICK_OP_RET(raw_spinlock_t, spin, _can_lock, lock))
#define read_can_lock(lock)	__cond_lock(PICK_RW_OP_RET(raw_rwlock_t, read, _can_lock, lock))
#define write_can_lock(lock)	__cond_lock(PICK_RW_OP_RET(raw_rwlock_t, write, _can_lock, lock))

// #define spin_lock(lock)	_spin_lock(lock)
#define spin_lock(lock)		PICK_OP(raw_spinlock_t, spin, _lock, lock)

//#define write_lock(lock)	_write_lock(lock)
#define write_lock(lock)	PICK_RW_OP(raw_rwlock_t, write, _lock, lock)

// #define read_lock(lock)		_read_lock(lock)
#define read_lock(lock)		PICK_RW_OP(raw_rwlock_t, read, _lock, lock)

#ifdef CONFIG_SMP
// #define spin_lock_irqsave(lock, flags)	flags = _spin_lock_irqsave(lock)
// #define read_lock_irqsave(lock, flags)	flags = _read_lock_irqsave(lock)
// #define write_lock_irqsave(lock, flags)	flags = _write_lock_irqsave(lock)
#else
// #define spin_lock_irqsave(lock, flags)	_spin_lock_irqsave(lock, flags)
// #define read_lock_irqsave(lock, flags)	_read_lock_irqsave(lock, flags)
// #define write_lock_irqsave(lock, flags)	_write_lock_irqsave(lock, flags)
#endif

# define spin_lock_irqsave(lock, flags) \
	flags = PICK_OP_RET(raw_spinlock_t, spin, _lock_irqsave, lock)
# define read_lock_irqsave(lock, flags) \
	flags = PICK_RW_OP_RET(raw_rwlock_t, read, _lock_irqsave, lock)
# define write_lock_irqsave(lock, flags) \
	flags = PICK_RW_OP_RET(raw_rwlock_t, write, _lock_irqsave, lock)

// #define spin_lock_irq(lock)	_spin_lock_irq(lock)
// #define spin_lock_bh(lock)	_spin_lock_bh(lock)
#define spin_lock_irq(lock)	PICK_OP(raw_spinlock_t, spin, _lock_irq, lock)
#define spin_lock_bh(lock)	PICK_OP(raw_spinlock_t, spin, _lock_bh, lock)

// #define read_lock_irq(lock)	_read_lock_irq(lock)
// #define read_lock_bh(lock)	_read_lock_bh(lock)
#define read_lock_irq(lock)	PICK_RW_OP(raw_rwlock_t, read, _lock_irq, lock)
#define read_lock_bh(lock)	PICK_RW_OP(raw_rwlock_t, read, _lock_bh, lock)

// #define write_lock_irq(lock)		_write_lock_irq(lock)
// #define write_lock_bh(lock)		_write_lock_bh(lock)
#define write_lock_irq(lock)	PICK_RW_OP(raw_rwlock_t, write, _lock_irq, lock)
#define write_lock_bh(lock)	PICK_RW_OP(raw_rwlock_t, write, _lock_bh, lock)

// #define spin_unlock(lock)	_spin_unlock(lock)
// #define write_unlock(lock)	_write_unlock(lock)
// #define read_unlock(lock)	_read_unlock(lock)
#define spin_unlock(lock)	PICK_OP(raw_spinlock_t, spin, _unlock, lock)
#define read_unlock(lock)	PICK_RW_OP(raw_rwlock_t, read, _unlock, lock)
#define write_unlock(lock)	PICK_RW_OP(raw_rwlock_t, write, _unlock, lock)

//#define spin_unlock_irqrestore(lock, flags)
//		_spin_unlock_irqrestore(lock, flags)
//#define spin_unlock_irq(lock)	_spin_unlock_irq(lock)
//#define spin_unlock_bh(lock)	_spin_unlock_bh(lock)
#define spin_unlock_irqrestore(lock, flags) \
	PICK_OP2(raw_spinlock_t, spin, _unlock_irqrestore, lock, flags)
#define spin_unlock_irq(lock)	PICK_OP(raw_spinlock_t, spin, _unlock_irq, lock)
#define spin_unlock_bh(lock)	PICK_OP(raw_spinlock_t, spin, _unlock_bh, lock)

// #define read_unlock_irqrestore(lock, flags)
// 		_read_unlock_irqrestore(lock, flags)
// #define read_unlock_irq(lock)	_read_unlock_irq(lock)
// #define read_unlock_bh(lock)	_read_unlock_bh(lock)
#define read_unlock_irqrestore(lock, flags) \
		PICK_RW_OP2(raw_rwlock_t, read, _unlock_irqrestore, lock, flags)
#define read_unlock_irq(lock) PICK_RW_OP(raw_rwlock_t, read, _unlock_irq, lock)
#define read_unlock_bh(lock) PICK_RW_OP(raw_rwlock_t, read, _unlock_bh, lock)

// #define write_unlock_irqrestore(lock, flags)
// 	_write_unlock_irqrestore(lock, flags)
// #define write_unlock_irq(lock)			_write_unlock_irq(lock)
// #define write_unlock_bh(lock)			_write_unlock_bh(lock)
#define write_unlock_irqrestore(lock, flags) \
	PICK_RW_OP2(raw_rwlock_t, write, _unlock_irqrestore, lock, flags)
#define write_unlock_irq(lock) PICK_RW_OP(raw_rwlock_t, write, _unlock_irq, lock)
#define write_unlock_bh(lock) PICK_RW_OP(raw_rwlock_t, write, _unlock_bh, lock)

// #define spin_trylock_bh(lock)	_spin_trylock_bh(lock)
#define spin_trylock_bh(lock)	__cond_lock(PICK_OP_RET(raw_spinlock_t, spin, _trylock_bh, lock))

// #define spin_trylock_irq(lock)

#define spin_trylock_irq(lock)	__cond_lock(PICK_OP_RET(raw_spinlock_t, spin, _trylock_irq, lock))

// #define spin_trylock_irqsave(lock, flags)

#define spin_trylock_irqsave(lock, flags)	__cond_lock(PICK_OP2_RET(raw_spinlock_t, spin, _trylock_irqsave, lock, &flags))

/* "lock on reference count zero" */
#ifndef ATOMIC_DEC_AND_LOCK
# include <asm/atomic.h>
  extern int _atomic_dec_and_raw_spin_lock(atomic_t *atomic, raw_spinlock_t *lock);
#endif

#define atomic_dec_and_lock(atomic, lock)				\
__cond_lock(({								\
	int __ret;							\
									\
	if (TYPE_EQUAL(lock, raw_spinlock_t))				\
		__ret = _atomic_dec_and_raw_spin_lock(atomic,		\
					(raw_spinlock_t *)(lock));	\
	else if (TYPE_EQUAL(lock, spinlock_t))				\
		__ret = atomic_dec_and_spin_lock(atomic,		\
					(spinlock_t *)(lock));		\
	else __ret = __bad_spinlock_type();				\
									\
	__ret;								\
}))


/*
 *  bit-based spin_lock()
 *
 * Don't use this unless you really need to: spin_lock() and spin_unlock()
 * are significantly faster.
 */
static inline void bit_spin_lock(int bitnum, unsigned long *addr)
{
	/*
	 * Assuming the lock is uncontended, this never enters
	 * the body of the outer loop. If it is contended, then
	 * within the inner loop a non-atomic test is used to
	 * busywait with less bus contention for a good time to
	 * attempt to acquire the lock bit.
	 */
#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) || defined(CONFIG_PREEMPT)
	while (test_and_set_bit(bitnum, addr))
		while (test_bit(bitnum, addr))
			cpu_relax();
#endif
	__acquire(bitlock);
}

/*
 * Return true if it was acquired
 */
static inline int bit_spin_trylock(int bitnum, unsigned long *addr)
{
#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) || defined(CONFIG_PREEMPT)
	if (test_and_set_bit(bitnum, addr))
		return 0;
#endif
	__acquire(bitlock);
	return 1;
}

/*
 *  bit-based spin_unlock()
 */
static inline void bit_spin_unlock(int bitnum, unsigned long *addr)
{
#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) || defined(CONFIG_PREEMPT)
	BUG_ON(!test_bit(bitnum, addr));
	smp_mb__before_clear_bit();
	clear_bit(bitnum, addr);
#endif
	__release(bitlock);
}

/*
 * Return true if the lock is held.
 */
static inline int bit_spin_is_locked(int bitnum, unsigned long *addr)
{
#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) || defined(CONFIG_PREEMPT)
	return test_bit(bitnum, addr);
#else
	return 1;
#endif
}

#define DEFINE_SPINLOCK(name) \
	spinlock_t name __cacheline_aligned_in_smp = _SPIN_LOCK_UNLOCKED(name)

#define DEFINE_RAW_SPINLOCK(name) \
	raw_spinlock_t name __cacheline_aligned_in_smp = RAW_SPIN_LOCK_UNLOCKED

#define DEFINE_RWLOCK(name) \
	rwlock_t name __cacheline_aligned_in_smp = _RW_LOCK_UNLOCKED(name)

#define DEFINE_RAW_RWLOCK(name) \
	raw_rwlock_t name __cacheline_aligned_in_smp = RAW_RW_LOCK_UNLOCKED

#ifndef CONFIG_PREEMPT_RT
# define DECLARE_MUTEX_NOCHECK DECLARE_MUTEX
# define sema_count(sem) atomic_read(&(sem)->count)
# define sema_init_nocheck sema_init
#endif

/**
 * __raw_spin_can_lock - would __raw_spin_trylock() succeed?
 * @lock: the spinlock in question.
 */
#define __raw_spin_can_lock(lock)            (!__raw_spin_is_locked(lock))

#endif /* __LINUX_SPINLOCK_H */
