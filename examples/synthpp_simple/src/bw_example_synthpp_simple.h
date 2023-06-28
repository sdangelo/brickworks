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

#ifndef _BW_EXAMPLE_SYNTHPP_SIMPLE_H
#define _BW_EXAMPLE_SYNTHPP_SIMPLE_H

#include <bwpp_phase_gen.h>
#include <bwpp_osc_pulse.h>
#include <bwpp_osc_filt.h>
#include <bwpp_svf.h>
#include <bwpp_env_gen.h>
#include <bwpp_gain.h>
#include <bwpp_ppm.h>

using namespace Brickworks;

extern "C" {

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

#define BUFFER_SIZE	32

struct _bw_example_synthpp_simple {
	// Sub-components
	PhaseGen<1>	phaseGen;
	OscPulse<1>	oscPulse;
	OscFilt<1>	oscFilt;
	SVF<1>		svf;
	EnvGen<1>	envGen;
	Gain<1>		gain;
	PPM<1>		ppm;

	// Parameters
	float		params[p_n];

	// States
	int		note;
	
	// Buffers
	float		buf[BUFFER_SIZE];
};
typedef struct _bw_example_synthpp_simple bw_example_synthpp_simple;

void bw_example_synthpp_simple_init(bw_example_synthpp_simple *instance);
void bw_example_synthpp_simple_set_sample_rate(bw_example_synthpp_simple *instance, float sample_rate);
void bw_example_synthpp_simple_reset(bw_example_synthpp_simple *instance);
void bw_example_synthpp_simple_process(bw_example_synthpp_simple *instance, const float** x, float** y, int n_samples);
void bw_example_synthpp_simple_set_parameter(bw_example_synthpp_simple *instance, int index, float value);
float bw_example_synthpp_simple_get_parameter(bw_example_synthpp_simple *instance, int index);
void bw_example_synthpp_simple_note_on(bw_example_synthpp_simple *instance, char note, char velocity);
void bw_example_synthpp_simple_note_off(bw_example_synthpp_simple *instance, char note);

}

#endif
