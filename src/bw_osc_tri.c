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

#include <bw_osc_tri.h>

#include <bw_math.h>
#include <bw_inline_one_pole.h>

struct _bw_osc_tri {
	// Coefficients
	float	smooth_mA1;

	// Parameters
	char	first_run;
	char	antialiasing;
	float	slope;

	// State
	float	slope_z1;
};

bw_osc_tri bw_osc_tri_new() {
	bw_osc_tri instance = (bw_osc_tri)BW_MALLOC(sizeof(struct _bw_osc_tri));
	if (instance == NULL)
		return NULL;

	instance->antialiasing = 0;
	instance->slope = 0.5f;

	return instance;
}

void bw_osc_tri_free(bw_osc_tri instance) {
	BW_FREE(instance);
}

void bw_osc_tri_set_sample_rate(bw_osc_tri instance, float sample_rate) {
	instance->smooth_mA1 = bw_inline_one_pole_get_mA1(sample_rate, 0.005f);
}

void bw_osc_tri_reset(bw_osc_tri instance) {
	instance->first_run = 1;
}

// PolyBLAMP residual based on Parzen window (4th-order B-spline), one-sided (x in [0, 2])
static inline float blamp_diff(float x) {
	return x < 1.f
		? x * (x * ((0.05f * x - 0.1666666666666667f) * x * x + 0.6666666666666666f) - 1.0f) + 0.4666666666666667f
		: x * (x * (x * ((0.1666666666666667f - 0.01666666666666667f * x) * x - 0.6666666666666666f) + 1.333333333333333f) - 1.333333333333333f) + 0.5333333333333333f;
}

void bw_osc_tri_process(bw_osc_tri instance, const float *x, const float *x_phase_inc, float* y, int n_samples) {
	if (instance->first_run) {
		instance->slope_z1 = instance->slope;
		instance->first_run = 0;
	}

	if (instance->antialiasing) {
		for (int i = 0; i < n_samples; i++) {
			const float slope = bw_inline_one_pole(instance->slope, instance->slope_z1, instance->smooth_mA1);
			instance->slope_z1 = slope;

			const float s_1_p_pw = 1.f + slope;
			const float s_1_m_pw = 1.f - slope;
			const float phase_d = x[i] + x[i];
			float v = x[i] < slope ? (phase_d - slope) * bw_rcpf_2(slope) : (s_1_p_pw - phase_d) * bw_rcpf_2(s_1_m_pw);

			if (x_phase_inc[i] != 0.f) {
				const float phase_inc_2 = x_phase_inc[i] + x_phase_inc[i];
				const float phase_inc_rcp = bw_rcpf_2(x_phase_inc[i]);
				const float pw_m_phase = slope - x[i];
				const float phase_2 = bw_copysignf(0.5f, pw_m_phase) + 0.5f - pw_m_phase;
				const float s_1_m_phase = 1.f - x[i];
				const float s_1_m_phase_2 = 1.f - phase_2;
				float blamp = 0.f;
				if (s_1_m_phase_2 < phase_inc_2)
					blamp += blamp_diff(s_1_m_phase_2 * phase_inc_rcp);
				if (s_1_m_phase < phase_inc_2)
					blamp -= blamp_diff(s_1_m_phase * phase_inc_rcp);
				if (x[i] < phase_inc_2)
					blamp -= blamp_diff(x[i] * phase_inc_rcp);
				if (phase_2 < phase_inc_2)
					blamp += blamp_diff(phase_2 * phase_inc_rcp);
				v -= bw_rcpf_2(slope * s_1_m_pw) * bw_absf(x_phase_inc[i]) * blamp;
			}

			y[i] = v;
		}
	} else {
		for (int i = 0; i < n_samples; i++) {
			const float slope = bw_inline_one_pole(instance->slope, instance->slope_z1, instance->smooth_mA1);
			instance->slope_z1 = slope;

			const float phase_d = x[i] + x[i];
			y[i] = x[i] < slope ? (phase_d - slope) * bw_rcpf_2(slope) : (1.f + slope - phase_d) * bw_rcpf_2(1.f - slope);
		}
	}
}

void bw_osc_tri_set_antialiasing(bw_osc_tri instance, char value) {
	instance->antialiasing = value;
}

void bw_osc_tri_set_slope(bw_osc_tri instance, float value) {
	instance->slope = value;
}
