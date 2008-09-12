/*
    libparted - a library for manipulating disk partitions
    Copyright (C) 1999, 2000, 2001, 2002 Free Software Foundation, Inc.

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

#ifndef PED_DISK_H_INCLUDED
#define PED_DISK_H_INCLUDED

typedef struct _PedDisk			PedDisk;
typedef struct _PedPartition		PedPartition;
typedef const struct _PedDiskOps	PedDiskOps;
typedef struct _PedDiskType		PedDiskType;
typedef const struct _PedDiskArchOps	PedDiskArchOps;

#include <parted/device.h>
#include <parted/filesys.h>
#include <parted/natmath.h>
#include <parted/geom.h>

typedef enum {
	PED_PARTITION_NORMAL		= 0x00,
	PED_PARTITION_LOGICAL		= 0x01,
	PED_PARTITION_EXTENDED		= 0x02,
	PED_PARTITION_FREESPACE		= 0x04,
	PED_PARTITION_METADATA		= 0x08
} PedPartitionType;

typedef enum {
	PED_PARTITION_BOOT=1,
	PED_PARTITION_ROOT=2,
	PED_PARTITION_SWAP=3,
	PED_PARTITION_HIDDEN=4,
	PED_PARTITION_RAID=5,
	PED_PARTITION_LVM=6,
	PED_PARTITION_LBA=7,
	PED_PARTITION_HPSERVICE=8,
	PED_PARTITION_PALO=9
} PedPartitionFlag;
#define PED_PARTITION_FIRST_FLAG	PED_PARTITION_BOOT
#define PED_PARTITION_LAST_FLAG		PED_PARTITION_PALO

typedef enum {
	PED_DISK_TYPE_EXTENDED=1,	/* supports extended partitions */
	PED_DISK_TYPE_PARTITION_NAME=2	/* supports partition names */
} PedDiskTypeFeature;
#define PED_DISK_TYPE_FIRST_FEATURE    PED_DISK_TYPE_EXTENDED
#define PED_DISK_TYPE_LAST_FEATURE     PED_DISK_TYPE_PARTITION_NAME

struct _PedPartition {
	PedPartition*		prev;
	PedPartition*		next;

	PedDisk*		disk;
	PedGeometry		geom;
	int			num;

	PedPartitionType	type;
	const PedFileSystemType* fs_type;
	PedPartition*		part_list;	/* for extended partitions */

	void*			disk_specific;
};

struct _PedDisk {
	PedDevice*		dev;
	const PedDiskType*	type;
	PedPartition*		part_list;

	void*			disk_specific;

/* office use only ;-) */
	int			needs_clobber;	/* clobber before write? */
	int			update_mode;	/* mode without free/metadata
						   partitions, for easier
						   update */
};

struct _PedDiskOps {
	/* disk label operations */
	int (*probe) (PedDevice *dev);
	int (*clobber) (PedDevice* dev);
	PedDisk* (*alloc) (PedDevice* dev);
	PedDisk* (*duplicate) (const PedDisk* disk);
	void (*free) (PedDisk* disk);
	int (*read) (PedDisk* disk);
	int (*write) (PedDisk* disk);

	/* partition operations */
	PedPartition* (*partition_new) (
		const PedDisk* disk,
	       	PedPartitionType part_type,
		const PedFileSystemType* fs_type,
		PedSector start,
		PedSector end);
	PedPartition* (*partition_duplicate) (const PedPartition* part);
	void (*partition_destroy) (PedPartition* part);
	int (*partition_set_system) (PedPartition* part,
				     const PedFileSystemType* fs_type);
	int (*partition_set_flag) (
		PedPartition* part,
	       	PedPartitionFlag flag,
		int state);
	int (*partition_get_flag) (
		const PedPartition* part,
		PedPartitionFlag flag);
	int (*partition_is_flag_available) (
		const PedPartition* part,
		PedPartitionFlag flag);
	void (*partition_set_name) (PedPartition* part, const char* name);
	const char* (*partition_get_name) (const PedPartition* part);
	int (*partition_align) (PedPartition* part,
				const PedConstraint* constraint);
	int (*partition_enumerate) (PedPartition* part);

	/* other */
	int (*alloc_metadata) (PedDisk* disk);
	int (*get_max_primary_partition_count) (const PedDisk* disk);
};

struct _PedDiskType {
	PedDiskType*		next;
	const char*		name;
	PedDiskOps* const	ops;

	PedDiskTypeFeature	features;   /* bitmap of supported features */
};

/* Architecture specific operations.  i.e. communication with kernel (or
 * whatever) about changes, etc.
 */
