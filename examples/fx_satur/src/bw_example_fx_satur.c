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

#include "bw_example_fx_satur.h"

void bw_example_fx_satur_init(bw_example_fx_satur *instance) {
	bw_satur_init(&instance->satur_coeffs);
	bw_src_int_init(&instance->src_up_coeffs, 2);
	bw_src_int_init(&instance->src_down_coeffs, -2);
	bw_satur_set_gain_compensation(&instance->satur_coeffs, 1);
}

void bw_example_fx_satur_set_sample_rate(bw_example_fx_satur *instance, float sample_rate) {
	bw_satur_set_sample_rate(&instance->satur_coeffs, 2.f * sample_rate);
}

void bw_example_fx_satur_reset(bw_example_fx_satur *instance) {
	bw_satur_reset_coeffs(&instance->satur_coeffs);
	bw_satur_reset_state(&instance->satur_coeffs, &instance->satur_state);
	bw_src_int_reset_state(&instance->src_up_coeffs, &instance->src_up_state, 0.f);
	bw_src_int_reset_state(&instance->src_down_coeffs, &instance->src_down_state, 0.f);
}

void bw_example_fx_satur_process(bw_example_fx_satur *instance, const float** x, float** y, int n_samples) {
	int i = 0;
	while (i < n_samples) {
		int n = bw_mini32(n_samples - i, BUF_SIZE >> 1);
		bw_src_int_process(&instance->src_up_coeffs, &instance->src_up_state, x[0] + i, instance->buf, n);
		bw_satur_process(&instance->satur_coeffs, &instance->satur_state, instance->buf, instance->buf, n << 1);
		bw_src_int_process(&instance->src_down_coeffs, &instance->src_down_state, instance->buf, y[0] + i, n << 1);
		i += n;
	}
}

void bw_example_fx_satur_set_parameter(bw_example_fx_satur *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_bias:
		bw_satur_set_bias(&instance->satur_coeffs, 5.f * value - 2.5f);
		break;
	case p_gain:
		bw_satur_set_gain(&instance->satur_coeffs, 0.1f + (10.f - 0.1f) * value * value * value);
		break;
	}
}

float bw_example_fx_satur_get_parameter(bw_example_fx_satur *instance, int index) {
	return instance->params[index];
}
