/*
 * include/linux/frd.h
 *
 * support for Fast Real Time Domain threads package.
 *
 * Author: Sven Thorsten Dietrich <sdietrich@mvista.com>
 *
 * 2005 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#ifndef _LINUX_FRD_H
#define _LINUX_FRD_H

#include <linux/config.h>

#ifdef CONFIG_FRD
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/kernel_stat.h>
#include <linux/proc_fs.h>
#include <asm/preempt.h>
#include <asm/frd.h>

#define FRD_THREADS CONFIG_FRD_THREADS
#define FRD_DELAY_TICKS CONFIG_FRD_DELAY * HZ /* OS ticks to wait before scheduling FRD */

#define MAXULL ((unsigned long long) ((long long) -1))

#define PREEMPT_LATENCY_TYPE 	0
#define SWITCH_LATENCY_TYPE 	1
#define SLEEP_LATENCY_TYPE 	2
#define EXEC_LATENCY_TYPE 	3
#define ABS_LATENCY_TYPE 	4
#define IRQ_LATENCY_TYPE 	5	
#define LATENCY_TYPES 		6

#ifdef CONFIG_FRD_BUCKET_SIZE
#define PLAT_BUCKET_SIZE    CONFIG_FRD_BUCKET_SIZE
#else
#define PLAT_BUCKET_SIZE     1
#endif

#define PLAT_BUCKETS         10000/PLAT_BUCKET_SIZE
#define PLAT_LAST_BUCKET     PLAT_BUCKETS
#define PLAT_UPPER_LIMIT     PLAT_BUCKETS * PLAT_BUCKET_SIZE

extern unsigned long diff_time(unsigned long long t_first, unsigned long long t_second);
extern void frd_latency_init(void);
extern void frd_latency_log(int frd_id, int type, unsigned long latency);
extern void (*check_frd_func_ptr)(void);
extern int frd_read_procmem(char *, char **, off_t, int, int *, void *);

typedef struct frd_param {
        int frd_id;
        int latency_type;
} frd_param_t;

typedef struct cpu_utils_struct {
        u64 user;
        u64 nice;
        u64 system;
        u64 softirq;
        u64 irq;
        u64 idle;
        u64 iowait;
} frd_cpu_utils_t;

typedef struct percent_struct {
        u64 high;
        u64 low;
} percent_t;

typedef struct cpu_util_percent_struct {
        percent_t user;
        percent_t nice;
        percent_t system;
        percent_t softirq;
        percent_t irq;
        percent_t idle;
        percent_t iowait;
} cpu_util_percent_t;

#define FRD_LATSIZE	256
extern int frd_latdat[FRD_LATSIZE];
extern int frd_latcnt;

# ifndef FRD_TIMER_IRQ_ACK
#  define FRD_TIMER_IRQ_ACK
# endif

# ifndef FRD_TIMER_INIT
#  define FRD_TIMER_INIT	0
# endif

# ifndef FRD_TIMER_LATCH
#  define FRD_TIMER_LATCH	0
# endif

# ifndef FRD_TIMER_START
#  define FRD_TIMER_START	/* default OS timer is started by kernel */
# endif

#endif
#endif
