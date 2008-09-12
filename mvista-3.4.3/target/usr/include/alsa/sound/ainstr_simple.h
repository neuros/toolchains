/*
 *  Advanced Linux Sound Architecture
 *
 *  Simple (MOD player) Instrument Format
 *  Copyright (c) 1994-99 by Jaroslav Kysela <perex@suse.cz>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#ifndef __SOUND_AINSTR_SIMPLE_H
#define __SOUND_AINSTR_SIMPLE_H

/*
 *  share types (share ID 1)
 */

#define SIMPLE_SHARE_FILE		0

/*
 *  wave formats
 */

#define SIMPLE_WAVE_16BIT		0x0001  /* 16-bit wave */
#define SIMPLE_WAVE_UNSIGNED		0x0002  /* unsigned wave */
#define SIMPLE_WAVE_INVERT		0x0002  /* same as unsigned wave */
#define SIMPLE_WAVE_BACKWARD		0x0004  /* backward mode (maybe used for reverb or ping-ping loop) */
#define SIMPLE_WAVE_LOOP		0x0008  /* loop mode */
#define SIMPLE_WAVE_BIDIR		0x0010  /* bidirectional mode */
#define SIMPLE_WAVE_STEREO		0x0100	/* stereo wave */
#define SIMPLE_WAVE_ULAW		0x0200	/* uLaw compression mode */

/*
 *  instrument effects
 */

#define SIMPLE_EFFECT_NONE		0
#define SIMPLE_EFFECT_REVERB		1
#define SIMPLE_EFFECT_CHORUS		2
#define SIMPLE_EFFECT_ECHO		3

/*
 *  instrument info
 */

typedef struct simple_instrument_info {
	unsigned int format;		/* supported format bits */
	unsigned int effects;		/* supported effects (1 << SIMPLE_EFFECT_*) */
	unsigned int max8_len;		/* maximum 8-bit wave length */
	unsigned int max16_len;		/* maximum 16-bit wave length */
} simple_instrument_info_t;

/*
 *  Instrument
 */

typedef struct {
	unsigned int share_id[4];	/* share id - zero = no sharing */
	unsigned int format;		/* wave format */

	struct {
		unsigned int number;	/* some other ID for this instrument */
		unsigned int memory;	/* begin of waveform in onboard memory */
		unsigned char *ptr;	/* pointer to waveform in system memory */
	} address;

	unsigned int size;		/* size of waveform in samples */
	unsigned int start;		/* start offset in samples * 16 (lowest 4 bits - fraction) */
	unsigned int loop_start;	/* loop start offset in samples * 16 (lowest 4 bits - fraction) */
	unsigned int loop_end;		/* loop end offset in samples * 16 (lowest 4 bits - fraction) */
	unsigned short loop_repeat;	/* loop repeat - 0 = forever */

	unsigned char effect1;		/* effect 1 */
	unsigned char effect1_depth;	/* 0-127 */
	unsigned char effect2;		/* effect 2 */
	unsigned char effect2_depth;	/* 0-127 */
} simple_instrument_t;

/*
 *
 *    Kernel <-> user space
 *    Hardware (CPU) independent section
 *
 *    * = zero or more
 *    + = one or more
 *
 *    simple_xinstrument	SIMPLE_STRU_INSTR
 *
 */

#define SIMPLE_STRU_INSTR	cpu_to_be32(('I'<<24)|('N'<<16)|('S'<<8)|'T')

/*
 *  Instrument
 */

typedef struct simple_xinstrument {
	u_int32_t stype;

	u_int32_t share_id[4];		/* share id - zero = no sharing */
	u_int32_t format;			/* wave format */

	u_int32_t size;			/* size of waveform in samples */
	u_int32_t start;			/* start offset in samples * 16 (lowest 4 bits - fraction) */
	u_int32_t loop_start;		/* bits loop start offset in samples * 16 (lowest 4 bits - fraction) */
	u_int32_t loop_end;			/* loop start offset in samples * 16 (lowest 4 bits - fraction) */
	u_int16_t loop_repeat;		/* loop repeat - 0 = forever */
	
	u_int8_t effect1;			/* effect 1 */
	u_int8_t effect1_depth;		/* 0-127 */
	u_int8_t effect2;			/* effect 2 */
	u_int8_t effect2_depth;		/* 0-127 */
} simple_xinstrument_t;

#endif /* __SOUND_AINSTR_SIMPLE_H */
