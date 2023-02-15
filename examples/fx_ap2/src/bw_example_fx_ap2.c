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

#include "bw_example_fx_ap2.h"

void bw_example_fx_ap2_init(bw_example_fx_ap2 *instance) {
	bw_ap2_init(&instance->ap2_coeffs);
}

void bw_example_fx_ap2_set_sample_rate(bw_example_fx_ap2 *instance, float sample_rate) {
	bw_ap2_set_sample_rate(&instance->ap2_coeffs, sample_rate);
}

void bw_example_fx_ap2_reset(bw_example_fx_ap2 *instance) {
	bw_ap2_reset_coeffs(&instance->ap2_coeffs);
	bw_ap2_reset_state(&instance->ap2_coeffs, &instance->ap2_state);
}

void bw_example_fx_ap2_process(bw_example_fx_ap2 *instance, const float** x, float** y, int n_samples) {
	bw_ap2_process(&instance->ap2_coeffs, &instance->ap2_state, x[0], y[0], n_samples);
}

void bw_example_fx_ap2_set_parameter(bw_example_fx_ap2 *instance, int index, float value) {
	instance->params[index] = value;
	if (index == 0)
		bw_ap2_set_cutoff(&instance->ap2_coeffs, 20.f + (20e3f - 20.f) * value * value * value);
	else
		bw_ap2_set_Q(&instance->ap2_coeffs, 0.5f + 9.5f * value);
}

float bw_example_fx_ap2_get_parameter(bw_example_fx_ap2 *instance, int index) {
	return instance->params[index];
}
