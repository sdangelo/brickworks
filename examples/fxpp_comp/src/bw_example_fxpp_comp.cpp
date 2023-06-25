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

#include "bw_example_fxpp_comp.h"

void bw_example_fxpp_comp_init(bw_example_fxpp_comp *instance) {
	(void)instance;
}

void bw_example_fxpp_comp_set_sample_rate(bw_example_fxpp_comp *instance, float sample_rate) {
	instance->comp.setSampleRate(sample_rate);
}

void bw_example_fxpp_comp_reset(bw_example_fxpp_comp *instance) {
	instance->comp.reset();
}

void bw_example_fxpp_comp_process(bw_example_fxpp_comp *instance, const float** x, float** y, int n_samples) {
	instance->comp.process({x[0]}, {x[0]}, {y[0]}, n_samples);
}

void bw_example_fxpp_comp_set_parameter(bw_example_fxpp_comp *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_thresh:
		instance->comp.setTreshDBFS(60.f * value - 60.f);
		break;
	case p_ratio:
		instance->comp.setRatio(value);
		break;
	case p_attack:
		instance->comp.setAttackTau(value);
		break;
	case p_release:
		instance->comp.setReleaseTau(value);
		break;
	case p_gain:
		instance->comp.setGainDB(60.f * value);
		break;
	}
}

float bw_example_fxpp_comp_get_parameter(bw_example_fxpp_comp *instance, int index) {
	return instance->params[index];
}
