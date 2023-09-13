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

#include "bw_example_fx_clip.h"

void bw_example_fx_clip_init(bw_example_fx_clip *instance) {
	bw_clip_init(&instance->clip_coeffs);
	bw_src_int_init(&instance->src_up_coeffs, 2);
	bw_src_int_init(&instance->src_down_coeffs, -2);
	bw_clip_set_gain_compensation(&instance->clip_coeffs, 1);
}

void bw_example_fx_clip_set_sample_rate(bw_example_fx_clip *instance, float sample_rate) {
	bw_clip_set_sample_rate(&instance->clip_coeffs, 2.f * sample_rate);
}

void bw_example_fx_clip_reset(bw_example_fx_clip *instance) {
	bw_clip_reset_coeffs(&instance->clip_coeffs);
	bw_clip_reset_state(&instance->clip_coeffs, &instance->clip_state, 0.f);
	bw_src_int_reset_state(&instance->src_up_coeffs, &instance->src_up_state, 0.f);
	bw_src_int_reset_state(&instance->src_down_coeffs, &instance->src_down_state, 0.f);
}

void bw_example_fx_clip_process(bw_example_fx_clip *instance, const float** x, float** y, int n_samples) {
	int i = 0;
	while (i < n_samples) {
		int n = bw_mini32(n_samples - i, BUF_SIZE >> 1);
		bw_src_int_process(&instance->src_up_coeffs, &instance->src_up_state, x[0] + i, instance->buf, n);
		bw_clip_process(&instance->clip_coeffs, &instance->clip_state, instance->buf, instance->buf, n << 1);
		bw_src_int_process(&instance->src_down_coeffs, &instance->src_down_state, instance->buf, y[0] + i, n << 1);
		i += n;
	}
}

void bw_example_fx_clip_set_parameter(bw_example_fx_clip *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_bias:
		bw_clip_set_bias(&instance->clip_coeffs, 5.f * value - 2.5f);
		break;
	case p_gain:
		bw_clip_set_gain(&instance->clip_coeffs, 0.1f + (10.f - 0.1f) * value * value * value);
		break;
	}
}

float bw_example_fx_clip_get_parameter(bw_example_fx_clip *instance, int index) {
	return instance->params[index];
}
