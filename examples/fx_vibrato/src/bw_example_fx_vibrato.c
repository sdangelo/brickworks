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

#include "bw_example_fx_vibrato.h"

void bw_example_fx_vibrato_init(bw_example_fx_vibrato *instance) {
	bw_chorus_init(&instance->chorus_coeffs, 0.005f);
	bw_chorus_set_coeff_x(&instance->chorus_coeffs, 0.f);
	bw_chorus_set_coeff_mod(&instance->chorus_coeffs, 1.f);
	bw_chorus_set_coeff_fb(&instance->chorus_coeffs, 0.f);
}

void bw_example_fx_vibrato_set_sample_rate(bw_example_fx_vibrato *instance, float sample_rate) {
	bw_chorus_set_sample_rate(&instance->chorus_coeffs, sample_rate);
}

size_t bw_example_fx_vibrato_mem_req(bw_example_fx_vibrato *instance) {
	return bw_chorus_mem_req(&instance->chorus_coeffs);
}

void bw_example_fx_vibrato_mem_set(bw_example_fx_vibrato *instance, void *mem) {
	bw_chorus_mem_set(&instance->chorus_coeffs, &instance->chorus_state, mem);
}

void bw_example_fx_vibrato_reset(bw_example_fx_vibrato *instance) {
	bw_chorus_reset_coeffs(&instance->chorus_coeffs);
	bw_chorus_reset_state(&instance->chorus_coeffs, &instance->chorus_state);
}

void bw_example_fx_vibrato_process(bw_example_fx_vibrato *instance, const float** x, float** y, int n_samples) {
	bw_chorus_process(&instance->chorus_coeffs, &instance->chorus_state, x[0], y[0], n_samples);
}

void bw_example_fx_vibrato_set_parameter(bw_example_fx_vibrato *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_rate:
		bw_chorus_set_rate(&instance->chorus_coeffs, 2.f + 8.f * value * value * value);
		break;
	case p_amount:
		{
			float v = 0.0025f * value;
			bw_chorus_set_delay(&instance->chorus_coeffs, v);
			bw_chorus_set_amount(&instance->chorus_coeffs, v);
		}
		break;
	}
}

float bw_example_fx_vibrato_get_parameter(bw_example_fx_vibrato *instance, int index) {
	return instance->params[index];
}
