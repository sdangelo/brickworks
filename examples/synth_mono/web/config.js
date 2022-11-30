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
 *
 * File author: Stefano D'Angelo
 */

var buses = [
	{
		stereo:		false,
		output:		true
	}
];

var parameters = [
	{
		name:		"Volume",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"Master tune",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"Portamento",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"Modulation mix",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCO1 modulation",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCO1 coarse",
		output:		false,
		defaultValue:	0.5,
		step:		6
	},
	{
		name:		"VCO1 fine",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"VCO1 waveform",
		output:		false,
		defaultValue:	0.5,
		step:		2
	},
	{
		name:		"VCO1 pulse width/slope",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"VCO1 volume",
		output:		false,
		defaultValue:	1.0
	},
	{
		name:		"VCO2 modulation",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCO2 coarse",
		output:		false,
		defaultValue:	0.5,
		step:		6
	},
	{
		name:		"VCO2 fine",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"VCO2 waveform",
		output:		false,
		defaultValue:	0.5,
		step:		2
	},
	{
		name:		"VCO2 pulse width/slope",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"VCO2 volume",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCO3 keyboard control",
		output:		false,
		defaultValue:	1.0,
		step:		1
	},
	{
		name:		"VCO3 coarse",
		output:		false,
		defaultValue:	0.5,
		step:		6
	},
	{
		name:		"VCO3 fine",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"VCO3 waveform",
		output:		false,
		defaultValue:	0.5,
		step:		2
	},
	{
		name:		"VCO3 pulse width/slope",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"VCO3 volume",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"Noise color",
		output:		false,
		defaultValue:	0.0,
		step:		1
	},
	{
		name:		"Noise volume",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCF modulation",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCF keyboard control",
		output:		false,
		defaultValue:	0.0,
		step:		3
	},
	{
		name:		"VCF cutoff",
		output:		false,
		defaultValue:	1.0
	},
	{
		name:		"VCF Q",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCF contour",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCF attack",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCF decay",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCF sustain",
		output:		false,
		defaultValue:	1.0
	},
	{
		name:		"VCF release",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCA dttack",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCA decay",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"VCA sustain",
		output:		false,
		defaultValue:	1.0
	},
	{
		name:		"VCA release",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"A440",
		output:		false,
		defaultValue:	0.0,
		step:		1
	},
	{
		name:		"Level",
		output:		true,
		defaultValue:	0.0
	}
];
