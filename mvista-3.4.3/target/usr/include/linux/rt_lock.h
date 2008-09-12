#ifndef __LINUX_RT_LOCK_H
#define __LINUX_RT_LOCK_H

#include <linux/config.h>
#include <linux/list.h>

/*
 * These are the basic SMP spinlocks, allowing only a single CPU anywhere.
 * We use a generic definition on all architectures.
 */

#ifdef CONFIG_SMP
typedef struct {
	volatile unsigned long lock;
# ifdef CONFIG_DEBUG_SPINLOCK
# ifdef CONFIG_PPC32
	volatile unsigned long owner_pc;
	volatile unsigned long owner_cpu;
# else
	unsigned int magic;
# endif
# endif
# ifdef CONFIG_PREEMPT
	unsigned int break_lock;
# endif
} raw_spinlock_t;
#else
  typedef struct { } raw_spinlock_t;
# define __RAW_SPIN_LOCK_UNLOCKED { }
# define RAW_SPIN_LOCK_UNLOCKED (raw_spinlock_t) __RAW_SPIN_LOCK_UNLOCKED
#endif

#include <asm/atomic.h>

/*
 * Read-write spinlocks, allowing multiple readers
 * but only one writer.
 */
#ifdef CONFIG_SMP
typedef struct {
#if defined(CONFIG_PPC32) || defined(CONFIG_PPC64)
	volatile signed int lock;
#else
	volatile unsigned long lock;
#endif
# ifdef CONFIG_DEBUG_SPINLOCK
	unsigned magic;
# endif
# ifdef CONFIG_PREEMPT
	unsigned int break_lock;
# endif
} raw_rwlock_t;
#else
  typedef struct { } raw_rwlock_t;
# define __RAW_RW_LOCK_UNLOCKED { }
# define RAW_RW_LOCK_UNLOCKED (raw_rwlock_t) __RAW_RW_LOCK_UNLOCKED
#endif

#ifdef CONFIG_PREEMPT_RT

/*
 * This is the core locking object used by PREEMPT_RT.
 * This one handles all the logic necessary, the other locking
 * objects (spinlocks, rwlocks, semaphores and rw-semaphores)
 * all use this synchronization object internally:
 */
struct rt_mutex {
	raw_spinlock_t		wait_lock;
	struct list_head	wait_list;
	struct task_struct	*owner;
	int			owner_prio;
# ifdef CONFIG_RT_DEADLOCK_DETECT
	int			debug;
	int			save_state;
	struct list_head	held_list;
	unsigned long		acquire_eip;
	char 			*name, *file;
	int			line;
# endif
};

/*
 * This is the control structure for tasks blocked on an
 * RT mutex:
 */
struct rt_mutex_waiter {
	struct rt_mutex *lock;
	struct list_head list;
	struct list_head pi_list;
	struct task_struct *task;

	unsigned long eip;	// for debugging
};

#ifdef CONFIG_RT_DEADLOCK_DETECT
# define ___RT_MUTEX_INITIALIZER(lockname) \
	.wait_lock = RAW_SPIN_LOCK_UNLOCKED, \
	.wait_list = LIST_HEAD_INIT((lockname).wait_list), \
	.name = #lockname, .file = __FILE__, .line = __LINE__
# define __RT_MUTEX_INITIALIZER(lockname) \
	{ .debug = 1, ___RT_MUTEX_INITIALIZER(lockname) }
# define __RT_MUTEX_INITIALIZER_NOCHECK(lockname) \
	{ .debug = 0, ___RT_MUTEX_INITIALIZER(lockname) }
#else
# define __RT_MUTEX_INITIALIZER(lockname) \
	{ .wait_lock = RAW_SPIN_LOCK_UNLOCKED, \
	   LIST_HEAD_INIT((lockname).wait_list) }
# define __RT_MUTEX_INITIALIZER_NOCHECK(lockname) \
		__RT_MUTEX_INITIALIZER(lockname)
#endif
/*
 * RW-semaphores are an RT mutex plus a reader-depth count.
 *
 * Note that the semantics are different from the usual
 * Linux rw-sems, in PREEMPT_RT mode we do not allow
 * multiple readers to hold the lock at once, we only allow
 * a read-lock owner to read-lock recursively. This is
 * better for latency, makes the implementation inherently
 * fair and makes it simpler as well:
 */
struct rw_semaphore {
	struct rt_mutex		lock;
	int			read_depth;
};

/*
 * rwlocks - an RW semaphore plus lock-break field:
 */
typedef struct {
	struct rw_semaphore	lock;
	unsigned int		break_lock;
} rwlock_t;

# ifdef CONFIG_RT_DEADLOCK_DETECT
#  define __RW_LOCK_UNLOCKED \
	.wait_lock = __RAW_SPIN_LOCK_UNLOCKED, .save_state = 1, \
	.debug = .1, .file = __FILE__, .line = __LINE__
