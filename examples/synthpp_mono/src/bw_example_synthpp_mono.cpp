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

#include "bw_example_synthpp_mono.h"

#include <bw_math.h>
#include <bw_buf.h>

void bw_example_synthpp_mono_init(bw_example_synthpp_mono *instance) {
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
}

void bw_example_synthpp_mono_set_sample_rate(bw_example_synthpp_mono *instance, float sample_rate) {
	instance->vco1PhaseGen.setSampleRate(sample_rate);
	instance->vco1OscPulse.setSampleRate(sample_rate);
	instance->vco1OscTri.setSampleRate(sample_rate);
	instance->vco1Gain.setSampleRate(sample_rate);
	instance->vco2PhaseGen.setSampleRate(sample_rate);
	instance->vco2OscPulse.setSampleRate(sample_rate);
	instance->vco2OscTri.setSampleRate(sample_rate);
	instance->vco2Gain.setSampleRate(sample_rate);
	instance->vco3PhaseGen.setSampleRate(sample_rate);
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
}

void bw_example_synthpp_mono_reset(bw_example_synthpp_mono *instance) {
	const float v = instance->params[p_vcf_cutoff];
	const float cutoff = 20.f + (20e3f - 20.f) * v * v * v;
	instance->vcf.setCutoff(bw_clipf(cutoff, 20.f, 20e3f));

	instance->vco1PhaseGen.reset();
	instance->vco1OscPulse.reset();
	instance->vco1OscTri.reset();
	instance->vco1Gain.reset();
	instance->vco2PhaseGen.reset();
	instance->vco2OscPulse.reset();
	instance->vco2OscTri.reset();
	instance->vco2Gain.reset();
	instance->vco3PhaseGen.reset();
	instance->vco3OscPulse.reset();
	instance->vco3OscTri.reset();
	instance->vco3Gain.reset();
	instance->oscFilt.reset();
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
	instance->pitch_bend = 0.f;
	instance->mod_wheel = 0.f;
	for (int i = 0; i < 128; i++)
		instance->notes_pressed[i] = 0;
}

