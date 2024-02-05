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

#include "bw_example_fx_cab.h"

void bw_example_fx_cab_init(bw_example_fx_cab *instance) {
	bw_cab_init(&instance->cab_coeffs);
}

void bw_example_fx_cab_set_sample_rate(bw_example_fx_cab *instance, float sample_rate) {
	bw_cab_set_sample_rate(&instance->cab_coeffs, sample_rate);
}

void bw_example_fx_cab_reset(bw_example_fx_cab *instance) {
	bw_cab_reset_coeffs(&instance->cab_coeffs);
	bw_cab_reset_state(&instance->cab_coeffs, &instance->cab_state, 0.f);
}

void bw_example_fx_cab_process(bw_example_fx_cab *instance, const float** x, float** y, int n_samples) {
	bw_cab_process(&instance->cab_coeffs, &instance->cab_state, x[0], y[0], n_samples);
}

void bw_example_fx_cab_set_parameter(bw_example_fx_cab *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_cutoff_low:
		bw_cab_set_cutoff_low(&instance->cab_coeffs, value);
		break;
	case p_cutoff_high:
		bw_cab_set_cutoff_high(&instance->cab_coeffs, value);
		break;
	case p_tone:
		bw_cab_set_tone(&instance->cab_coeffs, value);
		break;
	}
}

float bw_example_fx_cab_get_parameter(bw_example_fx_cab *instance, int index) {
	return instance->params[index];
}