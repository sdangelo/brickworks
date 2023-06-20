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

#ifndef _BW_EXAMPLE_FXPP_WAH_H
#define _BW_EXAMPLE_FXPP_WAH_H

#include <bwpp_wah.h>

using namespace Brickworks;

extern "C" {

enum {
	p_wah,
	p_n
};

struct _bw_example_fxpp_wah {
	// Sub-components
	Wah<1>	wah;

	// Parameters
	float	params[p_n];
};
typedef struct _bw_example_fxpp_wah bw_example_fxpp_wah;

void bw_example_fxpp_wah_init(bw_example_fxpp_wah *instance);
void bw_example_fxpp_wah_set_sample_rate(bw_example_fxpp_wah *instance, float sample_rate);
void bw_example_fxpp_wah_reset(bw_example_fxpp_wah *instance);
void bw_example_fxpp_wah_process(bw_example_fxpp_wah *instance, const float** x, float** y, int n_samples);
void bw_example_fxpp_wah_set_parameter(bw_example_fxpp_wah *instance, int index, float value);
float bw_example_fxpp_wah_get_parameter(bw_example_fxpp_wah *instance, int index);

}

#endif
