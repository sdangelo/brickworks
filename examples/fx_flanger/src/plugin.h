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
#include <bw_chorus.h>

typedef struct plugin {
	bw_chorus_coeffs	chorus_coeffs;
	bw_chorus_state		chorus_state;
} plugin;

static void plugin_init(plugin*instance) {
	bw_chorus_init(&instance->chorus_coeffs, 0.002f);
	bw_chorus_set_delay(&instance->chorus_coeffs, 0.001f);
	// values taken from Dattorro's paper
	bw_chorus_set_coeff_x(&instance->chorus_coeffs, 0.7071f);
	bw_chorus_set_coeff_mod(&instance->chorus_coeffs, 0.7071f);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_chorus_set_sample_rate(&instance->chorus_coeffs, sample_rate);
}

static size_t plugin_mem_req(plugin *instance) {
	return bw_chorus_mem_req(&instance->chorus_coeffs);
}

static void plugin_mem_set(plugin *instance, void *mem) {
	bw_chorus_mem_set(&instance->chorus_coeffs, &instance->chorus_state, mem);
}

static void plugin_reset(plugin *instance) {
	bw_chorus_reset_coeffs(&instance->chorus_coeffs);
	bw_chorus_reset_state(&instance->chorus_coeffs, &instance->chorus_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_chorus_set_rate(&instance->chorus_coeffs, value);
		break;
	case 1:
		bw_chorus_set_amount(&instance->chorus_coeffs, (0.01f * 0.001f) * value);
		break;
	case 2:
		bw_chorus_set_coeff_fb(&instance->chorus_coeffs, 0.01f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_chorus_process(&instance->chorus_coeffs, &instance->chorus_state, inputs[0], outputs[0], n_samples);
}
