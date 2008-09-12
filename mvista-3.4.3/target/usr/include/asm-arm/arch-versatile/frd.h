/*
 * include/asm-arm/arch-versatile/frd.h
 *  FRD Support on ARM Versatile.
 *
 * Author: Manish Lachwani <mlachwani@mvista.com>
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
#include <asm/arch/preempt.h>
#include <asm/arch/platform.h>

#define FRD_TIMER_IRQ		IRQ_TIMERINT2_3

/* frd default clock function using sched_clock  */
static inline unsigned long long frd_clock(void)
{
        return readclock();
}

#endif /* _ASM_ARCH_FRD_H */

