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

#include "bw_example_fx_svf.h"

void bw_example_fx_svf_init(bw_example_fx_svf *instance) {
	bw_svf_init(&instance->svf_coeffs);
}

void bw_example_fx_svf_set_sample_rate(bw_example_fx_svf *instance, float sample_rate) {
	bw_svf_set_sample_rate(&instance->svf_coeffs, sample_rate);
}

void bw_example_fx_svf_reset(bw_example_fx_svf *instance) {
	bw_svf_reset_coeffs(&instance->svf_coeffs);
	bw_svf_reset_state(&instance->svf_coeffs, &instance->svf_state);
}

void bw_example_fx_svf_process(bw_example_fx_svf *instance, const float** x, float** y, int n_samples) {
	bw_svf_process(&instance->svf_coeffs, &instance->svf_state, x[0], y[0], NULL, NULL, n_samples);
}

void bw_example_fx_svf_set_parameter(bw_example_fx_svf *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_cutoff:
		bw_svf_set_cutoff(&instance->svf_coeffs, (20e3f - 20.f) * value * value * value + 20.f);
		break;
	case p_Q:
		bw_svf_set_Q(&instance->svf_coeffs, 0.5f + 9.5f * value);
		break;
	}
}

float bw_example_fx_svf_get_parameter(bw_example_fx_svf *instance, int index) {
	return instance->params[index];
}
