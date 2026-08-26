/*
 * Brickworks
 *
 * Copyright (C) 2023-2026 Orastron Srl unipersonale
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
#include <bw_dist.h>
#include <bw_src_int.h>

using namespace Brickworks;

#define BUF_SIZE 32

typedef struct {
	Dist<>		dist;
	SRCInt<>	srcUp;
	SRCInt<>	srcDown;
	float		buf[BUF_SIZE];
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->dist) Dist<>();
	new(&instance->srcUp) SRCInt<>(2);
	new(&instance->srcDown) SRCInt<>(-2);
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->dist.setSampleRate(2.f * sample_rate);
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
	instance->dist.reset();
	instance->srcUp.reset();
	instance->srcDown.reset();
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_distortion:
		instance->dist.setDistortion(0.01f * value);
		break;
	case plugin_parameter_tone:
		instance->dist.setTone(0.01f * value);
		break;
	case plugin_parameter_volume:
		instance->dist.setVolume(0.01f * value);
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
#ifdef BW_CXX_NO_ARRAY
		const float *x[1] = {inputs[0] + i};
		float *y[1] = {outputs[0] + i};
		float *b[1] = {instance->buf};
		instance->srcUp.process(x, b, n);
		instance->dist.process(b, b, n << 1);
		instance->srcDown.process(b, y, n << 1);
#else
		instance->srcUp.process({inputs[0] + i}, {instance->buf}, n);
		instance->dist.process({instance->buf}, {instance->buf}, n << 1);
		instance->srcDown.process({instance->buf}, {outputs[0] + i}, n << 1);
#endif
		i += n;
	}
}
