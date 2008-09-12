/*
 * include/asm-arm/arch-versatile/hrtime.h
 *
 * Author: Manish Lachwani (mlachwani@mvista.com)
 * Copyright 2005 (c) MontaVista, Software, Inc. 
 * 
 * This file is licensed under  the terms of the GNU General Public 
 * License version 2. This program is licensed "as is" without any 
 * warranty of any kind, whether express or implied.
 */

/*
 * HRT support for ARM Versatile.
 */
#ifndef __ASM_ARCH_HRTIME_H_
#define __ASM_ARCH_HRTIME_H_
#ifdef CONFIG_HIGH_RES_TIMERS

#include <linux/config.h>
#include <linux/types.h>
#include <linux/timex.h>
#include <asm/hardware.h>

int schedule_hr_timer_int(unsigned, int);
int get_arch_cycles(unsigned);

#define	hrtimer_use			1
#define hr_time_resolution		10000
#define	arch_cycles_per_jiffy		(long)(CLOCK_TICK_RATE / HZ)
#define schedule_jiffies_int(x)		(get_arch_cycles(x) >= arch_cycles_per_jiffy)

#define arch_cycle_to_nsec(cycles)  \
	(((cycles) * ((1000000000LL << 32) / CLOCK_TICK_RATE)) >> 32)
                                                                                   
#define nsec_to_arch_cycle(nsec)  \
	(((nsec) * (((long long)CLOCK_TICK_RATE << 32) / 1000000000)) >> 32)

#define TIMER_RELOAD			(arch_cycles_per_jiffy - 1)

#endif	/* CONFIG_HIGH_RES_TIMERS */
#endif	/* __ASM_ARCH_HRTIME_H_ */
