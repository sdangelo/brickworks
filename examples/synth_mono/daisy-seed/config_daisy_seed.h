/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File authors: Stefano D'Angelo
 */

#ifndef _CONFIG_DAISY_SEED_H
#define _CONFIG_DAISY_SEED_H

struct config_cc {
	int		param_index;
	unsigned char	cc;
};

#define NUM_CCS 	37

static struct config_cc config_ccs[NUM_CCS] = {
	{ 0, 7 },	// Volume
	{ 1, 3 },	// Master tune
	{ 2, 5 },	// Portamento
	{ 3, 9 },	// Modulation mix
	{ 4, 14 },	// VCO1 modulation
	{ 5, 15 },	// VCO1 coarse
	{ 6, 20 },	// VCO1 fine
	{ 7, 21 },	// VCO1 waveform
	{ 8, 22 },	// VCO1 pulse width/slope
	{ 9, 23 },	// VCO1 level
	{ 10, 24 },	// VCO2 modulation
	{ 11, 25 },	// VCO2 coarse
	{ 12, 26 },	// VCO2 fine
	{ 13, 27 },	// VCO2 waveform
	{ 14, 28 },	// VCO2 pulse width/slope
	{ 15, 29 },	// VCO2 level
	{ 16, 30 },	// VCO3 keyboard control
	{ 17, 31 },	// VCO3 coarse
	{ 18, 85 },	// VCO3 fine
	{ 19, 86 },	// VCO3 waveform
	{ 20, 87 },	// VCO3 level
	{ 21, 89 },	// Noise color
	{ 22, 90 },	// Noise level
	{ 23, 102 },	// VCF modulation
	{ 24, 103 },	// VCF keyboard control
	{ 25, 74 },	// VCF cutoff
	{ 26, 71 },	// VCF Q
	{ 27, 104 },	// VCF contour
	{ 28, 105 },	// VCF attack
	{ 29, 106 },	// VCF decay
	{ 30, 107 },	// VCF sustain
	{ 31, 108 },	// VCF release
	{ 32, 73 },	// VCA attack
	{ 33, 109 },	// VCA decay
	{ 34, 110 },	// VCA sustain
	{ 35, 72 },	// VCA release
	{ 36, 111 }	// A440
};

#endif
