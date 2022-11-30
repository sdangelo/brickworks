/*
 * Brickworks
 *
 * Copyright (C) 2022 Orastron Srl unipersonale
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
 *
 * File author: Stefano D'Angelo
 */

#include "bw_example_synth_mono.h"

#ifdef __WASM__
# include "walloc.h"
#else
# include <stdlib.h>
#endif

#include <bw_math.h>
#include <bw_phase_gen.h>
#include <bw_osc_pulse.h>
#include <bw_osc_sin.h>
#include <bw_osc_filt.h>
#include <bw_svf.h>
#include <bw_env_gen.h>
#include <bw_vol.h>
#include <bw_env_follow.h>

enum {
	p_volume,
	p_master_tune,
	p_portamento,
	p_pulse_width,
	p_kbd_ctrl,
	p_cutoff,
	p_Q,
	p_contour,
	p_vcf_attack,
	p_vcf_decay,
	p_vcf_sustain,
	p_vcf_release,
	p_attack,
	p_decay,
	p_sustain,
	p_release,
	p_a440,
	p_n
};

#define BUFFER_SIZE 	128

struct _bw_example_synth_mono {
	// Sub-components
	bw_phase_gen_coeffs	phase_gen_coeffs;
	bw_phase_gen_state	phase_gen_state;
	bw_osc_pulse_coeffs	osc_pulse_coeffs;
	bw_osc_filt_state	osc_filt_state;	
	bw_env_gen_coeffs	vcf_env_gen_coeffs;
	bw_env_gen_state	vcf_env_gen_state;
	bw_svf_coeffs		svf_coeffs;
	bw_svf_state		svf_state;
	bw_env_gen_coeffs	env_gen_coeffs;
	bw_env_gen_state	env_gen_state;
	bw_phase_gen_coeffs	a440_phase_gen_coeffs;
	bw_phase_gen_state	a440_phase_gen_state;
	bw_vol_coeffs		vol_coeffs;
	bw_env_follow_coeffs	env_follow_coeffs;
	bw_env_follow_state	env_follow_state;

	// Parameters
	float			params[p_n];

	// States
	uint64_t		rand_state;
	int			note;
	char			gate;
	
	// Buffers
	float			buf[BUFFER_SIZE];
};

bw_example_synth_mono bw_example_synth_mono_new() {
	bw_example_synth_mono instance = (bw_example_synth_mono)malloc(sizeof(struct _bw_example_synth_mono));
	if (instance == NULL)
		return NULL;

	bw_phase_gen_init(&instance->phase_gen_coeffs);
	bw_osc_pulse_init(&instance->osc_pulse_coeffs);
	bw_env_gen_init(&instance->vcf_env_gen_coeffs);
	bw_svf_init(&instance->svf_coeffs);
	bw_env_gen_init(&instance->env_gen_coeffs);
	bw_phase_gen_init(&instance->a440_phase_gen_coeffs);
	bw_vol_init(&instance->vol_coeffs);
	bw_env_follow_init(&instance->env_follow_coeffs);
	
	bw_osc_pulse_set_antialiasing(&instance->osc_pulse_coeffs, 1);
	bw_phase_gen_set_frequency(&instance->a440_phase_gen_coeffs, 440.f);
	bw_env_follow_set_release_tau(&instance->env_follow_coeffs, 1.f);
	
	instance->rand_state = 0xbaddecaf600dfeed;

	return instance;
}

void bw_example_synth_mono_free(bw_example_synth_mono instance) {
	free(instance);
}

void bw_example_synth_mono_set_sample_rate(bw_example_synth_mono instance, float sample_rate) {
	bw_phase_gen_set_sample_rate(&instance->phase_gen_coeffs, sample_rate);
	bw_osc_pulse_set_sample_rate(&instance->osc_pulse_coeffs, sample_rate);
	bw_env_gen_set_sample_rate(&instance->vcf_env_gen_coeffs, sample_rate);
	bw_svf_set_sample_rate(&instance->svf_coeffs, sample_rate);
	bw_env_gen_set_sample_rate(&instance->env_gen_coeffs, sample_rate);
	bw_phase_gen_set_sample_rate(&instance->a440_phase_gen_coeffs, sample_rate);
	bw_vol_set_sample_rate(&instance->vol_coeffs, sample_rate);
	bw_env_follow_set_sample_rate(&instance->env_follow_coeffs, sample_rate);
}

void bw_example_synth_mono_reset(bw_example_synth_mono instance) {
	bw_phase_gen_reset_coeffs(&instance->phase_gen_coeffs);
	bw_phase_gen_reset_state(&instance->phase_gen_coeffs, &instance->phase_gen_state, 0.f);
	bw_osc_pulse_reset_coeffs(&instance->osc_pulse_coeffs);
	bw_osc_filt_reset_state(&instance->osc_filt_state);
	bw_env_gen_reset_coeffs(&instance->vcf_env_gen_coeffs);
	bw_env_gen_reset_state(&instance->vcf_env_gen_coeffs, &instance->vcf_env_gen_state);
	bw_svf_reset_coeffs(&instance->svf_coeffs);
	bw_svf_reset_state(&instance->svf_coeffs, &instance->svf_state);
	bw_env_gen_reset_coeffs(&instance->env_gen_coeffs);
	bw_env_gen_reset_state(&instance->env_gen_coeffs, &instance->env_gen_state);
	bw_phase_gen_reset_coeffs(&instance->a440_phase_gen_coeffs);
	bw_phase_gen_reset_state(&instance->a440_phase_gen_coeffs, &instance->a440_phase_gen_state, 0.f);
	bw_vol_reset_coeffs(&instance->vol_coeffs);
	bw_env_follow_reset_coeffs(&instance->env_follow_coeffs);
	bw_env_follow_reset_state(&instance->env_follow_coeffs, &instance->env_follow_state);
	instance->note = 60;
	instance->gate = 0;
}

