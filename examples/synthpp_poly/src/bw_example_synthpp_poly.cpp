/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
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

#include "bw_example_synthpp_poly.h"

#include <bw_math.h>
#include <bwpp_buf.h>
#include <bw_voice_alloc.h>

void bw_example_synthpp_poly_init(bw_example_synthpp_poly *instance) {
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

	instance->rand_state = 0xbaddecaf600dfeed;
	
	for (int i = 0; i < N_VOICES; i++) {
		instance->voices[i].instance = instance;
		instance->voices[i].index = i;
	}
}

void bw_example_synthpp_poly_set_sample_rate(bw_example_synthpp_poly *instance, float sample_rate) {
	instance->vco1OscPulse.setSampleRate(sample_rate);
	instance->vco1OscTri.setSampleRate(sample_rate);
	instance->vco1Gain.setSampleRate(sample_rate);
	instance->vco2OscPulse.setSampleRate(sample_rate);
	instance->vco2OscTri.setSampleRate(sample_rate);
	instance->vco2Gain.setSampleRate(sample_rate);
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
}

void bw_example_synthpp_poly_reset(bw_example_synthpp_poly *instance) {
	const float v = instance->params[p_vcf_cutoff];
	const float cutoff = 20.f + (20e3f - 20.f) * v * v * v;
	for (int i = 0; i < N_VOICES; i++)
		instance->voices[i].vcf.setCutoff(bw_clipf(cutoff, 20.f, 20e3f));

	instance->noteQueue = NoteQueue();
	instance->vco1OscPulse.reset();
	instance->vco1OscTri.reset();
	instance->vco1Gain.reset();
	instance->vco2OscPulse.reset();
	instance->vco2OscTri.reset();
	instance->vco2Gain.reset();
	instance->vco3OscPulse.reset();
	instance->vco3OscTri.reset();
	instance->vco3Gain.reset();
	instance->oscFilt.reset();
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

		instance->voices[i].gate = 0;
	}

	instance->pitch_bend = 0.f;
	instance->mod_wheel = 0.f;
}

static void note_on(void *BW_RESTRICT voice, unsigned char note, float velocity) {
	(void)velocity;
	bw_example_synthpp_poly_voice *v = (bw_example_synthpp_poly_voice *)voice;
	v->note = note;
	v->gate = 1;
}

static void note_off(void *BW_RESTRICT voice, float velocity) {
	(void)velocity;
	bw_example_synthpp_poly_voice *v = (bw_example_synthpp_poly_voice *)voice;
	v->gate = 0;
}

static unsigned char get_note(void *BW_RESTRICT voice) {
	bw_example_synthpp_poly_voice *v = (bw_example_synthpp_poly_voice *)voice;
	return v->note;
}

static char is_free(void *BW_RESTRICT voice) {
	bw_example_synthpp_poly_voice *v = (bw_example_synthpp_poly_voice *)voice;
	bw_env_gen_phase phase = v->instance->vcaEnvGen.getPhase(v->index);
	return !v->gate && phase == bw_env_gen_phase_off;
}

