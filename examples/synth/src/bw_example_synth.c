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

#include "bw_example_synth.h"

#include <bw_math.h>
#include <bw_phase_gen.h>
#include <bw_osc_pulse.h>
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
	p_cutoff,
	p_Q,
	p_attack,
	p_decay,
	p_sustain,
	p_release,
	p_n
};

#define BUFFER_SIZE	128

struct _bw_example_synth {
	// Sub-components
	bw_phase_gen		phase_gen;
	bw_osc_pulse		osc_pulse;
	bw_osc_filt	osc_filt;
	bw_svf			svf;
	bw_env_gen		env_gen;
	bw_vol			vol;
	bw_env_follow		env_follow;

	// Parameters
	float			params[p_n];

	// States
	int			note;
	float			level;

	// Buffers
	float			buf[BUFFER_SIZE];
};

bw_example_synth bw_example_synth_new() {
	bw_example_synth instance = (bw_example_synth)BW_MALLOC(sizeof(struct _bw_example_synth));
	if (instance == NULL)
		return NULL;

	instance->phase_gen = bw_phase_gen_new();
	if (instance->phase_gen == NULL)
		goto err_phase_gen;

	instance->osc_pulse = bw_osc_pulse_new();
	if (instance->osc_pulse == NULL)
		goto err_osc_pulse;

	instance->osc_filt = bw_osc_filt_new();
	if (instance->osc_filt == NULL)
		goto err_osc_filt;

	instance->svf = bw_svf_new();
	if (instance->svf == NULL)
		goto err_svf;

	instance->env_gen = bw_env_gen_new();
	if (instance->env_gen == NULL)
		goto err_env_gen;

	instance->vol = bw_vol_new();
	if (instance->vol == NULL)
		goto err_vol;

	instance->env_follow = bw_env_follow_new();
	if (instance->env_follow == NULL)
		goto err_env_follow;

	bw_osc_pulse_set_antialiasing(instance->osc_pulse, 1);
	bw_one_pole_set_cutoff_down(bw_env_follow_get_one_pole(instance->env_follow), 1.f);

	return instance;

err_env_follow:
	bw_vol_free(instance->vol);
err_vol:
	bw_env_gen_free(instance->env_gen);
err_env_gen:
	bw_svf_free(instance->svf);
err_svf:
	bw_osc_filt_free(instance->osc_filt);
err_osc_filt:
	bw_osc_pulse_free(instance->osc_pulse);
err_osc_pulse:
	bw_phase_gen_free(instance->phase_gen);
err_phase_gen:
	BW_FREE(instance);
	return NULL;
}

void bw_example_synth_free(bw_example_synth instance) {
	bw_env_follow_free(instance->env_follow);
	bw_vol_free(instance->vol);
	bw_env_gen_free(instance->env_gen);
	bw_svf_free(instance->svf);
	bw_osc_filt_free(instance->osc_filt);
	bw_osc_pulse_free(instance->osc_pulse);
	bw_phase_gen_free(instance->phase_gen);
	BW_FREE(instance);
}

void bw_example_synth_set_sample_rate(bw_example_synth instance, float sample_rate) {
	bw_phase_gen_set_sample_rate(instance->phase_gen, sample_rate);
	bw_osc_pulse_set_sample_rate(instance->osc_pulse, sample_rate);
	bw_svf_set_sample_rate(instance->svf, sample_rate);
	bw_env_gen_set_sample_rate(instance->env_gen, sample_rate);
	bw_vol_set_sample_rate(instance->vol, sample_rate);
	bw_env_follow_set_sample_rate(instance->env_follow, sample_rate);
}

void bw_example_synth_reset(bw_example_synth instance) {
	bw_phase_gen_reset(instance->phase_gen);
	bw_osc_pulse_reset(instance->osc_pulse);
	bw_osc_filt_reset(instance->osc_filt);
	bw_svf_reset(instance->svf);
	bw_env_gen_reset(instance->env_gen);
	bw_vol_reset(instance->vol);
	bw_env_follow_reset(instance->env_follow);
	instance->note = -1;
}

void bw_example_synth_process(bw_example_synth instance, const float** x, float** y, int n_samples) {
	// TODO: I was too lazy to keep track of master tune and note and only update when needed, could be improved
	if (instance->note != -1) {
		bw_phase_gen_set_frequency(instance->phase_gen,
				440.f * bw_pow2f_3(8.333333333333333e-2f * ((instance->note - 69) + 2.f * instance->params[p_master_tune] - 1.f)));
		bw_env_gen_set_gate(instance->env_gen, 1);
	} else
		bw_env_gen_set_gate(instance->env_gen, 0);

	for (int i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = y[0] + i;
		const uint32_t n = bw_minu32(n_samples - i, BUFFER_SIZE);
		bw_phase_gen_process(instance->phase_gen, NULL, out, instance->buf, n);
		bw_osc_pulse_process(instance->osc_pulse, out, instance->buf, out, n);
		bw_osc_filt_process(instance->osc_filt, out, out, n);
		bw_svf_process(instance->svf, out, out, NULL, NULL, n);
		bw_env_gen_process(instance->env_gen, instance->buf, n);
		for (int j = 0; j < n; j++)
			out[j] *= instance->buf[j];
		bw_vol_process(instance->vol, (const float **)&out, &out, 1, n);
		bw_env_follow_process(instance->env_follow, out, instance->buf, n);
		instance->level = instance->buf[i + n - 1];
	}
}

void bw_example_synth_set_parameter(bw_example_synth instance, int index, float value) {
	switch (index) {
	case p_volume:
		bw_vol_set_volume(instance->vol, value);
		break;
	case p_master_tune:
		instance->params[p_master_tune] = value;
		break;
	case p_portamento:
		bw_phase_gen_set_portamento_tau(instance->phase_gen, value);
		break;
	case p_pulse_width:
		bw_osc_pulse_set_pulse_width(instance->osc_pulse, value);
		break;
	case p_cutoff:
		bw_svf_set_cutoff(instance->svf, 20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_Q:
		bw_svf_set_Q(instance->svf, 0.5f + 9.5f * value);
		break;
	case p_attack:
		bw_env_gen_set_attack(instance->env_gen, value);
		break;
	case p_decay:
		bw_env_gen_set_decay(instance->env_gen, value);
		break;
	case p_sustain:
		bw_env_gen_set_sustain(instance->env_gen, value);
		break;
	case p_release:
		bw_env_gen_set_release(instance->env_gen, value);
		break;
	}
}

float bw_example_synth_get_parameter(bw_example_synth instance, int index) {
	return index < p_n ? instance->params[index] : instance->level;
}

void bw_example_synth_note_on(bw_example_synth instance, char note, char velocity) {
	instance->note = note;
}

void bw_example_synth_note_off(bw_example_synth instance, char note) {
	if (note == instance->note)
		instance->note = -1;
}
