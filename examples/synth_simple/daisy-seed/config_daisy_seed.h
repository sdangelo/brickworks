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
	{ 0, 32 },
	{ 1, 33 },
	{ 2, 34 },
	{ 3, 35 },
	{ 4, 36 },
	{ 5, 37 },
	{ 6, 38 },
	{ 7, 39 },
	{ 8, 40 },
	{ 9, 41 }
};

#endif
