/*
 * linux/include/asm-arm/arch-pxa/hrtime.h
 *
 * Author:	Nicolas Pitre
 * Created:	Jul 3, 2003
 * Copyright:	MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <asm/hardware.h>
#include <asm/arch/pxa-regs.h>

/*
 * The maximum resolution we can achieve is 1000000000 / 3686400 = 271.
 * Yet there seems to be a latency problem wwhile writing to the timer
 * regs in some cases where the write can take up to 6 timer ticks, (also
 * see comment in pxa_timer_interrupt() ), reducing this resolution to
 * around 2 usecs. 
 * Let give the system a bit of slack with 10000 (10 us) which should be
 * plenty anyway.
 */
#define hr_time_resolution  10000

#define arch_cycles_per_jiffy	LATCH

#define arch_cycle_to_nsec(cycles)  \
    (((cycles) * ((1000000000LL << 32) / CLOCK_TICK_RATE)) >> 32)

#define nsec_to_arch_cycle(nsec)  \
    (((nsec) * (((long long)CLOCK_TICK_RATE << 32) / 1000000000)) >> 32)

/* Note: OSMR0 is always the OSCR match value for jiffies+1 */
#define get_arch_cycles(ref)  ({  \
    unsigned long flags;  \
    long cycles;  \
    local_irq_save(flags);  \
    cycles = (jiffies - (ref) + 1) * arch_cycles_per_jiffy + OSCR - OSMR0;  \
    local_irq_restore(flags);  \
    cycles;  })

#define schedule_jiffies_int(x) (get_arch_cycles(x) >= arch_cycles_per_jiffy)
	
extern int schedule_hr_timer_int(unsigned ref_jiffies, int cycles);

#define hrtimer_use	1
