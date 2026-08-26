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
#include <bw_pan.h>
#include <bw_ppm.h>

using namespace Brickworks;

typedef struct {
	Pan<>	pan;
	PPM<2>	ppm;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->pan) Pan<>();
	new(&instance->ppm) PPM<2>();
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->pan.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);
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
	instance->pan.reset();
	instance->ppm.reset();
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	(void)index;
	instance->pan.setPan(0.01f * value);
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	float v = instance->ppm.getYZ1(index == plugin_parameter_l_level ? 0 : 1);
	return bw_clipf(v, -60.f, 0.f);
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
#ifdef BW_CXX_NO_ARRAY
	float *yL[1] = {outputs[0]};
	float *yR[1] = {outputs[1]};
	instance->pan.process(inputs, yL, yR, n_samples);
	instance->ppm.process(outputs, nullptr, n_samples);
#else
	instance->pan.process({inputs[0]}, {outputs[0]}, {outputs[1]}, n_samples);
	instance->ppm.process({outputs[0], outputs[1]}, {nullptr, nullptr}, n_samples);
#endif
}
