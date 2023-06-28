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

#define NUM_CCS 	10

static struct config_cc config_ccs[NUM_CCS] = {
	{ 0, 7 },	// Volume
	{ 1, 3 },	// Master tune
	{ 2, 5 },	// Portamento
	{ 3, 28 },	// Pulse width
	{ 4, 74 },	// Cutoff
	{ 5, 71 },	// Q
	{ 6, 73 },	// Attack
	{ 7, 109 },	// Decay
	{ 8, 110 },	// Sustain
	{ 9, 72 }	// Release
};

#endif
