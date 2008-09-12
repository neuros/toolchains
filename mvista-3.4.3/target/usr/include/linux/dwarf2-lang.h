#ifndef DWARF2_LANG
#define DWARF2_LANG

/*
 * This is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2, or (at your option) any later
 * version.
 */
/*
 * This file defines macros that allow generation of DWARF debug records
 * for asm files.  This file is platform independent.  Register numbers
 * (which are about the only thing that is platform dependent) are to be
 * supplied by a platform defined file.
 */
/*
 * We need this to work for both asm and C.  In asm we are using the 
 * old comment trick to concatenate while C uses the new ANSI thing.
 * Here we have concat macro...  The multi level thing is to allow and
 * macros used in the names to be resolved prior to the cat (at which
 * time they are no longer the same string).
 */
#define CAT3(a,b,c) _CAT3(a,b,c)
#define _CAT3(a,b,c) __CAT3(a,b,c)
#ifndef __STDC__
#define __CAT3(a,b,c) a/**/b/**/c
#else
#define __CAT3(a,b,c) a##b##c
#endif
#ifdef __ASSEMBLY__
#define IFC(a)
#define IFN_C(a) a
#define NL ;
#define QUOTE_THIS(a) a 
#define DWARF_preamble .section .debug_frame,"",%progbits;
#else
#define IFC(a) a 
#define IFN_C(a)
#define NL \n\t
#define QUOTE_THIS(a) _QUOTE_THIS(a)
#define _QUOTE_THIS(a) #a
/* Don't let CPP see the " and , \042=" \054=, */
#define DWARF_preamble .section .debug_frame \054\042\042\054%progbits
#endif

#ifdef CONFIG_64BIT
#define DATA_ALIGN_FACTOR	8
#define ADDR_LOC		.quad
#else
#define DATA_ALIGN_FACTOR	4
#define ADDR_LOC		.long
#endif

#include <linux/dwarf2-defs.h>
/*
 * This macro starts a debug frame section.  The debug_frame describes
 * where to find the registers that the enclosing function saved on
 * entry.
 *
 * ORD is use by the label generator and should be the same as what is
 * passed to CFI_postamble.
 *
 * pc,	pc register gdb ordinal.
 *
 * code_align this is the factor used to define locations or regions
 * where the given definitions apply.  If you use labels to define these
 * this should be 1.
 *
 * data_align this is the factor used to define register offsets.  If
 * you use struct offset, this should be the size of the register in
 * bytes or the negative of that.  This is how it is used: you will
 * define a register as the reference register, say the stack pointer,
 * then you will say where a register is located relative to this
 * reference registers value, say 40 for register 3 (the gdb register
 * number).  The <40> will be multiplied by <data_align> to define the
 * byte offset of the given register (3, in this example).  So if your
 * <40> is the byte offset and the reference register points at the
 * begining, you would want 1 for the data_offset.  If <40> was the 40th
 * 4-byte element in that structure you would want 4.  And if your
 * reference register points at the end of the structure you would want
 * a negative data_align value(and you would have to do other math as
 * well).
 */

#define CFI_preamble(ORD, pc, code_align, data_align)	\
         DWARF_preamble	NL				\
	.align DATA_ALIGN_FACTOR NL			\
        .globl CAT3(frame,_,ORD) NL			\
CAT3(frame,_,ORD): NL					\
	.long 7f-6f NL					\
6:							\
	.long	DW_CIE_ID NL				\
	.byte	DW_CIE_VERSION NL			\
	.byte 0	 NL					\
	.uleb128 code_align NL				\
	.sleb128 data_align NL				\
	.byte pc NL

/*
 * After the above macro and prior to the CFI_postamble, you need to
 * define the initial state.  This starts with defining the reference
 * register and, usually the pc.  Here are some helper macros:
 */

#define CFA_define_reference(reg, offset)	\
	.byte DW_CFA_def_cfa NL			\
	.uleb128 reg NL				\
	.uleb128 (offset) NL

#define CFA_define_offset(reg, offset)		\
	.byte (DW_CFA_offset + reg) NL		\
	.uleb128 (offset) NL

#define CFA_restore(reg)			\
        .byte (DW_CFA_restore + reg) NL

#define CFI_postamble()				\
	.align DATA_ALIGN_FACTOR NL				\
7: NL						\
.previous NL

/*
 * So now your code pushs stuff on the stack, you need a new location
 * and the rules for what to do.  This starts a running description of
 * the call frame.  You need to describe what changes with respect to
 * the call registers as the location of the pc moves through the code.
 * The following builds an FDE (fram descriptor entry?).  Like the
 * above, it has a preamble and a postamble.  It also is tied to the CFI
 * above.
 * The preamble macro is tied to the CFI thru the first parameter.  The 
 * second is the code start address and then the code end address+1.
 */
//         DWARF_preamble() NL				
#define FDE_preamble(ORD, initial_address, end_address)	\
        DWARF_preamble NL				\
	.align DATA_ALIGN_FACTOR NL					\
	.long 9f-8f NL					\
8:							\
	.long CAT3(frame,_,ORD) NL			\
	ADDR_LOC initial_address NL			\
	ADDR_LOC (end_address - initial_address) NL

#define FDE_postamble()				\
	.align DATA_ALIGN_FACTOR NL				\
9:	 NL					\
.previous NL

