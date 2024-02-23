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
#include <bw_one_pole.h>

using namespace Brickworks;

extern "C" {

impl impl_new(void) {
	OnePole<1> *instance = new OnePole<1>();
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	OnePole<1> *instance = reinterpret_cast<OnePole<1> *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	OnePole<1> *instance = reinterpret_cast<OnePole<1> *>(handle);
	instance->setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	OnePole<1> *instance = reinterpret_cast<OnePole<1> *>(handle);
	instance->reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	(void)index;
	OnePole<1> *instance = reinterpret_cast<OnePole<1> *>(handle);
	instance->setCutoff(value);
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	OnePole<1> *instance = reinterpret_cast<OnePole<1> *>(handle);
	instance->process(inputs, outputs, n_samples);
}

}