void bw_example_synthpp_poly_process(bw_example_synthpp_poly *instance, const float** x, float** y, int n_samples) {
	// FIXME: control-rate modulations are asynchronous here...
	// it's all good as long as hosts gives us buffers whose length is a multiple of 32,
	// otherwise it's probably still ok but a bit "swingy"
	
	(void)x;

	static bw_voice_alloc_opts alloc_opts = { bw_voice_alloc_priority_low, note_on, note_off, get_note, is_free };
	void *voices[N_VOICES];
	for (int i = 0; i < N_VOICES; i++)
		voices[i] = (void *)(instance->voices + i);
	bw_voice_alloc(&alloc_opts, &instance->noteQueue.queue, voices, N_VOICES);
	instance->noteQueue.clear();
	
	const float df1 =
		6.f * instance->params[p_vco1_coarse] - 3.f
		+ 2.f * instance->pitch_bend - 1.f
		+ 8.333333333333333e-2f * (2.f * (instance->params[p_master_tune] + instance->params[p_vco1_fine]) - 71.f);
	const float df2 =
		6.f * instance->params[p_vco2_coarse] - 3.f
		+ 2.f * instance->pitch_bend - 1.f
		+ 8.333333333333333e-2f * (2.f * (instance->params[p_master_tune] + instance->params[p_vco2_fine]) - 71.f);
	const float df3 =
		6.f * instance->params[p_vco3_coarse] - 3.f
		+ 2.f * instance->pitch_bend - 1.f
		+ 8.333333333333333e-2f * (2.f * (instance->params[p_master_tune] + instance->params[p_vco3_fine]) - 71.f);
	for (int i = 0; i < N_VOICES; i++) {
		int n3 = instance->params[p_vco3_kbd] >= 0.5f ? instance->voices[i].note : 0;
		instance->voices[i].vco1PhaseGen.setFrequency(440.f * bw_pow2f_3(df1 + 8.333333333333333e-2f * instance->voices[i].note));
		instance->voices[i].vco2PhaseGen.setFrequency(440.f * bw_pow2f_3(df2 + 8.333333333333333e-2f * instance->voices[i].note));
		instance->voices[i].vco3PhaseGen.setFrequency(440.f * bw_pow2f_3(df3 + 8.333333333333333e-2f * n3));
	}

	const float vcf_mod_k = 0.3f * instance->params[p_vcf_mod];

	std::array<float *, N_VOICES> b0, b1, b2, b3, b4, na;
	std::array<const float *, N_VOICES> cb0, cb1, cb2, cb3, cb4;
	std::array<char, N_VOICES> gates;
	for (int j = 0; j < N_VOICES; j++) {
		b0.data()[j] = instance->voices[j].buf[0];
		b1.data()[j] = instance->voices[j].buf[1];
		b2.data()[j] = instance->voices[j].buf[2];
		b3.data()[j] = instance->voices[j].buf[3];
		b4.data()[j] = instance->voices[j].buf[4];
		na.data()[j] = nullptr;
		cb0.data()[j] = instance->voices[j].buf[0];
		cb1.data()[j] = instance->voices[j].buf[1];
		cb2.data()[j] = instance->voices[j].buf[2];
		cb3.data()[j] = instance->voices[j].buf[3];
		cb4.data()[j] = instance->voices[j].buf[4];
		gates.data()[j] = instance->voices[j].gate;
	}

	for (int i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = y[0] + i;
		int n = bw_minf(n_samples - i, BUFFER_SIZE);

		for (int j = 0; j < N_VOICES; j++)
			instance->voices[j].vco3PhaseGen.process({nullptr}, {b0.data()[j]}, {b1.data()[j]}, n);
		if (instance->params[p_vco3_waveform] >= (1.f / 4.f + 1.f / 2.f)) {
			instance->vco3OscTri.process(cb0, cb1, b0, n);
			instance->vco3OscPulse.reset();
		} else if (instance->params[p_vco3_waveform] >= (1.f / 4.f)) {
			instance->vco3OscPulse.process(cb0, cb1, b0, n);
			instance->vco3OscTri.reset();
		} else {
			instance->vco3OscSaw.process(cb0, cb1, b0, n);
			instance->vco3OscPulse.reset();
			instance->vco3OscTri.reset();
		}
		
		instance->noiseGen.process(b1, n);
		if (instance->params[p_noise_color] >= 0.5f)
			instance->pinkFilt.process(cb1, b1, n);
		else
			instance->pinkFilt.reset(); // FIXME: calling this here is sloppy coding
		bufScale<N_VOICES>(b1, cb1, 5.f, n);

		float vcf_mod[N_VOICES];
		for (int j = 0; j < N_VOICES; j++) {
			for (int k = 0; k < n; k++)
				b2.data()[j][k] = instance->mod_wheel * (b0.data()[j][k] + instance->params[p_mod_mix] * (b1.data()[j][k] - b0.data()[j][k]));
			vcf_mod[j] = vcf_mod_k * b2.data()[j][0];
		}
		
		for (int j = 0; j < N_VOICES; j++) {
			bufScale<1>({b3.data()[j]}, {b2.data()[j]}, instance->params[p_vco1_mod], n);
			instance->voices[j].vco1PhaseGen.process({b3.data()[j]}, {b3.data()[j]}, {b4.data()[j]}, n);
		}
		if (instance->params[p_vco1_waveform] >= (1.f / 4.f + 1.f / 2.f)) {
			instance->vco1OscTri.process(cb3, cb4, b3, n);
			instance->vco1OscPulse.reset();
		} else if (instance->params[p_vco1_waveform] >= (1.f / 4.f)) {
			instance->vco1OscPulse.process(cb3, cb4, b3, n);
			instance->vco1OscTri.reset();
		} else {
			instance->vco1OscSaw.process(cb3, cb4, b3, n);
			instance->vco1OscPulse.reset();
			instance->vco1OscTri.reset();
		}

		for (int j = 0; j < N_VOICES; j++) {
			bufScale<1>({b2.data()[j]}, {b2.data()[j]}, instance->params[p_vco2_mod], n);
			instance->voices[j].vco2PhaseGen.process({b2.data()[j]}, {b2.data()[j]}, {b4.data()[j]}, n);
		}
		if (instance->params[p_vco2_waveform] >= (1.f / 4.f + 1.f / 2.f)) {
			instance->vco2OscTri.process(cb2, cb4, b2, n);
			instance->vco2OscPulse.reset();
		} else if (instance->params[p_vco2_waveform] >= (1.f / 4.f)) {
			instance->vco2OscPulse.process(cb2, cb4, b2, n);
			instance->vco2OscTri.reset();
		} else {
			instance->vco2OscSaw.process(cb2, cb4, b2, n);
			instance->vco2OscPulse.reset();
			instance->vco2OscTri.reset();
		}

		instance->vco1Gain.process(cb3, b3, n);
		instance->vco2Gain.process(cb2, b2, n);
		instance->vco3Gain.process(cb0, b0, n);
		instance->noiseGain.process(cb1, b1, n);
		bufMix<N_VOICES>(b0, cb0, cb2, n);
		bufMix<N_VOICES>(b0, cb0, cb3, n);

		instance->oscFilt.process(cb0, b0, n);

		const float k = instance->params[p_noise_color] >= 0.5f
			? 6.f * instance->noiseGen.getScalingK() * instance->pinkFilt.getScalingK()
			: 0.1f * instance->noiseGen.getScalingK();
		bufScale<N_VOICES>(b1, cb1, k, n);
		bufMix<N_VOICES>(b0, cb0, cb1, n);

		instance->vcfEnvGen.process(gates, na, n);
		for (int j = 0; j < N_VOICES; j++) {
			float v = instance->params[p_vcf_cutoff] + instance->params[p_vcf_contour] * instance->vcfEnvGen.getYZ1(j) + vcf_mod[j];
			float cutoff = 20.f + (20e3f - 20.f) * v * v * v;
			if (instance->params[p_vcf_kbd_ctrl] >= (1.f / 6.f + 2.f / 3.f))
				cutoff *= bw_pow2f_3(8.333333333333333e-2f * (instance->voices[j].note - 60));
			else if (instance->params[p_vcf_kbd_ctrl] >= (1.f / 6.f + 1.f / 3.f))
				cutoff *= bw_pow2f_3((0.793700525984100f * 8.333333333333333e-2f) * (instance->voices[j].note - 60));
			else if (instance->params[p_vcf_kbd_ctrl] >= (1.f / 6.f + 2.f / 3.f))
				cutoff *= bw_pow2f_3((0.629960524947437f * 8.333333333333333e-2f) * (instance->voices[j].note - 60));
			// otherwise no kbd control
			instance->voices[j].vcf.setCutoff(bw_clipf(cutoff, 20.f, 20e3f));
			instance->voices[j].vcf.process({b0.data()[j]}, {b0.data()[j]}, {nullptr}, {nullptr}, n);
		}

		instance->vcaEnvGen.process(gates, b1, n);
		bufMul<N_VOICES>(b0, cb0, cb1, n);

		bufFill<1>({out}, 0.f, n);
		for (int j = 0; j < N_VOICES; j++)
			bufMix<1>({out}, {out}, {b0.data()[j]}, n);

		instance->a440PhaseGen.process({nullptr}, {instance->buf}, {nullptr}, n);
		oscSinProcess<1>({instance->buf}, {instance->buf}, n);
		if (instance->params[p_a440] >= 0.5f)
			bufMix<1>({out}, {out}, {instance->buf}, n);

		instance->gain.process({out}, {out}, n);
		instance->ppm.process({out}, {nullptr}, n);
	}
}

