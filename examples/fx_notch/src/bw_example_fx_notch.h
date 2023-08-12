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

#ifndef _BW_EXAMPLE_FX_NOTCH_H
#define _BW_EXAMPLE_FX_NOTCH_H

#include "platform.h"

#include <bw_notch.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	p_cutoff,
	p_Q,
	p_n
};

struct _bw_example_fx_notch {
	// Sub-components
	bw_notch_coeffs	notch_coeffs;
	bw_notch_state	notch_state;

	// Parameters
	float		params[p_n];
};
typedef struct _bw_example_fx_notch bw_example_fx_notch;

void bw_example_fx_notch_init(bw_example_fx_notch *instance);
void bw_example_fx_notch_set_sample_rate(bw_example_fx_notch *instance, float sample_rate);
void bw_example_fx_notch_reset(bw_example_fx_notch *instance);
void bw_example_fx_notch_process(bw_example_fx_notch *instance, const float** x, float** y, int n_samples);
void bw_example_fx_notch_set_parameter(bw_example_fx_notch *instance, int index, float value);
float bw_example_fx_notch_get_parameter(bw_example_fx_notch *instance, int index);

#ifdef __cplusplus
}
#endif

#endif