struct _PedDiskArchOps {
	char* (*partition_get_path) (const PedPartition* part);
	int (*partition_is_busy) (const PedPartition* part);
	int (*disk_commit) (PedDisk* disk);
};

extern void ped_register_disk_type (PedDiskType* type);
extern void ped_unregister_disk_type (PedDiskType* type);
extern PedDiskType* ped_disk_type_get_next (PedDiskType* type);
extern PedDiskType* ped_disk_type_get (const char* name);
extern int ped_disk_type_check_feature (const PedDiskType* disk_type,
	       				PedDiskTypeFeature feature);

extern PedDiskType* ped_disk_probe (PedDevice* dev);
extern int ped_disk_clobber (PedDevice* dev);
extern int ped_disk_clobber_exclude (PedDevice* dev,
				     const PedDiskType* exclude);
extern PedDisk* ped_disk_new (PedDevice* dev);
extern PedDisk* ped_disk_new_fresh (PedDevice* dev,
				    const PedDiskType* disk_type);
extern PedDisk* ped_disk_duplicate (const PedDisk* old_disk);
extern void ped_disk_destroy (PedDisk* disk);
extern int ped_disk_commit (PedDisk* disk);
extern int ped_disk_commit_to_dev (PedDisk* disk);
extern int ped_disk_commit_to_os (PedDisk* disk);
extern int ped_disk_check (PedDisk* disk);
extern void ped_disk_print (PedDisk* disk);

extern int ped_disk_get_primary_partition_count (PedDisk* disk);
extern int ped_disk_get_last_partition_num (PedDisk* disk);
extern int ped_disk_get_max_primary_partition_count (const PedDisk* disk);

extern PedPartition* ped_partition_new (const PedDisk* disk,
					PedPartitionType type,
					const PedFileSystemType* fs_type,
					PedSector start,
					PedSector end);
extern void ped_partition_destroy (PedPartition* part);
extern int ped_partition_is_active (const PedPartition* part);
extern int ped_partition_set_flag (PedPartition* part, PedPartitionFlag flag,
	       			   int state);
extern int ped_partition_get_flag (const PedPartition* part,
	       			   PedPartitionFlag flag);
extern int ped_partition_is_flag_available (const PedPartition* part,
	       			      	    PedPartitionFlag flag);
extern int ped_partition_set_system (PedPartition* part,
				     const PedFileSystemType* fs_type);
extern int ped_partition_set_name (PedPartition* part, const char* name);
extern const char* ped_partition_get_name (const PedPartition* part);
extern int ped_partition_is_busy (const PedPartition* part);
extern char* ped_partition_get_path (const PedPartition* part);

extern const char* ped_partition_type_get_name (PedPartitionType part_type);
extern const char* ped_partition_flag_get_name (PedPartitionFlag flag);
extern PedPartitionFlag ped_partition_flag_get_by_name (const char* name);
extern PedPartitionFlag ped_partition_flag_next (PedPartitionFlag flag);

extern int ped_disk_add_partition (PedDisk* disk, PedPartition* part,
				   const PedConstraint* constraint);
extern int ped_disk_remove_partition (PedDisk* disk, PedPartition* part);
extern int ped_disk_delete_partition (PedDisk* disk, PedPartition* part);
extern int ped_disk_delete_all (PedDisk* disk);
extern int ped_disk_set_partition_geom (PedDisk* disk, PedPartition* part,
					const PedConstraint* constraint,
					PedSector start, PedSector end);
extern int ped_disk_maximize_partition (PedDisk* disk, PedPartition* part,
					const PedConstraint* constraint);
extern PedGeometry* ped_disk_get_max_partition_geometry (PedDisk* disk,
		PedPartition* part, const PedConstraint* constraint);
extern int ped_disk_minimize_extended_partition (PedDisk* disk);

extern PedPartition* ped_disk_next_partition (const PedDisk* disk,
					      const PedPartition* part);
extern PedPartition* ped_disk_get_partition (const PedDisk* disk, int num);
extern PedPartition* ped_disk_get_partition_by_sector (const PedDisk* disk,
						       PedSector sect);
extern PedPartition* ped_disk_extended_partition (const PedDisk* disk);

/* internal functions */
extern PedDisk* _ped_disk_alloc (PedDevice* dev, const PedDiskType* type);
extern void _ped_disk_free (PedDisk* disk);

extern PedPartition* _ped_partition_alloc (const PedDisk* disk,
					   PedPartitionType type,
					   const PedFileSystemType* fs_type,
					   PedSector start,
					   PedSector end);
extern void _ped_partition_free (PedPartition* part);

extern int _ped_partition_attempt_align (
		PedPartition* part, const PedConstraint* external,
		PedConstraint* internal);

#endif /* PED_DISK_H_INCLUDED */

