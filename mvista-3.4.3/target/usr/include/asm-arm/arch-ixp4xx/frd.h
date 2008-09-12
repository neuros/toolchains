/*
 * include/asm-arm/arch-omap/frd.h
 *
 * IXP4xx FRD implementation
 *
 * Author: Deepak Saxena <dsaxena@mvista.com>
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
#include <linux/timex.h>

#define	FRD_TIMER_INIT		LATCH
#define	FRD_TIMER_COUNTS_DOWN	1
#define	FRD_SCALE_ABS_TICKS	1

/*
 * Share the timer interrupt
 */
#ifdef	CONFIG_FRD_USE_TIMER_IRQ

#define	FRD_TIMER_IRQ		ARCH_TIMER_IRQ
#define	FRD_TIMER_LATCH		(*IXP4XX_OST1)

#else

#define	FRD_TIMER_IRQ		IRQ_IXP4XX_TIMER2
#define	FRD_TIMER_LATCH		(*IXP4XX_OST2)
#define	FRD_TIMER_START					\
	*IXP4XX_OSST = IXP4XX_OSST_TIMER_2_PEND;	\
	*IXP4XX_OSRT2 = (LATCH & ~IXP4XX_OST_RELOAD_MASK) | IXP4XX_OST_ENABLE
#define	FRD_TIMER_IRQ_ACK	(*IXP4XX_OSST = IXP4XX_OSST_TIMER_2_PEND)

#endif

static inline unsigned long long frd_clock(void)
{
	return (unsigned long long) readclock();
}

#endif
