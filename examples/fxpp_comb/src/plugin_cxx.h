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
#include <bw_comb.h>

using namespace Brickworks;

typedef struct {
	Comb<>	comb;
	size_t	memSize;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->comb) Comb<>(1.f);
	return 0;
}

static void plugin_fini(plugin *instance) {
	instance->comb.~Comb();
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->comb.setSampleRate(sample_rate, &instance->memSize);
}

static size_t plugin_mem_req(plugin *instance) {
	return instance->memSize;
}

static void plugin_mem_set(plugin *instance, void *mem) {
	instance->comb.memSet(mem);
}

static void plugin_reset(plugin *instance) {
	instance->comb.reset();
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_ff_delay:
		instance->comb.setDelayFF(0.001f * value);
		break;
	case plugin_parameter_fb_delay:
		instance->comb.setDelayFB(0.001f * value);
		break;
	case plugin_parameter_blend:
		instance->comb.setCoeffBlend(value);
		break;
	case plugin_parameter_ff:
		instance->comb.setCoeffFF(value);
		break;
	case plugin_parameter_fb:
		instance->comb.setCoeffFB(value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	instance->comb.process(inputs, outputs, n_samples);
}