/*
 * That done, you can now add registers, subtract registers, move the
 * reference and even change the reference.  You can also define a new
 * area of code the info applies to.  For discontinuous bits you should
 * start a new FDE.  You may have as many as you like.
 */

/*
 * To advance the stack address by <bytes> (0x3f max)
 */

#define CFA_advance_loc(bytes)			\
	.byte DW_CFA_advance_loc+bytes NL

/*
 * This one is good for 0xff or 255
 */
#define CFA_advance_loc1(bytes)			\
	.byte DW_CFA_advance_loc1 NL		\
        .byte bytes NL

#define CFA_undefine_reg(reg)			\
        .byte DW_CFA_undefined NL		\
	.uleb128 reg NL
/*
 * With the above you can define all the register locations.  But
 * suppose the reference register moves... Takes the new offset NOT an
 * increment.  This is how esp is tracked if it is not saved.
 */

#define CFA_define_cfa_offset(offset)		\
	.byte DW_CFA_def_cfa_offset NL		\
	.uleb128 (offset) NL
/*
 * Or suppose you want to use a different reference register...
 */
#define CFA_define_cfa_register(reg)		\
	.byte DW_CFA_def_cfa_register NL	\
	.uleb128 reg NL

/*
 * If you want to mess with the stack pointer, here is the expression.
 * The stack starts empty.
 */
#define CFA_def_cfa_expression 			\
        .byte DW_CFA_def_cfa_expression	NL	\
	.uleb128 20f-10f NL			\
10:     NL
/*
 * This expression is to be used for other regs.  The stack starts with the
 * stack address.
 */

#define CFA_expression(reg)			\
        .byte DW_CFA_expression	 NL		\
        .uleb128 reg NL				\
	.uleb128 20f-10f NL			\
10:     NL
/*
 * Here we do the expression stuff.  You should code the above followed
 *  by expression OPs followed by CFA_expression_end.
 */


#define CFA_expression_end			\
20:	 NL

#define CFA_exp_OP_const4s(a)			\
        .byte DW_OP_const4s NL			\
        .long a NL

#define  CFA_exp_OP_swap  .byte DW_OP_swap NL
#define  CFA_exp_OP_dup  .byte DW_OP_dup NL
#define  CFA_exp_OP_drop  .byte DW_OP_drop NL
/*
 * All these work on the top two elements on the stack, replacing them
 * with the result.  Top comes first where it matters.  True is 1, false 0.
 */
#define  CFA_exp_OP_deref .byte DW_OP_deref NL
#define  CFA_exp_OP_and   .byte DW_OP_and NL
#define  CFA_exp_OP_div   .byte DW_OP_div NL
#define  CFA_exp_OP_minus .byte DW_OP_minus NL
#define  CFA_exp_OP_mod   .byte DW_OP_mod NL
#define  CFA_exp_OP_neg   .byte DW_OP_neg NL
#define  CFA_exp_OP_plus  .byte DW_OP_plus NL
#define  CFA_exp_OP_not   .byte DW_OP_not NL
#define  CFA_exp_OP_or    .byte DW_OP_or NL
#define  CFA_exp_OP_xor   .byte DW_OP_xor NL
#define  CFA_exp_OP_le    .byte DW_OP_le NL
#define  CFA_exp_OP_ge    .byte DW_OP_ge NL
#define  CFA_exp_OP_eq    .byte DW_OP_eq NL
#define  CFA_exp_OP_lt    .byte DW_OP_lt NL
#define  CFA_exp_OP_gt    .byte DW_OP_gt NL
#define  CFA_exp_OP_ne    .byte DW_OP_ne NL
/*
 * These take a parameter as noted
 */
/*
 * Unconditional skip to loc. loc is a label (loc:) 
 */
#define CFA_exp_OP_skip(loc)			\
         .byte DW_OP_skip  NL 			\
	 .hword  loc-.-2 NL
/*
 * Conditional skip to loc (TOS != 0, TOS--) (loc is a label)
 */
#define CFA_exp_OP_bra(loc)			\
         .byte DW_OP_bra NL			\
	 .hword loc-.-2 NL

/*
 * TOS += no (an unsigned number)
 */
#define CFA_exp_OP_plus_uconst(no)		\
         .byte DW_OP_plus_uconst NL		\
         .uleb128 no NL

/*
 * ++TOS = no (a unsigned number)
 */
#define CFA_exp_OP_constu(no)			\
         .byte DW_OP_constu NL			\
	 .uleb128 no NL
/*
 * ++TOS = no (a signed number)
 */
#define CFA_exp_OP_consts(no)			\
         .byte DW_OP_consts NL			\
	 .sleb128 no NL
/*
 * ++TOS = no (an unsigned byte)
 */
#define CFA_exp_OP_const1u(no)			\
         .byte DW_OP_const1u NL			\
	 .byte no NL


/*
 * ++TOS = no (a address)
 */
#define CFA_exp_OP_addr(no)			\
         .byte DW_OP_addr NL			\
	 .long no NL

/*
 * Push current frames value for "reg" + offset
 * We take advantage of the opcode assignments to make this a litteral reg
 * rather than use the DW_OP_bregx opcode.
 */

#define CFA_exp_OP_breg(reg,offset)		\
         .byte DW_OP_breg0+reg NL		\
         .sleb128 offset NL
#endif
