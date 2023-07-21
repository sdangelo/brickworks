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

#ifndef _BW_EXAMPLE_FX_PHASER_H
#define _BW_EXAMPLE_FX_PHASER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

#include <bw_phaser.h>

enum {
	p_rate,
	p_center,
	p_amount,
	p_n
};

struct _bw_example_fx_phaser {
	// Sub-components
	bw_phaser_coeffs	phaser_coeffs;
	bw_phaser_state		phaser_state;

	// Parameters
	float			params[p_n];
};
typedef struct _bw_example_fx_phaser bw_example_fx_phaser;

void bw_example_fx_phaser_init(bw_example_fx_phaser *instance);
void bw_example_fx_phaser_set_sample_rate(bw_example_fx_phaser *instance, float sample_rate);
void bw_example_fx_phaser_reset(bw_example_fx_phaser *instance);
void bw_example_fx_phaser_process(bw_example_fx_phaser *instance, const float** x, float** y, int n_samples);
void bw_example_fx_phaser_set_parameter(bw_example_fx_phaser *instance, int index, float value);
float bw_example_fx_phaser_get_parameter(bw_example_fx_phaser *instance, int index);

#ifdef __cplusplus
}
#endif

#endif
