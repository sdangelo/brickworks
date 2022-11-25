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
 *
 * File author: Stefano D'Angelo
 */

#include "bw_example_fx_wah.h"

#include <bw_wah.h>
#ifdef __WASM__
# include "walloc.h"
#else
# include <stdlib.h>
#endif


enum {
	p_wah,
	p_n
};

struct _bw_example_fx_wah {
	// Sub-components
	bw_wah_coeffs	wah_coeffs;
	bw_wah_state	wah_state;

	// Parameters
	float		params[p_n];
};

bw_example_fx_wah bw_example_fx_wah_new() {
	bw_example_fx_wah instance = (bw_example_fx_wah)malloc(sizeof(struct _bw_example_fx_wah));
	if (instance != NULL)
		bw_wah_init(&instance->wah_coeffs);
	return instance;
}

void bw_example_fx_wah_free(bw_example_fx_wah instance) {
	free(instance);
}

void bw_example_fx_wah_set_sample_rate(bw_example_fx_wah instance, float sample_rate) {
	bw_wah_set_sample_rate(&instance->wah_coeffs, sample_rate);
}

void bw_example_fx_wah_reset(bw_example_fx_wah instance) {
	bw_wah_reset_coeffs(&instance->wah_coeffs);
	bw_wah_reset_state(&instance->wah_coeffs, &instance->wah_state);
}

void bw_example_fx_wah_process(bw_example_fx_wah instance, const float** x, float** y, int n_samples) {
	bw_wah_process(&instance->wah_coeffs, &instance->wah_state, x[0], y[0], n_samples);
}

void bw_example_fx_wah_set_parameter(bw_example_fx_wah instance, int index, float value) {
	instance->params[index] = value;
	bw_wah_set_wah(&instance->wah_coeffs, value);
}

float bw_example_fx_wah_get_parameter(bw_example_fx_wah instance, int index) {
	return instance->params[index];
}
