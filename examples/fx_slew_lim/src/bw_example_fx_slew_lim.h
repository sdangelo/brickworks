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

#ifndef _BW_EXAMPLE_FX_SLEW_LIM_H
#define _BW_EXAMPLE_FX_SLEW_LIM_H

#include "platform.h"

#include <bw_slew_lim.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	p_max_rate,
	p_n
};

struct _bw_example_fx_slew_lim {
	// Sub-components
	bw_slew_lim_coeffs	slew_lim_coeffs;
	bw_slew_lim_state	slew_lim_state;

	// Parameters
	float			params[p_n];
};
typedef struct _bw_example_fx_slew_lim bw_example_fx_slew_lim;

void bw_example_fx_slew_lim_init(bw_example_fx_slew_lim *instance);
void bw_example_fx_slew_lim_set_sample_rate(bw_example_fx_slew_lim *instance, float sample_rate);
void bw_example_fx_slew_lim_reset(bw_example_fx_slew_lim *instance);
void bw_example_fx_slew_lim_process(bw_example_fx_slew_lim *instance, const float** x, float** y, int n_samples);
void bw_example_fx_slew_lim_set_parameter(bw_example_fx_slew_lim *instance, int index, float value);
float bw_example_fx_slew_lim_get_parameter(bw_example_fx_slew_lim *instance, int index);

#ifdef __cplusplus
}
#endif

#endif
