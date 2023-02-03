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
 * File author: Stefano D'Angelo
 */

#ifndef _BW_EXAMPLE_FX_BITCRUSH_H
#define _BW_EXAMPLE_FX_BITCRUSH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_sr_reduce.h>
#include <bw_bd_reduce.h>

enum {
	p_sr_ratio,
	p_bit_depth,
	p_n
};

struct _bw_example_fx_bitcrush {
	// Sub-components
	bw_sr_reduce_coeffs	sr_reduce_coeffs;
	bw_sr_reduce_state	sr_reduce_state;
	bw_bd_reduce_coeffs	bd_reduce_coeffs;

	// Parameters
	float			params[p_n];
};
typedef struct _bw_example_fx_bitcrush bw_example_fx_bitcrush;

void bw_example_fx_bitcrush_init(bw_example_fx_bitcrush *instance);
void bw_example_fx_bitcrush_set_sample_rate(bw_example_fx_bitcrush *instance, float sample_rate);
void bw_example_fx_bitcrush_reset(bw_example_fx_bitcrush *instance);
void bw_example_fx_bitcrush_process(bw_example_fx_bitcrush *instance, const float** x, float** y, int n_samples);
void bw_example_fx_bitcrush_set_parameter(bw_example_fx_bitcrush *instance, int index, float value);
float bw_example_fx_bitcrush_get_parameter(bw_example_fx_bitcrush *instance, int index);

#ifdef __cplusplus
}
#endif

#endif
