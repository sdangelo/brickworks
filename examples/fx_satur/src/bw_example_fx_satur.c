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

#include "bw_example_fx_satur.h"

#include <bw_satur.h>
#ifdef __WASM__
# include "walloc.h"
#else
# include <stdlib.h>
#endif


enum {
	p_bias,
	p_gain,
	p_n
};

struct _bw_example_fx_satur {
	// Sub-components
	bw_satur_coeffs	satur_coeffs;
	bw_satur_state	satur_state;

	// Parameters
	float		params[p_n];
};

bw_example_fx_satur bw_example_fx_satur_new() {
	bw_example_fx_satur instance = (bw_example_fx_satur)malloc(sizeof(struct _bw_example_fx_satur));
	if (instance != NULL)
		bw_satur_init(&instance->satur_coeffs);
	return instance;
}

void bw_example_fx_satur_free(bw_example_fx_satur instance) {
	free(instance);
}

void bw_example_fx_satur_set_sample_rate(bw_example_fx_satur instance, float sample_rate) {
	bw_satur_set_sample_rate(&instance->satur_coeffs, sample_rate);
}

void bw_example_fx_satur_reset(bw_example_fx_satur instance) {
	bw_satur_reset_coeffs(&instance->satur_coeffs);
	bw_satur_reset_state(&instance->satur_coeffs, &instance->satur_state);
}

void bw_example_fx_satur_process(bw_example_fx_satur instance, const float** x, float** y, int n_samples) {
	bw_satur_process(&instance->satur_coeffs, &instance->satur_state, x[0], y[0], n_samples);
}

void bw_example_fx_satur_set_parameter(bw_example_fx_satur instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_bias:
		bw_satur_set_bias(&instance->satur_coeffs, 5.f * value - 2.5f);
		break;
	case p_gain:
		bw_satur_set_gain(&instance->satur_coeffs, 0.1f + (10.f - 0.1f) * value * value * value);
		break;
	}
}

float bw_example_fx_satur_get_parameter(bw_example_fx_satur instance, int index) {
	return instance->params[index];
}
