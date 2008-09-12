/*
 *  Advanced Linux Sound Architecture
 *
 *  FM (OPL2/3) Instrument Format
 *  Copyright (c) 2000 Uros Bizjak <uros@kss-loka.si>
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

#ifndef __SOUND_AINSTR_FM_H
#define __SOUND_AINSTR_FM_H

/*
 *  share types (share ID 1)
 */

#define FM_SHARE_FILE		0

/*
 * FM operator
 */

typedef struct fm_operator {
	unsigned char am_vib;
	unsigned char ksl_level;
	unsigned char attack_decay;
	unsigned char sustain_release;
	unsigned char wave_select;
} fm_operator_t;

/*
 *  Instrument
 */

#define FM_PATCH_OPL2	0x01		/* OPL2 2 operators FM instrument */
#define FM_PATCH_OPL3	0x02		/* OPL3 4 operators FM instrument */

typedef struct {
	unsigned int share_id[4];	/* share id - zero = no sharing */
	unsigned char type;		/* instrument type */

	fm_operator_t op[4];
	unsigned char feedback_connection[2];

	unsigned char echo_delay;
	unsigned char echo_atten;
	unsigned char chorus_spread;
	unsigned char trnsps;
	unsigned char fix_dur;
	unsigned char modes;
	unsigned char fix_key;
} fm_instrument_t;

/*
 *
 *    Kernel <-> user space
 *    Hardware (CPU) independent section
 *
 *    * = zero or more
 *    + = one or more
 *
 *    fm_xinstrument	FM_STRU_INSTR
 *
 */

#define FM_STRU_INSTR	cpu_to_be32(('I'<<24)|('N'<<16)|('S'<<8)|'T')

/*
 * FM operator
 */

typedef struct fm_xoperator {
	u_int8_t am_vib;
	u_int8_t ksl_level;
	u_int8_t attack_decay;
	u_int8_t sustain_release;
	u_int8_t wave_select;
} fm_xoperator_t;

/*
 *  Instrument
 */

typedef struct fm_xinstrument {
	u_int32_t stype;			/* structure type */

	u_int32_t share_id[4];		/* share id - zero = no sharing */
	u_int8_t type;			/* instrument type */

	fm_xoperator_t op[4];		/* fm operators */
	u_int8_t feedback_connection[2];

	u_int8_t echo_delay;
	u_int8_t echo_atten;
	u_int8_t chorus_spread;
	u_int8_t trnsps;
	u_int8_t fix_dur;
	u_int8_t modes;
	u_int8_t fix_key;
} fm_xinstrument_t;

#endif	/* __SOUND_AINSTR_FM_H */
