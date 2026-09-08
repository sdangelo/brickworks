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
#include <bw_note_queue.h>
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
#include <bw_voice_alloc.h>

using namespace Brickworks;

#define BUFFER_SIZE	128
#define SYNC_RATE	1e-3f	// synchronous control rate, seconds
#define N_VOICES	8

typedef struct plugin plugin;

struct Voice {
	PhaseGen<>		vco1PhaseGen;
	PhaseGen<>		vco2PhaseGen;
	PhaseGen<>		vco3PhaseGen;
	SVF<>			vcf;

	unsigned char		note;
	char			gate;
	float			modK;
	float			vcfEnvK;

	float			buf[5][BUFFER_SIZE];

	plugin *		instance;
	int			index;
};

struct plugin {
	NoteQueue		noteQueue;
	OscSaw<N_VOICES>	vco1OscSaw;
	OscPulse<N_VOICES>	vco1OscPulse;
	OscTri<N_VOICES>	vco1OscTri;
	Gain<N_VOICES>		vco1Gain;
	OscSaw<N_VOICES>	vco2OscSaw;
	OscPulse<N_VOICES>	vco2OscPulse;
	OscTri<N_VOICES>	vco2OscTri;
	Gain<N_VOICES>		vco2Gain;
	OscSaw<N_VOICES>	vco3OscSaw;
	OscPulse<N_VOICES>	vco3OscPulse;
	OscTri<N_VOICES>	vco3OscTri;
	Gain<N_VOICES>		vco3Gain;
	OscFilt<N_VOICES>	oscFilt;
	NoiseGen<N_VOICES>	noiseGen;
	PinkFilt<N_VOICES>	pinkFilt;
	Gain<N_VOICES>		noiseGain;
	EnvGen<N_VOICES>	vcfEnvGen;
	EnvGen<N_VOICES>	vcaEnvGen;
	PhaseGen<>		a440PhaseGen;
	Gain<>			gain;
	PPM<>			ppm;

	Voice			voices[N_VOICES];

	size_t			syncCount;
	float			noiseKV[2];

	uint64_t		randState;
	float			masterTune;
	float			modulationMix;
	float			vco1Modulation;
	float			vco1Coarse;
	float			vco1Fine;
	char			vco1Waveform;
	float			vco2Modulation;
	float			vco2Coarse;
	float			vco2Fine;
	char			vco2Waveform;
	char			vco3KbdCtrl;
	float			vco3Coarse;
	float			vco3Fine;
	char			vco3Waveform;
	char			noiseColor;
	float			vcfModulation;
	char			vcfKbdCtrl;
	float			vcfCutoff;
	float			vcfContour;
	char			a440;

	float			pitchBend;
	float			modWheel;
	size_t			syncLeft;
	char			vco3WaveformCur;
	char			vco1WaveformCur;
	char			vco2WaveformCur;

	float			buf[BUFFER_SIZE];

	float *			b0[N_VOICES];
	float *			b1[N_VOICES];
	float *			b2[N_VOICES];
	float *			b3[N_VOICES];
	float *			b4[N_VOICES];
};

