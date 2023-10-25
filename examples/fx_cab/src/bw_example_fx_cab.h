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

#ifndef _BW_EXAMPLE_FX_CAB_H
#define _BW_EXAMPLE_FX_CAB_H

#include "platform.h"

#include <bw_cab.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	p_cutoff_low,
	p_cutoff_high,
	p_tone,
	p_n
};

struct _bw_example_fx_cab {
	// Sub-components
	bw_cab_coeffs	cab_coeffs;
	bw_cab_state	cab_state;

	// Parameters
	float		params[p_n];
};
typedef struct _bw_example_fx_cab bw_example_fx_cab;

void bw_example_fx_cab_init(bw_example_fx_cab *instance);
void bw_example_fx_cab_set_sample_rate(bw_example_fx_cab *instance, float sample_rate);
void bw_example_fx_cab_reset(bw_example_fx_cab *instance);
void bw_example_fx_cab_process(bw_example_fx_cab *instance, const float** x, float** y, int n_samples);
void bw_example_fx_cab_set_parameter(bw_example_fx_cab *instance, int index, float value);
float bw_example_fx_cab_get_parameter(bw_example_fx_cab *instance, int index);

#ifdef __cplusplus
}
#endif

#endif
