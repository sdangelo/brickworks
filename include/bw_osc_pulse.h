/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
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
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

/*!
 *  module_type {{{ dsp }}}
 *  version {{{ 0.4.0 }}}
 *  requires {{{ bw_config bw_common bw_one_pole bw_math }}}
 *  description {{{
 *    Pulse oscillator waveshaper with variable pulse width (actually, duty
 *    cycle) and PolyBLEP antialiasing.
 *
 *    It turns a normalized phase signal, such as that geneated by
 *    [bw\_phase\_gen](bw_phase_gen), into a pulse wave.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Fixed unused parameter warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>Refactored API.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.1.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_OSC_PULSE_H
#define _BW_OSC_PULSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_osc_pulse_coeffs
 *  ```>>> */
typedef struct _bw_osc_pulse_coeffs bw_osc_pulse_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_osc_pulse_init()
 *  ```>>> */
static inline void bw_osc_pulse_init(bw_osc_pulse_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_osc_pulse_set_sample_rate()
 *  ```>>> */
static inline void bw_osc_pulse_set_sample_rate(bw_osc_pulse_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_osc_pulse_reset_coeffs()
 *  ```>>> */
static inline void bw_osc_pulse_reset_coeffs(bw_osc_pulse_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_osc_pulse_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_osc_pulse_update_coeffs_ctrl(bw_osc_pulse_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_osc_pulse_update_coeffs_audio()
 *  ```>>> */
static inline void bw_osc_pulse_update_coeffs_audio(bw_osc_pulse_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_osc_pulse_process1\*()
 *  ```>>> */
static inline float bw_osc_pulse_process1(const bw_osc_pulse_coeffs *BW_RESTRICT coeffs, float x);
static inline float bw_osc_pulse_process1_antialias(const bw_osc_pulse_coeffs *BW_RESTRICT coeffs, float x, float x_phase_inc);
/*! <<<```
 *    These function process one input sample `x`, indicating the normalized
 *    phase, using `coeffs`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_osc_pulse_process1()` assumes that antialiasing is disabled;
 *     * `bw_osc_pulse_process1_antialias()` assumes that antialiasing is
 *       enabled and requires the corresponding phase increment value to be
 *       passed via `x_phase_inc`.
 *
 *    #### bw_osc_pulse_process()
 *  ```>>> */
static inline void bw_osc_pulse_process(bw_osc_pulse_coeffs *BW_RESTRICT coeffs, const float *x, const float *x_phase_inc, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x`, containing the
 *    normalized phase signal, and fills the first `n_samples` of the output
 *    buffer `y`, while using and updating `coeffs`.
 *
 *    If antialiasing is enabled, `x_phase_inc` must contain phase increment
 *    values, otherwise it is ignored and can be `NULL`.
 *
 *    #### bw_osc_pulse_set_antialiasing()
 *  ```>>> */
static inline void bw_osc_pulse_set_antialiasing(bw_osc_pulse_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the antialiasing is on (`value` non-`0`) or off (`0`) in
 *    `coeffs`.
 *
 *    Default value: `0` (off).
 *
 *    #### bw_osc_pulse_set_pulse_width()
 *  ```>>> */
static inline void bw_osc_pulse_set_pulse_width(bw_osc_pulse_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the pulse width (actually, the duty cycle) to `value` (range
 *    [`0.f`, `1.f`]) in `coeffs`.
 *
 *    Default value: `0.5f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_osc_pulse_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_state;

	// Parameters
	char			antialiasing;
	float			pulse_width;
};

static inline void bw_osc_pulse_init(bw_osc_pulse_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	coeffs->antialiasing = 0;
	coeffs->pulse_width = 0.5f;
}

static inline void bw_osc_pulse_set_sample_rate(bw_osc_pulse_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void bw_osc_pulse_reset_coeffs(bw_osc_pulse_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->pulse_width);
}

static inline void bw_osc_pulse_update_coeffs_ctrl(bw_osc_pulse_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline void bw_osc_pulse_update_coeffs_audio(bw_osc_pulse_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->pulse_width);
}

static inline float bw_osc_pulse_process1(const bw_osc_pulse_coeffs *BW_RESTRICT coeffs, float x) {
	const float pw = bw_one_pole_get_y_z1(&coeffs->smooth_state);
	return bw_signf(pw - x);
}

// PolyBLEP residual based on Parzen window (4th-order B-spline), one-sided (x in [0, 2])
static inline float _bw_osc_pulse_blep_diff(float x) {
	return x < 1.f
		? x * ((0.25f * x - 0.6666666666666666f) * x * x + 1.333333333333333f) - 1.f
		: x * (x * ((0.6666666666666666f - 0.08333333333333333f * x) * x - 2.f) + 2.666666666666667f) - 1.333333333333333f;
}

static inline float bw_osc_pulse_process1_antialias(const bw_osc_pulse_coeffs *BW_RESTRICT coeffs, float x, float x_phase_inc) {
	const float pw = bw_one_pole_get_y_z1(&coeffs->smooth_state);
	const float pw_m_phase = pw - x;
	float v = bw_copysignf(1.f, pw_m_phase); // pw = phase case should be properly compensated by the AA
	if (x_phase_inc != 0.f) {
		const float phase_inc_2 = x_phase_inc + x_phase_inc;
		const float phase_inc_rcp = bw_rcpf_2(x_phase_inc);
		const float phase_2 = 0.5f * v + 0.5f - pw_m_phase;
		const float s_1_m_phase = 1.f - x;
		const float s_1_m_phase_2 = 1.f - phase_2;
		if (s_1_m_phase < phase_inc_2)
			v -= _bw_osc_pulse_blep_diff(s_1_m_phase * phase_inc_rcp);
		if (s_1_m_phase_2 < phase_inc_2)
			v += _bw_osc_pulse_blep_diff(s_1_m_phase_2 * phase_inc_rcp);
		if (x < phase_inc_2)
			v += _bw_osc_pulse_blep_diff(x * phase_inc_rcp);
		if (phase_2 < phase_inc_2)
			v -= _bw_osc_pulse_blep_diff(phase_2 * phase_inc_rcp);
	}
	return v;
}

static inline void bw_osc_pulse_process(bw_osc_pulse_coeffs *BW_RESTRICT coeffs, const float *x, const float *x_phase_inc, float *y, int n_samples) {
	if (coeffs->antialiasing)
		for (int i = 0; i < n_samples; i++) {
			bw_osc_pulse_update_coeffs_audio(coeffs);
			y[i] = bw_osc_pulse_process1_antialias(coeffs, x[i], x_phase_inc[i]);
		}
	else
		for (int i = 0; i < n_samples; i++) {
			bw_osc_pulse_update_coeffs_audio(coeffs);
			y[i] = bw_osc_pulse_process1(coeffs, x[i]);
		}
}

static inline void bw_osc_pulse_set_antialiasing(bw_osc_pulse_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->antialiasing = value;
}

static inline void bw_osc_pulse_set_pulse_width(bw_osc_pulse_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->pulse_width = value;
}

#ifdef __cplusplus
}
#endif

#endif
