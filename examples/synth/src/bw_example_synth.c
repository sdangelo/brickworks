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

#ifdef __WASM__
# include "walloc.h"
#else
# include <stdlib.h>
#endif

#include <bw_math.h>
#include <bw_noise_gen.h>
#include <bw_phase_gen.h>
#include <bw_osc_sin.h>
#include <bw_vol.h>
#include <bw_env_follow.h>

/*
#include <bw_osc_pulse.h>
#include <bw_osc_filt.h>
#include <bw_svf.h>
#include <bw_env_gen.h>
#include <bw_vol.h>
#include <bw_env_follow.h>
*/

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
	p_a440,
	p_n
};

#define BUFFER_SIZE	128

struct _bw_example_synth {
	// Sub-components
	bw_phase_gen_coeffs	phase_gen_coeffs;
	bw_phase_gen_state	phase_gen_state;
	bw_noise_gen_coeffs	noise_gen_coeffs;
	bw_phase_gen_coeffs	a440_phase_gen_coeffs;
	bw_phase_gen_state	a440_phase_gen_state;
	bw_vol_coeffs		vol_coeffs;
	bw_env_follow_coeffs	env_follow_coeffs;
	bw_env_follow_state	env_follow_state;
	/*
	bw_osc_pulse		osc_pulse;
	bw_osc_filt		osc_filt;
	bw_svf			svf;
	bw_env_gen		env_gen;
	bw_vol			vol;
	bw_env_follow		env_follow;
	*/

	// Parameters
	float			params[p_n];

	// States
	uint64_t		rand_state;
	int			note;
	float			level;

	// Buffers
	float			buf[BUFFER_SIZE];
};

bw_example_synth bw_example_synth_new() {
	bw_example_synth instance = (bw_example_synth)malloc(sizeof(struct _bw_example_synth));
	if (instance == NULL)
		return NULL;

	bw_phase_gen_init(&instance->phase_gen_coeffs);
	bw_noise_gen_init(&instance->noise_gen_coeffs, &instance->rand_state);
	bw_phase_gen_init(&instance->a440_phase_gen_coeffs);
	bw_vol_init(&instance->vol_coeffs);
	bw_env_follow_init(&instance->env_follow_coeffs);

	bw_phase_gen_set_frequency(&instance->a440_phase_gen_coeffs, 440.f);
	bw_noise_gen_set_sample_rate_scaling(&instance->noise_gen_coeffs, 1);
	bw_env_follow_set_release_tau(&instance->env_follow_coeffs, 1.f);
	
	instance->rand_state = 0xbaddecaf600dfeed;
	/*
	bw_osc_pulse_init(&instance->osc_pulse);
	bw_osc_filt_init(&instance->osc_filt);
	bw_svf_init(&instance->svf);
	bw_env_gen_init(&instance->env_gen);

	bw_osc_pulse_set_antialiasing(&instance->osc_pulse, 1);
	*/

	return instance;
}

void bw_example_synth_free(bw_example_synth instance) {
	free(instance);
}

void bw_example_synth_set_sample_rate(bw_example_synth instance, float sample_rate) {
	bw_phase_gen_set_sample_rate(&instance->phase_gen_coeffs, sample_rate);
	bw_noise_gen_set_sample_rate(&instance->noise_gen_coeffs, sample_rate);
	bw_phase_gen_set_sample_rate(&instance->a440_phase_gen_coeffs, sample_rate);
	bw_vol_set_sample_rate(&instance->vol_coeffs, sample_rate);
	bw_env_follow_set_sample_rate(&instance->env_follow_coeffs, sample_rate);
	/*
	bw_osc_pulse_set_sample_rate(&instance->osc_pulse, sample_rate);
	bw_svf_set_sample_rate(&instance->svf, sample_rate);
	bw_env_gen_set_sample_rate(&instance->env_gen, sample_rate);
	*/
}