void bw_example_synthpp_mono_process(bw_example_synthpp_mono *instance, const float** x, float** y, int n_samples) {
	// FIXME: control-rate modulations are asynchronous here...
	// it's all good as long as hosts gives us buffers whose length is a multiple of 32,
	// otherwise it's probably still ok but a bit "swingy"
	
	(void)x;

	int n = instance->params[p_vco3_kbd] >= 0.5f ? instance->note : 0;
	instance->vco1PhaseGen.setFrequency(440.f *
		bw_pow2f_3(6.f * instance->params[p_vco1_coarse] - 3.f
			+ 2.f * instance->pitch_bend - 1.f
			+ 8.333333333333333e-2f * ((instance->note - 69) + 2.f * (instance->params[p_master_tune] + instance->params[p_vco1_fine]) - 2.f)));
	instance->vco2PhaseGen.setFrequency(440.f *
		bw_pow2f_3(6.f * instance->params[p_vco2_coarse] - 3.f
			+ 2.f * instance->pitch_bend - 1.f
			+ 8.333333333333333e-2f * ((instance->note - 69) + 2.f * (instance->params[p_master_tune] + instance->params[p_vco2_fine]) - 2.f)));
	instance->vco3PhaseGen.setFrequency(440.f *
		bw_pow2f_3(6.f * instance->params[p_vco3_coarse] - 3.f
			+ 2.f * instance->pitch_bend - 1.f
			+ 8.333333333333333e-2f * ((n - 69) + 2.f * (instance->params[p_master_tune] + instance->params[p_vco3_fine]) - 2.f)));

	for (int i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = y[0] + i;
		int n = bw_minf(n_samples - i, BUFFER_SIZE);
		
		instance->vco3PhaseGen.process({nullptr}, {out}, {instance->buf[0]}, n);
		if (instance->params[p_vco3_waveform] >= (1.f / 4.f + 1.f / 2.f)) {
			instance->vco3OscTri.process({out}, {instance->buf[0]}, {out}, n);
			instance->vco3OscPulse.reset();
		} else if (instance->params[p_vco3_waveform] >= (1.f / 4.f)) {
			instance->vco3OscPulse.process({out}, {instance->buf[0]}, {out}, n);
			instance->vco3OscTri.reset();
		} else {
			instance->vco3OscSaw.process({out}, {instance->buf[0]}, {out}, n);
			instance->vco3OscPulse.reset();
			instance->vco3OscTri.reset();
		}
		
		instance->noiseGen.process({instance->buf[0]}, n);
		if (instance->params[p_noise_color] >= 0.5f)
			instance->pinkFilt.process({instance->buf[0]}, {instance->buf[0]}, n);
		else
			instance->pinkFilt.reset(); // FIXME: calling this here is sloppy coding
		bw_buf_scale(instance->buf[0], instance->buf[0], 5.f, n);

		for (int j = 0; j < n; j++)
			instance->buf[1][j] = instance->mod_wheel * (out[j] + instance->params[p_mod_mix] * (instance->buf[0][j] - out[j]));
		const float vcf_mod = 0.3f * instance->params[p_vcf_mod] * instance->buf[1][0];

		bw_buf_scale(instance->buf[2], instance->buf[1], instance->params[p_vco1_mod], n);
		instance->vco1PhaseGen.process({instance->buf[2]}, {instance->buf[2]}, {instance->buf[3]}, n);
		if (instance->params[p_vco1_waveform] >= (1.f / 4.f + 1.f / 2.f)) {
			instance->vco1OscTri.process({instance->buf[2]}, {instance->buf[3]}, {instance->buf[2]}, n);
			instance->vco1OscPulse.reset();
		} else if (instance->params[p_vco1_waveform] >= (1.f / 4.f)) {
			instance->vco1OscPulse.process({instance->buf[2]}, {instance->buf[3]}, {instance->buf[2]}, n);
			instance->vco1OscTri.reset();
		} else {
			instance->vco1OscSaw.process({instance->buf[2]}, {instance->buf[3]}, {instance->buf[2]}, n);
			instance->vco1OscPulse.reset();
			instance->vco1OscTri.reset();
		}

		bw_buf_scale(instance->buf[1], instance->buf[1], instance->params[p_vco1_mod], n);
		instance->vco2PhaseGen.process({instance->buf[1]}, {instance->buf[1]}, {instance->buf[3]}, n);
		if (instance->params[p_vco2_waveform] >= (1.f / 4.f + 1.f / 2.f)) {
			instance->vco2OscTri.process({instance->buf[1]}, {instance->buf[3]}, {instance->buf[1]}, n);
			instance->vco2OscPulse.reset();
		} else if (instance->params[p_vco2_waveform] >= (1.f / 4.f)) {
			instance->vco2OscPulse.process({instance->buf[1]}, {instance->buf[3]}, {instance->buf[1]}, n);
			instance->vco2OscTri.reset();
		} else {
			instance->vco2OscSaw.process({instance->buf[1]}, {instance->buf[3]}, {instance->buf[1]}, n);
			instance->vco2OscPulse.reset();
			instance->vco2OscTri.reset();
		}

		instance->vco1Gain.process({instance->buf[2]}, {instance->buf[2]}, n);
		instance->vco2Gain.process({instance->buf[1]}, {instance->buf[1]}, n);
		instance->vco3Gain.process({out}, {out}, n);
		instance->noiseGain.process({instance->buf[0]}, {instance->buf[0]}, n);
		bw_buf_mix(out, out, instance->buf[1], n);
		bw_buf_mix(out, out, instance->buf[2], n);

		instance->oscFilt.process({out}, {out}, n);

		const float k = instance->params[p_noise_color] >= 0.5f
			? 6.f * instance->noiseGen.getScalingK() * instance->pinkFilt.getScalingK()
			: 0.1f * instance->noiseGen.getScalingK();
		bw_buf_scale(instance->buf[0], instance->buf[0], k, n);
		bw_buf_mix(out, out, instance->buf[0], n);

		instance->vcfEnvGen.process({instance->gate}, {nullptr}, n);
		float v = instance->params[p_vcf_cutoff] + instance->params[p_vcf_contour] * instance->vcfEnvGen.getYZ1(0) + vcf_mod;
		float cutoff = 20.f + (20e3f - 20.f) * v * v * v;
		if (instance->params[p_vcf_kbd_ctrl] >= (1.f / 6.f + 2.f / 3.f))
			cutoff *= bw_pow2f_3(8.333333333333333e-2f * (instance->note - 60));
		else if (instance->params[p_vcf_kbd_ctrl] >= (1.f / 6.f + 1.f / 3.f))
			cutoff *= bw_pow2f_3((0.793700525984100f * 8.333333333333333e-2f) * (instance->note - 60));
		else if (instance->params[p_vcf_kbd_ctrl] >= (1.f / 6.f + 2.f / 3.f))
			cutoff *= bw_pow2f_3((0.629960524947437f * 8.333333333333333e-2f) * (instance->note - 60));
		// otherwise no kbd control
		instance->vcf.setCutoff(bw_clipf(cutoff, 20.f, 20e3f));
		instance->vcf.process({out}, {out}, {nullptr}, {nullptr}, n);

		instance->vcaEnvGen.process({instance->gate}, {instance->buf[0]}, n);
		bw_buf_mul(out, out, instance->buf[0], n);

		instance->a440PhaseGen.process({nullptr}, {instance->buf[0]}, {nullptr}, n);
		oscSinProcess<1>({instance->buf[0]}, {instance->buf[0]}, n);
		if (instance->params[p_a440] >= 0.5f)
			bw_buf_mix(out, out, instance->buf[0], n);

		instance->gain.process({out}, {out}, n);
		instance->ppm.process({out}, {nullptr}, n);
	}
}

