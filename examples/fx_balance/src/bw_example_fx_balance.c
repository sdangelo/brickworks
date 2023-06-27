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

#include "bw_example_fx_balance.h"

void bw_example_fx_balance_init(bw_example_fx_balance *instance) {
	bw_balance_init(&instance->balance_coeffs);
	bw_ppm_init(&instance->ppm_coeffs);
}

void bw_example_fx_balance_set_sample_rate(bw_example_fx_balance *instance, float sample_rate) {
	bw_balance_set_sample_rate(&instance->balance_coeffs, sample_rate);
	bw_ppm_set_sample_rate(&instance->ppm_coeffs, sample_rate);
}

void bw_example_fx_balance_reset(bw_example_fx_balance *instance) {
	bw_balance_reset_coeffs(&instance->balance_coeffs);
	bw_ppm_reset_coeffs(&instance->ppm_coeffs);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_l_state);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_r_state);
}

void bw_example_fx_balance_process(bw_example_fx_balance *instance, const float** x, float** y, int n_samples) {
	bw_balance_process(&instance->balance_coeffs, x[0], x[1], y[0], y[1], n_samples);
	bw_ppm_state *ppm_states[2] = { &instance->ppm_l_state, &instance->ppm_r_state };
	bw_ppm_process_multi(&instance->ppm_coeffs, ppm_states, (const float **)y, NULL, 2, n_samples);
}

void bw_example_fx_balance_set_parameter(bw_example_fx_balance *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_balance:
		bw_balance_set_balance(&instance->balance_coeffs, value + value - 1.f);
		break;
	}
}

float bw_example_fx_balance_get_parameter(bw_example_fx_balance *instance, int index) {
	float r = 0.f;
	switch (index) {
	case p_balance:
		r = instance->params[p_balance];
		break;
	case p_balance + 1:
	{
		const float v = bw_ppm_get_y_z1(&instance->ppm_l_state);
		r = v < -200.f ? 0.f : bw_clipf(0.01666666666666667f * v + 1.f, 0.f, 1.f);
		break;
	}
	case p_balance + 2:
	{
		const float v = bw_ppm_get_y_z1(&instance->ppm_r_state);
		r = v < -200.f ? 0.f : bw_clipf(0.01666666666666667f * v + 1.f, 0.f, 1.f);
		break;
	}
	}
	return r;
}
