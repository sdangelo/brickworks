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

#include "bw_example_fx_flanger.h"

void bw_example_fx_flanger_init(bw_example_fx_flanger *instance) {
	bw_chorus_init(&instance->chorus_coeffs, 0.002f);
	bw_chorus_set_delay(&instance->chorus_coeffs, 0.001f);
	bw_chorus_set_coeff_x(&instance->chorus_coeffs, 0.7071f);
	bw_chorus_set_coeff_mod(&instance->chorus_coeffs, 0.7071f);
	bw_chorus_set_coeff_fb(&instance->chorus_coeffs, 0.7071f);
}

void bw_example_fx_flanger_set_sample_rate(bw_example_fx_flanger *instance, float sample_rate) {
	bw_chorus_set_sample_rate(&instance->chorus_coeffs, sample_rate);
}

size_t bw_example_fx_flanger_mem_req(bw_example_fx_flanger *instance) {
	return bw_chorus_mem_req(&instance->chorus_coeffs);
}

void bw_example_fx_flanger_mem_set(bw_example_fx_flanger *instance, void *mem) {
	bw_chorus_mem_set(&instance->chorus_coeffs, &instance->chorus_state, mem);
}

void bw_example_fx_flanger_reset(bw_example_fx_flanger *instance) {
	bw_chorus_reset_coeffs(&instance->chorus_coeffs);
	bw_chorus_reset_state(&instance->chorus_coeffs, &instance->chorus_state, 0.f);
}

void bw_example_fx_flanger_process(bw_example_fx_flanger *instance, const float** x, float** y, int n_samples) {
	bw_chorus_process(&instance->chorus_coeffs, &instance->chorus_state, x[0], y[0], n_samples);
}

void bw_example_fx_flanger_set_parameter(bw_example_fx_flanger *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_rate:
		bw_chorus_set_rate(&instance->chorus_coeffs, 0.01f + 1.99f * value * value * value);
		break;
	case p_amount:
		bw_chorus_set_amount(&instance->chorus_coeffs, 0.001f * value);
		break;
	}
}

float bw_example_fx_flanger_get_parameter(bw_example_fx_flanger *instance, int index) {
	return instance->params[index];
}
