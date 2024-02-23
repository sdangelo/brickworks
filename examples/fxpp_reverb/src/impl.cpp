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
#include <bw_reverb.h>

using namespace Brickworks;

extern "C" {

impl impl_new(void) {
	Reverb<1> *instance = new Reverb<1>();
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	Reverb<1> *instance = reinterpret_cast<Reverb<1> *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Reverb<1> *instance = reinterpret_cast<Reverb<1> *>(handle);
	instance->setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Reverb<1> *instance = reinterpret_cast<Reverb<1> *>(handle);
	instance->reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Reverb<1> *instance = reinterpret_cast<Reverb<1> *>(handle);
	switch (index) {
	case 0:
		instance->setPredelay(0.001f * value);
		break;
	case 1:
		instance->setBandwidth(value);
		break;
	case 2:
		instance->setDamping(value);
		break;
	case 3:
		instance->setDecay(0.01f * bw_minf(value, 99.9f));
		break;
	case 4:
		instance->setWet(0.01f * value);
		break;
	}
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	Reverb<1> *instance = reinterpret_cast<Reverb<1> *>(handle);
#ifdef WASM
	const float *xL[1] = {inputs[0]};
	const float *xR[1] = {inputs[1]};
	float *yL[1] = {outputs[0]};
	float *yR[1] = {outputs[1]};
	instance->process(xL, xR, yL, yR, n_samples);
#else
	instance->process({inputs[0]}, {inputs[1]}, {outputs[0]}, {outputs[1]}, n_samples);
#endif
}

}
