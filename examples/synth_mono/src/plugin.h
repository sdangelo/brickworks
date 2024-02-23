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

#define BUFFER_SIZE	128
#define SYNC_RATE	1e-3f	// synchronous control rate, seconds

typedef struct plugin {
	bw_osc_saw_coeffs	vco_saw_coeffs;
	bw_phase_gen_coeffs	vco1_phase_gen_coeffs;
	bw_phase_gen_state	vco1_phase_gen_state;
	bw_osc_pulse_coeffs	vco1_pulse_coeffs;
	bw_osc_tri_coeffs	vco1_tri_coeffs;
	bw_gain_coeffs		vco1_gain_coeffs;
	bw_phase_gen_coeffs	vco2_phase_gen_coeffs;
	bw_phase_gen_state	vco2_phase_gen_state;
	bw_osc_pulse_coeffs	vco2_pulse_coeffs;
	bw_osc_tri_coeffs	vco2_tri_coeffs;
	bw_gain_coeffs		vco2_gain_coeffs;
	bw_phase_gen_coeffs	vco3_phase_gen_coeffs;
	bw_phase_gen_state	vco3_phase_gen_state;
	bw_osc_pulse_coeffs	vco3_pulse_coeffs;
	bw_osc_tri_coeffs	vco3_tri_coeffs;
	bw_gain_coeffs		vco3_gain_coeffs;
	bw_osc_filt_state	osc_filt_state;
	bw_noise_gen_coeffs	noise_gen_coeffs;
	bw_pink_filt_coeffs	pink_filt_coeffs;
	bw_pink_filt_state	pink_filt_state;
	bw_gain_coeffs		noise_gain_coeffs;
	bw_env_gen_coeffs	vcf_env_gen_coeffs;
	bw_env_gen_state	vcf_env_gen_state;
	bw_svf_coeffs		vcf_coeffs;
	bw_svf_state		vcf_state;
	bw_env_gen_coeffs	vca_env_gen_coeffs;
	bw_env_gen_state	vca_env_gen_state;
	bw_phase_gen_coeffs	a440_phase_gen_coeffs;
	bw_phase_gen_state	a440_phase_gen_state;
	bw_gain_coeffs		gain_coeffs;
	bw_ppm_coeffs		ppm_coeffs;
	bw_ppm_state		ppm_state;

	size_t			sync_count;

	uint64_t		rand_state;
	float			master_tune;
	float			modulation_mix;
	float			vco1_modulation;
	float			vco1_coarse;
	float			vco1_fine;
	char			vco1_waveform;
	float			vco2_modulation;
	float			vco2_coarse;
	float			vco2_fine;
	char			vco2_waveform;
	char			vco3_kbd_ctrl;
	float			vco3_coarse;
	float			vco3_fine;
	char			vco3_waveform;
	char			noise_color;
	float			vcf_modulation;
	char			vcf_kbd_ctrl;
	float			vcf_cutoff;
	float			vcf_contour;
	char			a440;

	int			note;
	char			gate;
	float			pitch_bend;
	float			mod_wheel;
	char			notes_pressed[128];
	size_t			sync_left;
	char			vco3_waveform_cur;
	char			noise_color_cur;
	float			mod_k;
	char			vco1_waveform_cur;
	char			vco2_waveform_cur;
	float			vcf_env_k;

	float			buf[4][BUFFER_SIZE];
} plugin;

