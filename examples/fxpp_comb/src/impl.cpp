/*
 * Brickworks
 *
 * Copyright (C) 2023, 2024 Orastron Srl unipersonale
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

#include "impl.h"

#include "common.h"
#include <bw_comb.h>

using namespace Brickworks;

extern "C" {

impl impl_new(void) {
	Comb<1> *instance = new Comb<1>(1.f);
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	Comb<1> *instance = reinterpret_cast<Comb<1> *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Comb<1> *instance = reinterpret_cast<Comb<1> *>(handle);
	instance->setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Comb<1> *instance = reinterpret_cast<Comb<1> *>(handle);
	instance->reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Comb<1> *instance = reinterpret_cast<Comb<1> *>(handle);
	switch (index) {
	case 0:
		instance->setDelayFF(0.001f * value);
		break;
	case 1:
		instance->setDelayFB(0.001f * value);
		break;
	case 2:
		instance->setCoeffBlend(value);
		break;
	case 3:
		instance->setCoeffFF(value);
		break;
	case 4:
		instance->setCoeffFB(value);
		break;
	}
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	Comb<1> *instance = reinterpret_cast<Comb<1> *>(handle);
	instance->process(inputs, outputs, n_samples);
}

}
