/*
 * Brickworks
 *
 * Copyright (C) 2022-2024 Orastron Srl unipersonale
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
#include <bw_satur.h>
#include <bw_src_int.h>

using namespace Brickworks;

#define BUF_SIZE	32

class Engine {
public:
	Engine() : srcUp(2), srcDown(-2)  {}

	Satur<1>	satur;
	SRCInt<1>	srcUp;
	SRCInt<1>	srcDown;

	float		buf[BUF_SIZE];
};

extern "C" {

impl impl_new(void) {
	Engine *instance = new Engine();
	instance->satur.setGainCompensation(true);
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->satur.setSampleRate(2.f * sample_rate);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->satur.reset();
	instance->srcUp.reset();
	instance->srcDown.reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (index) {
	case 0:
		instance->satur.setGain(value);
		break;
	case 1:
		instance->satur.setBias(value);
		break;
	}
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	size_t i = 0;
	while (i < n_samples) {
		int n = bw_mini32(n_samples - i, BUF_SIZE >> 1);
#ifdef WASM
		const float *x[1] = {inputs[0] + i};
		float *y[1] = {outputs[0] + i};
		float *b[1] = {instance->buf};
		instance->srcUp.process(x, b, n);
		instance->satur.process(b, b, n << 1);
		instance->srcDown.process(b, y, n << 1);
#else
		instance->srcUp.process({inputs[0] + i}, {instance->buf}, n);
		instance->satur.process({instance->buf}, {instance->buf}, n << 1);
		instance->srcDown.process({instance->buf}, {outputs[0] + i}, n << 1);
#endif
		i += n;
	}
}

}
