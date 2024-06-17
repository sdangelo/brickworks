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
#include <bw_one_pole.h>

typedef struct plugin {
	bw_one_pole_coeffs	one_pole_coeffs;
	bw_one_pole_state	one_pole_state;
} plugin;

static void plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	bw_one_pole_init(&instance->one_pole_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_one_pole_set_sample_rate(&instance->one_pole_coeffs, sample_rate);
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
	bw_one_pole_reset_coeffs(&instance->one_pole_coeffs);
	bw_one_pole_reset_state(&instance->one_pole_coeffs, &instance->one_pole_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	(void)index;
	bw_one_pole_set_cutoff(&instance->one_pole_coeffs, value);
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_one_pole_process(&instance->one_pole_coeffs, &instance->one_pole_state, inputs[0], outputs[0], n_samples);
}
