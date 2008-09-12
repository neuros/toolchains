/*
 * asm-arm/arch-versatile/preempt.h
 *
 * Copyright (C) 2004 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _ASM_ARCH_PREEMT_H
#define _ASM_ARCH_PREEMT_H

static inline unsigned long clock_diff(unsigned long start, unsigned long stop)
{
	return (stop - start);
}

extern unsigned long versatile_timer_read(int nr);

/*
 * timer 1 runs @ 1Mhz, 1 tick = 1 microsecond
 * and is configured as a count down timer.
 */

#define TICKS_PER_USEC		1
#define ARCH_PREDEFINES_TICKS_PER_USEC
#define readclock()		(~versatile_timer_read(1))
#define clock_to_usecs(x)	((x) / TICKS_PER_USEC)
#define INTERRUPTS_ENABLED(x)	(!(x & PSR_I_BIT))

#endif