static void plugin_init(plugin *instance) {
	bw_osc_saw_init(&instance->vco_saw_coeffs);
	bw_phase_gen_init(&instance->vco1_phase_gen_coeffs);
	bw_osc_pulse_init(&instance->vco1_pulse_coeffs);
	bw_osc_tri_init(&instance->vco1_tri_coeffs);
	bw_gain_init(&instance->vco1_gain_coeffs);
	bw_phase_gen_init(&instance->vco2_phase_gen_coeffs);
	bw_osc_pulse_init(&instance->vco2_pulse_coeffs);
	bw_osc_tri_init(&instance->vco2_tri_coeffs);
	bw_gain_init(&instance->vco2_gain_coeffs);
	bw_phase_gen_init(&instance->vco3_phase_gen_coeffs);
	bw_osc_pulse_init(&instance->vco3_pulse_coeffs);
	bw_osc_tri_init(&instance->vco3_tri_coeffs);
	bw_gain_init(&instance->vco3_gain_coeffs);
	bw_noise_gen_init(&instance->noise_gen_coeffs, &instance->rand_state);
	bw_pink_filt_init(&instance->pink_filt_coeffs);
	bw_gain_init(&instance->noise_gain_coeffs);
	bw_env_gen_init(&instance->vcf_env_gen_coeffs);
	bw_svf_init(&instance->vcf_coeffs);
	bw_env_gen_init(&instance->vca_env_gen_coeffs);
	bw_phase_gen_init(&instance->a440_phase_gen_coeffs);
	bw_gain_init(&instance->gain_coeffs);
	bw_ppm_init(&instance->ppm_coeffs);
	
	bw_osc_saw_set_antialiasing(&instance->vco_saw_coeffs, 1);
	bw_osc_pulse_set_antialiasing(&instance->vco1_pulse_coeffs, 1);
	bw_osc_tri_set_antialiasing(&instance->vco1_tri_coeffs, 1);
	bw_osc_pulse_set_antialiasing(&instance->vco2_pulse_coeffs, 1);
	bw_osc_tri_set_antialiasing(&instance->vco2_tri_coeffs, 1);
	bw_osc_pulse_set_antialiasing(&instance->vco3_pulse_coeffs, 1);
	bw_osc_tri_set_antialiasing(&instance->vco3_tri_coeffs, 1);
	bw_phase_gen_set_frequency(&instance->a440_phase_gen_coeffs, 440.f);
	
	instance->rand_state = 0xbaddecaf600dfeed;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_osc_saw_set_sample_rate(&instance->vco_saw_coeffs, sample_rate);
	bw_phase_gen_set_sample_rate(&instance->vco1_phase_gen_coeffs, sample_rate);
	bw_osc_pulse_set_sample_rate(&instance->vco1_pulse_coeffs, sample_rate);
	bw_osc_tri_set_sample_rate(&instance->vco1_tri_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->vco1_gain_coeffs, sample_rate);
	bw_phase_gen_set_sample_rate(&instance->vco2_phase_gen_coeffs, sample_rate);
	bw_osc_pulse_set_sample_rate(&instance->vco2_pulse_coeffs, sample_rate);
	bw_osc_tri_set_sample_rate(&instance->vco2_tri_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->vco2_gain_coeffs, sample_rate);
	bw_phase_gen_set_sample_rate(&instance->vco3_phase_gen_coeffs, sample_rate);
	bw_osc_pulse_set_sample_rate(&instance->vco3_pulse_coeffs, sample_rate);
	bw_osc_tri_set_sample_rate(&instance->vco3_tri_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->vco3_gain_coeffs, sample_rate);
	bw_noise_gen_set_sample_rate(&instance->noise_gen_coeffs, sample_rate);
	bw_pink_filt_set_sample_rate(&instance->pink_filt_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->noise_gain_coeffs, sample_rate);
	bw_env_gen_set_sample_rate(&instance->vcf_env_gen_coeffs, sample_rate);
	bw_svf_set_sample_rate(&instance->vcf_coeffs, sample_rate);
	bw_env_gen_set_sample_rate(&instance->vca_env_gen_coeffs, sample_rate);
	bw_phase_gen_set_sample_rate(&instance->a440_phase_gen_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->gain_coeffs, sample_rate);
	bw_ppm_set_sample_rate(&instance->ppm_coeffs, sample_rate);

	bw_osc_saw_reset_coeffs(&instance->vco_saw_coeffs);
	bw_noise_gen_reset_coeffs(&instance->noise_gen_coeffs);
	bw_pink_filt_reset_coeffs(&instance->pink_filt_coeffs);

	instance->sync_count = (size_t)bw_roundf(sample_rate * SYNC_RATE);
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
	bw_svf_set_cutoff(&instance->vcf_coeffs, instance->vcf_cutoff);

	float p, pi;
	bw_phase_gen_reset_coeffs(&instance->vco1_phase_gen_coeffs);
	bw_phase_gen_reset_state(&instance->vco1_phase_gen_coeffs, &instance->vco1_phase_gen_state, 0.f, &p, &pi);
	bw_osc_pulse_reset_coeffs(&instance->vco1_pulse_coeffs);
	bw_osc_tri_reset_coeffs(&instance->vco1_tri_coeffs);
	bw_gain_reset_coeffs(&instance->vco1_gain_coeffs);
	bw_phase_gen_reset_coeffs(&instance->vco2_phase_gen_coeffs);
	bw_phase_gen_reset_state(&instance->vco2_phase_gen_coeffs, &instance->vco2_phase_gen_state, 0.f, &p, &pi);
	bw_osc_pulse_reset_coeffs(&instance->vco2_pulse_coeffs);
	bw_osc_tri_reset_coeffs(&instance->vco2_tri_coeffs);
	bw_gain_reset_coeffs(&instance->vco2_gain_coeffs);
	bw_phase_gen_reset_coeffs(&instance->vco3_phase_gen_coeffs);
	bw_phase_gen_reset_state(&instance->vco3_phase_gen_coeffs, &instance->vco3_phase_gen_state, 0.f, &p, &pi);
	bw_osc_pulse_reset_coeffs(&instance->vco3_pulse_coeffs);
	bw_osc_tri_reset_coeffs(&instance->vco3_tri_coeffs);
	bw_gain_reset_coeffs(&instance->vco3_gain_coeffs);
	bw_osc_filt_reset_state(&instance->osc_filt_state, 0.f);
	bw_pink_filt_reset_state(&instance->pink_filt_coeffs, &instance->pink_filt_state, 0.f);
	bw_gain_reset_coeffs(&instance->noise_gain_coeffs);
	bw_env_gen_reset_coeffs(&instance->vcf_env_gen_coeffs);
	bw_env_gen_reset_state(&instance->vcf_env_gen_coeffs, &instance->vcf_env_gen_state, 0);
	bw_svf_reset_coeffs(&instance->vcf_coeffs);
	float lp, bp, hp;
	bw_svf_reset_state(&instance->vcf_coeffs, &instance->vcf_state, 0.f, &lp, &bp, &hp);
	bw_env_gen_reset_coeffs(&instance->vca_env_gen_coeffs);
	bw_env_gen_reset_state(&instance->vca_env_gen_coeffs, &instance->vca_env_gen_state, 0);
	bw_phase_gen_reset_coeffs(&instance->a440_phase_gen_coeffs);
	bw_phase_gen_reset_state(&instance->a440_phase_gen_coeffs, &instance->a440_phase_gen_state, 0.f, &p, &pi);
	bw_gain_reset_coeffs(&instance->gain_coeffs);
	bw_ppm_reset_coeffs(&instance->ppm_coeffs);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_state, 0.f);
	instance->note = 60;
	instance->gate = 0;
	instance->pitch_bend = 0.f;
	instance->mod_wheel = 0.f;
	for (int i = 0; i < 128; i++)
		instance->notes_pressed[i] = 0;
	instance->sync_left = instance->sync_count;
	instance->vco3_waveform_cur = instance->vco3_waveform;
	instance->noise_color_cur = instance->noise_color;
	instance->vco1_waveform_cur = instance->vco1_waveform;
	instance->vco2_waveform_cur = instance->vco2_waveform;
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->gain_coeffs, v * v * v);
	}
		break;
	case 1:
		instance->master_tune = value;
		break;
	case 2:
	{
		// using portamento time 0% -> 90%: tau = portamento time / log(10)
		const float v = (0.001f * 0.4342944819032517f) * value;
		bw_phase_gen_set_portamento_tau(&instance->vco1_phase_gen_coeffs, v);
		bw_phase_gen_set_portamento_tau(&instance->vco2_phase_gen_coeffs, v);
		bw_phase_gen_set_portamento_tau(&instance->vco3_phase_gen_coeffs, v);
		break;
	}
	case 3:
		instance->modulation_mix = 0.01f * value;
		break;
	case 4:
		instance->vco1_modulation = 0.01f * value;
		break;
	case 5:
		instance->vco1_coarse = value;
		break;
	case 6:
		instance->vco1_fine = value;
		break;
	case 7:
		instance->vco1_waveform = value;
		break;
	case 8:
	{
		const float v = 0.01f * value;
		bw_osc_pulse_set_pulse_width(&instance->vco1_pulse_coeffs, v);
		bw_osc_tri_set_slope(&instance->vco1_tri_coeffs, bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case 9:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->vco1_gain_coeffs, v * v * v);
		break;
	}
	case 10:
		instance->vco2_modulation = 0.01f * value;
		break;
	case 11:
		instance->vco2_coarse = value;
		break;
	case 12:
		instance->vco2_fine = value;
		break;
	case 13:
		instance->vco2_waveform = value;
		break;
	case 14:
	{
		const float v = 0.01f * value;
		bw_osc_pulse_set_pulse_width(&instance->vco2_pulse_coeffs, v);
		bw_osc_tri_set_slope(&instance->vco2_tri_coeffs, bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case 15:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->vco2_gain_coeffs, v * v * v);
		break;
	}
	case 16:
		instance->vco3_kbd_ctrl = value >= 0.5f;
		break;
	case 17:
		instance->vco3_coarse = value;
		break;
	case 18:
		instance->vco3_fine = value;
		break;
	case 19:
		instance->vco3_waveform = value;
		break;
	case 20:
	{
		const float v = 0.01f * value;
		bw_osc_pulse_set_pulse_width(&instance->vco3_pulse_coeffs, v);
		bw_osc_tri_set_slope(&instance->vco3_tri_coeffs, bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case 21:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->vco3_gain_coeffs, v * v * v);
		break;
	}
	case 22:
		instance->noise_color = value;
		break;
	case 23:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->noise_gain_coeffs, v * v * v);
		break;
	}
	case 24:
		instance->vcf_modulation = 0.01f * value;
		break;
	case 25:
		instance->vcf_kbd_ctrl = value;
		break;
	case 26:
		instance->vcf_cutoff = value;
		break;
	case 27:
		bw_svf_set_Q(&instance->vcf_coeffs, 0.5f + (0.01f * 9.5f) * value);
		break;
	case 28:
		instance->vcf_contour = 0.01f * value;
		break;
	case 29:
		bw_env_gen_set_attack(&instance->vcf_env_gen_coeffs, 0.001f * value);
		break;
	case 30:
		bw_env_gen_set_decay(&instance->vcf_env_gen_coeffs, 0.001f * value);
		break;
	case 31:
		bw_env_gen_set_sustain(&instance->vcf_env_gen_coeffs, 0.01f * value);
		break;
	case 32:
		bw_env_gen_set_release(&instance->vcf_env_gen_coeffs, 0.001f * value);
		break;
	case 33:
		bw_env_gen_set_attack(&instance->vca_env_gen_coeffs, 0.001f * value);
		break;
	case 34:
		bw_env_gen_set_decay(&instance->vca_env_gen_coeffs, 0.001f * value);
		break;
	case 35:
		bw_env_gen_set_sustain(&instance->vca_env_gen_coeffs, 0.01f * value);
		break;
	case 36:
		bw_env_gen_set_release(&instance->vca_env_gen_coeffs, 0.001f * value);
		break;
	case 37:
		instance->a440 = value >= 0.5f;
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)index;
	return bw_clipf(bw_ppm_get_y_z1(&instance->ppm_state), -60.f, 0.f);
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	(void)inputs;

	// asynchronous control-rate operations

	int n = instance->note - 69;
	int n3 = instance->vco3_kbd_ctrl ? instance->note - 69 : -69;
	bw_phase_gen_set_frequency(&instance->vco1_phase_gen_coeffs,
		instance->master_tune
		* bw_pow2f(instance->vco1_coarse + instance->pitch_bend
			+ 8.333333333333333e-2f * (n + 0.01f * instance->vco1_fine)));
	bw_phase_gen_set_frequency(&instance->vco2_phase_gen_coeffs,
		instance->master_tune
		* bw_pow2f(instance->vco2_coarse + instance->pitch_bend
			+ 8.333333333333333e-2f * (n + 0.01f * instance->vco2_fine)));
	bw_phase_gen_set_frequency(&instance->vco3_phase_gen_coeffs,
		instance->master_tune
		* bw_pow2f(instance->vco3_coarse + instance->pitch_bend
			+ 8.333333333333333e-2f * (n3 + 0.01f * instance->vco3_fine)));

	if (instance->vco3_waveform_cur != instance->vco3_waveform) {
		switch (instance->vco3_waveform) {
		case 2:
			bw_osc_pulse_reset_coeffs(&instance->vco3_pulse_coeffs);
			break;
		case 3:
			bw_osc_tri_reset_coeffs(&instance->vco3_tri_coeffs);
			break;
		}
		instance->vco3_waveform_cur = instance->vco3_waveform;
	}

	if (instance->noise_color_cur != instance->noise_color) {
		if (instance->noise_color == 2)
			bw_pink_filt_reset_state(&instance->pink_filt_coeffs, &instance->pink_filt_state, 0.f);
		instance->noise_color_cur = instance->noise_color;
	}

	if (instance->vco1_waveform_cur != instance->vco1_waveform) {
		switch (instance->vco1_waveform) {
		case 2:
			bw_osc_pulse_reset_coeffs(&instance->vco1_pulse_coeffs);
			break;
		case 3:
			bw_osc_tri_reset_coeffs(&instance->vco1_tri_coeffs);
			break;
		}
		instance->vco1_waveform_cur = instance->vco1_waveform;
	}

	if (instance->vco2_waveform_cur != instance->vco2_waveform) {
		switch (instance->vco2_waveform) {
		case 2:
			bw_osc_pulse_reset_coeffs(&instance->vco2_pulse_coeffs);
			break;
		case 3:
			bw_osc_tri_reset_coeffs(&instance->vco2_tri_coeffs);
			break;
		}
		instance->vco2_waveform_cur = instance->vco2_waveform;
	}

	// synchronous control-rate and audio-rate operations

	for (size_t i = 0; i < n_samples; ) {
		float *out = outputs[0] + i;
		int n = bw_minf(bw_minf(n_samples - i, BUFFER_SIZE), instance->sync_left);

		const char sync = instance->sync_left == instance->sync_count;

		// osc 3

		bw_phase_gen_process(&instance->vco3_phase_gen_coeffs, &instance->vco3_phase_gen_state, NULL, out, instance->buf[0], n);
		switch (instance->vco3_waveform_cur) {
		case 1:
			bw_osc_saw_process(&instance->vco_saw_coeffs, out, instance->buf[0], out, n);
			break;
		case 2:
			bw_osc_pulse_process(&instance->vco3_pulse_coeffs, out, instance->buf[0], out, n);
			break;
		default:
			bw_osc_tri_process(&instance->vco3_tri_coeffs, out, instance->buf[0], out, n);
			break;
		}

		// noise generator
		
		bw_noise_gen_process(&instance->noise_gen_coeffs, instance->buf[0], n);
		if (instance->noise_color_cur == 2)
			bw_pink_filt_process(&instance->pink_filt_coeffs, &instance->pink_filt_state, instance->buf[0], instance->buf[0], n);
		bw_buf_scale(instance->buf[0], 5.f, instance->buf[0], n);

		// modulation signals

		for (int j = 0; j < n; j++)
			instance->buf[1][j] = instance->mod_wheel * (out[j] + instance->modulation_mix * (instance->buf[0][j] - out[j]));
		if (sync)
			instance->mod_k = instance->buf[1][0];

		// osc 1

		bw_buf_scale(instance->buf[1], instance->vco1_modulation, instance->buf[2], n);
		bw_phase_gen_process(&instance->vco1_phase_gen_coeffs, &instance->vco1_phase_gen_state, instance->buf[2], instance->buf[2], instance->buf[3], n);
		switch (instance->vco1_waveform_cur) {
		case 1:
			bw_osc_saw_process(&instance->vco_saw_coeffs, instance->buf[2], instance->buf[3], instance->buf[2], n);
			break;
		case 2:
			bw_osc_pulse_process(&instance->vco1_pulse_coeffs, instance->buf[2], instance->buf[3], instance->buf[2], n);
			break;
		default:
			bw_osc_tri_process(&instance->vco1_tri_coeffs, instance->buf[2], instance->buf[3], instance->buf[2], n);
			break;
		}

		// osc 2

		bw_buf_scale(instance->buf[1], instance->vco2_modulation, instance->buf[1], n);
		bw_phase_gen_process(&instance->vco2_phase_gen_coeffs, &instance->vco2_phase_gen_state, instance->buf[1], instance->buf[1], instance->buf[3], n);
		switch (instance->vco2_waveform_cur) {
		case 1:
			bw_osc_saw_process(&instance->vco_saw_coeffs, instance->buf[1], instance->buf[3], instance->buf[1], n);
			break;
		case 2:
			bw_osc_pulse_process(&instance->vco2_pulse_coeffs, instance->buf[1], instance->buf[3], instance->buf[1], n);
			break;
		default:
			bw_osc_tri_process(&instance->vco2_tri_coeffs, instance->buf[1], instance->buf[3], instance->buf[1], n);
			break;
		}

		// mixer

		bw_gain_process(&instance->vco1_gain_coeffs, instance->buf[2], instance->buf[2], n);
		bw_gain_process(&instance->vco2_gain_coeffs, instance->buf[1], instance->buf[1], n);
		bw_gain_process(&instance->vco3_gain_coeffs, out, out, n);
		bw_gain_process(&instance->noise_gain_coeffs, instance->buf[0], instance->buf[0], n);
		bw_buf_mix(out, instance->buf[1], out, n);
		bw_buf_mix(out, instance->buf[2], out, n);

		bw_osc_filt_process(&instance->osc_filt_state, out, out, n);

		const float k = instance->noise_color_cur == 2
			? 6.f * bw_noise_gen_get_scaling_k(&instance->noise_gen_coeffs) * bw_pink_filt_get_scaling_k(&instance->pink_filt_coeffs)
			: 0.1f * bw_noise_gen_get_scaling_k(&instance->noise_gen_coeffs);
		bw_buf_scale(instance->buf[0], k, instance->buf[0], n);
		bw_buf_mix(out, instance->buf[0], out, n);

		// vcf

		bw_env_gen_process(&instance->vcf_env_gen_coeffs, &instance->vcf_env_gen_state, instance->gate, NULL, n);
		if (sync)
			instance->vcf_env_k = bw_env_gen_get_y_z1(&instance->vcf_env_gen_state);
		const float cutoff_unmapped = 0.1447648273010839f * bw_logf(0.05f * instance->vcf_cutoff);
		const float cutoff_vpos = cutoff_unmapped + instance->vcf_contour * instance->vcf_env_k + 0.3f * instance->vcf_modulation * instance->mod_k;
		float cutoff = 20.f * bw_expf(6.907755278982137 * cutoff_vpos);
		switch (instance->vcf_kbd_ctrl) {
		case 2: // 1/3
			cutoff *= bw_pow2f((0.629960524947437f * 8.333333333333333e-2f) * (instance->note - 60));
			break;
		case 3: // 2/3
			cutoff *= bw_pow2f((0.793700525984100f * 8.333333333333333e-2f) * (instance->note - 60));
			break;
		case 4: // full
			cutoff *= bw_pow2f(8.333333333333333e-2f * (instance->note - 60));
			break;
		default: // off, do nothing
			break;
		}
		bw_svf_set_cutoff(&instance->vcf_coeffs, bw_clipf(cutoff, 20.f, 20e3f));
		bw_svf_process(&instance->vcf_coeffs, &instance->vcf_state, out, out, NULL, NULL, n);

		// vca

		bw_env_gen_process(&instance->vca_env_gen_coeffs, &instance->vca_env_gen_state, instance->gate, instance->buf[0], n);
		bw_buf_mul(out, instance->buf[0], out, n);

		// A 440 Hz osc

		if (instance->a440) { // nobody will notice or care about phase issues here
			bw_phase_gen_process(&instance->a440_phase_gen_coeffs, &instance->a440_phase_gen_state, NULL, instance->buf[0], NULL, n);
			bw_osc_sin_process(instance->buf[0], instance->buf[0], n);
			bw_buf_mix(out, instance->buf[0], out, n);
		}

		// output
		
		bw_gain_process(&instance->gain_coeffs, out, out, n);
		bw_ppm_process(&instance->ppm_coeffs, &instance->ppm_state, out, NULL, n);

		instance->sync_left -= n;
		if (instance->sync_left == 0)
			instance->sync_left = instance->sync_count;

		i += n;
	}
}

