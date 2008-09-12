/*
 * Copyright (c) 2003 Bob Deblier
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*!\file mpnumber.h
 * \brief Multi-precision numbers, headers.
 * \author Bob Deblier <bob.deblier@pandora.be>
 * \ingroup MP_m
 */

#ifndef _MPNUMBER_H
#define _MPNUMBER_H

#include "mp.h"

typedef struct
{
	size_t	size;
	mpw*	data;
} mpnumber;

#ifdef __cplusplus
extern "C" {
#endif

BEECRYPTAPI
void mpnzero(mpnumber*);
BEECRYPTAPI
void mpnsize(mpnumber*, size_t);
BEECRYPTAPI
void mpninit(mpnumber*, size_t, const mpw*);
BEECRYPTAPI
void mpnfree(mpnumber*);
BEECRYPTAPI
void mpncopy(mpnumber*, const mpnumber*);
BEECRYPTAPI
void mpnwipe(mpnumber*);

BEECRYPTAPI
void mpnset   (mpnumber*, size_t, const mpw*);
BEECRYPTAPI
void mpnsetw  (mpnumber*, mpw);
BEECRYPTAPI
void mpnsethex(mpnumber*, const char*);

BEECRYPTAPI
int  mpninv(mpnumber*, const mpnumber*, const mpnumber*);

#ifdef __cplusplus
}
#endif

#endif
