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

#ifndef _BW_EXAMPLE_SYNTH_SIMPLE_H
#define _BW_EXAMPLE_SYNTH_SIMPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

#include <bw_math.h>
#include <bw_phase_gen.h>
#include <bw_osc_pulse.h>
#include <bw_osc_filt.h>
#include <bw_svf.h>
#include <bw_env_gen.h>
#include <bw_gain.h>
#include <bw_ppm.h>

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

#define BUFFER_SIZE 	128

struct _bw_example_synth_simple {
	// Sub-components
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

	// Parameters
	float			params[p_n];

	// States
	int			note;
	
	// Buffers
	float			buf[BUFFER_SIZE];
};
typedef struct _bw_example_synth_simple bw_example_synth_simple;

void bw_example_synth_simple_init(bw_example_synth_simple *instance);
void bw_example_synth_simple_set_sample_rate(bw_example_synth_simple *instance, float sample_rate);
void bw_example_synth_simple_reset(bw_example_synth_simple *instance);
void bw_example_synth_simple_process(bw_example_synth_simple *instance, const float** x, float** y, int n_samples);
void bw_example_synth_simple_set_parameter(bw_example_synth_simple *instance, int index, float value);
float bw_example_synth_simple_get_parameter(bw_example_synth_simple *instance, int index);
void bw_example_synth_simple_note_on(bw_example_synth_simple *instance, char note, char velocity);
void bw_example_synth_simple_note_off(bw_example_synth_simple *instance, char note);

#ifdef __cplusplus
}
#endif

#endif
