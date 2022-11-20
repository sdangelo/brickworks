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

#include <bw_common.h>
#include <bw_wah.h>

enum {
	p_wah,
	p_n
};

struct _bw_example_fx_wah {
	// Sub-components
	bw_wah		wah;

	// Parameters
	float		params[p_n];
};

bw_example_fx_wah bw_example_fx_wah_new() {
	bw_example_fx_wah instance = (bw_example_fx_wah)BW_MALLOC(sizeof(struct _bw_example_fx_wah));
	if (instance != NULL)
		bw_wah_init(&instance->wah);
	return instance;
}

void bw_example_fx_wah_free(bw_example_fx_wah instance) {
	BW_FREE(instance);
}

void bw_example_fx_wah_set_sample_rate(bw_example_fx_wah instance, float sample_rate) {
	bw_wah_set_sample_rate(&instance->wah, sample_rate);
}

void bw_example_fx_wah_reset(bw_example_fx_wah instance) {
	bw_wah_reset(&instance->wah);
}

void bw_example_fx_wah_process(bw_example_fx_wah instance, const float** x, float** y, int n_samples) {
	bw_wah_process(&instance->wah, x[0], y[0], n_samples);
}

void bw_example_fx_wah_set_parameter(bw_example_fx_wah instance, int index, float value) {
	bw_wah_set_wah(&instance->wah, value);
}

float bw_example_fx_wah_get_parameter(bw_example_fx_wah instance, int index) {
	return instance->params[index];
}
