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

#ifndef _BW_EXAMPLE_FXPP_SATUR_H
#define _BW_EXAMPLE_FXPP_SATUR_H

#include <bwpp_satur.h>
#include <bwpp_src_int.h>

using namespace Brickworks;

extern "C" {

enum {
	p_bias,
	p_gain,
	p_n
};

#define BUF_SIZE	32

struct _bw_example_fxpp_satur {
	// Sub-components
	Satur<1>	satur;
	SRCInt<1>	srcUp;
	SRCInt<1>	srcDown;

	// Parameters
	float		params[p_n];

	// Buffers
	float		buf[BUF_SIZE];

	_bw_example_fxpp_satur() : srcUp(2), srcDown(-2) {}
};
typedef struct _bw_example_fxpp_satur bw_example_fxpp_satur;

void bw_example_fxpp_satur_init(bw_example_fxpp_satur *instance);
void bw_example_fxpp_satur_set_sample_rate(bw_example_fxpp_satur *instance, float sample_rate);
void bw_example_fxpp_satur_reset(bw_example_fxpp_satur *instance);
void bw_example_fxpp_satur_process(bw_example_fxpp_satur *instance, const float** x, float** y, int n_samples);
void bw_example_fxpp_satur_set_parameter(bw_example_fxpp_satur *instance, int index, float value);
float bw_example_fxpp_satur_get_parameter(bw_example_fxpp_satur *instance, int index);

}

#endif
