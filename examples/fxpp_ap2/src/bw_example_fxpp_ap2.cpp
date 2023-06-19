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

#include "bw_example_fxpp_ap2.h"

void bw_example_fxpp_ap2_init(bw_example_fxpp_ap2 *instance) {
	(void)instance;
}

void bw_example_fxpp_ap2_set_sample_rate(bw_example_fxpp_ap2 *instance, float sample_rate) {
	instance->ap2.setSampleRate(sample_rate);
}

void bw_example_fxpp_ap2_reset(bw_example_fxpp_ap2 *instance) {
	instance->ap2.reset();
}

void bw_example_fxpp_ap2_process(bw_example_fxpp_ap2 *instance, const float** x, float** y, int n_samples) {
	instance->ap2.process({x[0]}, {y[0]}, n_samples);
}

void bw_example_fxpp_ap2_set_parameter(bw_example_fxpp_ap2 *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_cutoff:
		instance->ap2.setCutoff((20e3f - 20.f) * value * value * value + 20.f);
		break;
	case p_Q:
		instance->ap2.setQ(0.5f + 9.5f * value);
		break;
	}
}

float bw_example_fxpp_ap2_get_parameter(bw_example_fxpp_ap2 *instance, int index) {
	return instance->params[index];
}
