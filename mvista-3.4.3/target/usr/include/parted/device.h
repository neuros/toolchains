/*
    libparted - a library for manipulating disk partitions
    Copyright (C) 1998, 1999, 2000, 2001 Free Software Foundation, Inc.

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

#ifndef PED_DEVICE_H_INCLUDED
#define PED_DEVICE_H_INCLUDED

#include <parted/timer.h>

#define PED_SECTOR_SIZE		512
typedef long long PedSector;

typedef enum {
	PED_DEVICE_UNKNOWN	= 0,
	PED_DEVICE_SCSI		= 1,
	PED_DEVICE_IDE		= 2,
	PED_DEVICE_DAC960	= 3,
	PED_DEVICE_CPQARRAY	= 4,
	PED_DEVICE_FILE		= 5,
	PED_DEVICE_ATARAID	= 6,
	PED_DEVICE_I2O		= 7
} PedDeviceType;

typedef struct _PedDevice PedDevice;
typedef struct _PedDeviceArchOps PedDeviceArchOps;
typedef struct _PedCHSGeometry PedCHSGeometry;

struct _PedCHSGeometry {
	int		cylinders;
	int		heads;
	int		sectors;
};

/* A hard disk device - eg. /dev/hda, not /dev/hda3 */
struct _PedDevice {
	PedDevice*	next;

	char*		model;			/* description of hardware */
	char*		path;			/* device /dev entry */

	PedDeviceType	type;			/* SCSI, IDE, etc. */
	int		sector_size;
	PedSector	length;

	int		open_count;
	int		read_only;
	int		external_mode;
	int		dirty;
	int		boot_dirty;

	PedCHSGeometry	hw_geom;
	PedCHSGeometry	bios_geom;
	short		host, did;

	void*		arch_specific;
};

struct _PedDeviceArchOps {
	PedDevice* (*_new) (const char* path);
	void (*destroy) (PedDevice* dev);
	int (*is_busy) (PedDevice* dev);
	int (*open) (PedDevice* dev);
	int (*refresh_open) (PedDevice* dev);
	int (*close) (PedDevice* dev);
	int (*refresh_close) (PedDevice* dev);
	int (*read) (PedDevice* dev, void* buffer,
		     PedSector start, PedSector count);
	int (*write) (PedDevice* dev, const void* buffer,
		      PedSector start, PedSector count);
	int (*sync) (PedDevice* dev);
	PedSector (*check) (PedDevice* dev, void* buffer,
			    PedSector start, PedSector count);
	void (*probe_all) ();
};

extern void ped_device_probe_all ();
extern void ped_device_free_all ();

extern PedDevice* ped_device_get (const char* name);
extern PedDevice* ped_device_get_next (const PedDevice* dev);
extern int ped_device_is_busy (PedDevice* dev);
extern int ped_device_open (PedDevice* dev);
extern int ped_device_close (PedDevice* dev);
extern void ped_device_destroy (PedDevice* dev);

extern int ped_device_begin_external_access (PedDevice* dev);
extern int ped_device_end_external_access (PedDevice* dev);

extern int ped_device_read (PedDevice* dev, void* buffer,
			    PedSector start, PedSector count);
extern int ped_device_write (PedDevice* dev, const void* buffer,
			     PedSector start, PedSector count);
extern int ped_device_sync (PedDevice* dev);
extern PedSector ped_device_check (PedDevice* dev, void* buffer,
				   PedSector start, PedSector count);

/* private stuff ;-) */

extern void _ped_device_probe (const char* path);

#endif /* PED_DEVICE_H_INCLUDED */

