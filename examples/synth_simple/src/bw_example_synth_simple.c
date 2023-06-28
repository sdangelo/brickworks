/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
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

#include "bw_example_synth_simple.h"

#include <bw_buf.h>

void bw_example_synth_simple_init(bw_example_synth_simple *instance) {
	bw_phase_gen_init(&instance->phase_gen_coeffs);
	bw_osc_pulse_init(&instance->osc_pulse_coeffs);
	bw_svf_init(&instance->svf_coeffs);
	bw_env_gen_init(&instance->env_gen_coeffs);
	bw_gain_init(&instance->gain_coeffs);
	bw_ppm_init(&instance->ppm_coeffs);
	
	bw_osc_pulse_set_antialiasing(&instance->osc_pulse_coeffs, 1);
}

void bw_example_synth_simple_set_sample_rate(bw_example_synth_simple *instance, float sample_rate) {
	bw_phase_gen_set_sample_rate(&instance->phase_gen_coeffs, sample_rate);
	bw_osc_pulse_set_sample_rate(&instance->osc_pulse_coeffs, sample_rate);
	bw_svf_set_sample_rate(&instance->svf_coeffs, sample_rate);
	bw_env_gen_set_sample_rate(&instance->env_gen_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->gain_coeffs, sample_rate);
	bw_ppm_set_sample_rate(&instance->ppm_coeffs, sample_rate);
}

void bw_example_synth_simple_reset(bw_example_synth_simple *instance) {
	bw_phase_gen_reset_coeffs(&instance->phase_gen_coeffs);
	bw_phase_gen_reset_state(&instance->phase_gen_coeffs, &instance->phase_gen_state, 0.f);
	bw_osc_pulse_reset_coeffs(&instance->osc_pulse_coeffs);
	bw_osc_filt_reset_state(&instance->osc_filt_state);
	bw_svf_reset_coeffs(&instance->svf_coeffs);
	bw_svf_reset_state(&instance->svf_coeffs, &instance->svf_state, 0.f);
	bw_env_gen_reset_coeffs(&instance->env_gen_coeffs);
	bw_env_gen_reset_state(&instance->env_gen_coeffs, &instance->env_gen_state);
	bw_gain_reset_coeffs(&instance->gain_coeffs);
	bw_ppm_reset_coeffs(&instance->ppm_coeffs);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_state);
	instance->note = -1;
}

void bw_example_synth_simple_process(bw_example_synth_simple *instance, const float** x, float** y, int n_samples) {
	(void)x;

	if (instance->note >= 0)
		bw_phase_gen_set_frequency(&instance->phase_gen_coeffs,
			440.f * bw_pow2f_3(8.333333333333333e-2f * ((instance->note - 69) + 2.f * instance->params[p_master_tune] - 1.f)));
	
	for (int i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = y[0] + i;
		int n = bw_minf(n_samples - i, BUFFER_SIZE);
		
		bw_phase_gen_process(&instance->phase_gen_coeffs, &instance->phase_gen_state, NULL, out, instance->buf, n);
		bw_osc_pulse_process(&instance->osc_pulse_coeffs, out, instance->buf, out, n);
		bw_osc_filt_process(&instance->osc_filt_state, out, out, n);
		bw_svf_process(&instance->svf_coeffs, &instance->svf_state, out, out, NULL, NULL, n);
		bw_env_gen_process(&instance->env_gen_coeffs, &instance->env_gen_state, instance->note >= 0, instance->buf, n);
		bw_buf_mul(out, out, instance->buf, n);
		bw_gain_process(&instance->gain_coeffs, out, out, n);
		bw_ppm_process(&instance->ppm_coeffs, &instance->ppm_state, out, NULL, n);
	}
}

void bw_example_synth_simple_set_parameter(bw_example_synth_simple *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_volume:
		bw_gain_set_gain_lin(&instance->gain_coeffs, value * value * value);
		break;
	case p_portamento:
		bw_phase_gen_set_portamento_tau(&instance->phase_gen_coeffs, value);
		break;
	case p_pulse_width:
		bw_osc_pulse_set_pulse_width(&instance->osc_pulse_coeffs, value);
		break;
	case p_cutoff:
		bw_svf_set_cutoff(&instance->svf_coeffs, 20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_Q:
		bw_svf_set_Q(&instance->svf_coeffs, 0.5f + 9.5f * value);
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

float bw_example_synth_simple_get_parameter(bw_example_synth_simple *instance, int index) {
	if (index < p_n)
		return instance->params[index];
	const float v = bw_ppm_get_y_z1(&instance->ppm_state);
	return v < -200.f ? 0.f : bw_clipf(0.01666666666666666f * v + 1.f, 0.f, 1.f);
}

void bw_example_synth_simple_note_on(bw_example_synth_simple *instance, char note, char velocity) {
	if (velocity == 0) 
		bw_example_synth_simple_note_off(instance, note);
	else
		instance->note = note;
}

void bw_example_synth_simple_note_off(bw_example_synth_simple *instance, char note) {
	if (note == instance->note)
		instance->note = -1;
}