#  define _RW_LOCK_UNLOCKED(lock) \
	(rwlock_t) { { { __RW_LOCK_UNLOCKED, .name = #lock } } }
#  define RW_LOCK_UNLOCKED \
	(rwlock_t) { { { __RW_LOCK_UNLOCKED } } }
# else
#  define RW_LOCK_UNLOCKED (rwlock_t) \
	{ { { .wait_lock = __RAW_SPIN_LOCK_UNLOCKED } } }
#  define _RW_LOCK_UNLOCKED(lock) RW_LOCK_UNLOCKED
# endif
#else /* !PREEMPT_RT */
  typedef raw_rwlock_t rwlock_t;
# define _RW_LOCK_UNLOCKED(lock)	RAW_RW_LOCK_UNLOCKED
# define RW_LOCK_UNLOCKED		RAW_RW_LOCK_UNLOCKED
#endif

#ifdef CONFIG_PREEMPT_RT

/*
 * spinlocks - an RT mutex plus lock-break field:
 */
typedef struct {
	struct rt_mutex lock;
	unsigned int break_lock;
} spinlock_t;

#ifdef CONFIG_RT_DEADLOCK_DETECT
# define __SPIN_LOCK_UNLOCKED \
	.wait_lock = __RAW_SPIN_LOCK_UNLOCKED, \
	.save_state = 1, .debug = 1, .file = __FILE__, .line = __LINE__
# define _SPIN_LOCK_UNLOCKED(lock) \
	(spinlock_t) { { __SPIN_LOCK_UNLOCKED, .name = #lock } }
# define SPIN_LOCK_UNLOCKED \
	(spinlock_t) { { __SPIN_LOCK_UNLOCKED } }
#else
# define SPIN_LOCK_UNLOCKED \
	(spinlock_t) { { .wait_lock = __RAW_SPIN_LOCK_UNLOCKED } }
# define _SPIN_LOCK_UNLOCKED(lock) SPIN_LOCK_UNLOCKED
#endif
#else /* !PREEMPT_RT */
  typedef raw_spinlock_t spinlock_t;
# define _SPIN_LOCK_UNLOCKED(lock)	RAW_SPIN_LOCK_UNLOCKED
# define SPIN_LOCK_UNLOCKED		RAW_SPIN_LOCK_UNLOCKED
#endif


#ifdef CONFIG_PREEMPT_RT

/*
 * semaphores - an RT-mutex plus the semaphore count:
 */
struct semaphore {
	atomic_t count;
	struct rt_mutex lock;
};

/*
 * Semaphores:
 */
#define __MUTEX_INITIALIZER(name) \
        { .count = { 1 }, .lock = __RT_MUTEX_INITIALIZER(name.lock) }

#define DECLARE_MUTEX(name) \
struct semaphore name = \
	{ .count = { 1 }, .lock = __RT_MUTEX_INITIALIZER(name.lock) }

#define DECLARE_MUTEX_NOCHECK(name) \
struct semaphore name = \
	{ .count = { 1 }, .lock = __RT_MUTEX_INITIALIZER_NOCHECK(name.lock) }

/*
 * DECLARE_MUTEX_LOCKED() is deprecated: very hard to initialize properly
 * and it also often signals abuse of semaphores.
 */

extern void FASTCALL(__sema_init(struct semaphore *sem, int val, int debug, char *name, char *file, int line));

#define sema_init(sem, val) \
		__sema_init(sem, val, 1, #sem, __FILE__, __LINE__)
#define sema_init_nocheck(sem, val) \
		__sema_init(sem, val, 0, #sem, __FILE__, __LINE__)
	
extern void FASTCALL(__init_MUTEX(struct semaphore *sem, char *name, char *file, int line));
extern void FASTCALL(__init_MUTEX_LOCKED(struct semaphore *sem, char *name, char *file, int line));
#define init_MUTEX(sem) \
		__init_MUTEX(sem, #sem, __FILE__, __LINE__)
#define init_MUTEX_LOCKED(sem) \
		__init_MUTEX_LOCKED(sem, #sem, __FILE__, __LINE__)
extern void FASTCALL(down(struct semaphore * sem));
extern int FASTCALL(down_interruptible(struct semaphore * sem));
extern int FASTCALL(down_trylock(struct semaphore * sem));
extern void FASTCALL(up(struct semaphore * sem));
extern int FASTCALL(sem_is_locked(struct semaphore *sem));
extern int FASTCALL(sema_count(struct semaphore * sem));


#define __RWSEM_INITIALIZER(lockname) \
	{ .lock = __RT_MUTEX_INITIALIZER(lockname.lock) }
#define __RWSEM_INITIALIZER_NOCHECK(lockname) \
	{ .lock = __RT_MUTEX_INITIALIZER_NOCHECK(lockname.lock) }

#define DECLARE_RWSEM(lockname) \
	struct rw_semaphore lockname = __RWSEM_INITIALIZER(lockname)

extern void FASTCALL(__init_rwsem(struct rw_semaphore *rwsem, int mutex,
				int debug, char *name, char *file, int line));

#define init_rwsem(sem) __init_rwsem(sem, 0, 1, #sem, __FILE__, __LINE__)

extern void FASTCALL(down_read(struct rw_semaphore *rwsem));

/*
 * trylock for reading -- returns 1 if successful, 0 if contention
 */
extern int FASTCALL(down_read_trylock(struct rw_semaphore *rwsem));

/*
 * lock for writing
 */
extern void FASTCALL(down_write(struct rw_semaphore *rwsem));
extern int FASTCALL(down_write_interruptible(struct rw_semaphore *rwsem));

/*
 * trylock for writing -- returns 1 if successful, 0 if contention
 */
extern int FASTCALL(down_write_trylock(struct rw_semaphore *rwsem));

/*
 * release a read lock
 */
extern void FASTCALL(up_read(struct rw_semaphore *rwsem));

/*
 * release a write lock
 */
extern void FASTCALL(up_write(struct rw_semaphore *rwsem));

/*
 * downgrade write lock to read lock
 */
extern void FASTCALL(downgrade_write(struct rw_semaphore *rwsem));

extern int FASTCALL(rwsem_is_locked(struct rw_semaphore *rwsem));

#endif /* CONFIG_PREEMPT_RT */

#endif

