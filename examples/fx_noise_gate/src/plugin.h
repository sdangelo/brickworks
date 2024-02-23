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
#include <bw_noise_gate.h>

typedef struct plugin {
	bw_noise_gate_coeffs	noise_gate_coeffs;
	bw_noise_gate_state	noise_gate_state;
	char			ext_sidechain;
} plugin;

static void plugin_init(plugin *instance) {
	bw_noise_gate_init(&instance->noise_gate_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_noise_gate_set_sample_rate(&instance->noise_gate_coeffs, sample_rate);
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
	bw_noise_gate_reset_coeffs(&instance->noise_gate_coeffs);
	bw_noise_gate_reset_state(&instance->noise_gate_coeffs, &instance->noise_gate_state, 0.f, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_noise_gate_set_thresh_dBFS(&instance->noise_gate_coeffs, value);
		break;
	case 1:
		bw_noise_gate_set_ratio(&instance->noise_gate_coeffs, value);
		break;
	case 2:
		// using rise time 10% -> 90%: tau = rise time / log(9)
		bw_noise_gate_set_attack_tau(&instance->noise_gate_coeffs, (0.001f * 0.4551196133134186f) * value);
		break;
	case 3:
		// as before
		bw_noise_gate_set_release_tau(&instance->noise_gate_coeffs, (0.001f * 0.4551196133134186f) * value);
		break;
	case 4:
		instance->ext_sidechain = (char)value;
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_noise_gate_process(&instance->noise_gate_coeffs, &instance->noise_gate_state, inputs[0], instance->ext_sidechain ? inputs[1] : inputs[0], outputs[0], n_samples);
}
