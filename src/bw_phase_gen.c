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

#include <bw_phase_gen.h>

#include <bw_math.h>

void bw_phase_gen_init(bw_phase_gen *instance) {
	instance->frequency = 1.f;
	instance->portamento_tau = 0.f;
}

void bw_phase_gen_set_sample_rate(bw_phase_gen *instance, float sample_rate) {
	instance->T = 1.f / sample_rate;
}

void bw_phase_gen_reset(bw_phase_gen *instance) {
	instance->first_run = 1;
	instance->param_changed = ~0;
}

#define PARAM_FREQUENCY		1
#define PARAM_PORTAMENTO_TAU	(1<<1)

void bw_phase_gen_process(bw_phase_gen *instance, const float *x_mod, float* y, float *y_phase_inc, int n_samples) {
	if (instance->param_changed) {
		if (instance->param_changed & PARAM_FREQUENCY)
			instance->portamento_target = instance->T * instance->frequency;
		if (instance->param_changed & PARAM_PORTAMENTO_TAU)
			instance->portamento_mA1 = bw_expf_3(-instance->T * bw_rcpf_2(instance->portamento_tau));
	}

	if (instance->first_run) {
		instance->phase = 0.f;
		instance->portamento_z1 = instance->portamento_target;
		instance->first_run = 0;
	}

	if (y_phase_inc == NULL)
		y_phase_inc = y;

	if (x_mod != NULL)
		for (int i = 0; i < n_samples; i++) {
			const float portamento = instance->portamento_target + instance->portamento_mA1 * (instance->portamento_z1 - instance->portamento_target);
			instance->portamento_z1 = portamento;

			const float phase_inc = portamento * bw_pow2f_3(x_mod[i]);
			instance->phase += phase_inc;
			instance->phase -= bw_floorf(instance->phase);

			y_phase_inc[i] = phase_inc;
			y[i] = instance->phase;
		}
	else
		for (int i = 0; i < n_samples; i++) {
			const float portamento = instance->portamento_target + instance->portamento_mA1 * (instance->portamento_z1 - instance->portamento_target);
			instance->portamento_z1 = portamento;

			instance->phase += portamento;
			instance->phase -= bw_floorf(instance->phase);

			y_phase_inc[i] = portamento;
			y[i] = instance->phase;
		}
}

void bw_phase_gen_set_frequency(bw_phase_gen *instance, float value) {
	if (instance->frequency != value) {
		instance->frequency = value;
		instance->param_changed |= PARAM_FREQUENCY;
	}
}

void bw_phase_gen_set_portamento_tau(bw_phase_gen *instance, float value) {
	if (instance->portamento_tau != value) {
		instance->portamento_tau = value;
		instance->param_changed |= PARAM_PORTAMENTO_TAU;
	}
}
