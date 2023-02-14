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

#ifndef _BW_EXAMPLE_FX_BALANCE_H
#define _BW_EXAMPLE_FX_BALANCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_balance.h>
#include <bw_ppm.h>
#include <bw_math.h>

enum {
	p_balance,
	p_n
};

struct _bw_example_fx_balance {
	// Sub-components
	bw_balance_coeffs	balance_coeffs;
	bw_ppm_coeffs		ppm_coeffs;
	bw_ppm_state		ppm_l_state;
	bw_ppm_state		ppm_r_state;

	// Parameters
	float		params[p_n];
};
typedef struct _bw_example_fx_balance bw_example_fx_balance;

void bw_example_fx_balance_init(bw_example_fx_balance *instance);
void bw_example_fx_balance_set_sample_rate(bw_example_fx_balance *instance, float sample_rate);
void bw_example_fx_balance_reset(bw_example_fx_balance *instance);
void bw_example_fx_balance_process(bw_example_fx_balance *instance, const float** x, float** y, int n_samples);
void bw_example_fx_balance_set_parameter(bw_example_fx_balance *instance, int index, float value);
float bw_example_fx_balance_get_parameter(bw_example_fx_balance *instance, int index);

#ifdef __cplusplus
}
#endif

#endif