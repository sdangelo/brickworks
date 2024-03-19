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
#include <bw_pan.h>
#include <bw_ppm.h>

using namespace Brickworks;

class Engine {
public:
	Pan<1>	pan;
	PPM<2>	ppm;
};

extern "C" {

impl impl_new(void) {
	Engine *instance = new Engine();
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->pan.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->pan.reset();
	instance->ppm.reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	(void)index;
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->pan.setPan(0.01f * value);
}

float impl_get_parameter(impl handle, size_t index) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	float v = instance->ppm.getYZ1(index - 1);
	return v < -60.f ? -60.f : (v > 0.f ? 0.f : v);
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
#ifdef WASM
	float *yL[1] = {outputs[0]};
	float *yR[1] = {outputs[1]};
	instance->pan.process(inputs, yL, yR, n_samples);
	instance->ppm.process(outputs, nullptr, n_samples);
#else
	instance->pan.process({inputs[0]}, {outputs[0]}, {outputs[1]}, n_samples);
	instance->ppm.process({outputs[0], outputs[1]}, {nullptr, nullptr}, n_samples);
#endif
}

}
