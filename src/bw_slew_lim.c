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

#include <bw_slew_lim.h>

#include <bw_math.h>

struct _bw_slew_lim {
	// Coefficients
	float	T;

	// Parameters
	float	init_val;
	float	max_inc_rate;
	float	max_dec_rate;

	// State
	char	first_run;
	float	y_z1;
};

bw_slew_lim bw_slew_lim_new() {
	bw_slew_lim instance = (bw_slew_lim)BW_MALLOC(sizeof(struct _bw_slew_lim));
	if (instance != NULL) {
		instance->init_val = 0.f;
		instance->max_inc = INFINITY;
		instance->max_dec = INFINITY;
	}
	return instance;
}

void bw_slew_lim_free(bw_slew_lim instance) {
	BW_FREE(instance);
}

void bw_slew_lim_set_sample_rate(bw_slew_lim instance, float sample_rate) {
	instance->T = 1.f / sample_rate;
}

void bw_slew_lim_reset(bw_slew_lim instance) {
	instance->first_run = 1;
	instance->param_changed = ~0;
}

void bw_slew_lim_process(bw_slew_lim instance, const float* x, float* y, int n_samples) {
	if (instance->first_run) {
		instance->y_z1 = instance->init_val;
		instance->first_run = 0;
	}
	
	if (instance->max_inc_rate != INFINITY) {
		const float max_inc = instance->T * instance->max_inc_rate;
		if (instance->max_dec_rate != INFINITY) {
			const float max_dec = instance->T * instance->max_dec_rate;
			for (int i = 0; i < n_samples; i++) {
				y[i] = bw_clipf(x[i], instance->y_z1 - max_dec, instance->y_z1 + max_inc);
				instance->y_z1 = y[i];
			}
		} else {
			for (int i = 0; i < n_samples; i++) {
				y[i] = bw_minf(x[i], instance->y_z1 + max_inc);
				instance->y_z1 = y[i];
			}
		}
	} else {
		if (instance->max_dec_rate != INFINITY) {
			const float max_dec = instance->T * instance->max_dec_rate;
			for (int i = 0; i < n_samples; i++) {
				y[i] = bw_maxf(x[i], instance->y_z1 - max_dec);
				instance->y_z1 = y[i];
			}
		} else {
			for (int i = 0; i < n_samples; i++) {
				y[i] = x[i];
			instance->y_z1 = y[n_samples - 1];
		}
	}
}

void bw_slew_lim_set_init_val(bw_slew_lim instance, float value) {
	instance->init_val = value;
}

void bw_slew_lim_set_max_rate(bw_slew_lim instance, float value) {
	instance->max_inc_rate = value;
	instance->max_dec_rate = value;
}

void bw_slew_lim_set_max_inc_rate(bw_slew_lim instance, float value) {
	instance->max_inc_rate = value;
}

void bw_slew_lim_set_max_dec_rate(bw_slew_lim instance, float value) {
	instance->max_dec_rate = value;
}
