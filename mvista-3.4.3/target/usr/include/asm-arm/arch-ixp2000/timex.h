/*
 * linux/include/asm-arm/arch-ixp2000/timex.h
 *
 * IXP2000 architecture timex specifications
 */


/*
 * Default clock is 50MHz APB, but platform code can override this
 */
#define CLOCK_TICK_RATE	50000000


#ifndef __ASSEMBLY__

static inline cycles_t get_cycles(void)
{
	cycles_t cycles;					

	__asm__ __volatile__("mrc p14, 0, %0, c1, c1, 0" : "=r" (cycles));

	return cycles;
}
#endif

