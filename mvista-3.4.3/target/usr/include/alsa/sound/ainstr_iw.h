/*
 *  Advanced Linux Sound Architecture
 *
 *  InterWave FFFF Instrument Format
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

#ifndef __SOUND_AINSTR_IW_H
#define __SOUND_AINSTR_IW_H

/*
 *  share types (share ID 1)
 */

#define IWFFFF_SHARE_FILE		0

/*
 *  wave formats
 */

#define IWFFFF_WAVE_16BIT		0x0001  /* 16-bit wave */
#define IWFFFF_WAVE_UNSIGNED		0x0002  /* unsigned wave */
#define IWFFFF_WAVE_INVERT		0x0002  /* same as unsigned wave */
#define IWFFFF_WAVE_BACKWARD		0x0004  /* backward mode (maybe used for reverb or ping-ping loop) */
#define IWFFFF_WAVE_LOOP		0x0008  /* loop mode */
#define IWFFFF_WAVE_BIDIR		0x0010  /* bidirectional mode */
#define IWFFFF_WAVE_ULAW		0x0020  /* uLaw compressed wave */
#define IWFFFF_WAVE_RAM			0x0040  /* wave is _preloaded_ in RAM (it is used for ROM simulation) */
#define IWFFFF_WAVE_ROM			0x0080  /* wave is in ROM */
#define IWFFFF_WAVE_STEREO		0x0100	/* wave is stereo */

/*
 *  Wavetable definitions
 */

typedef struct iwffff_wave {
	unsigned int share_id[4];	/* share id - zero = no sharing */
	unsigned int format;		/* wave format */

	struct {
		unsigned int number;	/* some other ID for this wave */
		unsigned int memory;	/* begin of waveform in onboard memory */
		unsigned char *ptr;	/* pointer to waveform in system memory */
	} address;

	unsigned int size;		/* size of waveform in samples */
	unsigned int start;		/* start offset in samples * 16 (lowest 4 bits - fraction) */
	unsigned int loop_start;	/* bits loop start offset in samples * 16 (lowest 4 bits - fraction) */
	unsigned int loop_end;		/* loop start offset in samples * 16 (lowest 4 bits - fraction) */
	unsigned short loop_repeat;	/* loop repeat - 0 = forever */
	unsigned int sample_ratio;	/* sample ratio (44100 * 1024 / rate) */
	unsigned char attenuation;	/* 0 - 127 (no corresponding midi controller) */
	unsigned char low_note;		/* lower frequency range for this waveform */
	unsigned char high_note;	/* higher frequency range for this waveform */
	unsigned char pad;
  
	struct iwffff_wave *next;
} iwffff_wave_t;

/*
 *  Layer
 */

#define IWFFFF_LFO_SHAPE_TRIANGLE	0
#define IWFFFF_LFO_SHAPE_POSTRIANGLE	1

typedef struct iwffff_lfo {
	unsigned short freq;		/* (0-2047) 0.01Hz - 21.5Hz */
	signed short depth;		/* volume +- (0-255) 0.48675dB/step */
	signed short sweep;		/* 0 - 950 deciseconds */
	unsigned char shape;		/* see to IWFFFF_LFO_SHAPE_XXXX */
	unsigned char delay;		/* 0 - 255 deciseconds */
} iwffff_lfo_t;

#define IWFFFF_ENV_FLAG_RETRIGGER	0x0001	/* flag - retrigger */

#define IWFFFF_ENV_MODE_ONE_SHOT	0x0001	/* mode - one shot */
#define IWFFFF_ENV_MODE_SUSTAIN		0x0002	/* mode - sustain */
#define IWFFFF_ENV_MODE_NO_SUSTAIN	0x0003	/* mode - no sustain */

#define IWFFFF_ENV_INDEX_VELOCITY	0x0001	/* index - velocity */
#define IWFFFF_ENV_INDEX_FREQUENCY	0x0002	/* index - frequency */

typedef struct iwffff_env_point {
	unsigned short offset;
	unsigned short rate;
} iwffff_env_point_t;

typedef struct iwffff_env_record {
	unsigned short nattack;
	unsigned short nrelease;
	unsigned short sustain_offset;
	unsigned short sustain_rate;
	unsigned short release_rate;
	unsigned char hirange;
	unsigned char pad;
	struct iwffff_env_record *next;
	/* points are stored here */
	/* count of points = nattack + nrelease */
} iwffff_env_record_t;

typedef struct iwffff_env {
	unsigned char flags;
  	unsigned char mode;
  	unsigned char index;
	unsigned char pad;
	struct iwffff_env_record *record;
} iwffff_env_t;

#define IWFFFF_LAYER_FLAG_RETRIGGER	0x0001	/* retrigger */

