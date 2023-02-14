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

var buses = [
	{
		stereo:		false,
		output:		false
	},
	{
		stereo:		true,
		output:		true
	}
];

var parameters = [
	{
		name:		"Balance",
		output:		false,
		defaultValue:	0.5
	},
	{
		name:		"Left level",
		output:		true,
		defaultValue:	0.0
	},
	{
		name:		"Right level",
		output:		true,
		defaultValue:	0.0
	}
];
