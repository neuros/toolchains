/*
 * Copyright (C) 1997 by the University of Kansas Center for Research,
 * Inc.  This software was developed by the Information and
 * Telecommunication Technology Center (ITTC) at the University of
 * Kansas.  Partial funding for this project was provided by Sprint. This
 * software may be used and distributed according to the terms of the GNU
 * Public License, incorporated herein by reference.  Neither ITTC nor
 * Sprint accept any liability whatsoever for this product.
 *
 * This project was developed under the direction of Dr. Douglas Niehaus.
 *
 * Authors: Shyam Pather, Balaji Srinivasan 
 *
 * Please send bug-reports/suggestions/comments to posix@ittc.ukans.edu
 *
 * Further details about this project can be obtained at
 *    http://hegel.ittc.ukans.edu/projects/posix/
 *
 * Currently part of the high-res-timers project:
 *         http://sourceforge.net/projects/high-res-timers/
 *
 */

/*********************** NOTES on this file.****************************
 *
 * This is a header file for use with the POSIX timers.  If you have a
 * current glibc, most of what is here is already defined and thus, not
 * needed. The only part you would need with the latest glibc (2.2.5)
 * for example, is the CLOCK_MONOTONIC, and the CLOCK_*_HR defines.
 *
 * When this code is integerated into glibc this file will go away.
 *
 * An effort was made to avoid errors because something is already defined
 * however, this is life, so there will always be some issues.  If anything
 * here collides with your normal libc header files, you can most likey
 * just comment out the offending parts.
 *
 ************************************************************************/



#ifndef __POSIX_TIME_H
#define __POSIX_TIME_H
#ifndef __USE_POSIX199506
#define __USE_POSIX199506
#endif
#include <signal.h>
#include <time.h>

#ifdef _POSIX_TIMERS

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME		 0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC  	 1
#endif
#ifndef CLOCK_PROCESS_CPUTIME_ID
#define CLOCK_PROCESS_CPUTIME_ID 2
#endif
#ifndef CLOCK_THREAD_CPUTIME_ID
#define CLOCK_THREAD_CPUTIME_ID	 3
#endif
#ifndef CLOCK_REALTIME_HR
#define CLOCK_REALTIME_HR	 4
#endif
#ifndef CLOCK_MONOTONIC_HR
#define CLOCK_MONOTONIC_HR       5
#endif

#define NOF_CLOCKS 10

#undef  TIMER_ABSTIME
#define TIMER_ABSTIME 0x01

#undef TIMER_MAX
#define TIMER_MAX 32000

#ifndef NSEC_PER_SEC
#define NSEC_PER_SEC 1000000000L
#endif

#if !defined __clockid_t_defined
typedef int clockid_t;
# define __clockid_t_defined    1
#endif

#ifndef __timer_t_defined
# define __timer_t_defined      1
typedef int timer_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif
#if 0        // fix confused pretty printer
}
#endif
/* POSIX.1b structure for timer start values and intervals.  */
/* If the following is already defined, just change the 1 to a 0 */
#define NEED_ITIMERSPEC
#ifndef NEED_ITIMERSPEC
struct itimerspec
  {
    struct timespec it_interval;
    struct timespec it_value;
  };
#endif
/*
 * Proto types can be repeated, so this should cause no errors even
 * if time.h already defined it.
*/

#ifndef __THROW
#define __THROW
#endif

extern long sysconf(int name);

extern int timer_create(clockid_t which_clock, 
                        struct sigevent *timer_event_spec,
                        timer_t *created_timer_id) __THROW;

extern int timer_gettime(timer_t timer_id, struct itimerspec *setting) __THROW;

extern int timer_settime(timer_t timer_id,
                         int flags,
                         const struct itimerspec *new_setting,
                         struct itimerspec *old_setting) __THROW;

extern int timer_getoverrun(timer_t timer_id) __THROW;

extern int timer_delete(timer_t timer_id) __THROW;

extern int clock_gettime(clockid_t which_clock, struct timespec *ts) __THROW;

extern int clock_settime(clockid_t which_clock, 
                         const struct timespec *setting) __THROW;

extern int clock_getres(clockid_t which_clock, 
                        struct timespec *resolution) __THROW;

extern int clock_nanosleep(clockid_t which_clock,
                    int flags,
                    const struct timespec *new_setting, 
                    struct timespec *old_setting) __THROW;
 

#ifdef __cplusplus
}
#endif

#endif /* POSIX_TIMERS */

#endif /* __POSIX_TIME_H */
