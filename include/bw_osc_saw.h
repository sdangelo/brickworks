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
 *  requires {{{ bw_common bw_config bw_math }}}
 *  description {{{
 *    Sawtooth oscillator waveshaper with PolyBLEP antialiasing.
 *
 *    It turns a normalized phase signal, such as that geneated by
 *    [bw\_phase\_gen](bw_phase_gen), into a sawtooth wave.
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

#ifndef _BW_OSC_SAW_H
#define _BW_OSC_SAW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_osc_saw_coeffs
 *  ```>>> */
typedef struct _bw_osc_saw_coeffs bw_osc_saw_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_osc_saw_init()
 *  ```>>> */
static inline void bw_osc_saw_init(bw_osc_saw_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_osc_saw_process1\*()
 *  ```>>> */
static inline float bw_osc_saw_process1(const bw_osc_saw_coeffs *BW_RESTRICT coeffs, float x);
static inline float bw_osc_saw_process1_antialias(const bw_osc_saw_coeffs *BW_RESTRICT coeffs, float x, float x_phase_inc);
/*! <<<```
 *    These function process one input sample `x`, indicating the normalized
 *    phase, using `coeffs`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_osc_saw_process1()` assumes that antialiasing is disabled;
 *     * `bw_osc_saw_process1_antialias()` assumes that antialiasing is enabled
 *       and requires the corresponding phase increment value to be passed via
 *       `x_phase_inc`.
 *
 *    #### bw_osc_saw_process()
 *  ```>>> */
static inline void bw_osc_saw_process(bw_osc_saw_coeffs *BW_RESTRICT coeffs, const float *x, const float *x_phase_inc, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x`, containing the
 *    normalized phase signal, and fills the first `n_samples` of the output
 *    buffer `y`, while using and updating `coeffs`.
 *
 *    If antialiasing is enabled, `x_phase_inc` must contain phase increment
 *    values, otherwise it is ignored and can be `NULL`.
 *
 *    #### bw_osc_saw_set_antialiasing()
 *  ```>>> */
static inline void bw_osc_saw_set_antialiasing(bw_osc_saw_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the antialiasing is on (`value` non-`0`) or off (`0`) in
 *    `coeffs`.
 *
 *    Default value: `0` (off).
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

struct _bw_osc_saw_coeffs {
	// Parameters
	char	antialiasing;
};

static inline void bw_osc_saw_init(bw_osc_saw_coeffs *BW_RESTRICT coeffs) {
	coeffs->antialiasing = 0;
}

static inline float bw_osc_saw_process1(const bw_osc_saw_coeffs *BW_RESTRICT coeffs, float x) {
	(void)coeffs;
	return x + x - 1.f;
}

// PolyBLEP residual based on Parzen window (4th-order B-spline), one-sided (x in [0, 2])
static inline float _bw_osc_saw_blep_diff(float x) {
	return x < 1.f
		? x * ((0.25f * x - 0.6666666666666666f) * x * x + 1.333333333333333f) - 1.f
		: x * (x * ((0.6666666666666666f - 0.08333333333333333f * x) * x - 2.f) + 2.666666666666667f) - 1.333333333333333f;
}

static inline float bw_osc_saw_process1_antialias(const bw_osc_saw_coeffs *BW_RESTRICT coeffs, float x, float x_phase_inc) {
	(void)coeffs;
	const float s_1_m_phase = 1.f - x;
	float v = x - s_1_m_phase;
	if (x_phase_inc != 0.f) {
		const float phase_inc_2 = x_phase_inc + x_phase_inc;
		const float phase_inc_rcp = bw_rcpf_2(x_phase_inc);
		if (s_1_m_phase < phase_inc_2)
			v += _bw_osc_saw_blep_diff(s_1_m_phase * phase_inc_rcp);
		if (x < phase_inc_2)
			v -= _bw_osc_saw_blep_diff(x * phase_inc_rcp);
	}
	return v;
}

static inline void bw_osc_saw_process(bw_osc_saw_coeffs *BW_RESTRICT coeffs, const float *x, const float *x_phase_inc, float *y, int n_samples) {
	if (coeffs->antialiasing)
		for (int i = 0; i < n_samples; i++)
			y[i] = bw_osc_saw_process1_antialias(coeffs, x[i], x_phase_inc[i]);
	else
		for (int i = 0; i < n_samples; i++)
			y[i] = bw_osc_saw_process1(coeffs, x[i]);
}

static inline void bw_osc_saw_set_antialiasing(bw_osc_saw_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->antialiasing = value;
}

#ifdef __cplusplus
}
#endif

#endif
