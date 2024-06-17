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
#include <bw_ring_mod.h>
#include <bw_phase_gen.h>
#include <bw_osc_sin.h>

typedef struct plugin {
	bw_phase_gen_coeffs	phase_gen_coeffs;
	bw_phase_gen_state	phase_gen_state;
	bw_ring_mod_coeffs	ring_mod_coeffs;
} plugin;

static void plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	bw_phase_gen_init(&instance->phase_gen_coeffs);
	bw_ring_mod_init(&instance->ring_mod_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_phase_gen_set_sample_rate(&instance->phase_gen_coeffs, sample_rate);
	bw_ring_mod_set_sample_rate(&instance->ring_mod_coeffs, sample_rate);
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
	bw_phase_gen_reset_coeffs(&instance->phase_gen_coeffs);
	float v, v_inc;
	bw_phase_gen_reset_state(&instance->phase_gen_coeffs, &instance->phase_gen_state, 0.f, &v, &v_inc);
	bw_ring_mod_reset_coeffs(&instance->ring_mod_coeffs);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_phase_gen_set_frequency(&instance->phase_gen_coeffs, value);
		break;
	case 1:
		bw_ring_mod_set_amount(&instance->ring_mod_coeffs, 0.01f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_phase_gen_process(&instance->phase_gen_coeffs, &instance->phase_gen_state, NULL, outputs[0], NULL, n_samples);
	bw_osc_sin_process(outputs[0], outputs[0], n_samples);
	bw_ring_mod_process(&instance->ring_mod_coeffs, inputs[0], outputs[0], outputs[0], n_samples);
}
