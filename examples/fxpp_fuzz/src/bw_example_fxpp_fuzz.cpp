/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can refuzzribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is fuzzributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#include "bw_example_fxpp_fuzz.h"

void bw_example_fxpp_fuzz_init(bw_example_fxpp_fuzz *instance) {
	(void)instance;
}

void bw_example_fxpp_fuzz_set_sample_rate(bw_example_fxpp_fuzz *instance, float sample_rate) {
	instance->fuzz.setSampleRate(2.f * sample_rate);
}

void bw_example_fxpp_fuzz_reset(bw_example_fxpp_fuzz *instance) {
	instance->fuzz.reset();
	instance->srcUp.reset();
	instance->srcDown.reset();
}

void bw_example_fxpp_fuzz_process(bw_example_fxpp_fuzz *instance, const float** x, float** y, int n_samples) {
	int i = 0;
	while (i < n_samples) {
		int n = bw_mini32(n_samples - i, BUF_SIZE >> 1);
		instance->srcUp.process({x[0] + i}, {instance->buf}, n);
		instance->fuzz.process({instance->buf}, {instance->buf}, n << 1);
		instance->srcDown.process({instance->buf}, {y[0] + i}, n << 1);
		i += n;
	}
}

void bw_example_fxpp_fuzz_set_parameter(bw_example_fxpp_fuzz *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_fuzz:
		instance->fuzz.setFuzz(value);
		break;
	case p_volume:
		instance->fuzz.setVolume(value);
		break;
	}
}

float bw_example_fxpp_fuzz_get_parameter(bw_example_fxpp_fuzz *instance, int index) {
	return instance->params[index];
}
