/*
 * Brickworks
 *
 * Copyright (C) 2022 Orastron Srl unipersonale
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
 *
 * File author: Stefano D'Angelo
 */

#include <bw_osc_filt.h>

#include <bw_common.h>

void bw_osc_filt_init(bw_osc_filt *instance) {
	instance->enabled = 1;
}

void bw_osc_filt_reset(bw_osc_filt *instance) {
	instance->x_z1 = 0.f;
	instance->y_z1 = 0.f;
}

void bw_osc_filt_process(bw_osc_filt *instance, const float *x, float* y, int n_samples) {
	if (instance->enabled)
		for (int i = 0; i < n_samples; i++) {
			const float v = 1.371308261611209f * x[i] + 0.08785458027104826f * instance->x_z1 - 4.591628418822578e-1f * instance->y_z1;
			instance->x_z1 = x[i];
			instance->y_z1 = y[i];
			y[i] = v;
		}
	else {
		for (int i = 0; i < n_samples; i++)
			y[i] = x[i];
		instance->x_z1 = instance->y_z1 = x[n_samples - 1];
	}

}

void bw_osc_filt_set_enabled(bw_osc_filt *instance, char value) {
	instance->enabled = value;
}
