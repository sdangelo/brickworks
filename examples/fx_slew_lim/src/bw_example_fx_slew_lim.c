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

#include "bw_example_fx_slew_lim.h"

void bw_example_fx_slew_lim_init(bw_example_fx_slew_lim *instance) {
	bw_slew_lim_init(&instance->slew_lim_coeffs);
}

void bw_example_fx_slew_lim_set_sample_rate(bw_example_fx_slew_lim *instance, float sample_rate) {
	bw_slew_lim_set_sample_rate(&instance->slew_lim_coeffs, sample_rate);
}

void bw_example_fx_slew_lim_reset(bw_example_fx_slew_lim *instance) {
	bw_slew_lim_reset_coeffs(&instance->slew_lim_coeffs);
	bw_slew_lim_reset_state(&instance->slew_lim_coeffs, &instance->slew_lim_state, 0.f);
}

void bw_example_fx_slew_lim_process(bw_example_fx_slew_lim *instance, const float** x, float** y, int n_samples) {
	bw_slew_lim_process(&instance->slew_lim_coeffs, &instance->slew_lim_state, x[0], y[0], n_samples);
}

void bw_example_fx_slew_lim_set_parameter(bw_example_fx_slew_lim *instance, int index, float value) {
	instance->params[index] = value;
	bw_slew_lim_set_max_rate(&instance->slew_lim_coeffs, 20.f + (20e3f - 20.f) * value * value * value);
}

float bw_example_fx_slew_lim_get_parameter(bw_example_fx_slew_lim *instance, int index) {
	return instance->params[index];
}
