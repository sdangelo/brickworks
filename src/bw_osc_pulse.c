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

#include <bw_osc_pulse.h>

#include <bw_math.h>
#include <bw_inline_one_pole.h>

void bw_osc_pulse_init(bw_osc_pulse *instance) {
	instance->antialiasing = 0;
	instance->pulse_width = 0.5f;
}

void bw_osc_pulse_set_sample_rate(bw_osc_pulse *instance, float sample_rate) {
	instance->smooth_mA1 = bw_inline_one_pole_get_mA1(sample_rate, 0.005f);
}

void bw_osc_pulse_reset(bw_osc_pulse *instance) {
	instance->first_run = 1;
}

// PolyBLEP residual based on Parzen window (4th-order B-spline), one-sided (x in [0, 2])
static inline float blep_diff(float x) {
	return x < 1.f
		? x * ((0.25f * x - 0.6666666666666666f) * x * x + 1.333333333333333f) - 1.f
		: x * (x * ((0.6666666666666666f - 0.08333333333333333f * x) * x - 2.0f) + 2.666666666666667f) - 1.333333333333333f;
}

void bw_osc_pulse_process(bw_osc_pulse *instance, const float *x, const float *x_phase_inc, float* y, int n_samples) {
	if (instance->first_run) {
		instance->pw_z1 = instance->pulse_width;
		instance->first_run = 0;
	}

	if (instance->antialiasing) {
		for (int i = 0; i < n_samples; i++) {
			const float pw = bw_inline_one_pole(instance->pulse_width, instance->pw_z1, instance->smooth_mA1);
			instance->pw_z1 = pw;

			const float pw_m_phase = pw - x[i];
			float v = bw_copysignf(1.f, pw_m_phase); // pw = phase case should be properly compensated by the AA

			if (x_phase_inc[i] != 0.f) {
				const float phase_inc_2 = x_phase_inc[i] + x_phase_inc[i];
				const float phase_inc_rcp = bw_rcpf_2(x_phase_inc[i]);
				const float phase_2 = 0.5f * v + 0.5f - pw_m_phase;
				const float s_1_m_phase = 1.f - x[i];
				const float s_1_m_phase_2 = 1.f - phase_2;
				if (s_1_m_phase < phase_inc_2)
					v -= blep_diff(s_1_m_phase * phase_inc_rcp);
				if (s_1_m_phase_2 < phase_inc_2)
					v += blep_diff(s_1_m_phase_2 * phase_inc_rcp);
				if (x[i] < phase_inc_2)
					v += blep_diff(x[i] * phase_inc_rcp);
				if (phase_2 < phase_inc_2)
					v -= blep_diff(phase_2 * phase_inc_rcp);
			}

			y[i] = v;
		}
	} else {
		for (int i = 0; i < n_samples; i++) {
			const float pw = bw_inline_one_pole(instance->pulse_width, instance->pw_z1, instance->smooth_mA1);
			instance->pw_z1 = pw;

			y[i] = bw_signf(pw - x[i]);
		}
	}
}

void bw_osc_pulse_set_antialiasing(bw_osc_pulse *instance, char value) {
	instance->antialiasing = value;
}

void bw_osc_pulse_set_pulse_width(bw_osc_pulse *instance, float value) {
	instance->pulse_width = value;
}
