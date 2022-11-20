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

#include "bw_example_fx_svf.h"

#include <bw_common.h>
#include <bw_svf.h>

enum {
	p_cutoff,
	p_Q,
	p_n
};

struct _bw_example_fx_svf {
	// Sub-components
	bw_svf		svf;

	// Parameters
	float		params[p_n];
};

bw_example_fx_svf bw_example_fx_svf_new() {
	bw_example_fx_svf instance = (bw_example_fx_svf)BW_MALLOC(sizeof(struct _bw_example_fx_svf));
	if (instance != NULL)
		bw_svf_init(&instance->svf);
	return instance;
}

void bw_example_fx_svf_free(bw_example_fx_svf instance) {
	BW_FREE(instance);
}

void bw_example_fx_svf_set_sample_rate(bw_example_fx_svf instance, float sample_rate) {
	bw_svf_set_sample_rate(&instance->svf, sample_rate);
}

void bw_example_fx_svf_reset(bw_example_fx_svf instance) {
	bw_svf_reset(&instance->svf);
}

void bw_example_fx_svf_process(bw_example_fx_svf instance, const float** x, float** y, int n_samples) {
	bw_svf_process(&instance->svf, x[0], y[0], NULL, NULL, n_samples);
}

void bw_example_fx_svf_set_parameter(bw_example_fx_svf instance, int index, float value) {
	switch (index) {
	case p_cutoff:
		bw_svf_set_cutoff(&instance->svf, (20e3f - 20.f) * value * value * value + 20.f);
		break;
	case p_Q:
		bw_svf_set_Q(&instance->svf, 0.5f + 9.5f * value);
		break;
	}
}

float bw_example_fx_svf_get_parameter(bw_example_fx_svf instance, int index) {
	return instance->params[index];
}
