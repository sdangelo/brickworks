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
#include <bw_pan.h>
#include <bw_ppm.h>

typedef struct plugin {
	bw_pan_coeffs	pan_coeffs;
	bw_ppm_coeffs	ppm_coeffs;
	bw_ppm_state	ppm_l_state;
	bw_ppm_state	ppm_r_state;
} plugin;

static void plugin_init(plugin *instance) {
	bw_pan_init(&instance->pan_coeffs);
	bw_ppm_init(&instance->ppm_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_pan_set_sample_rate(&instance->pan_coeffs, sample_rate);
	bw_ppm_set_sample_rate(&instance->ppm_coeffs, sample_rate);
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
	bw_pan_reset_coeffs(&instance->pan_coeffs);
	bw_ppm_reset_coeffs(&instance->ppm_coeffs);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_l_state, 0.f);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_r_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	(void)index;
	bw_pan_set_pan(&instance->pan_coeffs, 0.01f * value);
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	float v = 0.f;
	switch (index) {
	case 1:
		v = bw_ppm_get_y_z1(&instance->ppm_l_state);
		break;
	case 2:
		v = bw_ppm_get_y_z1(&instance->ppm_r_state);
		break;
	}
	return bw_clipf(v, -60.f, 0.f);
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_pan_process(&instance->pan_coeffs, inputs[0], outputs[0], outputs[1], n_samples);
	bw_ppm_state *ppm_states[2] = { &instance->ppm_l_state, &instance->ppm_r_state };
	bw_ppm_process_multi(&instance->ppm_coeffs, ppm_states, (const float **)outputs, NULL, 2, n_samples);
}
