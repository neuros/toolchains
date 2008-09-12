/* System dependent definitions for run-time dynamic loading.
   Copyright (C) 1996,1997,1998,1999,2000,2001 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _DLFCN_H
# error "Never use <bits/dlfcn.h> directly; include <dlfcn.h> instead."
#endif

/* The MODE argument to `dlopen' contains one of the following: */
#define RTLD_LAZY	0x00001	/* Lazy function call binding.  */
#define RTLD_NOW	0x00002	/* Immediate function call binding.  */
#define	RTLD_BINDING_MASK   0x3	/* Mask of binding time value.  */
#define RTLD_NOLOAD	0x00004	/* Do not load the object.  */

/* If the following bit is set in the MODE argument to `dlopen',
   the symbols of the loaded object and its dependencies are made
   visible as if the object were linked directly into the program.  */
#define RTLD_GLOBAL	0x00100

/* Lock the pages of the loaded library into memory so they won't be
   demand paged into memory.  If you choose this, you need to also
   choose one of RTLD_LOCK_MLOCK or RTLD_LOCK_READ below. */
#define RTLD_LOCK_LIB_PAGES	0x200

/* If the following bit is set in the MODE argument to `dlopen',
   the dependent files will also have their pages locked into memory. */
#define RTLD_LOCK_DEPENDENT_LIB_PAGES	0x400

/* If the following is specified in the MODE argument to `dlopen',
   the file will be read into memory but not mapped directly from
   the file.  This leave the file unused, thus it can be deleted,
   changed, etc. without any side effects.  However, the loaded
   pages cannot be shared with other processes. */
#define RTLD_LOCK_READ	0x000

/* If the following is specified in the MODE argument to `dlopen',
   the file's contents will be mlock()-ed into memory so they will
   not be demand paged or paged out.  You must be root to do this. */
#define RTLD_LOCK_MLOCK	0x800

/* Mask for the above two options. */
#define RTLD_LOCK_MASK	0x800

/* Unix98 demands the following flag which is the inverse to RTLD_GLOBAL.
   The implementation does this by default and so we can define the
   value to zero.  */
#define RTLD_LOCAL	0

/* Do not delete object when closed.  */
#define RTLD_NODELETE	0x01000

#ifdef __USE_GNU
/* To support profiling of shared objects it is a good idea to call
   the function found using `dlsym' using the following macro since
   these calls do not use the PLT.  But this would mean the dynamic
   loader has no chance to find out when the function is called.  The
   macro applies the necessary magic so that profiling is possible.
   Rewrite
	foo = (*fctp) (arg1, arg2);
   into
        foo = DL_CALL_FCT (fctp, (arg1, arg2));
*/
# define DL_CALL_FCT(fctp, args) \
  (_dl_mcount_wrapper_check ((void *) (fctp)), (*(fctp)) args)

__BEGIN_DECLS

/* This function calls the profiling functions.  */
extern void _dl_mcount_wrapper_check (void *__selfpc) __THROW;

__END_DECLS

#endif
