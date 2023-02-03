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

#include "bw_example_fx_noise_gate.h"

void bw_example_fx_noise_gate_free(bw_example_fx_noise_gate *instance) {
	bw_noise_gate_init(&instance->noise_gate_coeffs);
}

void bw_example_fx_noise_gate_set_sample_rate(bw_example_fx_noise_gate *instance, float sample_rate) {
	bw_noise_gate_set_sample_rate(&instance->noise_gate_coeffs, sample_rate);
}

void bw_example_fx_noise_gate_reset(bw_example_fx_noise_gate *instance) {
	bw_noise_gate_reset_coeffs(&instance->noise_gate_coeffs);
	bw_noise_gate_reset_state(&instance->noise_gate_coeffs, &instance->noise_gate_state);
}

void bw_example_fx_noise_gate_process(bw_example_fx_noise_gate *instance, const float** x, float** y, int n_samples) {
	bw_noise_gate_process(&instance->noise_gate_coeffs, &instance->noise_gate_state, x[0], x[0], y[0], n_samples);
}

void bw_example_fx_noise_gate_set_parameter(bw_example_fx_noise_gate *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_thresh:
		bw_noise_gate_set_thresh_dBFS(&instance->noise_gate_coeffs, 60.f * value - 60.f);
		break;
	case p_attenuation:
		bw_noise_gate_set_ratio(&instance->noise_gate_coeffs, value < 0.999f ? 1.f / (1.f - value) : INFINITY);
		break;
	case p_attack:
		bw_noise_gate_set_attack_tau(&instance->noise_gate_coeffs, value);
		break;
	case p_release:
		bw_noise_gate_set_release_tau(&instance->noise_gate_coeffs, value);
		break;
	}
}

float bw_example_fx_noise_gate_get_parameter(bw_example_fx_noise_gate *instance, int index) {
	return instance->params[index];
}
