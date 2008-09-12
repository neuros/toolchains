/*
 *  include/asm-arm/arch-pxa/frd.h
 *
 * Author: Alexey Makarov <amakarov@ru.mvista.com>
 *
 * 2005 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#ifndef _ASM_ARCH_FRD_H
#define _ASM_ARCH_FRD_H

#include <linux/config.h>
#include <asm/arch/timex.h>

#ifndef CONFIG_FRD_USE_TIMER_IRQ

# define FRD_TIMER_IRQ		IRQ_OST_4_11
# define FRD_TIMER_IRQ_ACK	do { OSSR = (1 << 5); } while(0)
# define FRD_TIMER_LATCH	OSCR5

# define FRD_TIMER_START	\
        OMCR5 = OMCR_C | OMCR_P | OMCR_R | OMCR_CRES_USEC; 	\
        OIER |= OIER_E5;				   	\
        OSMR5 = 4615; /* 216 Hz, as in omap */			\
        OSCR5 = 0;						\
        printk(KERN_INFO "FRD timer started\n");

#else

# define FRD_TIMER_IRQ		IRQ_OST0

#endif

/* frd default clock function using sched_clock  */
static inline unsigned long long frd_clock(void)
{
        return readclock();
}

#endif

