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

#include <bw_noise_gen.h>

#include <bw_rand.h>
#include <bw_math.h>

struct _bw_noise_gen {
	// Coefficients
	float		 scaling_k;

	// Parameters
	char		 sample_rate_scaling;

	// State
	uint64_t	*state;
};

bw_noise_gen bw_noise_gen_new(uint64_t *state) {
	bw_noise_gen instance = (bw_noise_gen)BW_MALLOC(sizeof(struct _bw_noise_gen));
	if (instance == NULL)
		return NULL;

	instance->state = state;
	instance->sample_rate_scaling = 0;
	return instance;
}

void bw_noise_gen_free(bw_noise_gen instance) {
	BW_FREE(instance);
}

void bw_noise_gen_set_sample_rate(bw_noise_gen instance, float sample_rate) {
	instance->scaling_k = 0.004761904761904762f * bw_sqrtf_2(sample_rate);
}

void bw_noise_gen_process(bw_noise_gen instance, float* y, int n_samples) {
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_rand_f(instance->state);

	if (instance->sample_rate_scaling)
		for (int i = 0; i < n_samples; i++)
			y[i] *= instance->scaling_k;
}

void bw_noise_gen_set_sample_rate_scaling(bw_noise_gen instance, char value) {
	instance->sample_rate_scaling = value;
}