void bw_example_synth_mono_process(bw_example_synth_mono instance, const float** x, float** y, int n_samples) {
	bw_env_gen_set_gate(&instance->vcf_env_gen_coeffs, instance->gate);
	bw_env_gen_set_gate(&instance->env_gen_coeffs, instance->gate);

	bw_phase_gen_set_frequency(&instance->phase_gen_coeffs, 440.f *
		bw_pow2f_3(8.333333333333333e-2f * (
			(instance->note - 69)
			+ 2.f * instance->params[p_master_tune] - 1.f)));
	
	for (int i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = y[0] + i;
		int n = bw_minf(n_samples - i, BUFFER_SIZE);
		
		bw_phase_gen_process(&instance->phase_gen_coeffs, &instance->phase_gen_state, NULL, out, instance->buf, n);
		bw_osc_pulse_process(&instance->osc_pulse_coeffs, out, instance->buf, out, n);
		bw_osc_filt_process(&instance->osc_filt_state, out, out, n);

		bw_env_gen_process(&instance->vcf_env_gen_coeffs, &instance->vcf_env_gen_state, NULL, n);
		float v = instance->params[p_cutoff] + instance->params[p_contour] * bw_env_gen_get_y_z1(&instance->vcf_env_gen_state);
		float cutoff = 20.f + (20e3f - 20.f) * v * v * v;
		if (instance->params[p_kbd_ctrl] < (1.f / 6.f))
			/* no keyboard control */;
		else if (instance->params[p_kbd_ctrl] < (1.f / 6.f + 1.f / 3.f))
			cutoff *= bw_pow2f_3((0.629960524947437f * 8.333333333333333e-2f) * (instance->note - 60));
		else if (instance->params[p_kbd_ctrl] < (1.f / 6.f + 2.f / 3.f))
			cutoff *= bw_pow2f_3((0.793700525984100f * 8.333333333333333e-2f) * (instance->note - 60));
		else
			cutoff *= bw_pow2f_3(8.333333333333333e-2f * (instance->note - 60));
		bw_svf_set_cutoff(&instance->svf_coeffs, bw_clipf(cutoff, 20.f, 20e3f));
		bw_svf_process(&instance->svf_coeffs, &instance->svf_state, out, out, NULL, NULL, n);

		bw_env_gen_process(&instance->env_gen_coeffs, &instance->env_gen_state, instance->buf, n);
		for (int j = 0; j < n; j++)
			out[j] *= instance->buf[j];

		bw_phase_gen_process(&instance->a440_phase_gen_coeffs, &instance->a440_phase_gen_state, NULL, instance->buf, NULL, n);
		bw_osc_sin_process(instance->buf, instance->buf, n);
		if (instance->params[p_a440] >= 0.5f)
			for (int j = 0; j < n; j++)
				out[j] += instance->buf[j];

		bw_vol_process(&instance->vol_coeffs, out, out, n);
		bw_env_follow_process(&instance->env_follow_coeffs, &instance->env_follow_state, out, NULL, n);
	}
}

void bw_example_synth_mono_set_parameter(bw_example_synth_mono instance, int index, float value) {
	if (instance->params[index] == value)
		return;
	instance->params[index] = value;
	switch (index) {
	case p_volume:
		bw_vol_set_volume(&instance->vol_coeffs, value);
		break;
	case p_portamento:
		bw_phase_gen_set_portamento_tau(&instance->phase_gen_coeffs, value);
		break;
	case p_pulse_width:
		bw_osc_pulse_set_pulse_width(&instance->osc_pulse_coeffs, value);
		break;
	case p_Q:
		bw_svf_set_Q(&instance->svf_coeffs, 0.5f + 9.5f * value);
		break;
	case p_vcf_attack:
		bw_env_gen_set_attack(&instance->vcf_env_gen_coeffs, value);
		break;
	case p_vcf_decay:
		bw_env_gen_set_decay(&instance->vcf_env_gen_coeffs, value);
		break;
	case p_vcf_sustain:
		bw_env_gen_set_sustain(&instance->vcf_env_gen_coeffs, value);
		break;
	case p_vcf_release:
		bw_env_gen_set_release(&instance->vcf_env_gen_coeffs, value);
		break;
	case p_attack:
		bw_env_gen_set_attack(&instance->env_gen_coeffs, value);
		break;
	case p_decay:
		bw_env_gen_set_decay(&instance->env_gen_coeffs, value);
		break;
	case p_sustain:
		bw_env_gen_set_sustain(&instance->env_gen_coeffs, value);
		break;
	case p_release:
		bw_env_gen_set_release(&instance->env_gen_coeffs, value);
		break;
	}
}

float bw_example_synth_mono_get_parameter(bw_example_synth_mono instance, int index) {
	return index < p_n ? instance->params[index] : bw_clipf(bw_env_follow_get_y_z1(&instance->env_follow_state), 0.f, 1.f);
}

void bw_example_synth_mono_note_on(bw_example_synth_mono instance, char note, char velocity) {
	instance->note = note;
	instance->gate = 1;
}

void bw_example_synth_mono_note_off(bw_example_synth_mono instance, char note) {
	if (note == instance->note)
		instance->gate = 0;
}
