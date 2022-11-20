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

#include <bw_wah.h>

#include <bw_common.h>

void bw_wah_init(bw_wah *instance) {
	bw_svf_init(&instance->svf);
	bw_wah_set_wah(instance, 0.5f);
	bw_svf_set_Q(&instance->svf, 9.f);
}

void bw_wah_set_sample_rate(bw_wah *instance, float sample_rate) {
	bw_svf_set_sample_rate(&instance->svf, sample_rate);
}

void bw_wah_reset(bw_wah *instance) {
	bw_svf_reset(&instance->svf);
}

void bw_wah_process(bw_wah *instance, const float *x, float *y, int n_samples) {
	bw_svf_process(&instance->svf, x, NULL, y, NULL, n_samples);
}

void bw_wah_set_wah(bw_wah *instance, float value) {
	bw_svf_set_cutoff(&instance->svf, 400.f + (2e3f - 400.f) * value * value * value);
}