#define IWFFFF_LAYER_VELOCITY_TIME	0x0000	/* velocity mode = time */
#define IWFFFF_LAYER_VELOCITY_RATE	0x0001	/* velocity mode = rate */

#define IWFFFF_LAYER_EVENT_KUP		0x0000	/* layer event - key up */
#define IWFFFF_LAYER_EVENT_KDOWN	0x0001	/* layer event - key down */
#define IWFFFF_LAYER_EVENT_RETRIG	0x0002	/* layer event - retrigger */
#define IWFFFF_LAYER_EVENT_LEGATO	0x0003	/* layer event - legato */

typedef struct iwffff_layer {
	unsigned char flags;
	unsigned char velocity_mode;
      	unsigned char layer_event;
	unsigned char low_range;	/* range for layer based */
	unsigned char high_range;	/* on either velocity or frequency */
	unsigned char pan;		/* pan offset from CC1 (0 left - 127 right) */
	unsigned char pan_freq_scale;	/* position based on frequency (0-127) */
	unsigned char attenuation;	/* 0-127 (no corresponding midi controller) */
	iwffff_lfo_t tremolo;		/* tremolo effect */
	iwffff_lfo_t vibrato;		/* vibrato effect */
	unsigned short freq_scale;	/* 0-2048, 1024 is equal to semitone scaling */
	unsigned char freq_center;	/* center for keyboard frequency scaling */
	unsigned char pad;
	iwffff_env_t penv;		/* pitch envelope */
	iwffff_env_t venv;		/* volume envelope */

	iwffff_wave_t *wave;
	struct iwffff_layer *next;
} iwffff_layer_t;

/*
 *  Instrument
 */

#define IWFFFF_EXCLUDE_NONE		0x0000	/* exclusion mode - none */
#define IWFFFF_EXCLUDE_SINGLE		0x0001	/* exclude single - single note from the instrument group */
#define IWFFFF_EXCLUDE_MULTIPLE		0x0002	/* exclude multiple - stop only same note from this instrument */

#define IWFFFF_LAYER_NONE		0x0000	/* not layered */
#define IWFFFF_LAYER_ON			0x0001	/* layered */
#define IWFFFF_LAYER_VELOCITY		0x0002	/* layered by velocity */
#define IWFFFF_LAYER_FREQUENCY		0x0003	/* layered by frequency */

#define IWFFFF_EFFECT_NONE		0
#define IWFFFF_EFFECT_REVERB		1
#define IWFFFF_EFFECT_CHORUS		2
#define IWFFFF_EFFECT_ECHO		3

typedef struct {
	unsigned short exclusion;
	unsigned short layer_type;
	unsigned short exclusion_group;	/* 0 - none, 1-65535 */

	unsigned char effect1;		/* effect 1 */
	unsigned char effect1_depth;	/* 0-127 */
	unsigned char effect2;		/* effect 2 */
	unsigned char effect2_depth;	/* 0-127 */

	iwffff_layer_t *layer;		/* first layer */
} iwffff_instrument_t;

/*
 *
 *    Kernel <-> user space
 *    Hardware (CPU) independent section
 *
 *    * = zero or more
 *    + = one or more
 *
 *    iwffff_xinstrument		IWFFFF_STRU_INSTR
 *      +iwffff_xlayer			IWFFFF_STRU_LAYER
 *        *iwffff_xenv_record		IWFFFF_STRU_ENV_RECT (tremolo)
 *        *iwffff_xenv_record		IWFFFF_STRU_EVN_RECT (vibrato)
 *          +iwffff_xwave		IWFFFF_STRU_WAVE
 *
 */

#define IWFFFF_STRU_WAVE	cpu_to_be32(('W'<<24)|('A'<<16)|('V'<<8)|'E')
#define IWFFFF_STRU_ENV_RECP	cpu_to_be32(('E'<<24)|('N'<<16)|('R'<<8)|'P')
#define IWFFFF_STRU_ENV_RECV	cpu_to_be32(('E'<<24)|('N'<<16)|('R'<<8)|'V')
#define IWFFFF_STRU_LAYER 	cpu_to_be32(('L'<<24)|('A'<<16)|('Y'<<8)|'R')
#define IWFFFF_STRU_INSTR 	cpu_to_be32(('I'<<24)|('N'<<16)|('S'<<8)|'T')

/*
 *  Wavetable definitions
 */

