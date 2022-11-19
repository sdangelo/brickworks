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

#include <bw_vol.h>

#include <bw_inline_slew_lim.h>

void bw_vol_init(bw_vol *instance) {
	instance->volume = 1.f;
}

void bw_vol_set_sample_rate(bw_vol *instance, float sample_rate) {
	instance->max_var = bw_inline_slew_lim_get_max_var(sample_rate, 1.f / 0.05f);
}

void bw_vol_reset(bw_vol *instance) {
	instance->first_run = 1;
}

void bw_vol_process(bw_vol *instance, const float **x, float **y, int n_channels, int n_samples) {
	if (instance->first_run) {
		instance->volume_cur = instance->volume;
		instance->first_run = 0;
	}

	for (int i = 0; i < n_samples; i += _BW_VOL_BUFFER_SIZE) {
		const uint32_t n = bw_minu32(n_samples - i, _BW_VOL_BUFFER_SIZE);

		for (int j = 0; j < _BW_VOL_BUFFER_SIZE; j++) {
			instance->volume_cur = bw_inline_slew_lim(instance->volume, instance->volume_cur, instance->max_var, instance->max_var);
			instance->buf[j] = instance->volume_cur * instance->volume_cur * instance->volume_cur;
		}

		for (int j = 0; j < n_channels; j++) {
			const float *in = x[j] + i;
			float *out = y[j] + i;
			for (int k = 0; k < _BW_VOL_BUFFER_SIZE; k++)
				out[k] = instance->buf[k] * in[k];
		}
	}
}

void bw_vol_set_volume(bw_vol *instance, float value) {
	instance->volume = value;
}