static int plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
	new(&instance->noteQueue) NoteQueue();
	new(&instance->vco1OscSaw) OscSaw<N_VOICES>();
	new(&instance->vco1OscPulse) OscPulse<N_VOICES>();
	new(&instance->vco1OscTri) OscTri<N_VOICES>();
	new(&instance->vco1Gain) Gain<N_VOICES>();
	new(&instance->vco2OscSaw) OscSaw<N_VOICES>();
	new(&instance->vco2OscPulse) OscPulse<N_VOICES>();
	new(&instance->vco2OscTri) OscTri<N_VOICES>();
	new(&instance->vco2Gain) Gain<N_VOICES>();
	new(&instance->vco3OscSaw) OscSaw<N_VOICES>();
	new(&instance->vco3OscPulse) OscPulse<N_VOICES>();
	new(&instance->vco3OscTri) OscTri<N_VOICES>();
	new(&instance->vco3Gain) Gain<N_VOICES>();
	new(&instance->oscFilt) OscFilt<N_VOICES>();
	new(&instance->noiseGen) NoiseGen<N_VOICES>(&instance->randState);
	new(&instance->pinkFilt) PinkFilt<N_VOICES>();
	new(&instance->noiseGain) Gain<N_VOICES>();
	new(&instance->vcfEnvGen) EnvGen<N_VOICES>();
	new(&instance->vcaEnvGen) EnvGen<N_VOICES>();
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

	for (int i = 0; i < N_VOICES; i++) {
		new(&instance->voices[i].vco1PhaseGen) PhaseGen<>();
		new(&instance->voices[i].vco2PhaseGen) PhaseGen<>();
		new(&instance->voices[i].vco3PhaseGen) PhaseGen<>();
		new(&instance->voices[i].vcf) SVF<>();

		instance->voices[i].instance = instance;
		instance->voices[i].index = i;
		instance->b0[i] = instance->voices[i].buf[0];
		instance->b1[i] = instance->voices[i].buf[1];
		instance->b2[i] = instance->voices[i].buf[2];
		instance->b3[i] = instance->voices[i].buf[3];
		instance->b4[i] = instance->voices[i].buf[4];
	}

	return 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	instance->vco1OscSaw.setSampleRate(sample_rate);
	instance->vco1OscPulse.setSampleRate(sample_rate);
	instance->vco1OscTri.setSampleRate(sample_rate);
	instance->vco1Gain.setSampleRate(sample_rate);
	instance->vco2OscSaw.setSampleRate(sample_rate);
	instance->vco2OscPulse.setSampleRate(sample_rate);
	instance->vco2OscTri.setSampleRate(sample_rate);
	instance->vco2Gain.setSampleRate(sample_rate);
	instance->vco3OscSaw.setSampleRate(sample_rate);
	instance->vco3OscPulse.setSampleRate(sample_rate);
	instance->vco3OscTri.setSampleRate(sample_rate);
	instance->vco3Gain.setSampleRate(sample_rate);
	instance->noiseGen.setSampleRate(sample_rate);
	instance->pinkFilt.setSampleRate(sample_rate);
	instance->noiseGain.setSampleRate(sample_rate);
	instance->vcfEnvGen.setSampleRate(sample_rate);
	instance->vcaEnvGen.setSampleRate(sample_rate);
	instance->a440PhaseGen.setSampleRate(sample_rate);
	instance->gain.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);

	for (int i = 0; i < N_VOICES; i++) {
		instance->voices[i].vco1PhaseGen.setSampleRate(sample_rate);
		instance->voices[i].vco2PhaseGen.setSampleRate(sample_rate);
		instance->voices[i].vco3PhaseGen.setSampleRate(sample_rate);
		instance->voices[i].vcf.setSampleRate(sample_rate);
	}

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
	for (int i = 0; i < N_VOICES; i++)
		instance->voices[i].vcf.setCutoff(instance->vcfCutoff);

	instance->noteQueue = NoteQueue();
	instance->vco1OscSaw.reset();
	instance->vco1OscPulse.reset();
	instance->vco1OscTri.reset();
	instance->vco1Gain.reset();
	instance->vco2OscSaw.reset();
	instance->vco2OscPulse.reset();
	instance->vco2OscTri.reset();
	instance->vco2Gain.reset();
	instance->vco3OscSaw.reset();
	instance->vco3OscPulse.reset();
	instance->vco3OscTri.reset();
	instance->vco3Gain.reset();
	instance->oscFilt.reset();
	instance->noiseGen.reset();
	instance->pinkFilt.reset();
	instance->noiseGain.reset();
	instance->vcfEnvGen.reset();
	instance->vcaEnvGen.reset();
	instance->a440PhaseGen.reset();
	instance->gain.reset();
	instance->ppm.reset();
	for (int i = 0; i < N_VOICES; i++) {
		instance->voices[i].vco1PhaseGen.reset();
		instance->voices[i].vco2PhaseGen.reset();
		instance->voices[i].vco3PhaseGen.reset();
		instance->voices[i].vcf.reset();

		instance->voices[i].note = 60;
		instance->voices[i].gate = 0;
	}
	instance->pitchBend = 0.f;
	instance->modWheel = 0.f;
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
		for (int i = 0; i < N_VOICES; i++) {
			instance->voices[i].vco1PhaseGen.setPortamentoTau(v);
			instance->voices[i].vco2PhaseGen.setPortamentoTau(v);
			instance->voices[i].vco3PhaseGen.setPortamentoTau(v);
		}
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
	{
		const float v = 0.5f + (0.01f * 9.5f) * value;
		for (int i = 0; i < N_VOICES; i++)
			instance->voices[i].vcf.setQ(v);
		break;
	}
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

static void noteOn(void *BW_RESTRICT handle, unsigned char note, float velocity) {
	(void)velocity;
	Voice *v = (Voice *)handle;
	v->note = note;
	v->gate = 1;
}

static void noteOff(void *BW_RESTRICT handle, float velocity) {
	(void)velocity;
	Voice *v = (Voice *)handle;
	v->gate = 0;
}

