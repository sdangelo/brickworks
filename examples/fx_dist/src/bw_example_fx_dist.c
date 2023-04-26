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

#include "bw_example_fx_dist.h"

void bw_example_fx_dist_init(bw_example_fx_dist *instance) {
	bw_dist_init(&instance->dist_coeffs);
	bw_src_int_init(&instance->src_up_coeffs, 2);
	bw_src_int_init(&instance->src_down_coeffs, -2);
}

void bw_example_fx_dist_set_sample_rate(bw_example_fx_dist *instance, float sample_rate) {
	bw_dist_set_sample_rate(&instance->dist_coeffs, 2.f * sample_rate);
}

void bw_example_fx_dist_reset(bw_example_fx_dist *instance) {
	bw_dist_reset_coeffs(&instance->dist_coeffs);
	bw_dist_reset_state(&instance->dist_coeffs, &instance->dist_state);
	bw_src_int_reset_state(&instance->src_up_coeffs, &instance->src_up_state, 0.f);
	bw_src_int_reset_state(&instance->src_down_coeffs, &instance->src_down_state, 0.f);
}

void bw_example_fx_dist_process(bw_example_fx_dist *instance, const float** x, float** y, int n_samples) {
	int i = 0;
	while (i < n_samples) {
		int n = bw_mini32(n_samples - i, BUF_SIZE >> 1);
		bw_src_int_process(&instance->src_up_coeffs, &instance->src_up_state, x[0] + i, instance->buf, n);
		bw_dist_process(&instance->dist_coeffs, &instance->dist_state, instance->buf, instance->buf, n << 1);
		bw_src_int_process(&instance->src_down_coeffs, &instance->src_down_state, instance->buf, y[0] + i, n << 1);
		i += n;
	}
}

void bw_example_fx_dist_set_parameter(bw_example_fx_dist *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_dist:
		bw_dist_set_distortion(&instance->dist_coeffs, value);
		break;
	case p_tone:
		bw_dist_set_tone(&instance->dist_coeffs, value);
		break;
	case p_volume:
		bw_dist_set_volume(&instance->dist_coeffs, value);
		break;
	}
}

float bw_example_fx_dist_get_parameter(bw_example_fx_dist *instance, int index) {
	return instance->params[index];
}
