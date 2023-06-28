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

#define NUM_CCS 	38

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
	{ 20, 87 },	// VCO3 puslse width/slope
	{ 21, 89 },	// VCO3 level
	{ 22, 90 },	// Noise color
	{ 23, 102 },	// Noise level
	{ 24, 103 },	// VCF modulation
	{ 25, 104 },	// VCF keyboard control
	{ 26, 74 },	// VCF cutoff
	{ 27, 71 },	// VCF Q
	{ 28, 105 },	// VCF contour
	{ 29, 106 },	// VCF attack
	{ 30, 107 },	// VCF decay
	{ 31, 108 },	// VCF sustain
	{ 32, 109 },	// VCF release
	{ 33, 73 },	// VCA attack
	{ 34, 110 },	// VCA decay
	{ 35, 111 },	// VCA sustain
	{ 36, 72 },	// VCA release
	{ 37, 112 }	// A440
};

#endif