static unsigned char getNote(const void *BW_RESTRICT handle) {
	Voice *v = (Voice *)handle;
	return v->note;
}

static char isFree(const void *BW_RESTRICT handle) {
	Voice *v = (Voice *)handle;
	bw_env_gen_phase phase = v->instance->vcaEnvGen.getPhase(v->index);
	return !v->gate && phase == bw_env_gen_phase_off;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	(void)inputs;

	// voice allocation

	static bw_voice_alloc_opts alloc_opts = { bw_voice_alloc_priority_low, noteOn, noteOff, getNote, isFree };
	void *voices[N_VOICES];
	for (int i = 0; i < N_VOICES; i++)
		voices[i] = (void *)(instance->voices + i);
	bw_voice_alloc(&alloc_opts, &instance->noteQueue.queue, voices, N_VOICES);
	instance->noteQueue.clear();

	// asynchronous control-rate operations

	const float df1 = instance->vco1Coarse + instance->pitchBend + (8.333333333333333e-2f * 0.01f) * instance->vco1Fine;
	const float df2 = instance->vco2Coarse + instance->pitchBend + (8.333333333333333e-2f * 0.01f) * instance->vco2Fine;
	const float df3 = instance->vco3Coarse + instance->pitchBend + (8.333333333333333e-2f * 0.01f) * instance->vco3Fine;
	for (int i = 0; i < N_VOICES; i++) {
		int n = instance->voices[i].note - 69;
		int n3 = instance->vco3KbdCtrl ? n : -69;
		instance->voices[i].vco1PhaseGen.setFrequency(instance->masterTune * bw_pow2f(df1 + 8.333333333333333e-2f * n));
		instance->voices[i].vco2PhaseGen.setFrequency(instance->masterTune * bw_pow2f(df2 + 8.333333333333333e-2f * n));
		instance->voices[i].vco3PhaseGen.setFrequency(instance->masterTune * bw_pow2f(df3 + 8.333333333333333e-2f * n3));
	}

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
	float cutoffKbdK[N_VOICES];
	for (int i = 0; i < N_VOICES; i++)
		cutoffKbdK[i] = bw_pow2f(cutoffKbdKV[instance->vcfKbdCtrl - 1] * (instance->voices[i].note - 60));

	const float noiseK = instance->noiseKV[instance->noiseColor - 1];

	char gates[N_VOICES];
	for (int j = 0; j < N_VOICES; j++)
		gates[j] = instance->voices[j].gate;

	// synchronous control-rate and audio-rate operations

	for (size_t i = 0; i < n_samples; ) {
		float *out = outputs[0] + i;
		size_t ni = n_samples - i;
		size_t n = ni < BUFFER_SIZE ? ni : BUFFER_SIZE;
		n = n < instance->syncLeft ? n : instance->syncLeft;

		const char sync = instance->syncLeft == instance->syncCount;
		float *y[1] = {out};

		// vco 3

		for (int j = 0; j < N_VOICES; j++) {
			float *vb0[1] = {instance->voices[j].buf[0]};
			float *vb1[1] = {instance->voices[j].buf[1]};
			instance->voices[j].vco3PhaseGen.process(nullptr, vb0, vb1, n);
		}
		switch (instance->vco3WaveformCur) {
		case 1:
			instance->vco3OscSaw.process(instance->b0, instance->b1, instance->b0, n);
			break;
		case 2:
			instance->vco3OscPulse.process(instance->b0, instance->b1, instance->b0, n);
			break;
		default:
			instance->vco3OscTri.process(instance->b0, instance->b1, instance->b0, n);
			break;
		}

		// noise generator

		instance->noiseGen.process(instance->b1, n);
		if (instance->noiseColor == 2)
			instance->pinkFilt.process(instance->b1, instance->b1, n);
			// no need to ever reset pink filt, as inputs are noise and filters are static
		bufScale<N_VOICES>(instance->b1, 5.f, instance->b1, n);

		// modulation signals

		for (int j = 0; j < N_VOICES; j++) {
			for (size_t k = 0; k < n; k++)
				instance->b2[j][k] = instance->modWheel * (instance->b0[j][k] + instance->modulationMix * (instance->b1[j][k] - instance->b0[j][k]));
		}
		if (sync)
			for (int j = 0; j < N_VOICES; j++)
				instance->voices[j].modK = instance->b2[j][0];

		// vco 1

		for (int j = 0; j < N_VOICES; j++) {
			float *vb2[1] = {instance->voices[j].buf[2]};
			float *vb3[1] = {instance->voices[j].buf[3]};
			float *vb4[1] = {instance->voices[j].buf[4]};
			bufScale<1>(vb2, instance->vco1Modulation, vb3, n);
			instance->voices[j].vco1PhaseGen.process(vb3, vb3, vb4, n);
		}
		switch (instance->vco1WaveformCur) {
		case 1:
			instance->vco1OscSaw.process(instance->b3, instance->b4, instance->b3, n);
			break;
		case 2:
			instance->vco1OscPulse.process(instance->b3, instance->b4, instance->b3, n);
			break;
		default:
			instance->vco1OscTri.process(instance->b3, instance->b4, instance->b3, n);
			break;
		}

		// vco 2

		for (int j = 0; j < N_VOICES; j++) {
			float *vb2[1] = {instance->voices[j].buf[2]};
			float *vb4[1] = {instance->voices[j].buf[4]};
			bufScale<1>(vb2, instance->vco2Modulation, vb2, n);
			instance->voices[j].vco2PhaseGen.process(vb2, vb2, vb4, n);
		}
		switch (instance->vco2WaveformCur) {
		case 1:
			instance->vco2OscSaw.process(instance->b2, instance->b4, instance->b2, n);
			break;
		case 2:
			instance->vco2OscPulse.process(instance->b2, instance->b4, instance->b2, n);
			break;
		default:
			instance->vco2OscTri.process(instance->b2, instance->b4, instance->b2, n);
			break;
		}

		// mixer

		instance->vco1Gain.process(instance->b3, instance->b3, n);
		instance->vco2Gain.process(instance->b2, instance->b2, n);
		instance->vco3Gain.process(instance->b0, instance->b0, n);
		instance->noiseGain.process(instance->b1, instance->b1, n);
		bufMix<N_VOICES>(instance->b0, instance->b2, instance->b0, n);
		bufMix<N_VOICES>(instance->b0, instance->b3, instance->b0, n);

		instance->oscFilt.process(instance->b0, instance->b0, n);

		bufScale<N_VOICES>(instance->b1, noiseK, instance->b1, n);
		bufMix<N_VOICES>(instance->b0, instance->b1, instance->b0, n);

		// vcf

		if (sync)
			for (int j = 0; j < N_VOICES; j++) {
				instance->voices[j].vcfEnvK = instance->vcfEnvGen.getYZ1(j);
				const float cutoffVpos =
					cutoffUnmapped
					+ instance->vcfContour * instance->voices[j].vcfEnvK
					+ 0.3f * instance->vcfModulation * instance->voices[j].modK;
				float cutoff = cutoffKbdK[j] * 20.f * bw_expf(6.907755278982137 * cutoffVpos);
				instance->voices[j].vcf.setCutoff(bw_clipf(cutoff, 20.f, 20e3f));
			}
		instance->vcfEnvGen.process(gates, nullptr, n);
		for (int j = 0; j < N_VOICES; j++) {
			float *vb0[1] = {instance->voices[j].buf[0]};
			instance->voices[j].vcf.process(vb0, vb0, nullptr, nullptr, n);
		}

		// vca

		instance->vcaEnvGen.process(gates, instance->b1, n);
		bufMul<N_VOICES>(instance->b0, instance->b1, instance->b0, n);

		// mix voices

		bufFill<1>(0.f, y, n);
		for (int j = 0; j < N_VOICES; j++) {
			float *vb0[1] = {instance->voices[j].buf[0]};
			bufMix<1>(y, vb0, y, n);
		}

		// A 440 Hz osc

		if (instance->a440) { // nobody will notice or care about phase issues here
			float *b[1] = {instance->buf};
			instance->a440PhaseGen.process(nullptr, b, nullptr, n);
			oscSinProcess<1>(b, b, n);
			bufMix<1>(y, b, y, n);
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

static void plugin_midi_msg_in(plugin *instance, size_t index, const uint8_t * data) {
	(void)index;
	switch (data[0] & 0xf0) {
	case 0x90: // note on
		instance->noteQueue.add(data[1], data[2] != 0, (1.f / 127.f) * data[2], false);
		break;
	case 0x80: // note off
		instance->noteQueue.add(data[1], false, 0, false);
		break;
	case 0xe0: // pitch bend
	{
		const uint16_t v = (data[2] << 7) | data[1];
		instance->pitchBend = 2.f * bw_maxf((1.f / 16383.f) * (v - 0x2000), -1.f) - 1.f;
		break;
	}
	case 0xb0: // control change
		if (data[1] == 1) // mod wheel
			instance->modWheel = (1.f / 127.f) * data[2];
		break;
	}
}
