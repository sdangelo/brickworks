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

#include "common.h"
#include <bw_reverb.h>

typedef struct plugin {
	bw_reverb_coeffs	reverb_coeffs;
	bw_reverb_state		reverb_state;
} plugin;

static void plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	bw_reverb_init(&instance->reverb_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_reverb_set_sample_rate(&instance->reverb_coeffs, sample_rate);
}

static size_t plugin_mem_req(plugin *instance) {
	return bw_reverb_mem_req(&instance->reverb_coeffs);
}

static void plugin_mem_set(plugin *instance, void *mem) {
	bw_reverb_mem_set(&instance->reverb_coeffs, &instance->reverb_state, mem);
}

static void plugin_reset(plugin *instance) {
	bw_reverb_reset_coeffs(&instance->reverb_coeffs);
	float yl, yr;
	bw_reverb_reset_state(&instance->reverb_coeffs, &instance->reverb_state, 0.f, 0.f, &yl, &yr);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_reverb_set_predelay(&instance->reverb_coeffs, 0.001f * value);
		break;
	case 1:
		bw_reverb_set_bandwidth(&instance->reverb_coeffs, value);
		break;
	case 2:
		bw_reverb_set_damping(&instance->reverb_coeffs, value);
		break;
	case 3:
		bw_reverb_set_decay(&instance->reverb_coeffs, 0.01f * bw_minf(value, 99.9f));
		break;
	case 4:
		bw_reverb_set_wet(&instance->reverb_coeffs, 0.01f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_reverb_process(&instance->reverb_coeffs, &instance->reverb_state, inputs[0], inputs[1], outputs[0], outputs[1], n_samples);
}
