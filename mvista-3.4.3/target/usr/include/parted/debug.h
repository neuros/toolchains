/*
    libparted - a library for manipulating disk partitions
    Copyright (C) 1998, 1999, 2000, 2002 Free Software Foundation, Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef PED_DEBUG_H_INCLUDED
#define PED_DEBUG_H_INCLUDED

#include "config.h"

#ifdef DEBUG

extern int
ped_assert (int cond, const char* cond_text,
	    const char* file, int line, const char* function);

#ifdef __GNUC__

#define PED_ASSERT(cond, action)				\
	do {							\
	if (!ped_assert (cond,					\
			 #cond,					\
			 __FILE__,				\
			 __LINE__,				\
			 __PRETTY_FUNCTION__))			\
	{							\
		action;						\
	}							\
	} while (0)

#else /* !__GNUC__ */

#define PED_ASSERT(cond, action)				\
	do {							\
	if (!ped_assert (cond,					\
			 #cond,					\
			 "unknown",				\
			 0,					\
			 "unknown"))				\
	{							\
		action;						\
	}							\
	} while (0)


#endif /* __GNUC__ */

#else /* !DEBUG */

#define PED_ASSERT(cond, action)	while (0) {}

#endif /* DEBUG */

#endif /* PED_DEBUG_H_INCLUDED */

