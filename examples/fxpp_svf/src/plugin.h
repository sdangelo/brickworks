/*
 * Brickworks
 *
 * Copyright (C) 2022-2024 Orastron Srl unipersonale
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
#include <bw_svf.h>

typedef struct plugin {
	bw_svf_coeffs	svf_coeffs;
	bw_svf_state	svf_state;
} plugin;

static void plugin_init(plugin *instance) {
	bw_svf_init(&instance->svf_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_svf_set_sample_rate(&instance->svf_coeffs, sample_rate);
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
	bw_svf_reset_coeffs(&instance->svf_coeffs);
	float v_lp, v_bp, v_hp;
	bw_svf_reset_state(&instance->svf_coeffs, &instance->svf_state, 0.f, &v_lp, &v_bp, &v_hp);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_svf_set_cutoff(&instance->svf_coeffs, value);
		break;
	case 1:
		bw_svf_set_Q(&instance->svf_coeffs, value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_svf_process(&instance->svf_coeffs, &instance->svf_state, inputs[0], outputs[0], NULL, NULL, n_samples);
}
