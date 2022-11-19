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

#include <bw_env_follow.h>

#include <bw_math.h>

void bw_env_follow_init(bw_env_follow *instance) {
	bw_one_pole_init(&instance->one_pole);
}

void bw_env_follow_set_sample_rate(bw_env_follow *instance, float sample_rate) {
	bw_one_pole_set_sample_rate(&instance->one_pole, sample_rate);
}

void bw_env_follow_reset(bw_env_follow *instance) {
	bw_one_pole_reset(&instance->one_pole);
}

void bw_env_follow_process(bw_env_follow *instance, const float *x, float *y, int n_samples) {
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_absf(x[i]);
	bw_one_pole_process(&instance->one_pole, y, y, n_samples);
}

bw_one_pole *bw_env_follow_get_one_pole(bw_env_follow *instance) {
	return &instance->one_pole;
}
