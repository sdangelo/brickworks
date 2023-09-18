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

#include "bw_example_fx_ring_mod.h"

#include <bw_osc_sin.h>

void bw_example_fx_ring_mod_init(bw_example_fx_ring_mod *instance) {
	bw_phase_gen_init(&instance->phase_gen_coeffs);
	bw_ring_mod_init(&instance->ring_mod_coeffs);
}

void bw_example_fx_ring_mod_set_sample_rate(bw_example_fx_ring_mod *instance, float sample_rate) {
	bw_phase_gen_set_sample_rate(&instance->phase_gen_coeffs, sample_rate);
	bw_ring_mod_set_sample_rate(&instance->ring_mod_coeffs, sample_rate);
}

void bw_example_fx_ring_mod_reset(bw_example_fx_ring_mod *instance) {
	bw_phase_gen_reset_coeffs(&instance->phase_gen_coeffs);
	float v, v_inc;
	bw_phase_gen_reset_state(&instance->phase_gen_coeffs, &instance->phase_gen_state, 0.f, &v, &v_inc);
	bw_ring_mod_reset_coeffs(&instance->ring_mod_coeffs);
}

void bw_example_fx_ring_mod_process(bw_example_fx_ring_mod *instance, const float** x, float** y, int n_samples) {
	bw_phase_gen_process(&instance->phase_gen_coeffs, &instance->phase_gen_state, NULL, y[0], NULL, n_samples);
	bw_osc_sin_process(y[0], y[0], n_samples);
	bw_ring_mod_process(&instance->ring_mod_coeffs, x[0], y[0], y[0], n_samples);
}

void bw_example_fx_ring_mod_set_parameter(bw_example_fx_ring_mod *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_freq:
		bw_phase_gen_set_frequency(&instance->phase_gen_coeffs, 100.f + (1000.f - 100.f) * value * value * value);
		break;
	case p_amount:
		bw_ring_mod_set_amount(&instance->ring_mod_coeffs, -1.f + 2.f * value);
		break;
	}
}

float bw_example_fx_ring_mod_get_parameter(bw_example_fx_ring_mod *instance, int index) {
	return instance->params[index];
}
