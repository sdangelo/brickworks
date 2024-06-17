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
#include <bw_delay.h>

typedef struct plugin {
	bw_delay_coeffs	delay_coeffs;
	bw_delay_state	delay_state;
} plugin;

static void plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	bw_delay_init(&instance->delay_coeffs, 1.f);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_delay_set_sample_rate(&instance->delay_coeffs, sample_rate);
}

static size_t plugin_mem_req(plugin *instance) {
	return bw_delay_mem_req(&instance->delay_coeffs);
}

static void plugin_mem_set(plugin *instance, void *mem) {
	bw_delay_mem_set(&instance->delay_coeffs, &instance->delay_state, mem);
}

static void plugin_reset(plugin *instance) {
	bw_delay_reset_coeffs(&instance->delay_coeffs);
	bw_delay_reset_state(&instance->delay_coeffs, &instance->delay_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	(void)index;
	bw_delay_set_delay(&instance->delay_coeffs, 0.001f * value);
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_delay_process(&instance->delay_coeffs, &instance->delay_state, inputs[0], outputs[0], n_samples);
}
