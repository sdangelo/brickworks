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
#include <bw_phase_gen.h>
#include <bw_osc_pulse.h>
#include <bw_osc_filt.h>
#include <bw_svf.h>
#include <bw_env_gen.h>
#include <bw_gain.h>
#include <bw_ppm.h>
#include <bw_buf.h>

using namespace Brickworks;

#define BUFFER_SIZE 128

typedef struct {
	PhaseGen<>	phaseGen;
	OscPulse<>	oscPulse;
	OscFilt<>	oscFilt;
	SVF<>		svf;
	EnvGen<>	envGen;
	Gain<>		gain;
	PPM<>		ppm;

	float		masterTune;
	int		note;

	float		buf[BUFFER_SIZE];
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->phaseGen) PhaseGen<>();
	new(&instance->oscPulse) OscPulse<>();
	new(&instance->oscFilt) OscFilt<>();
	new(&instance->svf) SVF<>();
	new(&instance->envGen) EnvGen<>();
	new(&instance->gain) Gain<>();
	new(&instance->ppm) PPM<>();
	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->phaseGen.setSampleRate(sample_rate);
	instance->oscPulse.setSampleRate(sample_rate);
	instance->svf.setSampleRate(sample_rate);
	instance->envGen.setSampleRate(sample_rate);
	instance->gain.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);
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
	instance->oscPulse.reset();
	instance->oscFilt.reset();
	instance->svf.reset();
	instance->envGen.reset();
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
	case plugin_parameter_portamento:
		// using portamento time 0% -> 90%: tau = portamento time / log(10)
		instance->phaseGen.setPortamentoTau((0.001f * 0.4342944819032517f) * value);
		break;
	case plugin_parameter_pulse_width:
		instance->oscPulse.setPulseWidth(0.01f * value);
		break;
	case plugin_parameter_cutoff:
		instance->svf.setCutoff(value);
		break;
	case plugin_parameter_resonance:
		instance->svf.setQ(0.5f + (9.5f * 0.01f) * value);
		break;
	case plugin_parameter_attack:
		instance->envGen.setAttack(0.001f * value);
		break;
	case plugin_parameter_decay:
		instance->envGen.setDecay(0.001f * value);
		break;
	case plugin_parameter_sustain:
		instance->envGen.setSustain(0.01f * value);
		break;
	case plugin_parameter_release:
		instance->envGen.setRelease(0.001f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)index;
	return bw_clipf(instance->ppm.getYZ1(0), -60.f, 0.f);
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	(void)inputs;

	if (instance->note >= 0)
		instance->phaseGen.setFrequency(instance->masterTune * bw_pow2f(8.333333333333333e-2f * (instance->note - 69)));

	for (size_t i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = outputs[0] + i;
		size_t ni = n_samples - i;
		size_t n = ni < BUFFER_SIZE ? ni : BUFFER_SIZE;

#ifdef BW_CXX_NO_ARRAY
		float *y[1] = {out};
		float *b[1] = {instance->buf};
		char gate[1] = {instance->note >= 0};
		instance->phaseGen.process(nullptr, y, b, n);
		instance->oscPulse.process(y, b, y, n);
		instance->oscFilt.process(y, y, n);
		instance->svf.process(y, y, nullptr, nullptr, n);
		instance->envGen.process(gate, b, n);
		bufMul<1>(y, b, y, n);
		instance->gain.process(y, y, n);
		instance->ppm.process(y, nullptr, n);
#else
		instance->phaseGen.process({nullptr}, {out}, {instance->buf}, n);
		instance->oscPulse.process({out}, {instance->buf}, {out}, n);
		instance->oscFilt.process({out}, {out}, n);
		instance->svf.process({out}, {out}, {nullptr}, {nullptr}, n);
		instance->envGen.process({instance->note >= 0}, {instance->buf}, n);
		bufMul<1>({out}, {instance->buf}, {out}, n);
		instance->gain.process({out}, {out}, n);
		instance->ppm.process({out}, {nullptr}, n);
#endif
	}
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
