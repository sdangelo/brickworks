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
 *  requires {{{ bw_config bw_common bw_math }}}
 *  description {{{
 *    First-order allpass filter.
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

#ifndef _BW_ALLPASS1_H
#define _BW_ALLPASS1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_allpass1_coeffs
 *  ```>>> */
typedef struct _bw_allpass1_coeffs bw_allpass1_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_allpass1_state
 *  ```>>> */
typedef struct _bw_allpass1_state bw_allpass1_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_allpass1_init()
 *  ```>>> */
static inline void bw_allpass1_init(bw_allpass1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_allpass1_set_sample_rate()
 *  ```>>> */
static inline void bw_allpass1_set_sample_rate(bw_allpass1_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_allpass1_reset_coeffs()
 *  ```>>> */
static inline void bw_allpass1_reset_coeffs(bw_allpass1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_allpass1_reset_state()
 *  ```>>> */
static inline void bw_allpass1_reset_state(const bw_allpass1_coeffs *BW_RESTRICT coeffs, bw_allpass1_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_allpass1_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_allpass1_update_coeffs_ctrl(bw_allpass1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_allpass1_update_coeffs_audio()
 *  ```>>> */
static inline void bw_allpass1_update_coeffs_audio(bw_allpass1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_allpass1_process1()
 *  ```>>> */
static inline float bw_allpass1_process1(const bw_allpass1_coeffs *BW_RESTRICT coeffs, bw_allpass1_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_allpass1_process()
 *  ```>>> */
static inline void bw_allpass1_process(bw_allpass1_coeffs *BW_RESTRICT coeffs, bw_allpass1_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_allpass1_set_cutoff()
 *  ```>>> */
static inline void bw_allpass1_set_cutoff(bw_allpass1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

struct _bw_allpass1_coeffs {
	// Coefficients
	float	t_k;

	float	t;
	float	X_x;
	float	X_X_z1;
	float	lp_X;

	// Parameters
	float	cutoff;
	float	cutoff_prev;
};

struct _bw_allpass1_state {
	float	lp_z1;
	float	X_z1;
};

static inline void bw_allpass1_init(bw_allpass1_coeffs *BW_RESTRICT coeffs) {
	coeffs->cutoff = 1e3f;
}

static inline void bw_allpass1_set_sample_rate(bw_allpass1_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->t_k = 3.141592653589793f / sample_rate;
}

static inline void bw_allpass1_reset_coeffs(bw_allpass1_coeffs *BW_RESTRICT coeffs) {
	coeffs->cutoff_prev = -1.f;
	bw_allpass1_update_coeffs_ctrl(coeffs);
}

static inline void bw_allpass1_reset_state(const bw_allpass1_coeffs *BW_RESTRICT coeffs, bw_allpass1_state *BW_RESTRICT state) {
	state->lp_z1 = 0.f;
	state->X_z1 = 0.f;
}

static inline void bw_allpass1_update_coeffs_ctrl(bw_allpass1_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->cutoff != coeffs->cutoff_prev) {
		coeffs->t = bw_tanf_3(coeffs->t_k * coeffs->cutoff);
		const float k = bw_rcpf_2(1.f + coeffs->T);
		coeffs->X_x = k * coeffs->cutoff;
		coeffs->X_X_z1 = k * coeffs->t;
		coeffs->lp_X = bw_rcpf_2(coeffs->cutoff);
		coeffs->cutoff_prev = coeffs->cutoff;
	}
}

static inline void bw_allpass1_update_coeffs_audio(bw_allpass1_coeffs *BW_RESTRICT coeffs) {
}

static inline float bw_allpass1_process1(const bw_allpass1_coeffs *BW_RESTRICT coeffs, bw_allpass1_state *BW_RESTRICT state, float x) {
	const float X = coeffs->X_x * (x - state->lp_z1) - coeffs->X_X_z1 * state->X_z1;
	const float lp = x - coeffs->lp_X * X;
	state->X_z1 = X;
	state->lp_z1 = lp;
	return lp + lp - x;
}

static inline void bw_allpass1_process(bw_allpass1_coeffs *BW_RESTRICT coeffs, bw_allpass1_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_allpass1_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_allpass1_process1(coeffs, state, x[i]);
}

static inline void bw_allpass1_set_cutoff(bw_allpass1_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->cutoff = value;
}

#ifdef __cplusplus
}
#endif

#endif
