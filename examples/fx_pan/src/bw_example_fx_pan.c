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

#include "bw_example_fx_pan.h"

#include <bw_pan.h>
#include <bw_ppm.h>
#include <bw_math.h>
#ifdef __WASM__
# include "walloc.h"
#else
# include <stdlib.h>
#endif

enum {
	p_pan,
	p_n
};

struct _bw_example_fx_pan {
	// Sub-components
	bw_pan_coeffs	pan_coeffs;
	bw_ppm_coeffs	ppm_coeffs;
	bw_ppm_state	ppm_l_state;
	bw_ppm_state	ppm_r_state;

	// Parameters
	float		params[p_n];
};

bw_example_fx_pan bw_example_fx_pan_new() {
	bw_example_fx_pan instance = (bw_example_fx_pan)malloc(sizeof(struct _bw_example_fx_pan));
	if (instance == NULL)
		return NULL;
	bw_pan_init(&instance->pan_coeffs);
	bw_ppm_init(&instance->ppm_coeffs);
	return instance;
}

void bw_example_fx_pan_free(bw_example_fx_pan instance) {
	free(instance);
}

void bw_example_fx_pan_set_sample_rate(bw_example_fx_pan instance, float sample_rate) {
	bw_pan_set_sample_rate(&instance->pan_coeffs, sample_rate);
	bw_ppm_set_sample_rate(&instance->ppm_coeffs, sample_rate);
}

void bw_example_fx_pan_reset(bw_example_fx_pan instance) {
	bw_pan_reset_coeffs(&instance->pan_coeffs);
	bw_ppm_reset_coeffs(&instance->ppm_coeffs);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_l_state);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_r_state);
}

void bw_example_fx_pan_process(bw_example_fx_pan instance, const float** x, float** y, int n_samples) {
	bw_pan_process(&instance->pan_coeffs, x[0], y[0], y[1], n_samples);
	bw_ppm_update_coeffs_ctrl(&instance->ppm_coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_ppm_update_coeffs_audio(&instance->ppm_coeffs);
		bw_ppm_process1(&instance->ppm_coeffs, &instance->ppm_l_state, y[0][i]);
		bw_ppm_process1(&instance->ppm_coeffs, &instance->ppm_r_state, y[1][i]);
	}
}

void bw_example_fx_pan_set_parameter(bw_example_fx_pan instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_pan:
		bw_pan_set_pan(&instance->pan_coeffs, value + value - 1.f);
		break;
	}
}

float bw_example_fx_pan_get_parameter(bw_example_fx_pan instance, int index) {
	float r;
	switch (index) {
	case p_pan:
		r = instance->params[p_pan];
		break;
	case p_pan + 1:
	{
		const float v = bw_ppm_get_y_z1(&instance->ppm_l_state);
		r = v < -200.f ? 0.f : bw_clipf(0.01666666666666667f * v + 1.f, 0.f, 1.f);
		break;
	}
	case p_pan + 2:
	{
		const float v = bw_ppm_get_y_z1(&instance->ppm_r_state);
		r = v < -200.f ? 0.f : bw_clipf(0.01666666666666667f * v + 1.f, 0.f, 1.f);
		break;
	}
	}
	return r;
}
