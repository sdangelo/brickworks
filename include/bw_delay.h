/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
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
 *  requires {{{ bw_buf bw_config bw_common bw_math }}}
 *  description {{{
 *    Interpolated delay line.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_DELAY_H
#define _BW_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_delay_coeffs
 *  ```>>> */
typedef struct _bw_delay_coeffs bw_delay_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    ### bw_delay_state
 *  ```>>> */
typedef struct _bw_delay_state bw_delay_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_delay_init()
 *  ```>>> */
static inline void bw_delay_init(bw_delay_coeffs *BW_RESTRICT coeffs, float max_delay);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *    XXX
 *
 *    #### bw_delay_set_sample_rate()
 *  ```>>> */
static inline void bw_delay_set_sample_rate(bw_delay_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    ### bw_delay_mem_req()
 *  ```>>> */
static inline BW_SIZE_T bw_delay_mem_req(bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *   XXX
 *
 *    ### bw_delay_mem_set()
 *  ```>>> */
static inline void bw_delay_mem_set(bw_delay_state *BW_RESTRICT state, void *mem);
/*! <<<```
 *   XXX
 *
 *    #### bw_delay_reset_coeffs()
 *  ```>>> */
static inline void bw_delay_reset_coeffs(bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_delay_reset_state()
 *  ```>>> */
static inline void bw_delay_reset_state(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_delay_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_delay_update_coeffs_ctrl(bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_delay_update_coeffs_audio()
 *  ```>>> */
static inline void bw_delay_update_coeffs_audio(bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_delay_process1()
 *  ```>>> */
static inline float bw_delay_process1(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_env_follow_process()
 *  ```>>> */
static inline void bw_delay_process(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_delay_set_delay()
 *  ```>>> */
static inline void bw_delay_set_delay(bw_delay_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    XXX
 *
 *    Default value: `0.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_buf.h>
#include <bw_math.h>

struct _bw_delay_coeffs {
	// Coefficients
	float		fs;
	BW_SIZE_T	len;

	// Parameters
	float		max_delay;
	float		delay;
};

struct _bw_delay_state {
	float		*buf;
	BW_SIZE_T	 idx;
};

static inline void bw_delay_init(bw_delay_coeffs *BW_RESTRICT coeffs, float max_delay) {
	coeffs->max_delay = max_delay;
	coeffs->delay = 0.f;
}

static inline void bw_delay_set_sample_rate(bw_delay_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->fs = sample_rate;
	coeffs->len = (BW_SIZE_T)bw_ceilf(coeffs->fs * coeffs->max_delay) + 1;
}

static inline BW_SIZE_T bw_delay_mem_req(bw_delay_coeffs *BW_RESTRICT coeffs) {
	return coeffs->len * sizeof(float);
}

static inline void bw_delay_mem_set(bw_delay_state *BW_RESTRICT state, void *mem) {
	state->buf = (float *)mem;
}

static inline void bw_delay_reset_coeffs(bw_delay_coeffs *BW_RESTRICT coeffs) {
}

static inline void bw_delay_reset_state(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state) {
	bw_buf_fill(state->buf, 0.f, coeffs->len);
	state->idx = 0;
}

static inline void bw_delay_update_coeffs_ctrl(bw_delay_coeffs *BW_RESTRICT coeffs) {
}

static inline void bw_delay_update_coeffs_audio(bw_delay_coeffs *BW_RESTRICT coeffs) {
}

static inline float bw_delay_process1(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x) {
	// XXX optim, coeffs
	const float s = coeffs->delay * coeffs->fs;
	const float f = bw_floorf(s);
	const float d = s - f;
	const BW_SIZE_T j = (BW_SIZE_T)f;
	const BW_SIZE_T n = (state->idx + (state->idx >= j ? 0 : coeffs->len)) - j;
	const BW_SIZE_T p = (n ? n : coeffs->len) - 1;
	state->buf[state->idx] = x;
	const float y = state->buf[n] + d * (state->buf[p] - state->buf[n]);
	state->idx++;
	state->idx = state->idx == coeffs->len ? 0 : state->idx;
	return y;
}

static inline void bw_delay_process(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_delay_process1(coeffs, state, x[i]);
}

static inline void bw_delay_set_delay(bw_delay_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->delay = value;
}

#ifdef __cplusplus
}
#endif

#endif
