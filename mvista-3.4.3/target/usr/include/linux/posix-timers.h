#ifndef _linux_POSIX_TIMERS_H
#define _linux_POSIX_TIMERS_H

/*
 * include/linux/posix-timers.h
 *
 *
 * 2003-7-7  Posix Clocks & timers 
 *                           by George Anzinger george@mvista.com
 *
 *			     Copyright (C) 2003 by MontaVista Software.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * MontaVista Software | 1237 East Arques Avenue | Sunnyvale | CA 94085 | USA 
 */
#include <linux/config.h>
#include <linux/hrtime.h>
#include <linux/spinlock.h>
#include <linux/list.h>

/* POSIX.1b interval timer structure. */
struct k_itimer {
	struct list_head list;		/* free/ allocate list */
	spinlock_t it_lock;
	clockid_t it_clock;		/* which timer type */
	timer_t it_id;			/* timer id */
	int it_overrun;			/* overrun on pending signal  */
	int it_overrun_last;		/* overrun on last delivered signal */
	int it_requeue_pending;         /* waiting to requeue this timer */
	int it_sigev_notify;		/* notify word of sigevent struct */
	int it_sigev_signo;		/* signo word of sigevent struct */
	sigval_t it_sigev_value;	/* value word of sigevent struct */
	unsigned long it_incr;		/* interval specified in jiffies */
#ifdef CONFIG_HIGH_RES_TIMERS
	int it_arch_cycle_incr;		/* arch_cycle part of interval */
#endif
	struct task_struct *it_process;	/* process to send signal to */
	struct timer_list it_timer;
	struct sigqueue *sigq;		/* signal queue entry. */
	struct list_head abs_timer_entry; /* clock abs_timer_list */
	struct timespec wall_to_prev;   /* wall_to_monotonic used when set */
};

struct k_clock_abs {
	struct list_head list;
	spinlock_t lock;
};
struct k_clock {
	int res;		/* in nano seconds */
	struct k_clock_abs *abs_struct;
	int (*clock_set) (struct timespec * tp);
	int (*clock_get) (struct timespec * tp);
	int (*timer_create) (struct k_itimer *timer);
	int (*nsleep) (int which_clock, int flags,
		       struct timespec * t);
	int (*timer_set) (struct k_itimer * timr, int flags,
			  struct itimerspec * new_setting,
			  struct itimerspec * old_setting);
	int (*timer_del) (struct k_itimer * timr);
	void (*timer_get) (struct k_itimer * timr,
			   struct itimerspec * cur_setting);
};

void register_posix_clock(int clock_id, struct k_clock *new_clock);

/* Error handlers for timer_create, nanosleep and settime */
int do_posix_clock_notimer_create(struct k_itimer *timer);
int do_posix_clock_nonanosleep(int which_clock, int flags, struct timespec * t);
int do_posix_clock_nosettime(struct timespec *tp);

/* function to call to trigger timer event */
int posix_timer_event(struct k_itimer *timr, int si_private);

#if defined(CONFIG_HIGH_RES_TIMERS)

struct now_struct {
	unsigned long jiffies;
	long arch_cycle;
};
#define get_expire(now, timr) do {\
                  (now)->jiffies = (timr)->it_timer.expires; \
                  (now)->arch_cycle = (timr)->it_timer.arch_cycle_expires;}while (0)
#define put_expire(now, timr) do { \
                  (timr)->it_timer.expires = (now)->jiffies; \
                  (timr)->it_timer.arch_cycle_expires = (now)->arch_cycle;}while (0)
#define sub_now(now, then) do{ \
	          (now)->jiffies -= (then)->jiffies; \
                  (now)->arch_cycle -= (then)->arch_cycle; \
                  full_normalize_jiffies_now(now);} while (0)
static inline void
normalize_jiffies(unsigned long *jiff, long *arch_cycle)
{
	while ((*(arch_cycle) - arch_cycles_per_jiffy) >= 0) {
		*(arch_cycle) -= arch_cycles_per_jiffy;
		(*(jiff))++;
	}
}
static inline void
full_normalize_jiffies(unsigned long *jiff, long *arch_cycle)
{
	normalize_jiffies(jiff, arch_cycle);
	while (*(arch_cycle) < 0) {
		*(arch_cycle) += arch_cycles_per_jiffy;
		(*(jiff))--;
	}
}
#define normalize_jiffies_now(now) \
	normalize_jiffies(&(now)->jiffies, &(now)->arch_cycle);
