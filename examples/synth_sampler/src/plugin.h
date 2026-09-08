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
#include <bw_math.h>
#include <bw_sampler.h>
#include <bw_gain.h>
#include <bw_ppm.h>
#include <bw_buf.h>

#define SAMPLE_LENGTH	10 // seconds

typedef struct {
	bw_sampler_coeffs	sampler_coeffs;
	bw_sampler_state	sampler_state;
	bw_gain_coeffs		gain_coeffs;
	bw_ppm_coeffs		ppm_coeffs;
	bw_ppm_state		ppm_state;

	float			fs;
	float *			sample;
	size_t			sample_length;

	float			master_tune;
	int			note;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	bw_sampler_init(&instance->sampler_coeffs);
	bw_gain_init(&instance->gain_coeffs);
	bw_ppm_init(&instance->ppm_coeffs);
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_sampler_set_sample_rate(&instance->sampler_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->gain_coeffs, sample_rate);
	bw_ppm_set_sample_rate(&instance->ppm_coeffs, sample_rate);

	instance->fs = sample_rate;
	instance->sample_length = (size_t)bw_ceilf(sample_rate * SAMPLE_LENGTH);
}

static size_t plugin_mem_req(plugin *instance) {
	return instance->sample_length * sizeof(float);
}

static void plugin_mem_set(plugin *instance, void *mem) {
	instance->sample = (float *)mem;
}

static void plugin_reset(plugin *instance) {
	float p = 0.f;
	float inc = 440.f / instance->fs;
	for (size_t i = 0; i < instance->sample_length; i++) {
		p += inc;
		p = p - bw_floorf(p);
		instance->sample[i] = bw_sin2pif(p);
	}
	bw_sampler_reset_coeffs(&instance->sampler_coeffs);
	bw_sampler_reset_state(&instance->sampler_coeffs, &instance->sampler_state, instance->sample, instance->sample_length, 0.f);
	bw_gain_reset_coeffs(&instance->gain_coeffs);
	bw_ppm_reset_coeffs(&instance->ppm_coeffs);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_state, 0.f);
	instance->note = -1;
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_volume:
	{
		float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->gain_coeffs, v * v * v);
	}
		break;
	case plugin_parameter_master_tune:
		instance->master_tune = value;
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)index;
	return bw_clipf(bw_ppm_get_y_z1(&instance->ppm_state), -60.f, 0.f);
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	(void)inputs;

	if (instance->note >= 0) {
		bw_sampler_set_rate(&instance->sampler_coeffs,
			(1.f / 440.f) * instance->master_tune * bw_pow2f(8.333333333333333e-2f * (instance->note - 69)));
		bw_sampler_process(&instance->sampler_coeffs, &instance->sampler_state, instance->sample, instance->sample_length, outputs[0], n_samples);
	} else {
		bw_sampler_reset_state(&instance->sampler_coeffs, &instance->sampler_state, instance->sample, instance->sample_length, 0.f); // sloppy but simple coding
		bw_buf_fill(0.f, outputs[0], n_samples);
	}
	bw_gain_process(&instance->gain_coeffs, outputs[0], outputs[0], n_samples);
	bw_ppm_process(&instance->ppm_coeffs, &instance->ppm_state, outputs[0], NULL, n_samples);
}

static void plugin_midi_msg_in(plugin *instance, size_t index, const uint8_t * data) {
	(void)index;
	switch (data[0] & 0xf0) {
	case 0x90: // note on
		if (data[2] == 0) { // no, note off actually
			if (data[1] == instance->note)
				instance->note = -1;
		} else
			instance->note = data[1];
		break;
	case 0x80: // note off
		if (data[1] == instance->note)
			instance->note = -1;
		break;
	}
}