void bw_example_synthpp_poly_set_parameter(bw_example_synthpp_poly *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_volume:
		instance->gain.setGainLin(value * value * value);
		break;
	case p_portamento:
		for (int i = 0; i < N_VOICES; i++) {
			instance->voices[i].vco1PhaseGen.setPortamentoTau(value);
			instance->voices[i].vco2PhaseGen.setPortamentoTau(value);
			instance->voices[i].vco3PhaseGen.setPortamentoTau(value);
		}
		break;
	case p_vco1_pw_slope:
		instance->vco1OscPulse.setPulseWidth(value);
		instance->vco1OscTri.setSlope(bw_clipf(value, 0.001f, 0.999f));
		break;
	case p_vco1_level:
		instance->vco1Gain.setGainLin(value * value * value);
		break;
	case p_vco2_pw_slope:
		instance->vco2OscPulse.setPulseWidth(value);
		instance->vco2OscTri.setSlope(bw_clipf(value, 0.001f, 0.999f));
		break;
	case p_vco2_level:
		instance->vco2Gain.setGainLin(value * value * value);
		break;
	case p_vco3_pw_slope:
		instance->vco3OscPulse.setPulseWidth(value);
		instance->vco3OscTri.setSlope(bw_clipf(value, 0.001f, 0.999f));
		break;
	case p_vco3_level:
		instance->vco3Gain.setGainLin(value * value * value);
		break;
	case p_noise_level:
		instance->noiseGain.setGainLin(value * value * value);
		break;
	case p_vcf_Q:
	{
		const float v = 0.5f + 9.5f * value;
		for (int i = 0; i < N_VOICES; i++)
			instance->voices[i].vcf.setQ(v);
		break;
	}
	case p_vcf_attack:
		instance->vcfEnvGen.setAttack(value);
		break;
	case p_vcf_decay:
		instance->vcfEnvGen.setDecay(value);
		break;
	case p_vcf_sustain:
		instance->vcfEnvGen.setSustain(value);
		break;
	case p_vcf_release:
		instance->vcfEnvGen.setRelease(value);
		break;
	case p_vca_attack:
		instance->vcaEnvGen.setAttack(bw_maxf(0.002f, value));
		break;
	case p_vca_decay:
		instance->vcaEnvGen.setDecay(value);
		break;
	case p_vca_sustain:
		instance->vcaEnvGen.setSustain(value);
		break;
	case p_vca_release:
		instance->vcaEnvGen.setRelease(bw_maxf(0.002f, value));
		break;
	}
}

float bw_example_synthpp_poly_get_parameter(bw_example_synthpp_poly *instance, int index) {
	if (index < p_n)
		return instance->params[index];
	const float v = instance->ppm.getYZ1(0);
	return v < -200.f ? 0.f : bw_clipf(0.01666666666666666f * v + 1.f, 0.f, 1.f);
}

void bw_example_synthpp_poly_note_on(bw_example_synthpp_poly *instance, char note, char velocity) {
	instance->noteQueue.add(note, velocity != 0, (1.f / 127.f) * velocity, false);
}

void bw_example_synthpp_poly_note_off(bw_example_synthpp_poly *instance, char note) {
	instance->noteQueue.add(note, false, 0, false);
}

void bw_example_synthpp_poly_pitch_bend(bw_example_synthpp_poly *instance, int value) {
	instance->pitch_bend = (value - 0x2000) / (float)0x4000;
}

void bw_example_synthpp_poly_mod_wheel(bw_example_synthpp_poly *instance, char value) {
	instance->mod_wheel = (float)value / 0x80;
}
