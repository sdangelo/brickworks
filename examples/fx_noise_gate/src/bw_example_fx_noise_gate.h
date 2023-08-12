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

#ifndef _BW_EXAMPLE_FX_NOISE_GATE_H
#define _BW_EXAMPLE_FX_NOISE_GATE_H

#include "platform.h"

#include <bw_noise_gate.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	p_thresh,
	p_attenuation,
	p_attack,
	p_release,
	p_n
};

struct _bw_example_fx_noise_gate {
	// Sub-noise_gateonents
	bw_noise_gate_coeffs	noise_gate_coeffs;
	bw_noise_gate_state	noise_gate_state;

	// Parameters
	float			params[p_n];
};
typedef struct _bw_example_fx_noise_gate bw_example_fx_noise_gate;

void bw_example_fx_noise_gate_init(bw_example_fx_noise_gate *instance);
void bw_example_fx_noise_gate_set_sample_rate(bw_example_fx_noise_gate *instance, float sample_rate);
void bw_example_fx_noise_gate_reset(bw_example_fx_noise_gate *instance);
void bw_example_fx_noise_gate_process(bw_example_fx_noise_gate *instance, const float** x, float** y, int n_samples);
void bw_example_fx_noise_gate_set_parameter(bw_example_fx_noise_gate *instance, int index, float value);
float bw_example_fx_noise_gate_get_parameter(bw_example_fx_noise_gate *instance, int index);

#ifdef __cplusplus
}
#endif

#endif
