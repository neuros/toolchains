/*
 * include/asm-arm/hrtime.h
 *
 * IXP2000 hooks for HRT
 *
 * Author: Deepak Saxena <dsaxena@mvista.com>
 *
 * Copyright 2003-2005 (c) MontaVista, Software, Inc. 
 * 
 * This file is licensed under  the terms of the GNU General Public 
 * License version 2. This program is licensed "as is" without any 
 * warranty of any kind, whether express or implied.
 */


#ifndef __ASM_ARCH_HRTIME_H_
#define __ASM_ARCH_HRTIME_H_

#ifdef CONFIG_HIGH_RES_TIMERS

#include <linux/timex.h>

int schedule_hr_timer_int(unsigned, int);

int get_arch_cycles(unsigned);

int arch_cycle_to_nsec(int);

int nsec_to_arch_cycle(int);

void hr_time_init(void);

extern int arch_cycles_per_jiffy;

#define hr_time_resolution	(int)1000

#define schedule_jiffies_int(x)	0

#define	hrtimer_use		1

#else

#define hr_time_init() 

#endif	// CONFIG_HIGH_RES_TIMERS

#endif	// __ASM_ARCH_HRTIME_H_

