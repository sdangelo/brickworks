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

#include "bw_example_fx_comb.h"

void bw_example_fx_comb_init(bw_example_fx_comb *instance) {
	bw_comb_init(&instance->comb_coeffs, 1.f);
}

void bw_example_fx_comb_set_sample_rate(bw_example_fx_comb *instance, float sample_rate) {
	bw_comb_set_sample_rate(&instance->comb_coeffs, sample_rate);
}

BW_SIZE_T bw_example_fx_comb_mem_req(bw_example_fx_comb *instance) {
	return bw_comb_mem_req(&instance->comb_coeffs);
}

void bw_example_fx_comb_mem_set(bw_example_fx_comb *instance, void *mem) {
	bw_comb_mem_set(&instance->comb_state, mem);
}

void bw_example_fx_comb_reset(bw_example_fx_comb *instance) {
	bw_comb_reset_coeffs(&instance->comb_coeffs);
	bw_comb_reset_state(&instance->comb_coeffs, &instance->comb_state);
}

void bw_example_fx_comb_process(bw_example_fx_comb *instance, const float** x, float** y, int n_samples) {
	bw_comb_process(&instance->comb_coeffs, &instance->comb_state, x[0], NULL, y[0], n_samples);
}

void bw_example_fx_comb_set_parameter(bw_example_fx_comb *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_ff_delay:
		bw_comb_set_delay_ff(&instance->comb_coeffs, value);
		break;
	case p_fb_delay:
		bw_comb_set_delay_fb(&instance->comb_coeffs, value);
		break;
	case p_blend:
		bw_comb_set_coeff_blend(&instance->comb_coeffs, value);
		break;
	case p_ff:
		bw_comb_set_coeff_ff(&instance->comb_coeffs, value + value - 1.f);
		break;
	case p_fb:
		bw_comb_set_coeff_fb(&instance->comb_coeffs, 1.99f * value - 0.995f);
		break;
	}
}

float bw_example_fx_comb_get_parameter(bw_example_fx_comb *instance, int index) {
	return instance->params[index];
}
