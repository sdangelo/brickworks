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
 *  version {{{ 1.0.0 }}}
 *  requires {{{ bw_buf bw_common bw_math }}}
 *  description {{{
 *    Interpolated delay line, not smoothed.
 *
 *    You can either use the usual API for updating coefficients and processing
 *    signals or you can directly write and read from the delay line which,
 *    for example, allows you to implement smoothing and multi-tap output.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Removed <code>read()</code> and <code>write()</code> from C++
 *              API.</li>
 *          <li>Added initial value argument in
 *              <code>bw_delay_reset_state()</code>.</li>
 *          <li>Added <code>bw_delay_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_delay_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *          <li><code>bw_delay_process()</code> and
 *              <code>bw_delay_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Clearly specified parameter validity ranges.</li>
 *          <li>Added debugging code.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_delay_process_multi()</code>.</li>
 *          <li>Updated mem_req/set API.</li>
 *          <li>Added C++ wrapper.</li>
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

#ifndef BW_DELAY_H
#define BW_DELAY_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_delay_coeffs
 *  ```>>> */
typedef struct bw_delay_coeffs bw_delay_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_delay_state
 *  ```>>> */
typedef struct bw_delay_state bw_delay_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_delay_init()
 *  ```>>> */
static inline void bw_delay_init(
	bw_delay_coeffs * BW_RESTRICT coeffs,
	float                         max_delay);
/*! <<<```
 *    Initializes input parameter values in `coeffs` using `max_delay` (s) as
 *    the maximum delay time.
 *
 *    `max_delay` must be finite and non-negative.
 *
 *    #### bw_delay_set_sample_rate()
 *  ```>>> */
static inline void bw_delay_set_sample_rate(
	bw_delay_coeffs * BW_RESTRICT coeffs,
	float                         sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_delay_mem_req()
 *  ```>>> */
static inline size_t bw_delay_mem_req(
	const bw_delay_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the size, in bytes, of contiguous memory to be supplied to
 *    `bw_delay_mem_set()` using `coeffs`.
 *
 *    #### bw_delay_mem_set()
 *  ```>>> */
static inline void bw_delay_mem_set(
	const bw_delay_coeffs * BW_RESTRICT coeffs,
	bw_delay_state * BW_RESTRICT        state,
	void * BW_RESTRICT                  mem);
/*! <<<```
 *    Associates the contiguous memory block `mem` to the given `state` using
 *    `coeffs`.
 *
 *    #### bw_delay_reset_coeffs()
 *  ```>>> */
static inline void bw_delay_reset_coeffs(
	bw_delay_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_delay_reset_state()
 *  ```>>> */
static inline float bw_delay_reset_state(
	const bw_delay_coeffs * BW_RESTRICT coeffs,
	bw_delay_state * BW_RESTRICT        state,
	float                               x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_delay_reset_state_multi()
 *  ```>>> */
static inline void bw_delay_reset_state_multi(
	const bw_delay_coeffs * BW_RESTRICT              coeffs,
	bw_delay_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                    x_0,
	float *                                          y_0,
	size_t                                           n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `NULL`.
 *
 *    #### bw_delay_read()
 *  ```>>> */
static float bw_delay_read(
	const bw_delay_coeffs * BW_RESTRICT coeffs,
	const bw_delay_state * BW_RESTRICT  state,
	size_t                              di,
	float                               df);
/*! <<<```
 *    Returns the interpolated value read from the delay line identified by
 *    `coeffs` and `state` by applying a delay of `di` + `df` samples.
 *
 *    `df` must be in [`0.f`, `1.f`) and `di` + `df` must not exceed the delay
 *    line length (`max_delay * sample_rate`).
 *
 *    #### bw_delay_write()
 *  ```>>> */
static void bw_delay_write(
	const bw_delay_coeffs * BW_RESTRICT coeffs,
	bw_delay_state * BW_RESTRICT        state,
	float                               x);
/*! <<<```
 *    Pushes the new sample `x` on the delay line identified by `coeffs` and
 *    `state`.
 *
 *    #### bw_delay_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_delay_update_coeffs_ctrl(
	bw_delay_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_delay_update_coeffs_audio()
 *  ```>>> */
static inline void bw_delay_update_coeffs_audio(
	bw_delay_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_delay_process1()
 *  ```>>> */
static inline float bw_delay_process1(
	const bw_delay_coeffs * BW_RESTRICT coeffs,
	bw_delay_state * BW_RESTRICT        state,
	float                               x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_delay_process()
 *  ```>>> */
static inline void bw_delay_process(
	bw_delay_coeffs * BW_RESTRICT coeffs,
	bw_delay_state * BW_RESTRICT  state,
	const float *                 x,
	float *                       y,
	size_t                        n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_delay_process_multi()
 *  ```>>> */
static inline void bw_delay_process_multi(
	bw_delay_coeffs * BW_RESTRICT                    coeffs,
	bw_delay_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                            x,
	float * const *                                  y,
	size_t                                           n_channels,
	size_t                                           n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_delay_set_delay()
 *  ```>>> */
static inline void bw_delay_set_delay(
	bw_delay_coeffs * BW_RESTRICT coeffs,
	float                         value);
/*! <<<```
 *    Sets the delay time `value` (s) in `coeffs`.
 *
 *    Valid range: [`0.f`, `max_delay`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_delay_get_length()
 *  ```>>> */
static inline size_t bw_delay_get_length(
	const bw_delay_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the length of the delay line in samples as stored in `coeffs`.
 *
 *    `coeffs` must be at least in the "sample-rate-set" state.
 *
 *    #### bw_delay_coeffs_is_valid()
 *  ```>>> */
static inline char bw_delay_coeffs_is_valid(
	const bw_delay_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_delay_coeffs`.
 *
 *    #### bw_delay_state_is_valid()
 *  ```>>> */
static inline char bw_delay_state_is_valid(
	const bw_delay_coeffs * BW_RESTRICT coeffs,
	const bw_delay_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_delay_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_buf.h>
#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_delay_coeffs_state {
	bw_delay_coeffs_state_invalid,
	bw_delay_coeffs_state_init,
	bw_delay_coeffs_state_set_sample_rate,
	bw_delay_coeffs_state_reset_coeffs
};
#endif

#ifdef BW_DEBUG_DEEP
enum bw_delay_state_state {
	bw_delay_state_state_invalid,
	bw_delay_state_state_mem_set,
	bw_delay_state_state_reset_state
};
#endif

struct bw_delay_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_delay_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Coefficients
	float				fs;
	size_t				len;

	size_t				di;
	float				df;

	// Parameters
	float				max_delay;
	float				delay;
	char				delay_changed;
};

struct bw_delay_state {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_delay_state_state	state;
	uint32_t			coeffs_reset_id;
#endif

	// States
	float * BW_RESTRICT		buf;
	size_t				idx;
};

static inline void bw_delay_init(
		bw_delay_coeffs * BW_RESTRICT coeffs,
		float                         max_delay) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT(bw_is_finite(max_delay));
	BW_ASSERT(max_delay >= 0.f);

	coeffs->max_delay = max_delay;
	coeffs->delay = 0.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_delay_coeffs");
	coeffs->state = bw_delay_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_delay_coeffs_state_init);
}

static inline void bw_delay_set_sample_rate(
		bw_delay_coeffs * BW_RESTRICT coeffs,
		float                         sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	coeffs->fs = sample_rate;
	coeffs->len = (size_t)bw_ceilf(coeffs->fs * coeffs->max_delay) + 1;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_delay_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_delay_coeffs_state_set_sample_rate);
}

static inline size_t bw_delay_mem_req(
		const bw_delay_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_set_sample_rate);

	return coeffs->len * sizeof(float);
}

static inline void bw_delay_mem_set(
		const bw_delay_coeffs * BW_RESTRICT coeffs,
		bw_delay_state * BW_RESTRICT        state,
		void * BW_RESTRICT                  mem) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_set_sample_rate);
	BW_ASSERT(state != NULL);
	BW_ASSERT(mem != NULL);

	(void)coeffs;
	state->buf = (float *)mem;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_delay_state");
	state->state = bw_delay_state_state_mem_set;
#endif
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_set_sample_rate);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state == bw_delay_state_state_mem_set);
}

static inline void bw_delay_do_update_coeffs_ctrl(bw_delay_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->delay_changed) {
		float i;
		bw_intfracf(coeffs->fs * coeffs->delay, &i, &coeffs->df);
		coeffs->di = (size_t)i;
		coeffs->delay_changed = 0;
	}
}

static inline void bw_delay_reset_coeffs(
		bw_delay_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_set_sample_rate);

	coeffs->delay_changed = 1;
	bw_delay_do_update_coeffs_ctrl(coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_delay_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_delay_coeffs_state_reset_coeffs);
}

static inline float bw_delay_reset_state(
		const bw_delay_coeffs * BW_RESTRICT coeffs,
		bw_delay_state * BW_RESTRICT        state,
		float                               x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_mem_set);
	BW_ASSERT(bw_is_finite(x_0));

	bw_buf_fill(x_0, state->buf, coeffs->len);
	state->idx = 0;
	const float y = x_0;

#ifdef BW_DEBUG_DEEP
	state->state = bw_delay_state_state_reset_state;
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_reset_state);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_delay_reset_state_multi(
		const bw_delay_coeffs * BW_RESTRICT              coeffs,
		bw_delay_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                    x_0,
		float *                                          y_0,
		size_t                                           n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_delay_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_delay_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static float bw_delay_read(
		const bw_delay_coeffs * BW_RESTRICT coeffs,
		const bw_delay_state * BW_RESTRICT  state,
		size_t                              di,
		float                               df) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_reset_state);
	BW_ASSERT(bw_is_finite(df));
	BW_ASSERT(df >= 0.f && df < 1.f);
	BW_ASSERT(di + df <= coeffs->len);

	const size_t n = (state->idx + (state->idx >= di ? 0 : coeffs->len)) - di;
	const size_t p = (n ? n : coeffs->len) - 1;
	const float y = state->buf[n] + df * (state->buf[p] - state->buf[n]);

	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_reset_state);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static void bw_delay_write(
		const bw_delay_coeffs * BW_RESTRICT coeffs,
		bw_delay_state * BW_RESTRICT        state,
		float                               x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_reset_state);
	BW_ASSERT(bw_is_finite(x));

	state->idx++;
	state->idx = state->idx == coeffs->len ? 0 : state->idx;
	state->buf[state->idx] = x;

	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_reset_state);
}

static inline void bw_delay_update_coeffs_ctrl(
		bw_delay_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);

	bw_delay_do_update_coeffs_ctrl(coeffs);

	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
}

static inline void bw_delay_update_coeffs_audio(
		bw_delay_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline float bw_delay_process1(
		const bw_delay_coeffs * BW_RESTRICT coeffs,
		bw_delay_state * BW_RESTRICT        state,
		float                               x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_reset_state);
	BW_ASSERT(bw_is_finite(x));

	bw_delay_write(coeffs, state, x);
	const float y = bw_delay_read(coeffs, state, coeffs->di, coeffs->df);

	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_reset_state);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_delay_process(
		bw_delay_coeffs * BW_RESTRICT coeffs,
		bw_delay_state * BW_RESTRICT  state,
		const float *                 x,
		float *                       y,
		size_t                        n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_reset_state);
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != NULL);

	bw_delay_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++)
		y[i] = bw_delay_process1(coeffs, state, x[i]);

	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_delay_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_delay_state_state_reset_state);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_delay_process_multi(
		bw_delay_coeffs * BW_RESTRICT                    coeffs,
		bw_delay_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                            x,
		float * const *                                  y,
		size_t                                           n_channels,
		size_t                                           n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x != NULL);
	BW_ASSERT(y != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(y[i] != y[j]);
#endif

	bw_delay_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++)
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_delay_process1(coeffs, state[j], x[j][i]);

	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_reset_coeffs);
}

static inline void bw_delay_set_delay(
		bw_delay_coeffs * BW_RESTRICT coeffs,
		float                         value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	if (value != coeffs->delay) {
		coeffs->delay = value;
		coeffs->delay_changed = 1;
	}

	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_init);
}

static inline size_t bw_delay_get_length(
		const bw_delay_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_delay_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_delay_coeffs_state_set_sample_rate);

	return coeffs->len;
}

static inline char bw_delay_coeffs_is_valid(
		const bw_delay_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_delay_coeffs"))
		return 0;
	if (coeffs->state < bw_delay_coeffs_state_init || coeffs->state > bw_delay_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->max_delay) || coeffs->max_delay <= 0.f)
		return 0;
	if (!bw_is_finite(coeffs->delay) || coeffs->delay < 0.f || coeffs->delay > coeffs->max_delay)
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_delay_coeffs_state_set_sample_rate) {
		if (!bw_is_finite(coeffs->fs) || coeffs->fs <= 0.f)
			return 0;
		if (coeffs->len == 0)
			return 0;
	}

	if (coeffs->state >= bw_delay_coeffs_state_reset_coeffs) {
		if (!bw_is_finite(coeffs->df) || coeffs->df < 0.f || coeffs->df >= 1.f)
			return 0;
		if (coeffs->di + coeffs->df > coeffs->len)
			return 0;
	}
#endif

	return 1;
}

static inline char bw_delay_state_is_valid(
		const bw_delay_coeffs * BW_RESTRICT coeffs,
		const bw_delay_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_delay_state"))
		return 0;
	if (state->state < bw_delay_state_state_mem_set || state->state > bw_delay_state_state_reset_state)
		return 0;
#endif

	(void)coeffs;

	if (state->buf == NULL)
		return 0;

#ifdef BW_DEBUG_DEEP
	if (state->state >= bw_delay_state_state_reset_state && coeffs != NULL) {
		if (coeffs->reset_id != state->coeffs_reset_id)
			return 0;

		if (state->idx >= coeffs->len)
			return 0;
	}
#endif

	return 1;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Delay
 *  ```>>> */
template<size_t N_CHANNELS>
class Delay {
public:
	Delay(
		float maxDelay = 1.f);

	~Delay();

	void setSampleRate(
		float sampleRate);

	void reset(
		float               x0 = 0.f,
		float * BW_RESTRICT y0 = nullptr);

	void reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0);

	void reset(
		const float * x0,
		float *       y0 = nullptr);

	void reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0 = nullptr);

	void process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples);

	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);

	void setDelay(
		float value);

	size_t getLength();
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_delay_coeffs			coeffs;
	bw_delay_state			states[N_CHANNELS];
	bw_delay_state * BW_RESTRICT	statesP[N_CHANNELS];
	void * BW_RESTRICT		mem;
};

template<size_t N_CHANNELS>
inline Delay<N_CHANNELS>::Delay(float maxDelay) {
	bw_delay_init(&coeffs, maxDelay);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
	mem = nullptr;
}

template<size_t N_CHANNELS>
inline Delay<N_CHANNELS>::~Delay() {
	if (mem != nullptr)
		operator delete(mem);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_delay_set_sample_rate(&coeffs, sampleRate);
	size_t req = bw_delay_mem_req(&coeffs);
	if (mem != nullptr)
		operator delete(mem);
	mem = operator new(req * N_CHANNELS);
	void *m = mem;
	for (size_t i = 0; i < N_CHANNELS; i++, m = static_cast<char *>(m) + req)
		bw_delay_mem_set(&coeffs, states + i, m);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_delay_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_delay_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_delay_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_delay_reset_coeffs(&coeffs);
	bw_delay_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_delay_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::setDelay(
		float value) {
	bw_delay_set_delay(&coeffs, value);
}

template<size_t N_CHANNELS>
inline size_t Delay<N_CHANNELS>::getLength() {
	return bw_delay_get_length(&coeffs);
}

}
#endif

#endif
