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
#include <bw_ring_mod.h>
#include <bw_phase_gen.h>
#include <bw_osc_sin.h>

using namespace Brickworks;

typedef struct {
	PhaseGen<>	phaseGen;
	RingMod<>	ringMod;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->phaseGen) PhaseGen<>();
	new(&instance->ringMod) RingMod<>();
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->phaseGen.setSampleRate(sample_rate);
	instance->ringMod.setSampleRate(sample_rate);
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
	instance->phaseGen.reset();
	instance->ringMod.reset();
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_frequency:
		instance->phaseGen.setFrequency(value);
		break;
	case plugin_parameter_amount:
		instance->ringMod.setAmount(0.01f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	instance->phaseGen.process(nullptr, outputs, nullptr, n_samples);
	oscSinProcess<>(outputs, outputs, n_samples);
	instance->ringMod.process(inputs, outputs, outputs, n_samples);
}