void bw_example_synth_reset(bw_example_synth instance) {
	bw_phase_gen_reset_coeffs(&instance->phase_gen_coeffs);
	bw_phase_gen_reset_state(&instance->phase_gen_coeffs, &instance->phase_gen_state);
	bw_phase_gen_reset_coeffs(&instance->a440_phase_gen_coeffs);
	bw_phase_gen_reset_state(&instance->a440_phase_gen_coeffs, &instance->a440_phase_gen_state);
	bw_vol_reset_coeffs(&instance->vol_coeffs);
	bw_env_follow_reset_coeffs(&instance->env_follow_coeffs);
	bw_env_follow_reset_state(&instance->env_follow_coeffs, &instance->env_follow_state);
	/*
	bw_osc_pulse_reset(&instance->osc_pulse);
	bw_osc_filt_reset(&instance->osc_filt);
	bw_svf_reset(&instance->svf);
	bw_env_gen_reset(&instance->env_gen);
	bw_vol_reset(&instance->vol);
	bw_env_follow_reset(&instance->env_follow);
	*/
	instance->note = -1;
}

void bw_example_synth_process(bw_example_synth instance, const float** x, float** y, int n_samples) {
	// TODO: I was too lazy to keep track of master tune and note and only update when needed, could be improved
	if (instance->note != -1) {
		bw_phase_gen_set_frequency(&instance->phase_gen_coeffs,
				440.f * bw_pow2f_3(8.333333333333333e-2f * ((instance->note - 69) + 2.f * instance->params[p_master_tune] - 1.f)));
		//bw_env_gen_set_gate(&instance->env_gen, 1);
	} else
		;
		//bw_env_gen_set_gate(&instance->env_gen, 0);

	//bw_phase_gen_process(&instance->phase_gen_coeffs, &instance->phase_gen_state, NULL, y[0], instance->buf, n_samples);
	for (int i = 0; i < n_samples; i++)
		y[0][i] = 0.f;
	
	if (instance->note != -1)
		bw_noise_gen_process(&instance->noise_gen_coeffs, y[0], n_samples);
	
	/*
	for (int i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = y[0] + i;
		const uint32_t n = bw_minu32(n_samples - i, BUFFER_SIZE);
		bw_phase_gen_process(&instance->phase_gen, NULL, out, instance->buf, n);
		bw_osc_pulse_process(&instance->osc_pulse, out, instance->buf, out, n);
		bw_osc_filt_process(&instance->osc_filt, out, out, n);
		bw_svf_process(&instance->svf, out, out, NULL, NULL, n);
		bw_env_gen_process(&instance->env_gen, instance->buf, n);
		for (int j = 0; j < n; j++)
			out[j] *= instance->buf[j];
		bw_vol_process(&instance->vol, (const float **)&out, &out, 1, n);
		bw_env_follow_process(&instance->env_follow, out, instance->buf, n);
		instance->level = instance->buf[i + n - 1];
	}
	*/
	instance->level = 0.f;

	if (instance->params[p_a440] >= 0.5f)
		for (int i = 0; i < n_samples; i++) {
			float a440_y, a440_y_phase_inc;
			bw_phase_gen_process1(&instance->a440_phase_gen_coeffs, &instance->a440_phase_gen_state, &a440_y, &a440_y_phase_inc);
			a440_y = bw_osc_sin_process1(a440_y);
			y[0][i] += a440_y;
		}
	bw_vol_process(&instance->vol_coeffs, y[0], y[0], n_samples);
	
	for (int i = 0; i < n_samples; i++)
		bw_env_follow_process1(&instance->env_follow_coeffs, &instance->env_follow_state, y[0][i]);
	instance->level = bw_env_follow_get_y_z1(&instance->env_follow_state);
}

void bw_example_synth_set_parameter(bw_example_synth instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_volume:
		bw_vol_set_volume(&instance->vol_coeffs, value);
		break;
	case p_portamento:
		bw_phase_gen_set_portamento_tau(&instance->phase_gen_coeffs, value);
		break;
	/*
	case p_pulse_width:
		bw_osc_pulse_set_pulse_width(&instance->osc_pulse, value);
		break;
	case p_cutoff:
		bw_svf_set_cutoff(&instance->svf, 20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_Q:
		bw_svf_set_Q(&instance->svf, 0.5f + 9.5f * value);
		break;
	case p_attack:
		bw_env_gen_set_attack(&instance->env_gen, value);
		break;
	case p_decay:
		bw_env_gen_set_decay(&instance->env_gen, value);
		break;
	case p_sustain:
		bw_env_gen_set_sustain(&instance->env_gen, value);
		break;
	case p_release:
		bw_env_gen_set_release(&instance->env_gen, value);
		break;
	*/
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
