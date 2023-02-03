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

#define PLUGIN_NAME		"bw_example_fx_comp"
#define PLUGIN_VERSION		"0.3.0"

#define NUM_BUSES_IN		1
#define NUM_BUSES_OUT		1
#define NUM_CHANNELS_IN		1
#define NUM_CHANNELS_OUT	1

static struct config_io_bus config_buses_in[NUM_BUSES_IN] = {
	{ "Audio in", 0, 0, 0, IO_MONO }
};

static struct config_io_bus config_buses_out[NUM_BUSES_OUT] = {
	{ "Audio out", 1, 0, 0, IO_MONO }
};

#define NUM_PARAMETERS		5

static struct config_parameter config_parameters[NUM_PARAMETERS] = {
	{ "Threshold", "Thresh", "", 0, 0, 0, 1.f },
	{ "Ratio", "Ratio", "", 0, 0, 0, 1.f },
	{ "Attack", "Attack", "s", 0, 0, 0, 0.f },
	{ "Release", "Release", "s", 0, 0, 0, 0.f },
	{ "Makeup gain", "Gain", "", 0, 0, 0, 0.f }
};

// Internal API

#include "bw_example_fx_comp.h"

#define P_TYPE				bw_example_fx_comp
#define P_INIT				bw_example_fx_comp_init
#define P_SET_SAMPLE_RATE		bw_example_fx_comp_set_sample_rate
#define P_RESET				bw_example_fx_comp_reset
#define P_PROCESS			bw_example_fx_comp_process
#define P_SET_PARAMETER			bw_example_fx_comp_set_parameter
#define P_GET_PARAMETER			bw_example_fx_comp_get_parameter

#endif
