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
#include <bw_chorus.h>

using namespace Brickworks;

typedef struct {
	Chorus<>	chorus;
	size_t		memSize;
	float		rateK;
	float		amountK;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->chorus) Chorus<>(0.04f); // = 2 semitones * 2 @ 1hz, rounded
	instance->chorus.setCoeffX(0.f);
	instance->chorus.setCoeffMod(1.f);
	// for the first plugin_set_parameter()
	instance->rateK = 1.f / 6.283185307179586f;
	instance->amountK = 0.f;
	return 0;
}

static void plugin_fini(plugin *instance) {
	instance->chorus.~Chorus();
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->chorus.setSampleRate(sample_rate, &instance->memSize);
}

static size_t plugin_mem_req(plugin *instance) {
	return instance->memSize;
}

static void plugin_mem_set(plugin *instance, void *mem) {
	instance->chorus.memSet(mem);
}

static void plugin_reset(plugin *instance) {
	instance->chorus.reset();
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_rate:
		instance->chorus.setRate(value);
		instance->rateK = (1.f / 6.283185307179586f) * bw_rcpf(value);
		break;
	case plugin_parameter_amount:
		instance->amountK = bw_pow2f((1.f / 12.f) * value) - 1.f;
		break;
	}
	float v = instance->rateK * instance->amountK;
	instance->chorus.setDelay(v);
	instance->chorus.setAmount(v);
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	instance->chorus.process(inputs, outputs, n_samples);
}
