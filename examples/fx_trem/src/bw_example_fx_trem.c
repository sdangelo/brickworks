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

#include "bw_example_fx_trem.h"

void bw_example_fx_trem_init(bw_example_fx_trem *instance) {
	bw_trem_init(&instance->trem_coeffs);
}

void bw_example_fx_trem_set_sample_rate(bw_example_fx_trem *instance, float sample_rate) {
	bw_trem_set_sample_rate(&instance->trem_coeffs, sample_rate);
}

void bw_example_fx_trem_reset(bw_example_fx_trem *instance) {
	bw_trem_reset_coeffs(&instance->trem_coeffs);
	bw_trem_reset_state(&instance->trem_coeffs, &instance->trem_state);
}

void bw_example_fx_trem_process(bw_example_fx_trem *instance, const float** x, float** y, int n_samples) {
	bw_trem_process(&instance->trem_coeffs, &instance->trem_state, x[0], y[0], n_samples);
}

void bw_example_fx_trem_set_parameter(bw_example_fx_trem *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_rate:
		bw_trem_set_rate(&instance->trem_coeffs, (20.f - 1.f) * value * value * value + 1.f);
		break;
	case p_amount:
		bw_trem_set_amount(&instance->trem_coeffs, value);
		break;
	}
}

float bw_example_fx_trem_get_parameter(bw_example_fx_trem *instance, int index) {
	return instance->params[index];
}
