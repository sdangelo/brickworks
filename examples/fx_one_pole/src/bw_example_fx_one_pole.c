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

#include "bw_example_fx_one_pole.h"

void bw_example_fx_one_pole_init(bw_example_fx_one_pole *instance) {
	bw_one_pole_init(&instance->one_pole_coeffs);
}

void bw_example_fx_one_pole_set_sample_rate(bw_example_fx_one_pole *instance, float sample_rate) {
	bw_one_pole_set_sample_rate(&instance->one_pole_coeffs, sample_rate);
}

void bw_example_fx_one_pole_reset(bw_example_fx_one_pole *instance) {
	bw_one_pole_reset_coeffs(&instance->one_pole_coeffs);
	bw_one_pole_reset_state(&instance->one_pole_coeffs, &instance->one_pole_state, 0.f);
}

void bw_example_fx_one_pole_process(bw_example_fx_one_pole *instance, const float** x, float** y, int n_samples) {
	bw_one_pole_process(&instance->one_pole_coeffs, &instance->one_pole_state, x[0], y[0], n_samples);
}

void bw_example_fx_one_pole_set_parameter(bw_example_fx_one_pole *instance, int index, float value) {
	instance->params[index] = value;
	bw_one_pole_set_cutoff(&instance->one_pole_coeffs, 20.f + (20e3f - 20.f) * value * value * value);
}

float bw_example_fx_one_pole_get_parameter(bw_example_fx_one_pole *instance, int index) {
	return instance->params[index];
}
