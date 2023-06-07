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
 *  version {{{ 0.5.0 }}}
 *  requires {{{ bw_buf bw_common bw_config bw_math }}}
 *  description {{{
 *    Interpolated delay line, not smoothed.
 *
 *    You can either use the usual API for updating coefficients and processing
 *    signals or you can directly write and read from the delay line which,
 *    for example, allows you to implement smoothing and multi-tap output.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_delay_process_multi()</code>.</li>
 *          <li>Updated mem_req/set API.</li>
 *        </ul>
 *      </li>
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
 *    #### bw_delay_state
 *  ```>>> */
typedef struct _bw_delay_state bw_delay_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_delay_init()
 *  ```>>> */
static inline void bw_delay_init(bw_delay_coeffs *BW_RESTRICT coeffs, float max_delay);
/*! <<<```
 *    Initializes input parameter values in `coeffs` using `max_delay` (s) as
 *    the maximum delay time.
 *
 *    #### bw_delay_set_sample_rate()
 *  ```>>> */
static inline void bw_delay_set_sample_rate(bw_delay_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_delay_mem_req()
 *  ```>>> */
static inline BW_SIZE_T bw_delay_mem_req(const bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the size, in bytes, of contiguous memory to be supplied to
 *    `bw_delay_mem_set()` using `coeffs`.
 *
 *    #### bw_delay_mem_set()
 *  ```>>> */
static inline void bw_delay_mem_set(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, void *mem);
/*! <<<```
 *    Associates the contiguous memory block `mem` to the given `state`.
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
 *    #### bw_delay_read()
 *  ```>>> */
static float bw_delay_read(const bw_delay_coeffs *BW_RESTRICT coeffs, const bw_delay_state *BW_RESTRICT state, BW_SIZE_T di, float df);
/*! <<<```
 *    Returns the interpolated value read from the delay line identified by
 *    `coeffs` and `state` by applying a delay of `di` + `df` samples.
 *
 *    `df` must be in [`0.f`, `1.f`) and `di` + `df` must not exceed the delay
 *    line length (maximum delay times the sample rate).
 *
 *    #### bw_delay_write()
 *  ```>>> */
static void bw_delay_write(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Pushes the new sample `x` on the delay line identified by `coeffs` and
 *    `state`.
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
 *    #### bw_delay_process()
 *  ```>>> */
static inline void bw_delay_process(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_delay_process_multi()
 *  ```>>> */
static inline void bw_delay_process_multi(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_delay_set_delay()
 *  ```>>> */
static inline void bw_delay_set_delay(bw_delay_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the delay time `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_delay_get_length()
 *  ```>>> */
static inline BW_SIZE_T bw_delay_get_length(const bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the length of the delay line in samples.
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

	BW_SIZE_T	di;
	float		df;

	// Parameters
	float		max_delay;
	float		delay;
	char		delay_changed;
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

static inline BW_SIZE_T bw_delay_mem_req(const bw_delay_coeffs *BW_RESTRICT coeffs) {
	return coeffs->len * sizeof(float);
}

static inline void bw_delay_mem_set(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, void *mem) {
	(void)coeffs;
	state->buf = (float *)mem;
}

static inline void bw_delay_reset_coeffs(bw_delay_coeffs *BW_RESTRICT coeffs) {
	coeffs->delay_changed = 1;
	bw_delay_update_coeffs_ctrl(coeffs);
}

static inline void bw_delay_reset_state(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state) {
	bw_buf_fill(state->buf, 0.f, coeffs->len);
	state->idx = 0;
}

static float bw_delay_read(const bw_delay_coeffs *BW_RESTRICT coeffs, const bw_delay_state *BW_RESTRICT state, BW_SIZE_T di, float df) {
	const BW_SIZE_T n = (state->idx + (state->idx >= di ? 0 : coeffs->len)) - di;
	const BW_SIZE_T p = (n ? n : coeffs->len) - 1;
	return state->buf[n] + df * (state->buf[p] - state->buf[n]);
}

static void bw_delay_write(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x) {
	state->idx++;
	state->idx = state->idx == coeffs->len ? 0 : state->idx;
	state->buf[state->idx] = x;
}

static inline void bw_delay_update_coeffs_ctrl(bw_delay_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->delay_changed) {
		float i;
		bw_intfracf(coeffs->fs * coeffs->delay, &i, &coeffs->df);
		coeffs->di = (BW_SIZE_T)i;
		coeffs->delay_changed = 0;
	}
}

static inline void bw_delay_update_coeffs_audio(bw_delay_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline float bw_delay_process1(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x) {
	bw_delay_write(coeffs, state, x);
	return bw_delay_read(coeffs, state, coeffs->di, coeffs->df);
}

static inline void bw_delay_process(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_delay_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_delay_process1(coeffs, state, x[i]);
}

static inline void bw_delay_process_multi(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples) {
	bw_delay_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++)
		for (int j = 0; j < n_channels; j++)
			y[j][i] = bw_delay_process1(coeffs, state[j], x[j][i]);
}

static inline void bw_delay_set_delay(bw_delay_coeffs *BW_RESTRICT coeffs, float value) {
	if (value != coeffs->delay) {
		coeffs->delay = value;
		coeffs->delay_changed = 1;
	}
}

static inline BW_SIZE_T bw_delay_get_length(const bw_delay_coeffs *BW_RESTRICT coeffs) {
	return coeffs->len;
}

#ifdef __cplusplus
}
#endif

#endif
