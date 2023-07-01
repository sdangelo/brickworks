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

#include "bw_example_fxpp_delay.h"

void bw_example_fxpp_delay_init(bw_example_fxpp_delay *instance) {
	(void)instance;
}

void bw_example_fxpp_delay_set_sample_rate(bw_example_fxpp_delay *instance, float sample_rate) {
	instance->delay.setSampleRate(sample_rate);
}

void bw_example_fxpp_delay_reset(bw_example_fxpp_delay *instance) {
	instance->delay.reset();
}

void bw_example_fxpp_delay_process(bw_example_fxpp_delay *instance, const float** x, float** y, int n_samples) {
	instance->delay.process({x[0]}, {y[0]}, n_samples);
}

void bw_example_fxpp_delay_set_parameter(bw_example_fxpp_delay *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_delay:
		instance->delay.setDelay(value);
		break;
	}
}

float bw_example_fxpp_delay_get_parameter(bw_example_fxpp_delay *instance, int index) {
	return instance->params[index];
}
