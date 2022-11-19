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

void bw_noise_gen_init(bw_noise_gen *instance, uint64_t *state) {
	instance->state = state;
	instance->sample_rate_scaling = 0;
}

void bw_noise_gen_set_sample_rate(bw_noise_gen *instance, float sample_rate) {
	instance->scaling_k = 0.004761904761904762f * bw_sqrtf_2(sample_rate);
}

void bw_noise_gen_process(bw_noise_gen *instance, float* y, int n_samples) {
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_rand_f(instance->state);

	if (instance->sample_rate_scaling)
		for (int i = 0; i < n_samples; i++)
			y[i] *= instance->scaling_k;
}

void bw_noise_gen_set_sample_rate_scaling(bw_noise_gen *instance, char value) {
	instance->sample_rate_scaling = value;
}