#define full_normalize_jiffies_now(now) \
	full_normalize_jiffies(&(now)->jiffies, &(now)->arch_cycle);

/*
 * The following locking assumes that irq off.
 */
static inline void
posix_get_now(struct now_struct *now)
{
	unsigned long seq;
	
	(now)->jiffies = jiffies;
	do {
		seq = read_seqbegin(&xtime_lock);
		(now)->arch_cycle = get_arch_cycles((now)->jiffies);
	} while (read_seqretry(&xtime_lock, seq));
	
	normalize_jiffies_now(now); 
}

static inline int
posix_time_before (const struct timer_list* t, const struct now_struct* now)
{
	const long diff = (long)t->expires - (long)now->jiffies;

	return (diff < 0) 
		|| ((diff == 0) && (t->arch_cycle_expires < now->arch_cycle)); 
}

#define posix_bump_timer(timr, now) _posix_bump_timer(timr, now)
#if 0
static void _posix_bump_timer(struct k_itimer * timr, struct now_struct *now)
{ 
	struct now_struct tmp;
	u64 div = (u64)(timr)->it_incr * arch_cycles_per_jiffy +
		(timr)->it_arch_cycle_incr;
	u64 delta;
	unsigned long orun = 1;
	long arch_cycle;
		
	tmp.jiffies = now->jiffies - (timr)->it_timer.expires;
	tmp.arch_cycle = now->arch_cycle - (timr)->it_timer.arch_cycle_expires;
	full_normalize_jiffies_now(&tmp);
	if ((long)tmp.jiffies < 0) 
		return;
	delta = (u64)tmp.jiffies * arch_cycles_per_jiffy + tmp.arch_cycle;
	/*
	 * We need to div a long long by a long long to get a long
	 * result.  Such a mess.  First test if it is needed at all.
	 */
	if (delta > div) {   
		/*
		 * If we are dividing by less than 2^32 use do_div
		 * else reduce the num and denom to make it so.
		 */
		u64 sdiv = div, delta_i = delta;
		int shift = 0;

		while (sdiv >> 32) {
			sdiv >>= 1;
			shift += 1;
		};
		delta >>= shift;
		do_div(delta, (u32)sdiv);
		orun = (u32)delta;
		delta = orun * div;
		if (delta <= delta_i) {
			delta += div;
			orun++;
		}
	} else {
		delta = div;
	}
	(timr)->it_timer.expires += 
		div_long_long_rem(delta, 
				  arch_cycles_per_jiffy, 
				  &arch_cycle);
	(timr)->it_timer.arch_cycle_expires += arch_cycle;
	normalize_jiffies(&(timr)->it_timer.expires,
			  &(timr)->it_timer.arch_cycle_expires);
	(timr)->it_overrun += orun;
}
#else
#define __posix_bump_timer(timr) do {				\
          (timr)->it_timer.expires += (timr)->it_incr;		\
          (timr)->it_timer.arch_cycle_expires += (timr)->it_arch_cycle_incr;	\
          normalize_jiffies(&((timr)->it_timer.expires),		\
			  &((timr)->it_timer.arch_cycle_expires));	\
          (timr)->it_overrun++;					\
        }while (0)
#define _posix_bump_timer(timr, now) do {  __posix_bump_timer(timr); \
                  } while  (posix_time_before(&((timr)->it_timer), now))
#endif
#else /* defined(CONFIG_HIGH_RES_TIMERS) */

struct now_struct {
	unsigned long jiffies;
};
#define get_expire(now, timr) (now)->jiffies = (timr)->it_timer.expires
#define put_expire(now, timr) (timr)->it_timer.expires = (now)->jiffies
#define sub_now(now, then)    (now)->jiffies -= (then)->jiffies

#define posix_get_now(now) (now)->jiffies = jiffies;
#define posix_time_before(timer, now) \
                      time_before((timer)->expires, (now)->jiffies)

#define posix_bump_timer(timr, now)					\
         do {								\
              long delta, orun;						\
	      delta = (now)->jiffies - (timr)->it_timer.expires;	\
              if (delta >= 0) {						\
	           orun = 1 + (delta / (timr)->it_incr);		\
	          (timr)->it_timer.expires += orun * (timr)->it_incr;	\
                  (timr)->it_overrun += orun;				\
              }								\
            }while (0)
#endif /* defined(CONFIG_HIGH_RES_TIMERS) */
#endif
