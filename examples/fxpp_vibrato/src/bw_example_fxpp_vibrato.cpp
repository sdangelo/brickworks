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

#include "bw_example_fxpp_vibrato.h"

void bw_example_fxpp_vibrato_init(bw_example_fxpp_vibrato *instance) {
	instance->chorus.setCoeffX(0.f);
	instance->chorus.setCoeffMod(1.f);
	instance->chorus.setCoeffFB(0.f);
}

void bw_example_fxpp_vibrato_set_sample_rate(bw_example_fxpp_vibrato *instance, float sample_rate) {
	instance->chorus.setSampleRate(sample_rate);
}

void bw_example_fxpp_vibrato_reset(bw_example_fxpp_vibrato *instance) {
	instance->chorus.reset();
}

void bw_example_fxpp_vibrato_process(bw_example_fxpp_vibrato *instance, const float** x, float** y, int n_samples) {
	instance->chorus.process({x[0]}, {y[0]}, n_samples);
}

void bw_example_fxpp_vibrato_set_parameter(bw_example_fxpp_vibrato *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_rate:
		instance->chorus.setRate(2.f + 8.f * value * value * value);
		break;
	case p_depth:
		{
			float v = 0.0025f * value;
			instance->chorus.setDelay(v);
			instance->chorus.setAmount(v);
		}
		break;
	}
}

float bw_example_fxpp_vibrato_get_parameter(bw_example_fxpp_vibrato *instance, int index) {
	return instance->params[index];
}
