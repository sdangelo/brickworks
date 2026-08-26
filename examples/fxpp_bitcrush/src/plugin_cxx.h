/*
 * Brickworks
 *
 * Copyright (C) 2022-2026 Orastron Srl unipersonale
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
#include <bw_sr_reduce.h>
#include <bw_bd_reduce.h>

using namespace Brickworks;

typedef struct {
	SRReduce<>	sr;
	BDReduce<2>	bd;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->sr) SRReduce<>();
	new(&instance->bd) BDReduce<2>();
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->sr.setSampleRate(sample_rate);
	instance->bd.setSampleRate(sample_rate);
}

static size_t plugin_mem_req(plugin *instance) {
	(void)instance;
	return 0;
}

static void plugin_mem_set(plugin *instance, void *mem) {
	(void)instance;
	(void)mem;
}

static void plugin_reset(plugin *instance) {
	instance->sr.reset();
	instance->bd.reset();
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_sr_ratio:
		instance->sr.setRatio(0.01f * value);
		break;
	case plugin_parameter_bit_depth:
		instance->bd.setBitDepth((char)value);
		break;
	case plugin_parameter_gate:
		instance->bd.setGateLin(value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	instance->sr.process(inputs, outputs, n_samples);
	instance->bd.process(outputs, outputs, n_samples);
}
