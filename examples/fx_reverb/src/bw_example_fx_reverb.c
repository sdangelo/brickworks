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

#include "bw_example_fx_reverb.h"

void bw_example_fx_reverb_init(bw_example_fx_reverb *instance) {
	bw_reverb_init(&instance->reverb_coeffs);
}

void bw_example_fx_reverb_set_sample_rate(bw_example_fx_reverb *instance, float sample_rate) {
	bw_reverb_set_sample_rate(&instance->reverb_coeffs, sample_rate);
}

BW_SIZE_T bw_example_fx_reverb_mem_req(bw_example_fx_reverb *instance) {
	return bw_reverb_mem_req(&instance->reverb_coeffs);
}

void bw_example_fx_reverb_mem_set(bw_example_fx_reverb *instance, void *mem) {
	bw_reverb_mem_set(&instance->reverb_coeffs, &instance->reverb_state, mem);
}

void bw_example_fx_reverb_reset(bw_example_fx_reverb *instance) {
	bw_reverb_reset_coeffs(&instance->reverb_coeffs);
	bw_reverb_reset_state(&instance->reverb_coeffs, &instance->reverb_state);
}

void bw_example_fx_reverb_process(bw_example_fx_reverb *instance, const float** x, float** y, int n_samples) {
	bw_reverb_process(&instance->reverb_coeffs, &instance->reverb_state, x[0], x[1], y[0], y[1], n_samples);
}

void bw_example_fx_reverb_set_parameter(bw_example_fx_reverb *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_predelay:
		bw_reverb_set_predelay(&instance->reverb_coeffs, 0.1f * value);
		break;
	case p_bandwidth:
		bw_reverb_set_bandwidth(&instance->reverb_coeffs, 20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_damping:
		bw_reverb_set_damping(&instance->reverb_coeffs, 20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_decay:
		bw_reverb_set_decay(&instance->reverb_coeffs, bw_minf(value, 0.999f));
		break;
	case p_drywet:
		bw_reverb_set_wet(&instance->reverb_coeffs, value);
		break;
	}
}

float bw_example_fx_reverb_get_parameter(bw_example_fx_reverb *instance, int index) {
	return instance->params[index];
}
