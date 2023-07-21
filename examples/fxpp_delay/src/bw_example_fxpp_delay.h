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

#ifndef _BW_EXAMPLE_FXPP_DELAY_H
#define _BW_EXAMPLE_FXPP_DELAY_H

#include "platform.h"

#include <bwpp_delay.h>

using namespace Brickworks;

extern "C" {

enum {
	p_delay,
	p_n
};

struct _bw_example_fxpp_delay {
	// Sub-components
	Delay<1>	delay;

	// Parameters
	float		params[p_n];
};
typedef struct _bw_example_fxpp_delay bw_example_fxpp_delay;

void bw_example_fxpp_delay_init(bw_example_fxpp_delay *instance);
void bw_example_fxpp_delay_set_sample_rate(bw_example_fxpp_delay *instance, float sample_rate);
void bw_example_fxpp_delay_reset(bw_example_fxpp_delay *instance);
void bw_example_fxpp_delay_process(bw_example_fxpp_delay *instance, const float** x, float** y, int n_samples);
void bw_example_fxpp_delay_set_parameter(bw_example_fxpp_delay *instance, int index, float value);
float bw_example_fxpp_delay_get_parameter(bw_example_fxpp_delay *instance, int index);

}

#endif
