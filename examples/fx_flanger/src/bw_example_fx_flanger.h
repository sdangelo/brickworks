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

#ifndef _BW_EXAMPLE_FX_FLANGER_H
#define _BW_EXAMPLE_FX_FLANGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

#include <bw_chorus.h>

enum {
	p_rate,
	p_amount,
	p_n
};

struct _bw_example_fx_flanger {
	// Sub-components
	bw_chorus_coeffs	chorus_coeffs;
	bw_chorus_state		chorus_state;

	// Parameters
	float			params[p_n];
};
typedef struct _bw_example_fx_flanger bw_example_fx_flanger;

void bw_example_fx_flanger_init(bw_example_fx_flanger *instance);
void bw_example_fx_flanger_set_sample_rate(bw_example_fx_flanger *instance, float sample_rate);
BW_SIZE_T bw_example_fx_flanger_mem_req(bw_example_fx_flanger *instance);
void bw_example_fx_flanger_mem_set(bw_example_fx_flanger *instance, void *mem);
void bw_example_fx_flanger_reset(bw_example_fx_flanger *instance);
void bw_example_fx_flanger_process(bw_example_fx_flanger *instance, const float** x, float** y, int n_samples);
void bw_example_fx_flanger_set_parameter(bw_example_fx_flanger *instance, int index, float value);
float bw_example_fx_flanger_get_parameter(bw_example_fx_flanger *instance, int index);

#ifdef __cplusplus
}
#endif

#endif
