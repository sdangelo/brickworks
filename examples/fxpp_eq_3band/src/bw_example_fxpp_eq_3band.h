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

#ifndef _BW_EXAMPLE_FXPP_EQ_3BAND_H
#define _BW_EXAMPLE_FXPP_EQ_3BAND_H

#include "platform.h"

#include <bw_ls2.h>
#include <bw_hs2.h>
#include <bwpp_peak.h>

using namespace Brickworks;

extern "C" {

enum {
	p_ls_cutoff,
	p_ls_gain,
	p_ls_Q,
	p_peak_cutoff,
	p_peak_gain,
	p_peak_bw,
	p_hs_cutoff,
	p_hs_gain,
	p_hs_Q,
	p_n
};

struct _bw_example_fxpp_eq_3band {
	// Sub-components
	LS2<1>	ls2;
	Peak<1>	peak;
	HS2<1>	hs2;

	// Parameters
	float	params[p_n];
};
typedef struct _bw_example_fxpp_eq_3band bw_example_fxpp_eq_3band;

void bw_example_fxpp_eq_3band_init(bw_example_fxpp_eq_3band *instance);
void bw_example_fxpp_eq_3band_set_sample_rate(bw_example_fxpp_eq_3band *instance, float sample_rate);
void bw_example_fxpp_eq_3band_reset(bw_example_fxpp_eq_3band *instance);
void bw_example_fxpp_eq_3band_process(bw_example_fxpp_eq_3band *instance, const float** x, float** y, int n_samples);
void bw_example_fxpp_eq_3band_set_parameter(bw_example_fxpp_eq_3band *instance, int index, float value);
float bw_example_fxpp_eq_3band_get_parameter(bw_example_fxpp_eq_3band *instance, int index);

}

#endif
