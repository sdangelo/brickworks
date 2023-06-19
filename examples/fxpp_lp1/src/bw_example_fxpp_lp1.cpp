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

#include "bw_example_fxpp_lp1.h"

void bw_example_fxpp_lp1_init(bw_example_fxpp_lp1 *instance) {
	(void)instance;
}

void bw_example_fxpp_lp1_set_sample_rate(bw_example_fxpp_lp1 *instance, float sample_rate) {
	instance->lp1.setSampleRate(sample_rate);
}

void bw_example_fxpp_lp1_reset(bw_example_fxpp_lp1 *instance) {
	instance->lp1.reset();
}

void bw_example_fxpp_lp1_process(bw_example_fxpp_lp1 *instance, const float** x, float** y, int n_samples) {
	instance->lp1.process({x[0]}, {y[0]}, n_samples);
}

void bw_example_fxpp_lp1_set_parameter(bw_example_fxpp_lp1 *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_cutoff:
		instance->lp1.setCutoff((20e3f - 20.f) * value * value * value + 20.f);
		break;
	}
}

float bw_example_fxpp_lp1_get_parameter(bw_example_fxpp_lp1 *instance, int index) {
	return instance->params[index];
}
