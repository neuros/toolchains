/*
 *  include/asm-arm/arch-omap/frd.h
 *
 * Author: Sven Thorsten Dietrich <sdietrich@mvista.com>
 *
 * 2005 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#ifndef _ASM_ARCH_FRD_H
#define _ASM_ARCH_FRD_H

#include <linux/config.h>
#include <asm/arch/preempt.h>
#include <asm/arch/timex.h>

#if defined(CONFIG_ARCH_OMAP16XX) || defined(CONFIG_ARCH_OMAP1510)
# define FRD_TIMER_IRQ 	INT_TIMER3
#elif defined(CONFIG_ARCH_OMAP730)
# define FRD_TIMER_IRQ 	INT_730_MPU_EXT_NIRQ
#else
# define FRD_TIMER_IRQ 	TIMER_IRQ
#endif

/*
 * Let's setup mpu timer3 at the 4.615ms frequency for thread wakeup
 * At 6Mhz it's 4615 * 6 (0x6c2a).
 */
#define MPU_4_POINT_615_MS	0x6c2a

extern void omap_mpu_timer_start(int nr, unsigned long load_val);

/* These are used for absolute latency measurements */
#if defined CONFIG_FRD_USE_TIMER_IRQ
# define FRD_MPU_TIMER		1

# define FRD_TIMER_INIT		(CLOCK_TICK_RATE / HZ) - 1
# define FRD_TIMER_LATCH	omap_mpu_timer_read(FRD_MPU_TIMER)

#else
# define FRD_MPU_TIMER		2

# define FRD_TIMER_INIT		MPU_4_POINT_615_MS
# define FRD_TIMER_LATCH	omap_mpu_timer_read(FRD_MPU_TIMER)
# define FRD_TIMER_START	omap_mpu_timer_start(FRD_MPU_TIMER, FRD_TIMER_INIT)

#endif

#define FRD_SCALE_ABS_TICKS 	1
/* arm counters are count-down type */
#define FRD_TIMER_COUNTS_DOWN 	1 

/* frd default clock function using sched_clock  */
static inline unsigned long long frd_clock(void)
{
        return ~readclock();
}
#endif

