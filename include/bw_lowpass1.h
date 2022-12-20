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
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

/*!
 *  module_type {{{ dsp }}}
 *  version {{{ 0.3.0 }}}
 *  requires {{{ bw_config bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    First-order lowpass filter (6 dB/oct) with unitary DC gain.
 *
 *    This is better suited to filtering actual audio than
 *    [bw_one_pole](bw_one_pole).
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_LOWPASS1_H
#define _BW_LOWPASS1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_lowpass1_coeffs
 *  ```>>> */
typedef struct _bw_lowpass1_coeffs bw_lowpass1_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_lowpass1_state
 *  ```>>> */
typedef struct _bw_lowpass1_state bw_lowpass1_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_lowpass1_init()
 *  ```>>> */
static inline void bw_lowpass1_init(bw_lowpass1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_lowpass1_set_sample_rate()
 *  ```>>> */
static inline void bw_lowpass1_set_sample_rate(bw_lowpass1_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_lowpass1_reset_coeffs()
 *  ```>>> */
static inline void bw_lowpass1_reset_coeffs(bw_lowpass1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_lowpass1_reset_state()
 *  ```>>> */
static inline void bw_lowpass1_reset_state(const bw_lowpass1_coeffs *BW_RESTRICT coeffs, bw_lowpass1_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_lowpass1_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_lowpass1_update_coeffs_ctrl(bw_lowpass1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_lowpass1_update_coeffs_audio()
 *  ```>>> */
static inline void bw_lowpass1_update_coeffs_audio(bw_lowpass1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_lowpass1_process1()
 *  ```>>> */
static inline float bw_lowpass1_process1(const bw_lowpass1_coeffs *BW_RESTRICT coeffs, bw_lowpass1_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_lowpass1_process()
 *  ```>>> */
static inline void bw_lowpass1_process(bw_lowpass1_coeffs *BW_RESTRICT coeffs, bw_lowpass1_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_lowpass1_set_cutoff()
 *  ```>>> */
static inline void bw_lowpass1_set_cutoff(bw_lowpass1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_lowpass1_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_state;
	
	// Coefficients
	float			t_k;

	float			t;
	float			X_x;
	float			X_X_z1;
	float			y_X;

	// Parameters
	float			cutoff;
};

struct _bw_lowpass1_state {
	float	y_z1;
	float	X_z1;
};

static inline void bw_lowpass1_init(bw_lowpass1_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->cutoff = 1e3f;
}

static inline void bw_lowpass1_set_sample_rate(bw_lowpass1_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->t_k = 3.141592653589793f / sample_rate;
}

static inline void _bw_lowpass1_do_update_coeffs(bw_lowpass1_coeffs *BW_RESTRICT coeffs, char force) {
	float cutoff_cur = bw_one_pole_get_y_z1(&coeffs->smooth_state);
	const char cutoff_changed = force || coeffs->cutoff != cutoff_cur;
	if (cutoff_changed) {
		cutoff_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->cutoff);
		coeffs->t = bw_tanf_3(coeffs->t_k * cutoff_cur);
		const float k = bw_rcpf_2(1.f + coeffs->t);
		coeffs->X_x = k * cutoff_cur;
		coeffs->X_X_z1 = k * coeffs->t;
		coeffs->y_X = bw_rcpf_2(cutoff_cur);
	}
}

static inline void bw_lowpass1_reset_coeffs(bw_lowpass1_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->cutoff);
	_bw_lowpass1_do_update_coeffs(coeffs, 1);
}

static inline void bw_lowpass1_reset_state(const bw_lowpass1_coeffs *BW_RESTRICT coeffs, bw_lowpass1_state *BW_RESTRICT state) {
	state->y_z1 = 0.f;
	state->X_z1 = 0.f;
}

static inline void bw_lowpass1_update_coeffs_ctrl(bw_lowpass1_coeffs *BW_RESTRICT coeffs) {
}

static inline void bw_lowpass1_update_coeffs_audio(bw_lowpass1_coeffs *BW_RESTRICT coeffs) {
	_bw_lowpass1_do_update_coeffs(coeffs, 0);
}

static inline float bw_lowpass1_process1(const bw_lowpass1_coeffs *BW_RESTRICT coeffs, bw_lowpass1_state *BW_RESTRICT state, float x) {
	const float X = coeffs->X_x * (x - state->y_z1) - coeffs->X_X_z1 * state->X_z1;
	const float y = x - coeffs->y_X * X;
	state->y_z1 = y;
	state->X_z1 = X;
	return y;
}

static inline void bw_lowpass1_process(bw_lowpass1_coeffs *BW_RESTRICT coeffs, bw_lowpass1_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	for (int i = 0; i < n_samples; i++) {
		bw_lowpass1_update_coeffs_audio(coeffs);
		y[i] = bw_lowpass1_process1(coeffs, state, x[i]);
	}
}

static inline void bw_lowpass1_set_cutoff(bw_lowpass1_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->cutoff = value;
}

#ifdef __cplusplus
}
#endif

#endif
