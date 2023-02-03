/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
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

#include "bw_example_fx_mm1.h"

void bw_example_fx_mm1_init(bw_example_fx_mm1 *instance) {
	bw_mm1_init(&instance->mm1_coeffs);
}

void bw_example_fx_mm1_set_sample_rate(bw_example_fx_mm1 *instance, float sample_rate) {
	bw_mm1_set_sample_rate(&instance->mm1_coeffs, sample_rate);
}

void bw_example_fx_mm1_reset(bw_example_fx_mm1 *instance) {
	bw_mm1_reset_coeffs(&instance->mm1_coeffs);
	bw_mm1_reset_state(&instance->mm1_coeffs, &instance->mm1_state);
}

void bw_example_fx_mm1_process(bw_example_fx_mm1 *instance, const float** x, float** y, int n_samples) {
	bw_mm1_process(&instance->mm1_coeffs, &instance->mm1_state, x[0], y[0], n_samples);
}

void bw_example_fx_mm1_set_parameter(bw_example_fx_mm1 *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_cutoff:
		bw_mm1_set_cutoff(&instance->mm1_coeffs, (20e3f - 20.f) * value * value * value + 20.f);
		break;
	case p_input_coeff:
		bw_mm1_set_coeff_x(&instance->mm1_coeffs, 2.f * value - 1.f);
		break;
	case p_lp_coeff:
		bw_mm1_set_coeff_lp(&instance->mm1_coeffs, 2.f * value - 1.f);
		break;
	}
}

float bw_example_fx_mm1_get_parameter(bw_example_fx_mm1 *instance, int index) {
	return instance->params[index];
}
