/*
 * include/asm-arm/arch-ixp2000/frd.h
 *
 * IXP2000 FRD implementation
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
#define	FRD_TIMER_LATCH		(*IXP2000_T1_CSR)

#else

#define	FRD_TIMER_IRQ		IRQ_IXP2000_TIMER2
#define	FRD_TIMER_LATCH		(*IXP2000_T2_CSR)
#define	FRD_TIMER_START							\
	do {								\
		unsigned long ticks_per_jiffy;				\
		ticks_per_jiffy = (ixp2000_tick_rate + HZ/2) / HZ;	\
									\
		ixp2000_reg_write(IXP2000_T2_CLR, 0);			\
		ixp2000_reg_write(IXP2000_T2_CLD, ticks_per_jiffy);	\
		ixp2000_reg_write(IXP2000_T2_CTL, (1 << 7));		\
	} while (0)
#define	FRD_TIMER_IRQ_ACK	ixp2000_reg_write(IXP2000_T2_CLR, 1);

#endif

static inline unsigned long frd_clock(void)
{
	return readclock();
}

#endif
