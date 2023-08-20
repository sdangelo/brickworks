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

#include "bw_example_fx_delay.h"

void bw_example_fx_delay_init(bw_example_fx_delay *instance) {
	bw_delay_init(&instance->delay_coeffs, 1.f);
}

void bw_example_fx_delay_set_sample_rate(bw_example_fx_delay *instance, float sample_rate) {
	bw_delay_set_sample_rate(&instance->delay_coeffs, sample_rate);
}

size_t bw_example_fx_delay_mem_req(bw_example_fx_delay *instance) {
	return bw_delay_mem_req(&instance->delay_coeffs);
}

void bw_example_fx_delay_mem_set(bw_example_fx_delay *instance, void *mem) {
	bw_delay_mem_set(&instance->delay_coeffs, &instance->delay_state, mem);
}

void bw_example_fx_delay_reset(bw_example_fx_delay *instance) {
	bw_delay_reset_coeffs(&instance->delay_coeffs);
	bw_delay_reset_state(&instance->delay_coeffs, &instance->delay_state, 0.f);
}

void bw_example_fx_delay_process(bw_example_fx_delay *instance, const float** x, float** y, int n_samples) {
	bw_delay_process(&instance->delay_coeffs, &instance->delay_state, x[0], y[0], n_samples);
}

void bw_example_fx_delay_set_parameter(bw_example_fx_delay *instance, int index, float value) {
	instance->params[index] = value;
	bw_delay_set_delay(&instance->delay_coeffs, value);
}

float bw_example_fx_delay_get_parameter(bw_example_fx_delay *instance, int index) {
	return instance->params[index];
}
