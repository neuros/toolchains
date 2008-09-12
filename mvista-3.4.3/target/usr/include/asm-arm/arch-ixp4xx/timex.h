/*
 * linux/include/asm-arm/arch-ixp4xx/timex.h
 * 
 */

#include <asm/hardware.h>

/*
 * We use IXP425 General purpose timer for our timer needs, it runs at 
 * 66.66... MHz. We do a convulted calculation of CLOCK_TICK_RATE b/c the
 * timer register ignores the bottom 2 bits of the LATCH value.
 */
#define FREQ 66666666
#define CLOCK_TICK_RATE (((FREQ / HZ & ~3) + 1) * HZ)

#ifndef __ASSEMBLY__

static inline cycles_t get_cycles(void)
{
	cycles_t cycles;					

	__asm__ __volatile__("mrc p14, 0, %0, c1, c1, 0" : "=r" (cycles));

	return cycles;
}
#endif

