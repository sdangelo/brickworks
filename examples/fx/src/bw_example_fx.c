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

#include "bw_example_fx.h"

#include <bw_math.h>
#include <bw_svf.h>
#include <bw_env_follow.h>

enum {
	p_cutoff,
	p_Q,
	p_n
};

#define BUFFER_SIZE	128

struct _bw_example_fx {
	// Sub-components
	bw_svf		svf;
	bw_env_follow	env_follow;

	// Parameters
	float		params[p_n];

	// States
	float		level;

	// Buffers
	float		buf[BUFFER_SIZE];
};

bw_example_fx bw_example_fx_new() {
	bw_example_fx instance = (bw_example_fx)BW_MALLOC(sizeof(struct _bw_example_fx));
	if (instance == NULL)
		return NULL;

	bw_svf_init(&instance->svf);
	bw_env_follow_init(&instance->env_follow);

	bw_one_pole_set_cutoff_down(bw_env_follow_get_one_pole(&instance->env_follow), 1.f);
	
	return instance;
}

void bw_example_fx_free(bw_example_fx instance) {
	BW_FREE(instance);
}

void bw_example_fx_set_sample_rate(bw_example_fx instance, float sample_rate) {
	bw_svf_set_sample_rate(&instance->svf, sample_rate);
	bw_env_follow_set_sample_rate(&instance->env_follow, sample_rate);
}

void bw_example_fx_reset(bw_example_fx instance) {
	bw_svf_reset(&instance->svf);
	bw_env_follow_reset(&instance->env_follow);
}

void bw_example_fx_process(bw_example_fx instance, const float** x, float** y, int n_samples) {
	float a[n_samples];

	bw_svf_process(&instance->svf, x[0], y[0], NULL, NULL, n_samples);

	for (int i = 0; i < n_samples; i += BUFFER_SIZE) {
		const uint32_t n = bw_minu32(n_samples - i, BUFFER_SIZE);
		bw_env_follow_process(&instance->env_follow, y[0] + i, instance->buf, n);
		instance->level = instance->buf[i + n - 1];
	}
}

void bw_example_fx_set_parameter(bw_example_fx instance, int index, float value) {
	switch (index) {
	case p_cutoff:
		bw_svf_set_cutoff(&instance->svf, (20e3f - 20.f) * value * value * value + 20.f);
		break;
	case p_Q:
		bw_svf_set_Q(&instance->svf, 0.5f + 9.5f * value);
		break;
	}
}

float bw_example_fx_get_parameter(bw_example_fx instance, int index) {
	return index == 0 ? instance->params[0] : instance->level;
}