void bw_example_synthpp_mono_set_parameter(bw_example_synthpp_mono *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_volume:
		instance->gain.setGainLin(value * value * value);
		break;
	case p_portamento:
		instance->vco1PhaseGen.setPortamentoTau(value);
		instance->vco2PhaseGen.setPortamentoTau(value);
		instance->vco3PhaseGen.setPortamentoTau(value);
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
		instance->vcf.setQ(0.5f + 9.5f * value);
		break;
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

float bw_example_synthpp_mono_get_parameter(bw_example_synthpp_mono *instance, int index) {
	if (index < p_n)
		return instance->params[index];
	const float v = instance->ppm.getYZ1(0);
	return v < -200.f ? 0.f : bw_clipf(0.01666666666666666f * v + 1.f, 0.f, 1.f);
}

static void update_note_gate(bw_example_synthpp_mono *instance) {
	for (int i = 0; i < 128; i++)
		if (instance->notes_pressed[i]) {
			instance->note = i;
			instance->gate = 1;
			return;
		}
	instance->gate = 0;
}

void bw_example_synthpp_mono_note_on(bw_example_synthpp_mono *instance, char note, char velocity) {
	if (velocity == 0) 
		bw_example_synthpp_mono_note_off(instance, note);
	else {
		instance->notes_pressed[(int)note] = 1;
		update_note_gate(instance);
	}
}

void bw_example_synthpp_mono_note_off(bw_example_synthpp_mono *instance, char note) {
	if (instance->notes_pressed[(int)note]) {
		instance->notes_pressed[(int)note] = 0;
		update_note_gate(instance);
	}
}

void bw_example_synthpp_mono_pitch_bend(bw_example_synthpp_mono *instance, int value) {
	instance->pitch_bend = (value - 0x2000) / (float)0x4000;
}

void bw_example_synthpp_mono_mod_wheel(bw_example_synthpp_mono *instance, char value) {
	instance->mod_wheel = (float)value / 0x80;
}
