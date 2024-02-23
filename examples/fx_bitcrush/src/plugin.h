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
#include <bw_sr_reduce.h>
#include <bw_bd_reduce.h>

typedef struct plugin {
	bw_sr_reduce_coeffs	sr_reduce_coeffs;
	bw_sr_reduce_state	sr_reduce_state;
	bw_bd_reduce_coeffs	bd_reduce_coeffs;
} plugin;

static void plugin_init(plugin *instance) {
	bw_sr_reduce_init(&instance->sr_reduce_coeffs);
	bw_bd_reduce_init(&instance->bd_reduce_coeffs);
	bw_bd_reduce_set_silence_dc(&instance->bd_reduce_coeffs, 0);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_sr_reduce_set_sample_rate(&instance->sr_reduce_coeffs, sample_rate);
	bw_bd_reduce_set_sample_rate(&instance->bd_reduce_coeffs, sample_rate);
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
	bw_sr_reduce_reset_coeffs(&instance->sr_reduce_coeffs);
	bw_sr_reduce_reset_state(&instance->sr_reduce_coeffs, &instance->sr_reduce_state, 0.f);
	bw_bd_reduce_reset_coeffs(&instance->bd_reduce_coeffs);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_sr_reduce_set_ratio(&instance->sr_reduce_coeffs, 0.01f * value);
		break;
	case 1:
		bw_bd_reduce_set_bit_depth(&instance->bd_reduce_coeffs, (char)value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_sr_reduce_process(&instance->sr_reduce_coeffs, &instance->sr_reduce_state, inputs[0], outputs[0], n_samples);
	bw_bd_reduce_process(&instance->bd_reduce_coeffs, outputs[0], outputs[0], n_samples);
}
