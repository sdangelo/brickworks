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
#include <bw_cab.h>

typedef struct plugin {
	bw_cab_coeffs	cab_coeffs;
	bw_cab_state	cab_state;
} plugin;

static void plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	bw_cab_init(&instance->cab_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_cab_set_sample_rate(&instance->cab_coeffs, sample_rate);
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
	bw_cab_reset_coeffs(&instance->cab_coeffs);
	bw_cab_reset_state(&instance->cab_coeffs, &instance->cab_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_cab_set_cutoff_low(&instance->cab_coeffs, 0.01f * value);
		break;
	case 1:
		bw_cab_set_cutoff_high(&instance->cab_coeffs, 0.01f * value);
		break;
	case 2:
		bw_cab_set_tone(&instance->cab_coeffs, 0.01f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_cab_process(&instance->cab_coeffs, &instance->cab_state, inputs[0], outputs[0], n_samples);
}
