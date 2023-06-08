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

#ifndef _BW_EXAMPLE_SYNTH_POLY_H
#define _BW_EXAMPLE_SYNTH_POLY_H

#ifdef __cplusplus
extern "C" {
#endif

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

enum {
	p_volume,
	p_master_tune,
	p_portamento,
	p_mod_mix,
	p_vco1_mod,
	p_vco1_coarse,
	p_vco1_fine,
	p_vco1_waveform,
	p_vco1_pw_slope,
	p_vco1_level,
	p_vco2_mod,
	p_vco2_coarse,
	p_vco2_fine,
	p_vco2_waveform,
	p_vco2_pw_slope,
	p_vco2_level,
	p_vco3_kbd,
	p_vco3_coarse,
	p_vco3_fine,
	p_vco3_waveform,
	p_vco3_pw_slope,
	p_vco3_level,
	p_noise_color,
	p_noise_level,
	p_vcf_mod,
	p_vcf_kbd_ctrl,
	p_vcf_cutoff,
	p_vcf_Q,
	p_vcf_contour,
	p_vcf_attack,
	p_vcf_decay,
	p_vcf_sustain,
	p_vcf_release,
	p_vca_attack,
	p_vca_decay,
	p_vca_sustain,
	p_vca_release,
	p_a440,
	p_n
};

#define BUFFER_SIZE 	32
#define N_VOICES	8

struct _bw_example_synth_poly_voice {
	bw_phase_gen_coeffs	vco1_phase_gen_coeffs;
	bw_phase_gen_coeffs	vco2_phase_gen_coeffs;
	bw_phase_gen_coeffs	vco3_phase_gen_coeffs;
	bw_svf_coeffs		vcf_coeffs;

	bw_phase_gen_state	vco1_phase_gen_state;
	bw_phase_gen_state	vco2_phase_gen_state;
	bw_phase_gen_state	vco3_phase_gen_state;
	bw_osc_filt_state	osc_filt_state;
	bw_pink_filt_state	pink_filt_state;
	bw_env_gen_state	vcf_env_gen_state;
	bw_svf_state		vcf_state;
	bw_env_gen_state	vca_env_gen_state;

	unsigned char		note;
	char			gate;
	
	float			buf[5][BUFFER_SIZE];
};
typedef struct _bw_example_synth_poly_voice bw_example_synth_poly_voice;

struct _bw_example_synth_poly {
	// Sub-components
	bw_note_queue			note_queue;
	bw_osc_saw_coeffs		vco_saw_coeffs;
	bw_osc_pulse_coeffs		vco1_pulse_coeffs;
	bw_osc_tri_coeffs		vco1_tri_coeffs;
	bw_gain_coeffs			vco1_gain_coeffs;
	bw_osc_pulse_coeffs		vco2_pulse_coeffs;
	bw_osc_tri_coeffs		vco2_tri_coeffs;
	bw_gain_coeffs			vco2_gain_coeffs;
	bw_osc_pulse_coeffs		vco3_pulse_coeffs;
	bw_osc_tri_coeffs		vco3_tri_coeffs;
	bw_gain_coeffs			vco3_gain_coeffs;
	bw_noise_gen_coeffs		noise_gen_coeffs;
	bw_pink_filt_coeffs		pink_filt_coeffs;
	bw_gain_coeffs			noise_gain_coeffs;
	bw_env_gen_coeffs		vcf_env_gen_coeffs;
	bw_env_gen_coeffs		vca_env_gen_coeffs;
	bw_phase_gen_coeffs		a440_phase_gen_coeffs;
	bw_phase_gen_state		a440_phase_gen_state;
	bw_gain_coeffs			gain_coeffs;
	bw_ppm_coeffs			ppm_coeffs;
	bw_ppm_state			ppm_state;

	bw_example_synth_poly_voice	voices[N_VOICES];

	// Parameters
	float				params[p_n];

	// States
	uint64_t			rand_state;
	float				pitch_bend;
	float				mod_wheel;
	
	// Buffers
	float				buf[BUFFER_SIZE];
};
typedef struct _bw_example_synth_poly bw_example_synth_poly;

void bw_example_synth_poly_init(bw_example_synth_poly *instance);
void bw_example_synth_poly_set_sample_rate(bw_example_synth_poly *instance, float sample_rate);
void bw_example_synth_poly_reset(bw_example_synth_poly *instance);
void bw_example_synth_poly_process(bw_example_synth_poly *instance, const float** x, float** y, int n_samples);
void bw_example_synth_poly_set_parameter(bw_example_synth_poly *instance, int index, float value);
float bw_example_synth_poly_get_parameter(bw_example_synth_poly *instance, int index);
void bw_example_synth_poly_note_on(bw_example_synth_poly *instance, char note, char velocity);
void bw_example_synth_poly_note_off(bw_example_synth_poly *instance, char note);
void bw_example_synth_poly_pitch_bend(bw_example_synth_poly *instance, int value);
void bw_example_synth_poly_mod_wheel(bw_example_synth_poly *instance, char value);

#ifdef __cplusplus
}
#endif

#endif
