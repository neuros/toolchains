/*
 * include/asm-generic/kgdb.h
 *
 * This provides the assembly level information so that KGDB can provide
 * a GDB that has been patched with enough information to know to stop
 * trying to unwind the function.
 *
 * Author: Tom Rini <trini@kernel.crashing.org>
 *
 * 2005 (c) MontaVista Software, Inc. This file is licensed under the terms
 * of the GNU General Public License version 2. This program is licensed
 * "as is" without any warranty of any kind, whether express or implied.
 */

#ifndef __ASM_GENERIC_KGDB_H__
#define __ASM_GENERIC_KGDB_H__

#include <linux/dwarf2-lang.h>
#ifdef __ASSEMBLY__
#ifdef CONFIG_KGDB
/* This MUST be put at the end of a given assembly function */
#define __CFI_END_FRAME(pc,sp,func)			\
CAT3(.Lend_,func,:)					\
	CFI_preamble(func,pc,0x1,-DATA_ALIGN_FACTOR)	\
	CFA_define_reference(sp, 0)			\
	CFI_postamble()					\
	FDE_preamble(func,func,CAT3(.Lend,_,func))	\
	FDE_postamble()
#else
#define __CFI_END_FRAME(pc,sp,fn)
#endif				/* CONFIG_KGDB */
#endif				/* __ASSEMBLY__ */
#endif				/* __ASM_GENERIC_KGDB_H__ */
