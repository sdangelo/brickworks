/*
 * Brickworks
 *
 * Copyright (C) 2023-2026 Orastron Srl unipersonale
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

#include "common.h"
#include <bw_reverb.h>

using namespace Brickworks;

typedef struct {
	Reverb<>	reverb;
	size_t		memSize;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->reverb) Reverb<>();
	return 0;
}

static void plugin_fini(plugin *instance) {
	instance->reverb.~Reverb();
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->reverb.setSampleRate(sample_rate, &instance->memSize);
}

static size_t plugin_mem_req(plugin *instance) {
	return instance->memSize;
}

static void plugin_mem_set(plugin *instance, void *mem) {
	instance->reverb.memSet(mem);
}

static void plugin_reset(plugin *instance) {
	instance->reverb.reset();
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_predelay:
		instance->reverb.setPredelay(0.001f * value);
		break;
	case plugin_parameter_bandwidth:
		instance->reverb.setBandwidth(value);
		break;
	case plugin_parameter_damping:
		instance->reverb.setDamping(value);
		break;
	case plugin_parameter_decay:
		instance->reverb.setDecay(0.01f * bw_minf(value, 99.9f));
		break;
	case plugin_parameter_wet:
		instance->reverb.setWet(0.01f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
#ifdef WASM
	const float *xL[1] = {inputs[0]};
	const float *xR[1] = {inputs[1]};
	float *yL[1] = {outputs[0]};
	float *yR[1] = {outputs[1]};
	instance->reverb.process(xL, xR, yL, yR, n_samples);
#else
	instance->reverb.process({inputs[0]}, {inputs[1]}, {outputs[0]}, {outputs[1]}, n_samples);
#endif
}
