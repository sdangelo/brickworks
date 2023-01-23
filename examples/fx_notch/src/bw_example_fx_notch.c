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

#include "bw_example_fx_notch.h"

#include <bw_notch.h>
#ifdef __WASM__
# include "walloc.h"
#else
# include <stdlib.h>
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

bw_example_fx_notch bw_example_fx_notch_new() {
	bw_example_fx_notch instance = (bw_example_fx_notch)malloc(sizeof(struct _bw_example_fx_notch));
	if (instance != NULL)
		bw_notch_init(&instance->notch_coeffs);
	return instance;
}

void bw_example_fx_notch_free(bw_example_fx_notch instance) {
	free(instance);
}

void bw_example_fx_notch_set_sample_rate(bw_example_fx_notch instance, float sample_rate) {
	bw_notch_set_sample_rate(&instance->notch_coeffs, sample_rate);
}

void bw_example_fx_notch_reset(bw_example_fx_notch instance) {
	bw_notch_reset_coeffs(&instance->notch_coeffs);
	bw_notch_reset_state(&instance->notch_coeffs, &instance->notch_state);
}

void bw_example_fx_notch_process(bw_example_fx_notch instance, const float** x, float** y, int n_samples) {
	bw_notch_process(&instance->notch_coeffs, &instance->notch_state, x[0], y[0], n_samples);
}

void bw_example_fx_notch_set_parameter(bw_example_fx_notch instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_cutoff:
		bw_notch_set_cutoff(&instance->notch_coeffs, (20e3f - 20.f) * value * value * value + 20.f);
		break;
	case p_Q:
		bw_notch_set_Q(&instance->notch_coeffs, 0.5f + 9.5f * value);
		break;
	}
}

float bw_example_fx_notch_get_parameter(bw_example_fx_notch instance, int index) {
	return instance->params[index];
}
