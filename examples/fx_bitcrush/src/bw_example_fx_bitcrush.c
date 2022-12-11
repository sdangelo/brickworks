/*
 * Brickworks
 *
 * Copyright (C) 2022 Orastron Srl unipersonale
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

#include "bw_example_fx_bitcrush.h"

#include <bw_sr_reduce.h>
#include <bw_bd_reduce.h>
#ifdef __WASM__
# include "walloc.h"
#else
# include <stdlib.h>
#endif

enum {
	p_sr_ratio,
	p_bit_depth,
	p_n
};

struct _bw_example_fx_bitcrush {
	// Sub-components
	bw_sr_reduce_coeffs	sr_reduce_coeffs;
	bw_sr_reduce_state	sr_reduce_state;
	bw_bd_reduce_coeffs	bd_reduce_coeffs;

	// Parameters
	float			params[p_n];
};

bw_example_fx_bitcrush bw_example_fx_bitcrush_new() {
	bw_example_fx_bitcrush instance = (bw_example_fx_bitcrush)malloc(sizeof(struct _bw_example_fx_bitcrush));
	if (instance == NULL)
		return NULL;
	bw_sr_reduce_init(&instance->sr_reduce_coeffs);
	bw_bd_reduce_init(&instance->bd_reduce_coeffs);
	return instance;
}

void bw_example_fx_bitcrush_free(bw_example_fx_bitcrush instance) {
	free(instance);
}

void bw_example_fx_bitcrush_set_sample_rate(bw_example_fx_bitcrush instance, float sample_rate) {
}

void bw_example_fx_bitcrush_reset(bw_example_fx_bitcrush instance) {
	bw_sr_reduce_reset_state(&instance->sr_reduce_coeffs, &instance->sr_reduce_state);
	bw_bd_reduce_reset_coeffs(&instance->bd_reduce_coeffs);
}

void bw_example_fx_bitcrush_process(bw_example_fx_bitcrush instance, const float** x, float** y, int n_samples) {
	bw_sr_reduce_process(&instance->sr_reduce_coeffs, &instance->sr_reduce_state, x[0], y[0], n_samples);
	bw_bd_reduce_process(&instance->bd_reduce_coeffs, y[0], y[0], n_samples);
}

void bw_example_fx_bitcrush_set_parameter(bw_example_fx_bitcrush instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_sr_ratio:
		bw_sr_reduce_set_ratio(&instance->sr_reduce_coeffs, value);
		break;
	case p_bit_depth:
		bw_bd_reduce_set_bit_depth(&instance->bd_reduce_coeffs, 1 + (int)(15.f * value));
		break;
	}
}

float bw_example_fx_bitcrush_get_parameter(bw_example_fx_bitcrush instance, int index) {
	return instance->params[index];
}
