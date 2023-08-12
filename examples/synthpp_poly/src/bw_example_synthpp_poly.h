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

#ifndef _BW_EXAMPLE_SYNTHPP_MONO_H
#define _BW_EXAMPLE_SYNTHPP_MONO_H

#include "platform.h"

#include <bw_note_queue.h>
#include <bwpp_phase_gen.h>
#include <bwpp_osc_saw.h>
#include <bwpp_osc_pulse.h>
#include <bwpp_osc_tri.h>
#include <bwpp_osc_sin.h>
#include <bwpp_osc_filt.h>
#include <bwpp_noise_gen.h>
#include <bwpp_pink_filt.h>
#include <bwpp_svf.h>
#include <bw_env_gen.h>
#include <bw_gain.h>
#include <bwpp_ppm.h>

using namespace Brickworks;

extern "C" {

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

#define BUFFER_SIZE	32
#define N_VOICES	8

struct _bw_example_synthpp_poly;
typedef struct _bw_example_synthpp_poly bw_example_synthpp_poly;

struct _bw_example_synthpp_poly_voice {
	PhaseGen<1>		 vco1PhaseGen;
	PhaseGen<1>		 vco2PhaseGen;
	PhaseGen<1>		 vco3PhaseGen;
	SVF<1>			 vcf;

	unsigned char		 note;
	char			 gate;

	float			 buf[5][BUFFER_SIZE];

	bw_example_synthpp_poly	*instance;
	int			 index;
};
typedef struct _bw_example_synthpp_poly_voice bw_example_synthpp_poly_voice;

struct _bw_example_synthpp_poly {
	// Sub-components
	NoteQueue			noteQueue;
	OscSaw<N_VOICES>		vco1OscSaw;
	OscPulse<N_VOICES>		vco1OscPulse;
	OscTri<N_VOICES>		vco1OscTri;
	Gain<N_VOICES>			vco1Gain;
	OscSaw<N_VOICES>		vco2OscSaw;
	OscPulse<N_VOICES>		vco2OscPulse;
	OscTri<N_VOICES>		vco2OscTri;
	Gain<N_VOICES>			vco2Gain;
	OscSaw<N_VOICES>		vco3OscSaw;
	OscPulse<N_VOICES>		vco3OscPulse;
	OscTri<N_VOICES>		vco3OscTri;
	Gain<N_VOICES>			vco3Gain;
	OscFilt<N_VOICES>		oscFilt;
	NoiseGen<N_VOICES>		noiseGen;
	PinkFilt<N_VOICES>		pinkFilt;
	Gain<N_VOICES>			noiseGain;
	EnvGen<N_VOICES>		vcfEnvGen;
	EnvGen<N_VOICES>		vcaEnvGen;
	PhaseGen<1>			a440PhaseGen;
	Gain<1>				gain;
	PPM<1>				ppm;
	
	bw_example_synthpp_poly_voice	voices[N_VOICES];

	// Parameters
	float				params[p_n];

	// States
	uint64_t			rand_state;
	float				pitch_bend;
	float				mod_wheel;
	
	// Buffers
	float				buf[BUFFER_SIZE];

	_bw_example_synthpp_poly() : noiseGen(&rand_state) {}
};

void bw_example_synthpp_poly_init(bw_example_synthpp_poly *instance);
void bw_example_synthpp_poly_set_sample_rate(bw_example_synthpp_poly *instance, float sample_rate);
void bw_example_synthpp_poly_reset(bw_example_synthpp_poly *instance);
void bw_example_synthpp_poly_process(bw_example_synthpp_poly *instance, const float** x, float** y, int n_samples);
void bw_example_synthpp_poly_set_parameter(bw_example_synthpp_poly *instance, int index, float value);
float bw_example_synthpp_poly_get_parameter(bw_example_synthpp_poly *instance, int index);
void bw_example_synthpp_poly_note_on(bw_example_synthpp_poly *instance, char note, char velocity);
void bw_example_synthpp_poly_note_off(bw_example_synthpp_poly *instance, char note);
void bw_example_synthpp_poly_pitch_bend(bw_example_synthpp_poly *instance, int value);
void bw_example_synthpp_poly_mod_wheel(bw_example_synthpp_poly *instance, char value);

}

#endif
