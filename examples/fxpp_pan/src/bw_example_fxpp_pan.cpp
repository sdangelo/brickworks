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

#include "bw_example_fxpp_pan.h"

void bw_example_fxpp_pan_init(bw_example_fxpp_pan *instance) {
	(void)instance;
}

void bw_example_fxpp_pan_set_sample_rate(bw_example_fxpp_pan *instance, float sample_rate) {
	instance->pan.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);
}

void bw_example_fxpp_pan_reset(bw_example_fxpp_pan *instance) {
	instance->pan.reset();
	instance->ppm.reset();
}

void bw_example_fxpp_pan_process(bw_example_fxpp_pan *instance, const float** x, float** y, int n_samples) {
	instance->pan.process({x[0]}, {y[0]}, {y[1]}, n_samples);
	instance->ppm.process({y[0], y[1]}, {nullptr, nullptr}, n_samples);
}

void bw_example_fxpp_pan_set_parameter(bw_example_fxpp_pan *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_pan:
		instance->pan.setPan(value + value - 1.f);
		break;
	}
}

float bw_example_fxpp_pan_get_parameter(bw_example_fxpp_pan *instance, int index) {
	float r = 0.f;
	switch (index) {
	case p_pan:
		r = instance->params[p_pan];
		break;
	case p_pan + 1:
	{
		const float v = instance->ppm.getYZ1(0);
		r = v < -200.f ? 0.f : bw_clipf(0.01666666666666667f * v + 1.f, 0.f, 1.f);
		break;
	}
	case p_pan + 2:
	{
		const float v = instance->ppm.getYZ1(1);
		r = v < -200.f ? 0.f : bw_clipf(0.01666666666666667f * v + 1.f, 0.f, 1.f);
		break;
	}
	}
	return r;
}
