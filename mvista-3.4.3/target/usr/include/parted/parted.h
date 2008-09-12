/*
    libparted - a library for manipulating disk partitions
    Copyright (C) 1999, 2000, 2001 Free Software Foundation, Inc.

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

#ifndef PARTED_H_INCLUDED
#define PARTED_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PedArchitecture PedArchitecture;

#include <parted/constraint.h>
#include <parted/device.h>
#include <parted/disk.h>
#include <parted/exception.h>
#include <parted/filesys.h>
#include <parted/natmath.h>

#include <stdlib.h>

struct _PedArchitecture {
	PedDiskArchOps*		disk_ops;
	PedDeviceArchOps*	dev_ops;
};

extern const PedArchitecture*	ped_architecture;

/* the architecture can't be changed if there are any PedDevice's.
 * i.e. you should only be doing this if it's the FIRST thing you do...
 */
extern int ped_set_architecture (const PedArchitecture* arch);

extern const char* ped_get_version ();

extern void* ped_malloc (size_t size);
extern int ped_realloc (void** ptr, size_t size);
extern void ped_free (void* ptr);

#ifdef __cplusplus
}
#endif

#endif /* PARTED_H_INCLUDED */
