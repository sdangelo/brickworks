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
#include <bw_osc_saw.h>
#include <bw_osc_pulse.h>
#include <bw_osc_tri.h>
#include <bw_osc_sin.h>
#include <bw_osc_filt.h>
#include <bw_noise_gen.h>
#include <bw_pink_filt.h>
#include <bw_svf.h>
#include <bw_env_gen.h>
#include <bw_gain.h>
#include <bw_ppm.h>
#include <bw_buf.h>

using namespace Brickworks;

#define BUFFER_SIZE	128
#define SYNC_RATE	1e-3f	// synchronous control rate, seconds

typedef struct {
	PhaseGen<>	vco1PhaseGen;
	OscSaw<>	vco1OscSaw;
	OscPulse<>	vco1OscPulse;
	OscTri<>	vco1OscTri;
	Gain<>		vco1Gain;
	PhaseGen<>	vco2PhaseGen;
	OscSaw<>	vco2OscSaw;
	OscPulse<>	vco2OscPulse;
	OscTri<>	vco2OscTri;
	Gain<>		vco2Gain;
	PhaseGen<>	vco3PhaseGen;
	OscSaw<>	vco3OscSaw;
	OscPulse<>	vco3OscPulse;
	OscTri<>	vco3OscTri;
	Gain<>		vco3Gain;
	OscFilt<>	oscFilt;
	NoiseGen<>	noiseGen;
	PinkFilt<>	pinkFilt;
	Gain<>		noiseGain;
	EnvGen<>	vcfEnvGen;
	SVF<>		vcf;
	EnvGen<>	vcaEnvGen;
	PhaseGen<>	a440PhaseGen;
	Gain<>		gain;
	PPM<>		ppm;

	size_t		syncCount;
	float		noiseKV[2];

	uint64_t	randState;
	float		masterTune;
	float		modulationMix;
	float		vco1Modulation;
	float		vco1Coarse;
	float		vco1Fine;
	char		vco1Waveform;
	float		vco2Modulation;
	float		vco2Coarse;
	float		vco2Fine;
	char		vco2Waveform;
	char		vco3KbdCtrl;
	float		vco3Coarse;
	float		vco3Fine;
	char		vco3Waveform;
	char		noiseColor;
	float		vcfModulation;
	char		vcfKbdCtrl;
	float		vcfCutoff;
	float		vcfContour;
	char		a440;

	int		note;
	char		gate;
	float		pitchBend;
	float		modWheel;
	char		notesPressed[128];
	size_t		syncLeft;
	char		vco3WaveformCur;
	float		modK;
	char		vco1WaveformCur;
	char		vco2WaveformCur;
	float		vcfEnvK;

	float		buf[4][BUFFER_SIZE];
} plugin;

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->vco1PhaseGen) PhaseGen<>();
	new(&instance->vco1OscSaw) OscSaw<>();
	new(&instance->vco1OscPulse) OscPulse<>();
	new(&instance->vco1OscTri) OscTri<>();
	new(&instance->vco1Gain) Gain<>();
	new(&instance->vco2PhaseGen) PhaseGen<>();
	new(&instance->vco2OscSaw) OscSaw<>();
	new(&instance->vco2OscPulse) OscPulse<>();
	new(&instance->vco2OscTri) OscTri<>();
	new(&instance->vco2Gain) Gain<>();
	new(&instance->vco3PhaseGen) PhaseGen<>();
	new(&instance->vco3OscSaw) OscSaw<>();
	new(&instance->vco3OscPulse) OscPulse<>();
	new(&instance->vco3OscTri) OscTri<>();
	new(&instance->vco3Gain) Gain<>();
	new(&instance->oscFilt) OscFilt<>();
	new(&instance->noiseGen) NoiseGen<>(&instance->randState);
	new(&instance->pinkFilt) PinkFilt<>();
	new(&instance->noiseGain) Gain<>();
	new(&instance->vcfEnvGen) EnvGen<>();
	new(&instance->vcf) SVF<>();
	new(&instance->vcaEnvGen) EnvGen<>();
	new(&instance->a440PhaseGen) PhaseGen<>();
	new(&instance->gain) Gain<>();
	new(&instance->ppm) PPM<>();

	instance->vco1OscSaw.setAntialiasing(true);
	instance->vco1OscPulse.setAntialiasing(true);
	instance->vco1OscTri.setAntialiasing(true);
	instance->vco2OscSaw.setAntialiasing(true);
	instance->vco2OscPulse.setAntialiasing(true);
	instance->vco2OscTri.setAntialiasing(true);
	instance->vco3OscSaw.setAntialiasing(true);
	instance->vco3OscPulse.setAntialiasing(true);
	instance->vco3OscTri.setAntialiasing(true);
	instance->a440PhaseGen.setFrequency(440.f);

	instance->randState = 0xbaddecaf600dfeed;

	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->vco1PhaseGen.setSampleRate(sample_rate);
	instance->vco1OscSaw.setSampleRate(sample_rate);
	instance->vco1OscPulse.setSampleRate(sample_rate);
	instance->vco1OscTri.setSampleRate(sample_rate);
	instance->vco1Gain.setSampleRate(sample_rate);
	instance->vco2PhaseGen.setSampleRate(sample_rate);
	instance->vco2OscSaw.setSampleRate(sample_rate);
	instance->vco2OscPulse.setSampleRate(sample_rate);
	instance->vco2OscTri.setSampleRate(sample_rate);
	instance->vco2Gain.setSampleRate(sample_rate);
	instance->vco3PhaseGen.setSampleRate(sample_rate);
	instance->vco3OscSaw.setSampleRate(sample_rate);
	instance->vco3OscPulse.setSampleRate(sample_rate);
	instance->vco3OscTri.setSampleRate(sample_rate);
	instance->vco3Gain.setSampleRate(sample_rate);
	instance->noiseGen.setSampleRate(sample_rate);
	instance->pinkFilt.setSampleRate(sample_rate);
	instance->noiseGain.setSampleRate(sample_rate);
	instance->vcfEnvGen.setSampleRate(sample_rate);
	instance->vcf.setSampleRate(sample_rate);
	instance->vcaEnvGen.setSampleRate(sample_rate);
	instance->a440PhaseGen.setSampleRate(sample_rate);
	instance->gain.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);

	instance->syncCount = (size_t)bw_roundf(sample_rate * SYNC_RATE);

	instance->noiseKV[0] = 0.1f * instance->noiseGen.getScalingK();
	instance->noiseKV[1] = 6.f * instance->noiseGen.getScalingK() * instance->pinkFilt.getScalingK();
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
	instance->vcf.setCutoff(instance->vcfCutoff);

	instance->vco1PhaseGen.reset();
	instance->vco1OscSaw.reset();
	instance->vco1OscPulse.reset();
	instance->vco1OscTri.reset();
	instance->vco1Gain.reset();
	instance->vco2PhaseGen.reset();
	instance->vco2OscSaw.reset();
	instance->vco2OscPulse.reset();
	instance->vco2OscTri.reset();
	instance->vco2Gain.reset();
	instance->vco3PhaseGen.reset();
	instance->vco3OscSaw.reset();
	instance->vco3OscPulse.reset();
	instance->vco3OscTri.reset();
	instance->vco3Gain.reset();
	instance->oscFilt.reset();
	instance->noiseGen.reset();
	instance->pinkFilt.reset();
	instance->noiseGain.reset();
	instance->vcfEnvGen.reset();
	instance->vcf.reset();
	instance->vcaEnvGen.reset();
	instance->a440PhaseGen.reset();
	instance->gain.reset();
	instance->ppm.reset();
	instance->note = 60;
	instance->gate = 0;
	instance->pitchBend = 0.f;
	instance->modWheel = 0.f;
	for (int i = 0; i < 128; i++)
		instance->notesPressed[i] = 0;
	instance->syncLeft = instance->syncCount;
	instance->vco3WaveformCur = instance->vco3Waveform;
	instance->vco1WaveformCur = instance->vco1Waveform;
	instance->vco2WaveformCur = instance->vco2Waveform;
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case plugin_parameter_volume:
	{
		const float v = 0.01f * value;
		instance->gain.setGainLin(v * v * v);
	}
		break;
	case plugin_parameter_master_tune:
		instance->masterTune = value;
		break;
	case plugin_parameter_portamento:
	{
		// using portamento time 0% -> 90%: tau = portamento time / log(10)
		const float v = (0.001f * 0.4342944819032517f) * value;
		instance->vco1PhaseGen.setPortamentoTau(v);
		instance->vco2PhaseGen.setPortamentoTau(v);
		instance->vco3PhaseGen.setPortamentoTau(v);
		break;
	}
	case plugin_parameter_mod_mix:
		instance->modulationMix = 0.01f * value;
		break;
	case plugin_parameter_vco1_mod:
		instance->vco1Modulation = 0.01f * value;
		break;
	case plugin_parameter_vco1_coarse:
		instance->vco1Coarse = value;
		break;
	case plugin_parameter_vco1_fine:
		instance->vco1Fine = value;
		break;
	case plugin_parameter_vco1_wave:
		instance->vco1Waveform = value;
		break;
	case plugin_parameter_vco1_pw:
	{
		const float v = 0.01f * value;
		instance->vco1OscPulse.setPulseWidth(v);
		instance->vco1OscTri.setSlope(bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case plugin_parameter_vco1_level:
	{
		const float v = 0.01f * value;
		instance->vco1Gain.setGainLin(v * v * v);
		break;
	}
	case plugin_parameter_vco2_mod:
		instance->vco2Modulation = 0.01f * value;
		break;
	case plugin_parameter_vco2_coarse:
		instance->vco2Coarse = value;
		break;
	case plugin_parameter_vco2_fine:
		instance->vco2Fine = value;
		break;
	case plugin_parameter_vco2_wave:
		instance->vco2Waveform = value;
		break;
	case plugin_parameter_vco2_pw:
	{
		const float v = 0.01f * value;
		instance->vco2OscPulse.setPulseWidth(v);
		instance->vco2OscTri.setSlope(bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case plugin_parameter_vco2_level:
	{
		const float v = 0.01f * value;
		instance->vco2Gain.setGainLin(v * v * v);
		break;
	}
	case plugin_parameter_vco3_kbd_ctrl:
		instance->vco3KbdCtrl = value >= 0.5f;
		break;
	case plugin_parameter_vco3_coarse:
		instance->vco3Coarse = value;
		break;
	case plugin_parameter_vco3_fine:
		instance->vco3Fine = value;
		break;
	case plugin_parameter_vco3_wave:
		instance->vco3Waveform = value;
		break;
	case plugin_parameter_vco3_pw:
	{
		const float v = 0.01f * value;
		instance->vco3OscPulse.setPulseWidth(v);
		instance->vco3OscTri.setSlope(bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case plugin_parameter_vco3_level:
	{
		const float v = 0.01f * value;
		instance->vco3Gain.setGainLin(v * v * v);
		break;
	}
	case plugin_parameter_noise_color:
		instance->noiseColor = value;
		break;
	case plugin_parameter_noise_level:
	{
		const float v = 0.01f * value;
		instance->noiseGain.setGainLin(v * v * v);
		break;
	}
	case plugin_parameter_vcf_mod:
		instance->vcfModulation = 0.01f * value;
		break;
	case plugin_parameter_vcf_kbd_ctrl:
		instance->vcfKbdCtrl = value;
		break;
	case plugin_parameter_vcf_cutoff:
		instance->vcfCutoff = value;
		break;
	case plugin_parameter_vcf_resonance:
		instance->vcf.setQ(0.5f + (0.01f * 9.5f) * value);
		break;
	case plugin_parameter_vcf_contour:
		instance->vcfContour = 0.01f * value;
		break;
	case plugin_parameter_vcf_attack:
		instance->vcfEnvGen.setAttack(0.001f * value);
		break;
	case plugin_parameter_vcf_decay:
		instance->vcfEnvGen.setDecay(0.001f * value);
		break;
	case plugin_parameter_vcf_sustain:
		instance->vcfEnvGen.setSustain(0.01f * value);
		break;
	case plugin_parameter_vcf_release:
		instance->vcfEnvGen.setRelease(0.001f * value);
		break;
	case plugin_parameter_vca_attack:
		instance->vcaEnvGen.setAttack(0.001f * value);
		break;
	case plugin_parameter_vca_decay:
		instance->vcaEnvGen.setDecay(0.001f * value);
		break;
	case plugin_parameter_vca_sustain:
		instance->vcaEnvGen.setSustain(0.01f * value);
		break;
	case plugin_parameter_vca_release:
		instance->vcaEnvGen.setRelease(0.001f * value);
		break;
	case plugin_parameter_a440:
		instance->a440 = value >= 0.5f;
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)index;
	return bw_clipf(instance->ppm.getYZ1(0), -60.f, 0.f);
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	// here is a version without C++ arrays only as it'd be too cumbersome to maintain two versions

	(void)inputs;

	// asynchronous control-rate operations

	int n = instance->note - 69;
	int n3 = instance->vco3KbdCtrl ? n : -69;
	instance->vco1PhaseGen.setFrequency(
		instance->masterTune
		* bw_pow2f(instance->vco1Coarse + instance->pitchBend
			+ 8.333333333333333e-2f * (n + 0.01f * instance->vco1Fine)));
	instance->vco2PhaseGen.setFrequency(
		instance->masterTune
		* bw_pow2f(instance->vco2Coarse + instance->pitchBend
			+ 8.333333333333333e-2f * (n + 0.01f * instance->vco2Fine)));
	instance->vco3PhaseGen.setFrequency(
		instance->masterTune
		* bw_pow2f(instance->vco3Coarse + instance->pitchBend
			+ 8.333333333333333e-2f * (n3 + 0.01f * instance->vco3Fine)));

	if (instance->vco3WaveformCur != instance->vco3Waveform) {
		switch (instance->vco3Waveform) {
		case 2:
			instance->vco3OscPulse.reset();
			break;
		case 3:
			instance->vco3OscTri.reset();
			break;
		}
		instance->vco3WaveformCur = instance->vco3Waveform;
	}

	if (instance->vco1WaveformCur != instance->vco1Waveform) {
		switch (instance->vco1Waveform) {
		case 2:
			instance->vco1OscPulse.reset();
			break;
		case 3:
			instance->vco1OscTri.reset();
			break;
		}
		instance->vco1WaveformCur = instance->vco1Waveform;
	}

	if (instance->vco2WaveformCur != instance->vco2Waveform) {
		switch (instance->vco2Waveform) {
		case 2:
			instance->vco2OscPulse.reset();
			break;
		case 3:
			instance->vco2OscTri.reset();
			break;
		}
		instance->vco2WaveformCur = instance->vco2Waveform;
	}

	const float cutoffUnmapped = 0.1447648273010839f * bw_logf(0.05f * instance->vcfCutoff);
	static const float cutoffKbdKV[4] = {
		0.f, // off
		0.629960524947437f * 8.333333333333333e-2f, // 1/3
		0.793700525984100f * 8.333333333333333e-2f, // 2/3
		8.333333333333333e-2f // full
	};
	const float cutoffKbdK = bw_pow2f(cutoffKbdKV[instance->vcfKbdCtrl - 1] * (instance->note - 60));

	const float noiseK = instance->noiseKV[instance->noiseColor - 1];

	const char vcaOpen = instance->vcaEnvGen.getPhase(0) != bw_env_gen_phase_off || instance->gate;

	float *b0[1] = {instance->buf[0]};
	float *b1[1] = {instance->buf[1]};
	float *b2[1] = {instance->buf[2]};
	float *b3[1] = {instance->buf[3]};
	char g[1] = {instance->gate};
	for (size_t i = 0; i < n_samples; ) {
		float *out = outputs[0] + i;
		size_t ni = n_samples - i;
		size_t n = ni < BUFFER_SIZE ? ni : BUFFER_SIZE;
		n = n < instance->syncLeft ? n : instance->syncLeft;

		const char sync = instance->syncLeft == instance->syncCount;
		float *y[1] = {out};

		// vco 3

		instance->vco3PhaseGen.process(nullptr, y, b0, n);
		switch (instance->vco3WaveformCur) {
		case 1:
			instance->vco3OscSaw.process(y, b0, y, n);
			break;
		case 2:
			instance->vco3OscPulse.process(y, b0, y, n);
			break;
		default:
			instance->vco3OscTri.process(y, b0, y, n);
			break;
		}

		// noise generator

		instance->noiseGen.process(b0, n);
		if (instance->noiseColor == 2)
			instance->pinkFilt.process(b0, b0, n);
			// no need to ever reset pink filt, as input is noise and filter is static
		bufScale<1>(b0, 5.f, b0, n);

		// modulation signals

		for (size_t j = 0; j < n; j++)
			instance->buf[1][j] = instance->modWheel * (out[j] + instance->modulationMix * (instance->buf[0][j] - out[j]));
		if (sync)
			instance->modK = instance->buf[1][0];

		// vco 1

		bufScale<1>(b1, instance->vco1Modulation, b2, n);
		instance->vco1PhaseGen.process(b2, b2, b3, n);
		switch (instance->vco1WaveformCur) {
		case 1:
			instance->vco1OscSaw.process(b2, b3, b2, n);
			break;
		case 2:
			instance->vco1OscPulse.process(b2, b3, b2, n);
			break;
		default:
			instance->vco1OscTri.process(b2, b3, b2, n);
			break;
		}

		// vco 2

		bufScale<1>(b1, instance->vco2Modulation, b1, n);
		instance->vco2PhaseGen.process(b1, b1, b3, n);
		switch (instance->vco2WaveformCur) {
		case 1:
			instance->vco2OscSaw.process(b1, b3, b1, n);
			break;
		case 2:
			instance->vco2OscPulse.process(b1, b3, b1, n);
			break;
		default:
			instance->vco2OscTri.process(b1, b3, b1, n);
			break;
		}

		// mixer

		instance->vco1Gain.process(b2, b2, n);
		instance->vco2Gain.process(b1, b1, n);
		instance->vco3Gain.process(y, y, n);
		instance->noiseGain.process(b0, b0, n);
		bufMix<1>(y, b1, y, n);
		bufMix<1>(y, b2, y, n);

		instance->oscFilt.process(y, y, n);

		bufScale<1>(b0, noiseK, b0, n);
		bufMix<1>(y, b0, y, n);

		// vcf

		if (sync) {
			instance->vcfEnvK = instance->vcfEnvGen.getYZ1(0);
			const float cutoffVpos = cutoffUnmapped + instance->vcfContour * instance->vcfEnvK + 0.3f * instance->vcfModulation * instance->modK;
			const float cutoff = cutoffKbdK * 20.f * bw_expf(6.907755278982137 * cutoffVpos);
			instance->vcf.setCutoff(bw_clipf(cutoff, 20.f, 20e3f));
		}
		instance->vcfEnvGen.process(g, nullptr, n);
		instance->vcf.process(y, y, nullptr, nullptr, n);

		// vca

		if (vcaOpen) {
			instance->vcaEnvGen.process(g, b0, n);
			bufMul<1>(y, b0, y, n);
		} else
			bufFill<1>(0.f, y, n);

		// A 440 Hz osc

		if (instance->a440) { // nobody will notice or care about phase issues here
			instance->a440PhaseGen.process(nullptr, b0, nullptr, n);
			oscSinProcess<1>(b0, b0, n);
			bufMix<1>(y, b0, y, n);
		}

		// output

		instance->gain.process(y, y, n);
		instance->ppm.process(y, nullptr, n);

		instance->syncLeft -= n;
		if (instance->syncLeft == 0)
			instance->syncLeft = instance->syncCount;

		i += n;
	}
}

static void updateNoteGate(plugin *instance) {
	for (int i = 0; i < 128; i++)
		if (instance->notesPressed[i]) {
			instance->note = i;
			instance->gate = 1;
			return;
		}
	instance->gate = 0;
}

static void noteOn(plugin *instance, char note) {
	instance->notesPressed[(int)note] = 1;
	updateNoteGate(instance);
}

static void noteOff(plugin *instance, char note) {
	if (instance->notesPressed[(int)note]) {
		instance->notesPressed[(int)note] = 0;
		updateNoteGate(instance);
	}
}

static void plugin_midi_msg_in(plugin *instance, size_t index, const uint8_t * data) {
	(void)index;
	switch (data[0] & 0xf0) {
	case 0x90: // note on
		if (data[2] == 0) // no, note off actually
			noteOff(instance, data[1]);
		else
			noteOn(instance, data[1]);
		break;
	case 0x80: // note off
		noteOff(instance, data[1]);
		break;
	case 0xe0: // pitch bend
	{
		const uint16_t v = (data[2] << 7) | data[1];
		instance->pitchBend = ((int)v - 8192) / (v < 8192 ? 8192.f : 8191.f);
		break;
	}
	case 0xb0: // control change
		if (data[1] == 1) // mod wheel
			instance->modWheel = (1.f / 127.f) * data[2];
		break;
	}
}
