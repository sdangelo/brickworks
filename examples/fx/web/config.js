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
		output:		false
	},
	{
		stereo:		false,
		output:		true
	}
];

var parameters = [
	{
		name:		"Cutoff",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"Q",
		output:		false,
		defaultValue:	0.0
	},
	{
		name:		"Level",
		output:		true,
		defaultValue:	0.0
	}
];
