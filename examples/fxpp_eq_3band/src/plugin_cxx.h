/*
 * Brickworks
 *
 * Copyright (C) 2022-2026 Orastron Srl unipersonale
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

using namespace Brickworks;

typedef struct {
	LS2<>	ls2;
	Peak<>	peak;
	HS2<>	hs2;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->ls2) LS2<>();
	new(&instance->peak) Peak<>();
	new(&instance->hs2) HS2<>();
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->ls2.setSampleRate(sample_rate);
	instance->peak.setSampleRate(sample_rate);
	instance->hs2.setSampleRate(sample_rate);
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
	instance->ls2.reset();
	instance->peak.reset();
	instance->hs2.reset();
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_ls_cutoff:
		instance->ls2.setCutoff(value);
		break;
	case plugin_parameter_ls_gain:
		instance->ls2.setDcGainDB(value);
		break;
	case plugin_parameter_ls_q:
		instance->ls2.setQ(value);
		break;
	case plugin_parameter_peak_cutoff:
		instance->peak.setCutoff(value);
		break;
	case plugin_parameter_peak_gain:
		instance->peak.setPeakGainDB(value);
		break;
	case plugin_parameter_peak_bw:
		instance->peak.setBandwidth(value);
		break;
	case plugin_parameter_hs_cutoff:
		instance->hs2.setCutoff(value);
		break;
	case plugin_parameter_hs_gain:
		instance->hs2.setHighGainDB(value);
		break;
	case plugin_parameter_hs_q:
		instance->hs2.setQ(value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	instance->ls2.process(inputs, outputs, n_samples);
	instance->peak.process(outputs, outputs, n_samples);
	instance->hs2.process(outputs, outputs, n_samples);
}
