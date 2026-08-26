/*
 * Brickworks
 *
 * Copyright (C) 2025, 2026 Orastron Srl unipersonale
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
#include <bw_iir1.h>

using namespace Brickworks;

typedef struct {
	float	sample_rate;
	float	cutoff;
	float	coeff_x;
	float	coeff_lp;
	float	s;
	float	b0;
	float	b1;
	float	a1;
	bool	to_reset;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)instance;
	(void)cbs;
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->sample_rate = sample_rate;
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
	bw_iir1_coeffs_mm1(instance->sample_rate, instance->cutoff, 1, instance->cutoff, instance->coeff_x, instance->coeff_lp, &instance->b0, &instance->b1, &instance->a1);
	float x0[1] = { 0.f };
	iir1Reset<>(x0, BW_NULL, &instance->s, instance->b0, instance->b1, instance->a1);
	instance->to_reset = false;
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_cutoff:
		instance->cutoff = value;
		instance->to_reset = true;
		break;
	case plugin_parameter_in:
		instance->coeff_x = value;
		instance->to_reset = true;
		break;
	case plugin_parameter_lp:
		instance->coeff_lp = value;
		instance->to_reset = true;
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	if (instance->to_reset)
		plugin_reset(instance);
	iir1Process<>(inputs, outputs, &instance->s, instance->b0, instance->b1, instance->a1, n_samples);
}
