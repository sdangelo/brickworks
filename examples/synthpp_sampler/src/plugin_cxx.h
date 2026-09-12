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

using namespace Brickworks;

#define SAMPLE_LENGTH 10 // seconds

typedef struct {
	Sampler<>	sampler;
	Gain<>		gain;
	PPM<>		ppm;

	float		fs;
	float *		sample;
	size_t		sampleLength;

	float		masterTune;
	int		note;
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->sampler) Sampler<>();
	new(&instance->gain) Gain<>();
	new(&instance->ppm) PPM<>();
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->sampler.setSampleRate(sample_rate);
	instance->gain.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);

	instance->fs = sample_rate;
	instance->sampleLength = (size_t)bw_ceilf(sample_rate * SAMPLE_LENGTH);
}

static size_t plugin_mem_req(plugin *instance) {
	return instance->sampleLength * sizeof(float);
}

static void plugin_mem_set(plugin *instance, void *mem) {
	instance->sample = (float *)mem;
}

static void plugin_reset(plugin *instance) {
	float p = 0.f;
	float inc = 440.f / instance->fs;
	for (size_t i = 0; i < instance->sampleLength; i++) {
		p += inc;
		p = p - bw_floorf(p);
		instance->sample[i] = bw_sin2pif(p);
	}
#ifdef BW_CXX_NO_ARRAY
	const float *sample[1] = {instance->sample};
	const size_t sampleLength[1] = {instance->sampleLength};
	instance->sampler.reset(sample, sampleLength);
#else
	instance->sampler.reset({instance->sample}, {instance->sampleLength});
#endif
	instance->gain.reset();
	instance->ppm.reset();
	instance->note = -1;
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_volume:
	{
		float v = 0.01f * value;
		instance->gain.setGainLin(v * v * v);
	}
		break;
	case plugin_parameter_master_tune:
		instance->masterTune = value;
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)index;
	return bw_clipf(instance->ppm.getYZ1(0), -60.f, 0.f);
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	(void)inputs;
#ifdef BW_CXX_NO_ARRAY
	const float *sample[1] = {instance->sample};
	const size_t sampleLength[1] = {instance->sampleLength};
	float *y[1] = {outputs[0]};
	if (instance->note >= 0) {
		instance->sampler.setRate((1.f / 440.f) * instance->masterTune * bw_pow2f(8.333333333333333e-2f * (instance->note - 69)));
		instance->sampler.process(sample, sampleLength, y, n_samples);
	} else {
		instance->sampler.reset(sample, sampleLength); // sloppy but simple coding
		bufFill<1>(0.f, outputs, n_samples);
	}
#else
	if (instance->note >= 0) {
		instance->sampler.setRate((1.f / 440.f) * instance->masterTune * bw_pow2f(8.333333333333333e-2f * (instance->note - 69)));
		instance->sampler.process({instance->sample}, {instance->sampleLength}, {outputs[0]}, n_samples);
	} else {
		instance->sampler.reset({instance->sample}, {instance->sampleLength}); // sloppy but simple coding
		bufFill<1>(0.f, outputs, n_samples);
	}
#endif
	instance->gain.process(outputs, outputs, n_samples);
	instance->ppm.process(outputs, nullptr, n_samples);
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
