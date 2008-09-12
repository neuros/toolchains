/*
 * include/asm-arm/arch-omap/preempt.h
 *
 * Timing support for preempt-stats, kfi, ilatency patches
 *
 * Author: dsingleton <dsingleton@mvista.com>
 *
 * 2001-2004 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#ifndef _ASM_ARCH_PREEMT_H
#define _ASM_ARCH_PREEMT_H

extern unsigned long omap_mpu_timer_read(int timer);
static inline unsigned long clock_diff(unsigned long start, unsigned long stop)
{
        return (start - stop);
}

/* 
 * mputimer 1 runs @ 6Mhz  6 ticks = 1 microsecond 
 * and is configed as a count down timer.
 */
#define TICKS_PER_USEC		6
#define ARCH_PREDEFINES_TICKS_PER_USEC
#define readclock()		omap_mpu_timer_read(0)
#define clock_to_usecs(x)	((x) / 6)
#define INTERRUPTS_ENABLED(x)   (!(x & PSR_I_BIT))

#endif

