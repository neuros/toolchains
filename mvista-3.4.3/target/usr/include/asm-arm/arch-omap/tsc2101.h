/*
 * tsc2101.h
 *
 * TI TSC2101 Audio CODEC and TS control registers definition
 *
 *
 * Copyright 2003 MontaVista Software Inc.
 * Author: MontaVista Software, Inc.
 *         source@mvista.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __TSC2101_H
#define __TSC2101_H

/* Page 0 Touch Screen Data Registers */
#define TSC2101_TS_X            0
#define TSC2101_TS_Y            1
#define TSC2101_TS_Z1           2
#define TSC2101_TS_Z2           3
#define TSC2101_TS_BAT          5
#define TSC2101_TS_AUX1         7
#define TSC2101_TS_AUX2         8
#define TSC2101_TS_TEMP1        9
#define TSC2101_TS_TEMP2        10

/* Page 1 Touch Screen Control registers */
#define TSC2101_TS_ADC_CTRL           0
#define TSC2101_TS_STATUS             1
#define TSC2101_TS_BUFFER_CTRL        2
#define TSC2101_TS_REF_CTRL           3
#define TSC2101_TS_RESET_CTRL         4
#define TSC2101_TS_CONFIG_CTRL        5
#define TSC2101_TS_TEMP_MAX_THRESHOLD 6
#define TSC2101_TS_TEMP_MIN_THRESHOLD 7
#define TSC2101_TS_AUX1_MAX_THRESHOLD 8
#define TSC2101_TS_AUX1_MIN_THRESHOLD 9
#define TSC2101_TS_AUX2_MAX_THRESHOLD 10
#define TSC2101_TS_AUX2_MIN_THRESHOLD 11
#define TSC2101_TS_MEASURE_CONFIG     12
#define TSC2101_TS_PROG_DELAY         13

/* Page 2 Audio codec Control registers */
#define TSC2101_AUDIO_CTRL_1        0
#define TSC2101_HEADSET_GAIN_CTRL   1
#define TSC2101_DAC_GAIN_CTRL       2
#define TSC2101_MIXER_PGA_CTRL      3
#define TSC2101_AUDIO_CTRL_2        4
#define TSC2101_CODEC_POWER_CTRL    5
#define TSC2101_AUDIO_CTRL_3        6
#define TSC2101_LCH_BASS_BOOST_N0   7
#define TSC2101_LCH_BASS_BOOST_N1   8
#define TSC2101_LCH_BASS_BOOST_N2   9
#define TSC2101_LCH_BASS_BOOST_N3   10
#define TSC2101_LCH_BASS_BOOST_N4   11
#define TSC2101_LCH_BASS_BOOST_N5   12
#define TSC2101_LCH_BASS_BOOST_D1   13
#define TSC2101_LCH_BASS_BOOST_D2   14
#define TSC2101_LCH_BASS_BOOST_D4   15
#define TSC2101_LCH_BASS_BOOST_D5   16
#define TSC2101_RCH_BASS_BOOST_N0   17
#define TSC2101_RCH_BASS_BOOST_N1   18
#define TSC2101_RCH_BASS_BOOST_N2   19
#define TSC2101_RCH_BASS_BOOST_N3   20
#define TSC2101_RCH_BASS_BOOST_N4   21
#define TSC2101_RCH_BASS_BOOST_N5   22
#define TSC2101_RCH_BASS_BOOST_D1   23
#define TSC2101_RCH_BASS_BOOST_D2   24
#define TSC2101_RCH_BASS_BOOST_D4   25
#define TSC2101_RCH_BASS_BOOST_D5   26
#define TSC2101_PLL_PROG_1          27
#define TSC2101_PLL_PROG_2          28
#define TSC2101_AUDIO_CTRL_4        29
#define TSC2101_HANDSET_GAIN_CTRL   30
#define TSC2101_BUZZER_GAIN_CTRL    31
#define TSC2101_AUDIO_CTRL_5        32
#define TSC2101_AUDIO_CTRL_6        33
#define TSC2101_AUDIO_CTRL_7        34
#define TSC2101_GPIO_CTRL           35
#define TSC2101_AGC_CTRL            36
#define TSC2101_POWERDOWN_STS       37

extern int omap_tsc2101_configure(void);
extern u16 omap_tsc2101_read(int, u8);
extern void omap_tsc2101_reads(int, u8, u16 *, int);
extern void omap_tsc2101_write(int, u8, u16);
extern void omap_tsc2101_disable(void);
extern void omap_tsc2101_enable(void);

#endif /* __OMAP_TS_H */

