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

#include "bw_example_fxpp_comb.h"

void bw_example_fxpp_comb_init(bw_example_fxpp_comb *instance) {
	(void)instance;
}

void bw_example_fxpp_comb_set_sample_rate(bw_example_fxpp_comb *instance, float sample_rate) {
	instance->comb.setSampleRate(sample_rate);
}

void bw_example_fxpp_comb_reset(bw_example_fxpp_comb *instance) {
	instance->comb.reset();
}

void bw_example_fxpp_comb_process(bw_example_fxpp_comb *instance, const float** x, float** y, int n_samples) {
	instance->comb.process({x[0]}, {y[0]}, n_samples);
}

void bw_example_fxpp_comb_set_parameter(bw_example_fxpp_comb *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_ff_delay:
		instance->comb.setDelayFF(value);
		break;
	case p_fb_delay:
		instance->comb.setDelayFB(value);
		break;
	case p_blend:
		instance->comb.setCoeffBlend(value);
		break;
	case p_ff:
		instance->comb.setCoeffFF(value + value - 1.f);
		break;
	case p_fb:
		instance->comb.setCoeffFB(1.99f * value - 0.995f);
		break;
	}
}

float bw_example_fxpp_comb_get_parameter(bw_example_fxpp_comb *instance, int index) {
	return instance->params[index];
}
