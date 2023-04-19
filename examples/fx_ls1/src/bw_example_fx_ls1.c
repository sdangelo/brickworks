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

#include "bw_example_fx_ls1.h"

void bw_example_fx_ls1_init(bw_example_fx_ls1 *instance) {
	bw_ls1_init(&instance->ls1_coeffs);
}

void bw_example_fx_ls1_set_sample_rate(bw_example_fx_ls1 *instance, float sample_rate) {
	bw_ls1_set_sample_rate(&instance->ls1_coeffs, sample_rate);
}

void bw_example_fx_ls1_reset(bw_example_fx_ls1 *instance) {
	bw_ls1_reset_coeffs(&instance->ls1_coeffs);
	bw_ls1_reset_state(&instance->ls1_coeffs, &instance->ls1_state, 0.f);
}

void bw_example_fx_ls1_process(bw_example_fx_ls1 *instance, const float** x, float** y, int n_samples) {
	bw_ls1_process(&instance->ls1_coeffs, &instance->ls1_state, x[0], y[0], n_samples);
}

void bw_example_fx_ls1_set_parameter(bw_example_fx_ls1 *instance, int index, float value) {
	instance->params[index] = value;
	if (index == 0)
		bw_ls1_set_cutoff(&instance->ls1_coeffs, 20.f + (20e3f - 20.f) * value * value * value);
	else
		bw_ls1_set_dc_gain_dB(&instance->ls1_coeffs, -20.f + 40.f * value);
}

float bw_example_fx_ls1_get_parameter(bw_example_fx_ls1 *instance, int index) {
	return instance->params[index];
}