typedef struct iwffff_xwave {
	u_int32_t stype;			/* structure type */

	u_int32_t share_id[4];		/* share id - zero = no sharing */

	u_int32_t format;			/* wave format */
	u_int32_t offset;			/* offset to ROM (address) */

	u_int32_t size;			/* size of waveform in samples */
	u_int32_t start;			/* start offset in samples * 16 (lowest 4 bits - fraction) */
	u_int32_t loop_start;		/* bits loop start offset in samples * 16 (lowest 4 bits - fraction) */
	u_int32_t loop_end;			/* loop start offset in samples * 16 (lowest 4 bits - fraction) */
	u_int16_t loop_repeat;		/* loop repeat - 0 = forever */
	u_int32_t sample_ratio;		/* sample ratio (44100 * 1024 / rate) */
	u_int8_t attenuation;		/* 0 - 127 (no corresponding midi controller) */
	u_int8_t low_note;			/* lower frequency range for this waveform */
	u_int8_t high_note;			/* higher frequency range for this waveform */
	u_int8_t pad;
} iwffff_xwave_t;

/*
 *  Layer
 */

typedef struct iwffff_xlfo {
	u_int16_t freq;			/* (0-2047) 0.01Hz - 21.5Hz */
	int16_t depth;			/* volume +- (0-255) 0.48675dB/step */
	int16_t sweep;			/* 0 - 950 deciseconds */
	u_int8_t shape;			/* see to ULTRA_IW_LFO_SHAPE_XXXX */
	u_int8_t delay;			/* 0 - 255 deciseconds */
} iwffff_xlfo_t;

typedef struct iwffff_xenv_point {
	u_int16_t offset;
	u_int16_t rate;
} iwffff_xenv_point_t;

typedef struct iwffff_xenv_record {
	u_int32_t stype;
	u_int16_t nattack;
	u_int16_t nrelease;
	u_int16_t sustain_offset;
	u_int16_t sustain_rate;
	u_int16_t release_rate;
	u_int8_t hirange;
	u_int8_t pad;
	/* points are stored here.. */
	/* count of points = nattack + nrelease */
} iwffff_xenv_record_t;

typedef struct iwffff_xenv {
	u_int8_t flags;
  	u_int8_t mode;
  	u_int8_t index;
	u_int8_t pad;
} iwffff_xenv_t;

typedef struct iwffff_xlayer {
	u_int32_t stype;
	u_int8_t flags;
	u_int8_t velocity_mode;
      	u_int8_t layer_event;
	u_int8_t low_range;			/* range for layer based */
	u_int8_t high_range;		/* on either velocity or frequency */
	u_int8_t pan;			/* pan offset from CC1 (0 left - 127 right) */
	u_int8_t pan_freq_scale;		/* position based on frequency (0-127) */
	u_int8_t attenuation;		/* 0-127 (no corresponding midi controller) */
	iwffff_xlfo_t tremolo;		/* tremolo effect */
	iwffff_xlfo_t vibrato;		/* vibrato effect */
	u_int16_t freq_scale;		/* 0-2048, 1024 is equal to semitone scaling */
	u_int8_t freq_center;		/* center for keyboard frequency scaling */
	u_int8_t pad;
	iwffff_xenv_t penv;		/* pitch envelope */
	iwffff_xenv_t venv;		/* volume envelope */
} iwffff_xlayer_t;

/*
 *  Instrument
 */

typedef struct iwffff_xinstrument {
	u_int32_t stype;
	
	u_int16_t exclusion;
	u_int16_t layer_type;
	u_int16_t exclusion_group;		/* 0 - none, 1-65535 */

	u_int8_t effect1;			/* effect 1 */
	u_int8_t effect1_depth;		/* 0-127 */
	u_int8_t effect2;			/* effect 2 */
	u_int8_t effect2_depth;		/* 0-127 */
} iwffff_xinstrument_t;

/*
 *  ROM support
 *    InterWave ROMs are Little-Endian (x86)
 */

#define IWFFFF_ROM_HDR_SIZE	512

typedef struct {
	u_int8_t iwave[8];
	u_int8_t revision;
	u_int8_t series_number;
	u_int8_t series_name[16];
	u_int8_t date[10];
	u_int16_t vendor_revision_major;
	u_int16_t vendor_revision_minor;
	u_int32_t rom_size;
	u_int8_t copyright[128];
	u_int8_t vendor_name[64];
	u_int8_t description[128];
} iwffff_rom_header_t;

/*
 *  Instrument info
 */

#define IWFFFF_INFO_LFO_VIBRATO		(1<<0)
#define IWFFFF_INFO_LFO_VIBRATO_SHAPE	(1<<1)
#define IWFFFF_INFO_LFO_TREMOLO		(1<<2)
#define IWFFFF_INFO_LFO_TREMOLO_SHAPE	(1<<3)

typedef struct iwffff_info {
	unsigned int format;		/* supported format bits */
	unsigned int effects;		/* supported effects (1 << IWFFFF_EFFECT*) */
	unsigned int lfos;		/* LFO effects */
	unsigned int max8_len;		/* maximum 8-bit wave length */
	unsigned int max16_len;		/* maximum 16-bit wave length */
} iwffff_info_t;

#endif /* __SOUND_AINSTR_IW_H */
