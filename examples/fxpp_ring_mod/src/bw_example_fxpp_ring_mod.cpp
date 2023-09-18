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

#include "bw_example_fxpp_ring_mod.h"

#include <bw_osc_sin.h>

void bw_example_fxpp_ring_mod_init(bw_example_fxpp_ring_mod *instance) {
	(void)instance;
}

void bw_example_fxpp_ring_mod_set_sample_rate(bw_example_fxpp_ring_mod *instance, float sample_rate) {
	instance->phaseGen.setSampleRate(sample_rate);
	instance->ringMod.setSampleRate(sample_rate);
}

void bw_example_fxpp_ring_mod_reset(bw_example_fxpp_ring_mod *instance) {
	instance->phaseGen.reset();
	instance->ringMod.reset();
}

void bw_example_fxpp_ring_mod_process(bw_example_fxpp_ring_mod *instance, const float** x, float** y, int n_samples) {
	instance->phaseGen.process({}, {y[0]}, {}, n_samples);
	oscSinProcess<1>({y[0]}, {y[0]}, n_samples);
	instance->ringMod.process({x[0]}, {y[0]}, {y[0]}, n_samples);
}

void bw_example_fxpp_ring_mod_set_parameter(bw_example_fxpp_ring_mod *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_freq:
		instance->phaseGen.setFrequency(100.f + (1000.f - 100.f) * value * value * value);
		break;
	case p_amount:
		instance->ringMod.setAmount(-1.f + 2.f * value);
		break;
	}
}

float bw_example_fxpp_ring_mod_get_parameter(bw_example_fxpp_ring_mod *instance, int index) {
	return instance->params[index];
}
