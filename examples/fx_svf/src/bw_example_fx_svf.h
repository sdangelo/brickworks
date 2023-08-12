/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
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

#ifndef _BW_EXAMPLE_FX_SVF_H
#define _BW_EXAMPLE_FX_SVF_H

#include "platform.h"

#include <bw_svf.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	p_cutoff,
	p_Q,
	p_n
};

struct _bw_example_fx_svf {
	// Sub-components
	bw_svf_coeffs	svf_coeffs;
	bw_svf_state	svf_state;

	// Parameters
	float		params[p_n];
};
typedef struct _bw_example_fx_svf bw_example_fx_svf;

void bw_example_fx_svf_init(bw_example_fx_svf *instance);
void bw_example_fx_svf_set_sample_rate(bw_example_fx_svf *instance, float sample_rate);
void bw_example_fx_svf_reset(bw_example_fx_svf *instance);
void bw_example_fx_svf_process(bw_example_fx_svf *instance, const float** x, float** y, int n_samples);
void bw_example_fx_svf_set_parameter(bw_example_fx_svf *instance, int index, float value);
float bw_example_fx_svf_get_parameter(bw_example_fx_svf *instance, int index);

#ifdef __cplusplus
}
#endif

#endif
