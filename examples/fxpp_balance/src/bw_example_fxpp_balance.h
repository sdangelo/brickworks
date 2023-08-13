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

#ifndef _BW_EXAMPLE_FXPP_BALANCE_H
#define _BW_EXAMPLE_FXPP_BALANCE_H

#include "platform.h"

#include <bw_balance.h>
#include <bw_ppm.h>

using namespace Brickworks;

extern "C" {

enum {
	p_balance,
	p_n
};

struct _bw_example_fxpp_balance {
	// Sub-components
	Balance<1>	balance;
	PPM<2>		ppm;

	// Parameters
	float		params[p_n];
};
typedef struct _bw_example_fxpp_balance bw_example_fxpp_balance;

void bw_example_fxpp_balance_init(bw_example_fxpp_balance *instance);
void bw_example_fxpp_balance_set_sample_rate(bw_example_fxpp_balance *instance, float sample_rate);
void bw_example_fxpp_balance_reset(bw_example_fxpp_balance *instance);
void bw_example_fxpp_balance_process(bw_example_fxpp_balance *instance, const float** x, float** y, int n_samples);
void bw_example_fxpp_balance_set_parameter(bw_example_fxpp_balance *instance, int index, float value);
float bw_example_fxpp_balance_get_parameter(bw_example_fxpp_balance *instance, int index);

}

#endif
