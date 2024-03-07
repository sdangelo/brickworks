/*
 * Brickworks
 *
 * Copyright (C) 2022-2024 Orastron Srl unipersonale
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

#define BUFFER_SIZE	128

typedef struct plugin {
	bw_phase_gen_coeffs	phase_gen_coeffs;
	bw_phase_gen_state	phase_gen_state;
	bw_osc_pulse_coeffs	osc_pulse_coeffs;
	bw_osc_filt_state	osc_filt_state;	
	bw_svf_coeffs		svf_coeffs;
	bw_svf_state		svf_state;
	bw_env_gen_coeffs	env_gen_coeffs;
	bw_env_gen_state	env_gen_state;
	bw_gain_coeffs		gain_coeffs;
	bw_ppm_coeffs		ppm_coeffs;
	bw_ppm_state		ppm_state;

	float			master_tune;
	int			note;

	float			buf[BUFFER_SIZE];
} plugin;

static void plugin_init(plugin *instance) {
	bw_phase_gen_init(&instance->phase_gen_coeffs);
	bw_osc_pulse_init(&instance->osc_pulse_coeffs);
	bw_svf_init(&instance->svf_coeffs);
	bw_env_gen_init(&instance->env_gen_coeffs);
	bw_gain_init(&instance->gain_coeffs);
	bw_ppm_init(&instance->ppm_coeffs);
	bw_osc_pulse_set_antialiasing(&instance->osc_pulse_coeffs, 1);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_phase_gen_set_sample_rate(&instance->phase_gen_coeffs, sample_rate);
	bw_osc_pulse_set_sample_rate(&instance->osc_pulse_coeffs, sample_rate);
	bw_svf_set_sample_rate(&instance->svf_coeffs, sample_rate);
	bw_env_gen_set_sample_rate(&instance->env_gen_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->gain_coeffs, sample_rate);
	bw_ppm_set_sample_rate(&instance->ppm_coeffs, sample_rate);
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
	bw_phase_gen_reset_coeffs(&instance->phase_gen_coeffs);
	float p, inc;
	bw_phase_gen_reset_state(&instance->phase_gen_coeffs, &instance->phase_gen_state, 0.f, &p, &inc);
	bw_osc_pulse_reset_coeffs(&instance->osc_pulse_coeffs);
	bw_osc_filt_reset_state(&instance->osc_filt_state, 0.f);
	bw_svf_reset_coeffs(&instance->svf_coeffs);
	float lp, bp, hp;
	bw_svf_reset_state(&instance->svf_coeffs, &instance->svf_state, 0.f, &lp, &bp, &hp);
	bw_env_gen_reset_coeffs(&instance->env_gen_coeffs);
	bw_env_gen_reset_state(&instance->env_gen_coeffs, &instance->env_gen_state, 0);
	bw_gain_reset_coeffs(&instance->gain_coeffs);
	bw_ppm_reset_coeffs(&instance->ppm_coeffs);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_state, 0.f);
	instance->note = -1;
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
	{
		float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->gain_coeffs, v * v * v);
	}
		break;
	case 1:
		instance->master_tune = value;
		break;
	case 2:
		// using portamento time 0% -> 90%: tau = portamento time / log(10)
		bw_phase_gen_set_portamento_tau(&instance->phase_gen_coeffs, (0.001f * 0.4342944819032517f) * value);
		break;
	case 3:
		bw_osc_pulse_set_pulse_width(&instance->osc_pulse_coeffs, 0.01f * value);
		break;
	case 4:
		bw_svf_set_cutoff(&instance->svf_coeffs, value);
		break;
	case 5:
		bw_svf_set_Q(&instance->svf_coeffs, 0.5f + (9.5f * 0.01f) * value);
		break;
	case 6:
		bw_env_gen_set_attack(&instance->env_gen_coeffs, 0.001f * value);
		break;
	case 7:
		bw_env_gen_set_decay(&instance->env_gen_coeffs, 0.001f * value);
		break;
	case 8:
		bw_env_gen_set_sustain(&instance->env_gen_coeffs, 0.01f * value);
		break;
	case 9:
		bw_env_gen_set_release(&instance->env_gen_coeffs, 0.001f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)index;
	return bw_clipf(bw_ppm_get_y_z1(&instance->ppm_state), -60.f, 0.f);
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	(void)inputs;

	if (instance->note >= 0)
		bw_phase_gen_set_frequency(&instance->phase_gen_coeffs,
			instance->master_tune * bw_pow2f(8.333333333333333e-2f * (instance->note - 69)));
	
	for (size_t i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = outputs[0] + i;
		size_t ni = n_samples - i;
		size_t n = ni < BUFFER_SIZE ? ni : BUFFER_SIZE;
		
		bw_phase_gen_process(&instance->phase_gen_coeffs, &instance->phase_gen_state, NULL, out, instance->buf, n);
		bw_osc_pulse_process(&instance->osc_pulse_coeffs, out, instance->buf, out, n);
		bw_osc_filt_process(&instance->osc_filt_state, out, out, n);
		bw_svf_process(&instance->svf_coeffs, &instance->svf_state, out, out, NULL, NULL, n);
		bw_env_gen_process(&instance->env_gen_coeffs, &instance->env_gen_state, instance->note >= 0, instance->buf, n);
		bw_buf_mul(out, instance->buf, out, n);
		bw_gain_process(&instance->gain_coeffs, out, out, n);
		bw_ppm_process(&instance->ppm_coeffs, &instance->ppm_state, out, NULL, n);
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
