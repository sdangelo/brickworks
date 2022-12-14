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

#ifndef _BW_LP1_H
#define _BW_LP1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_lp1_coeffs
 *  ```>>> */
typedef struct _bw_lp1_coeffs bw_lp1_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_lp1_state
 *  ```>>> */
typedef struct _bw_lp1_state bw_lp1_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_lp1_init()
 *  ```>>> */
static inline void bw_lp1_init(bw_lp1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_lp1_set_sample_rate()
 *  ```>>> */
static inline void bw_lp1_set_sample_rate(bw_lp1_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_lp1_reset_coeffs()
 *  ```>>> */
static inline void bw_lp1_reset_coeffs(bw_lp1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_lp1_reset_state()
 *  ```>>> */
static inline void bw_lp1_reset_state(const bw_lp1_coeffs *BW_RESTRICT coeffs, bw_lp1_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_lp1_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_lp1_update_coeffs_ctrl(bw_lp1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_lp1_update_coeffs_audio()
 *  ```>>> */
static inline void bw_lp1_update_coeffs_audio(bw_lp1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_lp1_process1()
 *  ```>>> */
static inline float bw_lp1_process1(const bw_lp1_coeffs *BW_RESTRICT coeffs, bw_lp1_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_lp1_process()
 *  ```>>> */
static inline void bw_lp1_process(bw_lp1_coeffs *BW_RESTRICT coeffs, bw_lp1_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_lp1_set_cutoff()
 *  ```>>> */
static inline void bw_lp1_set_cutoff(bw_lp1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_lp1_set_prewarp_at_cutoff()
 *  ```>>> */
static inline void bw_lp1_set_prewarp_at_cutoff(bw_lp1_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether bilinear transform prewarping frequency should match the
 *    cutoff frequency (non-`0`) or not (`0`).
 *
 *    Default value: non-`0` (on).
 *
 *    #### bw_lp1_set_prewarp_freq()
 *  ```>>> */
static inline void bw_lp1_set_prewarp_freq(bw_lp1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the prewarping frequency `value` (Hz) in `coeffs`.
 *
 *    Only used when the prewarp\_at\_cutoff parameter is off.
 *
 *    Default value: `1e3f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_lp1_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_cutoff_state;
	bw_one_pole_state	smooth_prewarp_freq_state;
	
	// Coefficients
	float			t_k;

	float			prewarp_k;
	float			t;
	float			X_x;
	float			X_X_z1;
	float			y_X;

	// Parameters
	float			cutoff;
	float			prewarp_freq;
};

struct _bw_lp1_state {
	float	y_z1;
	float	X_z1;
};

static inline void bw_lp1_init(bw_lp1_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->cutoff = 1e3f;
	coeffs->prewarp_freq = 1e3f;
	coeffs->prewarp_k = 1.f;
}

static inline void bw_lp1_set_sample_rate(bw_lp1_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->t_k = 3.141592653589793f / sample_rate;
}

static inline void _bw_lp1_do_update_coeffs(bw_lp1_coeffs *BW_RESTRICT coeffs, char force) {
	const float prewarp_freq = coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq);
	float prewarp_freq_cur = bw_one_pole_get_y_z1(&coeffs->smooth_prewarp_freq_state);
	float cutoff_cur = bw_one_pole_get_y_z1(&coeffs->smooth_cutoff_state);
	const char prewarp_freq_changed = force || prewarp_freq != prewarp_freq_cur;
	const char cutoff_changed = force || coeffs->cutoff != cutoff_cur;
	if (prewarp_freq_changed || cutoff_changed) {
		if (prewarp_freq_changed) {
			prewarp_freq_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, prewarp_freq);
			coeffs->t = bw_tanf_3(coeffs->t_k * prewarp_freq_cur);
		}
		if (cutoff_changed) {
			cutoff_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
			coeffs->y_X = bw_rcpf_2(cutoff_cur);
		}
		const float k = cutoff_cur * bw_rcpf_2(cutoff_cur * coeffs->t + prewarp_freq_cur);
		coeffs->X_x = k * prewarp_freq_cur;
		coeffs->X_X_z1 = k * coeffs->t;
	}
}

static inline void bw_lp1_reset_coeffs(bw_lp1_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq));
	_bw_lp1_do_update_coeffs(coeffs, 1);
}

static inline void bw_lp1_reset_state(const bw_lp1_coeffs *BW_RESTRICT coeffs, bw_lp1_state *BW_RESTRICT state) {
	state->y_z1 = 0.f;
	state->X_z1 = 0.f;
}

static inline void bw_lp1_update_coeffs_ctrl(bw_lp1_coeffs *BW_RESTRICT coeffs) {
}

static inline void bw_lp1_update_coeffs_audio(bw_lp1_coeffs *BW_RESTRICT coeffs) {
	_bw_lp1_do_update_coeffs(coeffs, 0);
}

static inline float bw_lp1_process1(const bw_lp1_coeffs *BW_RESTRICT coeffs, bw_lp1_state *BW_RESTRICT state, float x) {
	const float X = coeffs->X_x * (x - state->y_z1) - coeffs->X_X_z1 * state->X_z1;
	const float y = x - coeffs->y_X * X;
	state->y_z1 = y;
	state->X_z1 = X;
	return y;
}

static inline void bw_lp1_process(bw_lp1_coeffs *BW_RESTRICT coeffs, bw_lp1_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	for (int i = 0; i < n_samples; i++) {
		bw_lp1_update_coeffs_audio(coeffs);
		y[i] = bw_lp1_process1(coeffs, state, x[i]);
	}
}

static inline void bw_lp1_set_cutoff(bw_lp1_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->cutoff = value;
}

static inline void bw_lp1_set_prewarp_at_cutoff(bw_lp1_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->prewarp_k = value ? 1.f : 0.f;
}

static inline void bw_lp1_set_prewarp_freq(bw_lp1_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->prewarp_freq = value;
}

#ifdef __cplusplus
}
#endif

#endif
