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

#include <bwpp_phase_gen.h>
#include <bwpp_osc_saw.h>
#include <bwpp_osc_pulse.h>
#include <bwpp_osc_tri.h>
#include <bwpp_osc_sin.h>
#include <bwpp_osc_filt.h>
#include <bw_noise_gen.h>
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

struct _bw_example_synthpp_mono {
	// Sub-components
	PhaseGen<1>	vco1PhaseGen;
	OscSaw<1>	vco1OscSaw;
	OscPulse<1>	vco1OscPulse;
	OscTri<1>	vco1OscTri;
	Gain<1>		vco1Gain;
	PhaseGen<1>	vco2PhaseGen;
	OscSaw<1>	vco2OscSaw;
	OscPulse<1>	vco2OscPulse;
	OscTri<1>	vco2OscTri;
	Gain<1>		vco2Gain;
	PhaseGen<1>	vco3PhaseGen;
	OscSaw<1>	vco3OscSaw;
	OscPulse<1>	vco3OscPulse;
	OscTri<1>	vco3OscTri;
	Gain<1>		vco3Gain;
	OscFilt<1>	oscFilt;
	NoiseGen<1>	noiseGen;
	PinkFilt<1>	pinkFilt;
	Gain<1>		noiseGain;
	EnvGen<1>	vcfEnvGen;
	SVF<1>		vcf;
	EnvGen<1>	vcaEnvGen;
	PhaseGen<1>	a440PhaseGen;
	Gain<1>		gain;
	PPM<1>		ppm;

	// Parameters
	float		params[p_n];

	// States
	uint64_t	rand_state;
	int		note;
	char		gate;
	float		pitch_bend;
	float		mod_wheel;
	char		notes_pressed[128];
	
	// Buffers
	float		buf[4][BUFFER_SIZE];

	_bw_example_synthpp_mono() : noiseGen(&rand_state) {}
};
typedef struct _bw_example_synthpp_mono bw_example_synthpp_mono;

void bw_example_synthpp_mono_init(bw_example_synthpp_mono *instance);
void bw_example_synthpp_mono_set_sample_rate(bw_example_synthpp_mono *instance, float sample_rate);
void bw_example_synthpp_mono_reset(bw_example_synthpp_mono *instance);
void bw_example_synthpp_mono_process(bw_example_synthpp_mono *instance, const float** x, float** y, int n_samples);
void bw_example_synthpp_mono_set_parameter(bw_example_synthpp_mono *instance, int index, float value);
float bw_example_synthpp_mono_get_parameter(bw_example_synthpp_mono *instance, int index);
void bw_example_synthpp_mono_note_on(bw_example_synthpp_mono *instance, char note, char velocity);
void bw_example_synthpp_mono_note_off(bw_example_synthpp_mono *instance, char note);
void bw_example_synthpp_mono_pitch_bend(bw_example_synthpp_mono *instance, int value);
void bw_example_synthpp_mono_mod_wheel(bw_example_synthpp_mono *instance, char value);

}

#endif
