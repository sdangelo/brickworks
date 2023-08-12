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
 * File author: Stefano D'Angelo
 */

#ifndef _BW_EXAMPLE_FXPP_CHORUS_H
#define _BW_EXAMPLE_FXPP_CHORUS_H

#include "platform.h"

#include <bw_chorus.h>

using namespace Brickworks;

extern "C" {

enum {
	p_rate,
	p_depth,
	p_n
};

struct _bw_example_fxpp_chorus {
	// Sub-components
	Chorus<1>	chorus;

	// Parameters
	float		params[p_n];
};
typedef struct _bw_example_fxpp_chorus bw_example_fxpp_chorus;

void bw_example_fxpp_chorus_init(bw_example_fxpp_chorus *instance);
void bw_example_fxpp_chorus_set_sample_rate(bw_example_fxpp_chorus *instance, float sample_rate);
void bw_example_fxpp_chorus_reset(bw_example_fxpp_chorus *instance);
void bw_example_fxpp_chorus_process(bw_example_fxpp_chorus *instance, const float** x, float** y, int n_samples);
void bw_example_fxpp_chorus_set_parameter(bw_example_fxpp_chorus *instance, int index, float value);
float bw_example_fxpp_chorus_get_parameter(bw_example_fxpp_chorus *instance, int index);

}

#endif
