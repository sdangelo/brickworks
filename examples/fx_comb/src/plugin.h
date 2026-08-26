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

typedef struct {
	bw_comb_coeffs	comb_coeffs;
	bw_comb_state	comb_state;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	bw_comb_init(&instance->comb_coeffs, 1.f);
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_comb_set_sample_rate(&instance->comb_coeffs, sample_rate);
}

static size_t plugin_mem_req(plugin *instance) {
	return bw_comb_mem_req(&instance->comb_coeffs);
}

static void plugin_mem_set(plugin *instance, void *mem) {
	bw_comb_mem_set(&instance->comb_coeffs, &instance->comb_state, mem);
}

static void plugin_reset(plugin *instance) {
	bw_comb_reset_coeffs(&instance->comb_coeffs);
	bw_comb_reset_state(&instance->comb_coeffs, &instance->comb_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_ff_delay:
		bw_comb_set_delay_ff(&instance->comb_coeffs, 0.001f * value);
		break;
	case plugin_parameter_fb_delay:
		bw_comb_set_delay_fb(&instance->comb_coeffs, 0.001f * value);
		break;
	case plugin_parameter_blend:
		bw_comb_set_coeff_blend(&instance->comb_coeffs, value);
		break;
	case plugin_parameter_ff:
		bw_comb_set_coeff_ff(&instance->comb_coeffs, value);
		break;
	case plugin_parameter_fb:
		bw_comb_set_coeff_fb(&instance->comb_coeffs, value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_comb_process(&instance->comb_coeffs, &instance->comb_state, inputs[0], outputs[0], n_samples);
}
