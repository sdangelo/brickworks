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
#include <bw_fuzz.h>
#include <bw_src_int.h>

#define BUF_SIZE	32

typedef struct plugin {
	bw_fuzz_coeffs		fuzz_coeffs;
	bw_fuzz_state		fuzz_state;
	bw_src_int_coeffs	src_up_coeffs;
	bw_src_int_state	src_up_state;
	bw_src_int_coeffs	src_down_coeffs;
	bw_src_int_state	src_down_state;

	float			buf[BUF_SIZE];
} plugin;

static void plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	bw_fuzz_init(&instance->fuzz_coeffs);
	bw_src_int_init(&instance->src_up_coeffs, 2);
	bw_src_int_init(&instance->src_down_coeffs, -2);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_fuzz_set_sample_rate(&instance->fuzz_coeffs, 2.f * sample_rate);
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
	bw_fuzz_reset_coeffs(&instance->fuzz_coeffs);
	bw_fuzz_reset_state(&instance->fuzz_coeffs, &instance->fuzz_state, 0.f);
	bw_src_int_reset_state(&instance->src_up_coeffs, &instance->src_up_state, 0.f);
	bw_src_int_reset_state(&instance->src_down_coeffs, &instance->src_down_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_fuzz_set_fuzz(&instance->fuzz_coeffs, 0.01f * value);
		break;
	case 1:
		bw_fuzz_set_volume(&instance->fuzz_coeffs, 0.01f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	size_t i = 0;
	while (i < n_samples) {
		int n = bw_mini32(n_samples - i, BUF_SIZE >> 1);
		bw_src_int_process(&instance->src_up_coeffs, &instance->src_up_state, inputs[0] + i, instance->buf, n);
		bw_fuzz_process(&instance->fuzz_coeffs, &instance->fuzz_state, instance->buf, instance->buf, n << 1);
		bw_src_int_process(&instance->src_down_coeffs, &instance->src_down_state, instance->buf, outputs[0] + i, n << 1);
		i += n;
	}
}
