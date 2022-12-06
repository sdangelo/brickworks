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
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File authors: Stefano D'Angelo
 */

#ifndef _CONFIG_H
#define _CONFIG_H

// Definitions

#define IO_MONO			1
#define IO_STEREO		(1<<1)

struct config_io_bus {
	const char	*name;
	char		 out;
	char		 aux;
	char		 cv;
	char		 configs;
};

struct config_parameter {
	const char	*name;
	const char	*shortName;
	const char	*units;
	char		 out;
	char		 bypass;
	int		 steps;
	float		 defaultValueUnmapped;
};

// Data

#define COMPANY_NAME		"Orastron"
#define COMPANY_WEBSITE		"https://www.orastron.com/"
#define COMPANY_MAILTO		"mailto:info@orastron.com"

#define PLUGIN_NAME		"bw_example_synth_mono"
#define PLUGIN_VERSION		"0.2.0"

#define NUM_BUSES_IN		0
#define NUM_BUSES_OUT		1
#define NUM_CHANNELS_IN		0
#define NUM_CHANNELS_OUT	1

static struct config_io_bus config_buses_out[NUM_BUSES_OUT] = {
	{ "Audio out", 1, 0, 0, IO_MONO }
};

#define NUM_PARAMETERS		39

static struct config_parameter config_parameters[NUM_PARAMETERS] = {
	{ "Volume", "Volume", "", 0, 0, 0, 0.5f },
	{ "Master tune", "Master tune", "st", 0, 0, 0, 0.5f },
	{ "Portamento", "Portamento", "s", 0, 0, 0, 0.f },
	{ "Modulation mix", "Mod mix", "%", 0, 0, 0, 0.f },
	{ "VCO1 modulation", "VCO1 mod", "%", 0, 0, 0, 0.f },
	{ "VCO1 coarse", "VCO1 coarse", "", 0, 0, 6, 0.5f },
	{ "VCO1 fine", "VCO1 fine", "st", 0, 0, 0, 0.5f },
	{ "VCO1 waveform", "VCO1 wave", "", 0, 0, 2, 0.f },
	{ "VCO1 pulse width/slope", "VCO1 pw/slope", "%", 0, 0, 0, 0.5f },
	{ "VCO1 volume", "VCO1 volume", "%", 0, 0, 0, 1.f },
	{ "VCO2 modulation", "VCO2 mod", "%", 0, 0, 0, 0.f },
	{ "VCO2 coarse", "VCO2 coarse", "", 0, 0, 6, 0.5f },
	{ "VCO2 fine", "VCO2 fine", "st", 0, 0, 0, 0.5f },
	{ "VCO2 waveform", "VCO2 wave", "", 0, 0, 2, 0.f },
	{ "VCO2 pulse width/slope", "VCO2 pw/slope", "%", 0, 0, 0, 0.5f },
	{ "VCO2 volume", "VCO2 volume", "%", 0, 0, 0, 0.f },
	{ "VCO3 kyboard control", "VCO3 kbd ctrl", "", 0, 0, 1, 1.f },
	{ "VCO3 coarse", "VCO3 coarse", "", 0, 0, 6, 0.5f },
	{ "VCO3 fine", "VCO3 fine", "st", 0, 0, 0, 0.5f },
	{ "VCO3 waveform", "VCO3 wave", "", 0, 0, 2, 0.f },
	{ "VCO3 pulse width/slope", "VCO3 pw/slope", "%", 0, 0, 0, 0.5f },
	{ "VCO3 volume", "VCO3 volume", "%", 0, 0, 0, 0.f },
	{ "Noise color", "Noise color", "", 0, 0, 1, 0.f },
	{ "Noise volume", "Noise volume", "%", 0, 0, 0, 0.f },
	{ "VCF modulation", "VCF mod", "%", 0, 0, 0, 0.f },
	{ "VCF keyboard control", "VCF kbd ctrl", "", 0, 0, 3, 0.f },
	{ "VCF cutoff", "VCF cutoff", "Hz", 0, 0, 0, 1.f },
	{ "VCF Q", "VCF Q", "", 0, 0, 0, 0.f },
	{ "VCF contour", "VCF contour", "%", 0, 0, 0, 0.f },
	{ "VCF attack", "VCF attack", "s", 0, 0, 0, 0.f },
	{ "VCF decay", "VCF decay", "s", 0, 0, 0, 0.f },
	{ "VCF sustain", "VCF sustain", "%", 0, 0, 0, 1.f },
	{ "VCF release", "VCF release", "s", 0, 0, 0, 0.f },
	{ "VCA attack", "VCA attack", "s", 0, 0, 0, 0.f },
	{ "VCA decay", "VCA decay", "s", 0, 0, 0, 0.f },
	{ "VCA sustain", "VCA sustain", "%", 0, 0, 0, 1.f },
	{ "VCA release", "VCA release", "s", 0, 0, 0, 0.f },
	{ "A440", "A440", "", 0, 0, 1, 0.f },
	{ "Level", "Level", "", 1, 0, 0, 0.f }
};

// Internal API

#include "bw_example_synth_mono.h"

#define P_TYPE				bw_example_synth_mono
#define P_NEW				bw_example_synth_mono_new
#define P_FREE				bw_example_synth_mono_free
#define P_SET_SAMPLE_RATE		bw_example_synth_mono_set_sample_rate
#define P_RESET				bw_example_synth_mono_reset
#define P_PROCESS			bw_example_synth_mono_process
#define P_SET_PARAMETER			bw_example_synth_mono_set_parameter
#define P_GET_PARAMETER			bw_example_synth_mono_get_parameter
#define P_NOTE_ON			bw_example_synth_mono_note_on
#define P_NOTE_OFF			bw_example_synth_mono_note_off
#define P_PITCH_BEND			bw_example_synth_mono_pitch_bend
#define P_MOD_WHEEL			bw_example_synth_mono_mod_wheel

#endif
