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
#include <bw_comp.h>

using namespace Brickworks;

typedef struct {
	Comp<>	comp;
	bool	extSidechain;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->comp) Comp<>();
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->comp.setSampleRate(sample_rate);
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
	instance->comp.reset();
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_threshold:
		instance->comp.setThreshDBFS(value);
		break;
	case plugin_parameter_ratio:
		instance->comp.setRatio(bw_rcpf(value));
		break;
	case plugin_parameter_attack:
		// using rise time 10% -> 90%: tau = rise time / log(9)
		instance->comp.setAttackTau((0.001f * 0.4551196133134186f) * value);
		break;
	case plugin_parameter_release:
		// as before
		instance->comp.setReleaseTau((0.001f * 0.4551196133134186f) * value);
		break;
	case plugin_parameter_gain:
		instance->comp.setGainDB(value);
		break;
	case plugin_parameter_ext_sidechain:
		instance->extSidechain = value >= 0.5f;
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
#ifdef BW_CXX_NO_ARRAY
	const float *x[1] = {inputs[0]};
	const float *xSc[1] = {instance->extSidechain ? inputs[1] : inputs[0]};
	float *y[1] = {outputs[0]};
	instance->comp.process(x, xSc, y, n_samples);
#else
	instance->comp.process({inputs[0]}, {instance->extSidechain ? inputs[1] : inputs[0]}, {outputs[0]}, n_samples);
#endif
}
