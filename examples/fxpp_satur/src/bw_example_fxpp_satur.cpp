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

#include "bw_example_fxpp_satur.h"

void bw_example_fxpp_satur_init(bw_example_fxpp_satur *instance) {
	(void)instance;
}

void bw_example_fxpp_satur_set_sample_rate(bw_example_fxpp_satur *instance, float sample_rate) {
	instance->satur.setSampleRate(2.f * sample_rate);
}

void bw_example_fxpp_satur_reset(bw_example_fxpp_satur *instance) {
	instance->satur.reset();
	instance->srcUp.reset();
	instance->srcDown.reset();
}

void bw_example_fxpp_satur_process(bw_example_fxpp_satur *instance, const float** x, float** y, int n_samples) {
	int i = 0;
	while (i < n_samples) {
		int n = bw_mini32(n_samples - i, BUF_SIZE >> 1);
		instance->srcUp.process({x[0] + i}, {instance->buf}, n);
		instance->satur.process({instance->buf}, {instance->buf}, n << 1);
		instance->srcDown.process({instance->buf}, {y[0] + i}, n << 1);
		i += n;
	}
}

void bw_example_fxpp_satur_set_parameter(bw_example_fxpp_satur *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_bias:
		instance->satur.setBias(5.f * value - 2.5f);
		break;
	case p_gain:
		instance->satur.setGain(0.1f + (10.f - 0.1f) * value * value * value);
		break;
	}
}

float bw_example_fxpp_satur_get_parameter(bw_example_fxpp_satur *instance, int index) {
	return instance->params[index];
}
