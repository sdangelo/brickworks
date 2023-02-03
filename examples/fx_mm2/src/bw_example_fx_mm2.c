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

#include "bw_example_fx_mm2.h"

void bw_example_fx_mm2_init(bw_example_fx_mm2 *instance) {
	bw_mm2_init(&instance->mm2_coeffs);
}

void bw_example_fx_mm2_set_sample_rate(bw_example_fx_mm2 *instance, float sample_rate) {
	bw_mm2_set_sample_rate(&instance->mm2_coeffs, sample_rate);
}

void bw_example_fx_mm2_reset(bw_example_fx_mm2 *instance) {
	bw_mm2_reset_coeffs(&instance->mm2_coeffs);
	bw_mm2_reset_state(&instance->mm2_coeffs, &instance->mm2_state);
}

void bw_example_fx_mm2_process(bw_example_fx_mm2 *instance, const float** x, float** y, int n_samples) {
	bw_mm2_process(&instance->mm2_coeffs, &instance->mm2_state, x[0], y[0], n_samples);
}

void bw_example_fx_mm2_set_parameter(bw_example_fx_mm2 *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_cutoff:
		bw_mm2_set_cutoff(&instance->mm2_coeffs, (20e3f - 20.f) * value * value * value + 20.f);
		break;
	case p_Q:
		bw_mm2_set_Q(&instance->mm2_coeffs, 0.5f + 9.5f * value);
		break;
	case p_input_coeff:
		bw_mm2_set_coeff_x(&instance->mm2_coeffs, 2.f * value - 1.f);
		break;
	case p_lp_coeff:
		bw_mm2_set_coeff_lp(&instance->mm2_coeffs, 2.f * value - 1.f);
		break;
	case p_bp_coeff:
		bw_mm2_set_coeff_bp(&instance->mm2_coeffs, 2.f * value - 1.f);
		break;
	case p_hp_coeff:
		bw_mm2_set_coeff_hp(&instance->mm2_coeffs, 2.f * value - 1.f);
		break;
	}
}

float bw_example_fx_mm2_get_parameter(bw_example_fx_mm2 *instance, int index) {
	return instance->params[index];
}