static void update_note_gate(plugin *instance) {
	for (int i = 0; i < 128; i++)
		if (instance->notes_pressed[i]) {
			instance->note = i;
			instance->gate = 1;
			return;
		}
	instance->gate = 0;
}

static void note_on(plugin *instance, char note) {
	instance->notes_pressed[(int)note] = 1;
	update_note_gate(instance);
}

static void note_off(plugin *instance, char note) {
	if (instance->notes_pressed[(int)note]) {
		instance->notes_pressed[(int)note] = 0;
		update_note_gate(instance);
	}
}

static void plugin_midi_msg_in(plugin *instance, size_t index, const uint8_t * data) {
	(void)index;
	switch (data[0] & 0xf0) {
	case 0x90: // note on
		if (data[2] == 0) // no, note off actually
			note_off(instance, data[1]);
		else
			note_on(instance, data[1]);
		break;
	case 0x80: // note off
		note_off(instance, data[1]);
		break;
	case 0xe0: // pitch bend
	{
		const uint16_t v = (data[2] << 7) | data[1];
		instance->pitch_bend = 2.f * bw_maxf((1.f / 16383.f) * (v - 0x2000), -1.f) - 1.f;
		break;
	}
	case 0xb0: // control change
		if (data[1] == 1) // mod wheel
			instance->mod_wheel = (1.f / 127.f) * data[2];
		break;
	}
}
