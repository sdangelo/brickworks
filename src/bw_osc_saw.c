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

#include <bw_osc_saw.h>

#include <bw_math.h>

void bw_osc_saw_init(bw_osc_saw *instance) {
	instance->antialiasing = 0;
}

// PolyBLEP residual based on Parzen window (4th-order B-spline), one-sided (x in [0, 2])
static inline float blep_diff(float x) {
	return x < 1.f
		? x * ((0.25f * x - 0.6666666666666666f) * x * x + 1.333333333333333f) - 1.f
		: x * (x * ((0.6666666666666666f - 0.08333333333333333f * x) * x - 2.0f) + 2.666666666666667f) - 1.333333333333333f;
}

void bw_osc_saw_process(bw_osc_saw *instance, const float *x, const float *x_phase_inc, float* y, int n_samples) {
	if (instance->antialiasing) {
		for (int i = 0; i < n_samples; i++) {
			const float s_1_m_phase = 1.f - x[i];
			float v = x[i] - s_1_m_phase;

			if (x_phase_inc[i] != 0.f) {
				const float phase_inc_2 = x_phase_inc[i] + x_phase_inc[i];
				const float phase_inc_rcp = bw_rcpf_2(x_phase_inc[i]);
				if (s_1_m_phase < phase_inc_2)
					v += blep_diff(s_1_m_phase * phase_inc_rcp);
				if (x[i] < phase_inc_2)
					v -= blep_diff(x[i] * phase_inc_rcp);
			}

			y[i] = v;
		}
	} else {
		for (int i = 0; i < n_samples; i++)
			y[i] = x[i] + x[i] - 1.f;
	}
}

void bw_osc_saw_set_antialiasing(bw_osc_saw *instance, char value) {
	instance->antialiasing = value;
}
