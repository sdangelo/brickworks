/*
 * Brickworks
 *
 * Copyright (C) 2021, 2022 Orastron Srl unipersonale
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

#include <bw_one_pole.h>

#include <bw_math.h>

void bw_one_pole_init(bw_one_pole *instance) {
	instance->init_val = 0.f;
	instance->cutoff_up = INFINITY;
	instance->cutoff_down = INFINITY;
	instance->sticky_thresh = 0.f;
}

void bw_one_pole_set_sample_rate(bw_one_pole *instance, float sample_rate) {
	instance->Ttm2pi = -6.283185307179586f / sample_rate;
}

void bw_one_pole_reset(bw_one_pole *instance) {
	instance->first_run = 1;
	instance->param_changed = ~0;
}

#define PARAM_CUTOFF_UP		1
#define PARAM_CUTOFF_DOWN	(1<<1)
#define PARAM_STICKY_THRESH	(1<<2)

void bw_one_pole_process(bw_one_pole *instance, const float* x, float* y, int n_samples) {
	if (instance->param_changed) {
		if (instance->param_changed & PARAM_CUTOFF_UP)
			instance->mA1u = bw_expf_3(instance->Ttm2pi * instance->cutoff_up);
		if (instance->param_changed & PARAM_CUTOFF_DOWN)
			instance->mA1d = bw_expf_3(instance->Ttm2pi * instance->cutoff_down);
		if (instance->param_changed & PARAM_STICKY_THRESH)
			instance->st2 = instance->sticky_thresh * instance->sticky_thresh;
		instance->param_changed = 0;
	}
	
	if (instance->first_run) {
		instance->x_z1 = instance->init_val;
		instance->y_z1 = instance->init_val;
		instance->first_run = 0;
	}

	if (instance->mA1u != instance->mA1d) {
		if (instance->st2 != 0.f) {
			if (instance->sticky_mode == bw_one_pole_sticky_mode_abs)
				for (int i = 0; i < n_samples; i++) {
					const float in = x[i];
					float out = in + (in >= instance->y_z1 ? instance->mA1u : instance->mA1d) * (instance->y_z1 - in);
					const float d = out - in;
					if (d * d <= instance->st2)
						out = in;
					instance->x_z1 = in;
					instance->y_z1 = out;
					y[i] = out;
				}
			else
				for (int i = 0; i < n_samples; i++) {
					const float in = x[i];
					float out = in + (in >= instance->y_z1 ? instance->mA1u : instance->mA1d) * (instance->y_z1 - in);
					const float d = out - in;
					if (d * d <= instance->st2 * in * in)
						out = in;
					instance->x_z1 = in;
					instance->y_z1 = out;
					y[i] = out;
				}
		}
		else {
			for (int i = 0; i < n_samples; i++) {
				const float in = x[i];
				const float out = in + (in >= instance->y_z1 ? instance->mA1u : instance->mA1d) * (instance->y_z1 - in);
				instance->x_z1 = in;
				instance->y_z1 = out;
				y[i] = out;
			}
		}
	}
	else {
		if (instance->st2 != 0.f) {
			if (instance->sticky_mode == bw_one_pole_sticky_mode_abs)
				for (int i = 0; i < n_samples; i++) {
					const float in = x[i];
					float out = in + instance->mA1u * (instance->y_z1 - in);
					const float d = out - in;
					if (d * d <= instance->st2)
						out = in;
					instance->x_z1 = in;
					instance->y_z1 = out;
					y[i] = out;
				}
			else
				for (int i = 0; i < n_samples; i++) {
					const float in = x[i];
					float out = in + instance->mA1u * (instance->y_z1 - in);
					const float d = out - in;
					if (d * d <= instance->st2 * in * in)
						out = in;
					instance->x_z1 = in;
					instance->y_z1 = out;
					y[i] = out;
				}
		}
		else {
			for (int i = 0; i < n_samples; i++) {
				const float in = x[i];
				const float out = in + instance->mA1u * (instance->y_z1 - in);
				instance->x_z1 = in;
				instance->y_z1 = out;
				y[i] = out;
			}
		}
	}
}

void bw_one_pole_set_init_val(bw_one_pole *instance, float value) {
	instance->init_val = value;
}

void bw_one_pole_set_cutoff(bw_one_pole *instance, float value) {
	bw_one_pole_set_cutoff_up(instance, value);
	bw_one_pole_set_cutoff_down(instance, value);
}

void bw_one_pole_set_cutoff_up(bw_one_pole *instance, float value) {
	if (instance->cutoff_up != value) {
		instance->cutoff_up = value;
		instance->param_changed |= PARAM_CUTOFF_UP;
	}
}

void bw_one_pole_set_cutoff_down(bw_one_pole *instance, float value) {
	if (instance->cutoff_down != value) {
		instance->cutoff_down = value;
		instance->param_changed |= PARAM_CUTOFF_DOWN;
	}
}

void bw_one_pole_set_tau(bw_one_pole *instance, float value) {
	bw_one_pole_set_tau_up(instance, value);
	bw_one_pole_set_tau_down(instance, value);
}

void bw_one_pole_set_tau_up(bw_one_pole *instance, float value) {
	bw_one_pole_set_cutoff_up(instance, value < 1e-9f ? INFINITY : 0.1591549430918953f * bw_rcpf_2(value));
	// tau < 1 ns is instantaneous for any practical purpose
}

void bw_one_pole_set_tau_down(bw_one_pole *instance, float value) {
	bw_one_pole_set_cutoff_down(instance, value < 1e-9f ? INFINITY : 0.1591549430918953f * bw_rcpf_2(value));
	// as before
}

void bw_one_pole_set_sticky_thresh(bw_one_pole *instance, float value) {
	if (instance->sticky_thresh != value) {
		instance->sticky_thresh = value;
		instance->param_changed |= PARAM_STICKY_THRESH;
	}
}

void bw_one_pole_set_sticky_mode(bw_one_pole *instance, bw_one_pole_sticky_mode value) {
	instance->sticky_mode = value;
}
