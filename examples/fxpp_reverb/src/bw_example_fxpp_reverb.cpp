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

#include "bw_example_fxpp_reverb.h"

void bw_example_fxpp_reverb_init(bw_example_fxpp_reverb *instance) {
	(void)instance;
}

void bw_example_fxpp_reverb_set_sample_rate(bw_example_fxpp_reverb *instance, float sample_rate) {
	instance->reverb.setSampleRate(sample_rate);
}

void bw_example_fxpp_reverb_reset(bw_example_fxpp_reverb *instance) {
	instance->reverb.reset();
}

void bw_example_fxpp_reverb_process(bw_example_fxpp_reverb *instance, const float** x, float** y, int n_samples) {
	instance->reverb.process({x[0]}, {x[1]}, {y[0]}, {y[1]}, n_samples);
}

void bw_example_fxpp_reverb_set_parameter(bw_example_fxpp_reverb *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_predelay:
		instance->reverb.setPredelay(0.1f * value);
		break;
	case p_bandwidth:
		instance->reverb.setBandwidth(20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_damping:
		instance->reverb.setDamping(20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_decay:
		instance->reverb.setDecay(bw_minf(value, 0.999f));
		break;
	case p_drywet:
		instance->reverb.setWet(value);
		break;
	}
}

float bw_example_fxpp_reverb_get_parameter(bw_example_fxpp_reverb *instance, int index) {
	return instance->params[index];
}
