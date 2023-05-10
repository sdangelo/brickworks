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

#ifndef _BW_EXAMPLE_FX_DRIVE_H
#define _BW_EXAMPLE_FX_DRIVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_drive.h>
#include <bw_src_int.h>

enum {
	p_drive,
	p_tone,
	p_volume,
	p_n
};

#define BUF_SIZE	32

struct _bw_example_fx_drive {
	// Sub-components
	bw_drive_coeffs		drive_coeffs;
	bw_drive_state		drive_state;
	bw_src_int_coeffs	src_up_coeffs;
	bw_src_int_state	src_up_state;
	bw_src_int_coeffs	src_down_coeffs;
	bw_src_int_state	src_down_state;

	// Parameters
	float		params[p_n];

	// Buffers
	float		buf[BUF_SIZE];
};
typedef struct _bw_example_fx_drive bw_example_fx_drive;

void bw_example_fx_drive_init(bw_example_fx_drive *instance);
void bw_example_fx_drive_set_sample_rate(bw_example_fx_drive *instance, float sample_rate);
void bw_example_fx_drive_reset(bw_example_fx_drive *instance);
void bw_example_fx_drive_process(bw_example_fx_drive *instance, const float** x, float** y, int n_samples);
void bw_example_fx_drive_set_parameter(bw_example_fx_drive *instance, int index, float value);
float bw_example_fx_drive_get_parameter(bw_example_fx_drive *instance, int index);

#ifdef __cplusplus
}
#endif

#endif
