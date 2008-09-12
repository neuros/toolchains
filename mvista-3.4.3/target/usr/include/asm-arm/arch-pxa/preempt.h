/*
 * include/asm-arm/arch-pxa/preem_latency.h
 *
 * Timing support for preempt-stats patch.
 *
 * Author: <source@mvista.com>
 *
 * 2003-2004 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 */
#include <asm/arch/pxa-regs.h>
#include "hardware.h"

static inline unsigned long clock_diff(unsigned long start, unsigned long stop)
{
        return (stop - start);
}

#define readclock_init()
#define readclock()		(OSCR)
#define INTERRUPTS_ENABLED(x)   (!((x) & PSR_I_BIT))	/* Assuming x is PSR, 
							 * check for I bit
							 */
#if defined(CONFIG_PXA27x)
/* On the Bulverde, OSCR0 increments at the rising edge of 3.25 MHz clock */
#define clock_to_usecs(x)       (((x) * 4) / 13)
#define TICKS_PER_USEC          (3)
#else
#define clock_to_usecs(x)	((x) * 136 / 501)	/* 501/136 ~= 3.6838, but we 
							 * have 3.6864 
							 */
#define TICKS_PER_USEC          (4)
#endif				/* defined(CONFIG_PXA27x) */
