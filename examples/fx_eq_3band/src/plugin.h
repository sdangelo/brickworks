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
#include <bw_ls2.h>
#include <bw_hs2.h>
#include <bw_peak.h>

typedef struct plugin {
	bw_ls2_coeffs	ls2_coeffs;
	bw_ls2_state	ls2_state;
	bw_peak_coeffs	peak_coeffs;
	bw_peak_state	peak_state;
	bw_hs2_coeffs	hs2_coeffs;
	bw_hs2_state	hs2_state;
} plugin;

static void plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	bw_ls2_init(&instance->ls2_coeffs);
	bw_peak_init(&instance->peak_coeffs);
	bw_hs2_init(&instance->hs2_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_ls2_set_sample_rate(&instance->ls2_coeffs, sample_rate);
	bw_peak_set_sample_rate(&instance->peak_coeffs, sample_rate);
	bw_hs2_set_sample_rate(&instance->hs2_coeffs, sample_rate);
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
	bw_ls2_reset_coeffs(&instance->ls2_coeffs);
	bw_ls2_reset_state(&instance->ls2_coeffs, &instance->ls2_state, 0.f);
	bw_peak_reset_coeffs(&instance->peak_coeffs);
	bw_peak_reset_state(&instance->peak_coeffs, &instance->peak_state, 0.f);
	bw_hs2_reset_coeffs(&instance->hs2_coeffs);
	bw_hs2_reset_state(&instance->hs2_coeffs, &instance->hs2_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_ls2_set_cutoff(&instance->ls2_coeffs, value);
		break;
	case 1:
		bw_ls2_set_dc_gain_dB(&instance->ls2_coeffs, value);
		break;
	case 2:
		bw_ls2_set_Q(&instance->ls2_coeffs, value);
		break;
	case 3:
		bw_peak_set_cutoff(&instance->peak_coeffs, value);
		break;
	case 4:
		bw_peak_set_peak_gain_dB(&instance->peak_coeffs, value);
		break;
	case 5:
		bw_peak_set_bandwidth(&instance->peak_coeffs, value);
		break;
	case 6:
		bw_hs2_set_cutoff(&instance->hs2_coeffs, value);
		break;
	case 7:
		bw_hs2_set_high_gain_dB(&instance->hs2_coeffs, value);
		break;
	case 8:
		bw_hs2_set_Q(&instance->hs2_coeffs, value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_ls2_process(&instance->ls2_coeffs, &instance->ls2_state, inputs[0], outputs[0], n_samples);
	bw_peak_process(&instance->peak_coeffs, &instance->peak_state, outputs[0], outputs[0], n_samples);
	bw_hs2_process(&instance->hs2_coeffs, &instance->hs2_state, outputs[0], outputs[0], n_samples);
}
