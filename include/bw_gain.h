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
 *  version {{{ 0.6.0 }}}
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    Gain.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_gain_process_multi()</code>.</li>
 *          <li>Added <code>bw_gain_get_gain()</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Renamed as bw_gain.</li>
 *          <li>Added new smooth_tau parameter.</li>
 *          <li>Changed gain parameter API to express values in linear gain
 *              and dB.</li>
 *          <li>Added C++ wrapper.</li>
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

#ifndef _BW_GAIN_H
#define _BW_GAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_gain_coeffs
 *  ```>>> */
typedef struct _bw_gain_coeffs bw_gain_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_gain_init()
 *  ```>>> */
static inline void bw_gain_init(bw_gain_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_gain_set_sample_rate()
 *  ```>>> */
static inline void bw_gain_set_sample_rate(bw_gain_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_gain_reset_coeffs()
 *  ```>>> */
static inline void bw_gain_reset_coeffs(bw_gain_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_gain_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_gain_update_coeffs_ctrl(bw_gain_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_gain_update_coeffs_audio()
 *  ```>>> */
static inline void bw_gain_update_coeffs_audio(bw_gain_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_gain_process1()
 *  ```>>> */
static inline float bw_gain_process1(const bw_gain_coeffs *BW_RESTRICT coeffs, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs` and returns the
 *    corresponding output sample.
 *
 *    #### bw_gain_process()
 *  ```>>> */
static inline void bw_gain_process(bw_gain_coeffs *BW_RESTRICT coeffs, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating
 *    `coeffs` (control and audio rate).
 *
 *    #### bw_gain_process_multi()
 *  ```>>> */
static inline void bw_gain_process_multi(bw_gain_coeffs *BW_RESTRICT coeffs, const float **x, float **y, int n_channels, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating the common `coeffs` (control and audio rate).
 *
 *    #### bw_gain_set_gain_lin()
 *  ```>>> */
static inline void bw_gain_set_gain_lin(bw_gain_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the gain parameter to the given `value` (linear gain) in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_gain_set_gain_dB()
 *  ```>>> */
static inline void bw_gain_set_gain_dB(bw_gain_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the gain parameter to the given `value` (dB) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_gain_set_smooth_tau()
 *  ```>>> */
static inline void bw_gain_set_smooth_tau(bw_gain_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the smoothing time constant `value` (s) in `coeffs`.
 *
 *    Default value: `0.05f`.
 *
 *    #### bw_gain_get_gain()
 *  ```>>> */
static inline float bw_gain_get_gain(const bw_gain_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the actual current gain coefficient (linear gain) in `coeffs`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_gain_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_state;

	// Parameters
	float			gain;
	float			smooth_tau;
	float			smooth_tau_prev;
};

static inline void bw_gain_init(bw_gain_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->gain = 1.f;
	coeffs->smooth_tau = 0.05f;
}

static inline void bw_gain_set_sample_rate(bw_gain_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void _bw_gain_do_update_coeffs(bw_gain_coeffs *BW_RESTRICT coeffs, char force) {
	if (force || coeffs->smooth_tau != coeffs->smooth_tau_prev) {
		bw_one_pole_set_tau(&coeffs->smooth_coeffs, coeffs->smooth_tau);
		bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
		coeffs->smooth_tau_prev = coeffs->smooth_tau;
	}
}

static inline void bw_gain_reset_coeffs(bw_gain_coeffs *BW_RESTRICT coeffs) {
	_bw_gain_do_update_coeffs(coeffs, 1);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->gain);
}

static inline void bw_gain_update_coeffs_ctrl(bw_gain_coeffs *BW_RESTRICT coeffs) {
	_bw_gain_do_update_coeffs(coeffs, 0);
}

static inline void bw_gain_update_coeffs_audio(bw_gain_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->gain);
}

static inline float bw_gain_process1(const bw_gain_coeffs *BW_RESTRICT coeffs, float x) {
	return bw_one_pole_get_y_z1(&coeffs->smooth_state) * x;
}

static inline void bw_gain_process(bw_gain_coeffs *BW_RESTRICT coeffs, const float *x, float *y, int n_samples) {
	bw_gain_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_gain_update_coeffs_audio(coeffs);
		y[i] = bw_gain_process1(coeffs, x[i]);
	}
}

static inline void bw_gain_process_multi(bw_gain_coeffs *BW_RESTRICT coeffs, const float **x, float **y, int n_channels, int n_samples) {
	bw_gain_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_gain_update_coeffs_audio(coeffs);
		for (int j = 0; j < n_channels; j++)
			y[j][i] = bw_gain_process1(coeffs, x[j][i]);
	}
}

static inline void bw_gain_set_gain_lin(bw_gain_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->gain = value;
}

static inline void bw_gain_set_gain_dB(bw_gain_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->gain = bw_dB2linf_3(value);
}

static inline void bw_gain_set_smooth_tau(bw_gain_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->smooth_tau = value;
}

static inline float bw_gain_get_gain(const bw_gain_coeffs *BW_RESTRICT coeffs) {
	return bw_one_pole_get_y_z1(&coeffs->smooth_state);
}

#ifdef __cplusplus
}
#endif

#endif
