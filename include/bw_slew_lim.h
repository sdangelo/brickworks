/*
 * Brickworks
 *
 * Copyright (C) 2022-2024 Orastron Srl unipersonale
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
 *  version {{{ 1.0.1 }}}
 *  requires {{{ bw_common bw_math }}}
 *  description {{{
 *    Slew-rate limiter with separate maximum increasing and decreasing rates.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.1</strong>:
 *        <ul>
 *          <li>Now using <code>BW_NULL</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_slew_lim_reset_state_multi()</code> and updated
 *              C++ API in this regard.</li>
 *          <li>Now <code>bw_slew_lim_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li>Added <code>bw_slew_lim_process1_none()</code>.</li>
 *          <li><code>bw_slew_lim_process()</code> and
 *              <code>bw_slew_lim_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Fixed setting of default parameter values in
 *              <code>bw_slew_lim_init()</code>.</li>
 *          <li>Fixed documentation of
 *              <code>bw_slew_lim_update_coeffs_audio()</code>.</li>
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
 *          <li>Added <code>bw_slew_lim_process_multi()</code>.</li>
 *          <li>Fixed documentation of <code>bw_slew_lim_process()</code>.</li>
 *          <li>Fixed unused parameter warnings.</li>
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

#ifndef BW_SLEW_LIM_H
#define BW_SLEW_LIM_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_slew_lim_coeffs
 *  ```>>> */
typedef struct bw_slew_lim_coeffs bw_slew_lim_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_slew_lim_state
 *  ```>>> */
typedef struct bw_slew_lim_state bw_slew_lim_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_slew_lim_init()
 *  ```>>> */
static inline void bw_slew_lim_init(
	bw_slew_lim_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_slew_lim_set_sample_rate()
 *  ```>>> */
static inline void bw_slew_lim_set_sample_rate(
	bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	float                            sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_slew_lim_reset_coeffs()
 *  ```>>> */
static inline void bw_slew_lim_reset_coeffs(
	bw_slew_lim_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_slew_lim_reset_state()
 *  ```>>> */
static inline float bw_slew_lim_reset_state(
	const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	bw_slew_lim_state * BW_RESTRICT        state,
	float                                  x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_slew_lim_reset_state_multi()
 *  ```>>> */
static inline void bw_slew_lim_reset_state_multi(
	const bw_slew_lim_coeffs * BW_RESTRICT              coeffs,
	bw_slew_lim_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                       x_0,
	float *                                             y_0,
	size_t                                              n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the
 *    `y_0` array, if not `BW_NULL`.
 *
 *    #### bw_slew_lim_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_slew_lim_update_coeffs_ctrl(
	bw_slew_lim_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_slew_lim_update_coeffs_audio()
 *  ```>>> */
static inline void bw_slew_lim_update_coeffs_audio(
	bw_slew_lim_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_slew_lim_process1\*()
 *  ```>>> */
static inline float bw_slew_lim_process1(
	const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	bw_slew_lim_state * BW_RESTRICT        state,
	float                                  x);

static inline float bw_slew_lim_process1_up(
	const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	bw_slew_lim_state * BW_RESTRICT        state,
	float                                  x);

static inline float bw_slew_lim_process1_down(
	const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	bw_slew_lim_state * BW_RESTRICT        state,
	float                                  x);

static inline float bw_slew_lim_process1_none(
	const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	bw_slew_lim_state * BW_RESTRICT        state,
	float                                  x);
/*! <<<```
 *    These function process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_slew_lim_process1()` assumes that both the maximum upgoing and
 *       downgoing variation rates are finite;
 *     * `bw_slew_lim_process1_up()` assumes that both the maximum upgoing
 *       variation rate is finite and the maximum downgoing variation rate is
 *       infinite;
 *     * `bw_slew_lim_process1_down()` assumes that both the maximum upgoing
 *       variation rate is infinite and the maximum downgoing variation rate is
 *       finite.
 *     * `bw_slew_lim_process1_none()` assumes that both the maximum upgoing and
 *       downgoing variation rates are infinite;
 *
 *    Whether maximum upgoing and downgoing variation rates are actually
 *    infinite is unchecked even for debugging purposes.
 *
 *    #### bw_slew_lim_process()
 *  ```>>> */
static inline void bw_slew_lim_process(
	bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	bw_slew_lim_state * BW_RESTRICT  state,
	const float *                    x,
	float *                          y,
	size_t                           n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    `y` may be `BW_NULL`.
 *
 *    #### bw_slew_lim_process_multi()
 *  ```>>> */
static inline void bw_slew_lim_process_multi(
	bw_slew_lim_coeffs * BW_RESTRICT                    coeffs,
	bw_slew_lim_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                               x,
	float * const *                                     y,
	size_t                                              n_channels,
	size_t                                              n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    `y` or any element of `y` may be `BW_NULL`.
 *
 *    #### bw_slew_lim_set_max_rate()
 *  ```>>> */
static inline void bw_slew_lim_set_max_rate(
	bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets both the maximum increasing and decreasing variation rate to the
 *    given `value` (1/s) in `coeffs`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    This is equivalent to calling both `bw_slew_lim_set_max_inc_rate()` and
 *    `bw_slew_lim_set_max_dec_rate()` with same `coeffs` and `value`.
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_max_rate_up()
 *  ```>>> */
static inline void bw_slew_lim_set_max_rate_up(
	bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets the maximum increasing variation rate to the given `value` (1/s) in
 *    `coeffs`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_max_inc_rate()
 *  ```>>> */
static inline void bw_slew_lim_set_max_rate_down(
	bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets the maximum decreasing variation rate to the given `value` (1/s) in
 *    `coeffs`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    Default value: `INFINITY`.
 *
 *    #### bw_slew_lim_get_y_z1()
 *  ```>>> */
static inline float bw_slew_lim_get_y_z1(
	const bw_slew_lim_state * BW_RESTRICT state);
/*! <<<```
 *    Returns the last output sample as stored in `state`.
 *
 *    #### bw_slew_lim_coeffs_is_valid()
 *  ```>>> */
static inline char bw_slew_lim_coeffs_is_valid(
	const bw_slew_lim_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_slew_lim_coeffs`.
 *
 *    #### bw_slew_lim_state_is_valid()
 *  ```>>> */
static inline char bw_slew_lim_state_is_valid(
	const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	const bw_slew_lim_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `BW_NULL` extra cross-checks might be performed
 *    (`state` is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_slew_lim_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_slew_lim_coeffs_state {
	bw_slew_lim_coeffs_state_invalid,
	bw_slew_lim_coeffs_state_init,
	bw_slew_lim_coeffs_state_set_sample_rate,
	bw_slew_lim_coeffs_state_reset_coeffs
};
#endif

struct bw_slew_lim_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_slew_lim_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Coefficients
	float				T;

	float				max_inc;
	float				max_dec;

	// Parameters
	float				max_rate_up;
	float				max_rate_down;
};

struct bw_slew_lim_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// States
	float		y_z1;
};

static inline void bw_slew_lim_init(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	coeffs->max_rate_up = INFINITY;
	coeffs->max_rate_down = INFINITY;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_slew_lim_coeffs");
	coeffs->state = bw_slew_lim_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_slew_lim_coeffs_state_init);
}

static inline void bw_slew_lim_set_sample_rate(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		float                            sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	coeffs->T = 1.f / sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_slew_lim_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_slew_lim_coeffs_state_set_sample_rate);
}

static inline void bw_slew_lim_do_update_coeffs_ctrl(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs) {
	// tracking parameter changes is more trouble than it's worth
	coeffs->max_inc = coeffs->T * coeffs->max_rate_up;
	coeffs->max_dec = coeffs->T * coeffs->max_rate_down;
}

static inline void bw_slew_lim_reset_coeffs(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_set_sample_rate);

	bw_slew_lim_do_update_coeffs_ctrl(coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_slew_lim_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_slew_lim_coeffs_state_reset_coeffs);
}

static inline float bw_slew_lim_reset_state(
		const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		bw_slew_lim_state * BW_RESTRICT        state,
		float                                  x_0) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT(bw_is_finite(x_0));

	(void)coeffs;
	const float y = x_0;
	state->y_z1 = x_0;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_slew_lim_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_slew_lim_reset_state_multi(
		const bw_slew_lim_coeffs * BW_RESTRICT              coeffs,
		bw_slew_lim_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                       x_0,
		float *                                             y_0,
		size_t                                              n_channels) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != BW_NULL);

	if (y_0 != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_slew_lim_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_slew_lim_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != BW_NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_slew_lim_update_coeffs_ctrl(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);

	bw_slew_lim_do_update_coeffs_ctrl(coeffs);

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
}

static inline void bw_slew_lim_update_coeffs_audio(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline float bw_slew_lim_process1(
		const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		bw_slew_lim_state * BW_RESTRICT        state,
		float                                  x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(bw_is_finite(coeffs->max_inc) && bw_is_finite(coeffs->max_dec));

	const float y = bw_clipf(x, state->y_z1 - coeffs->max_dec, state->y_z1 + coeffs->max_inc);
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_slew_lim_process1_up(
		const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		bw_slew_lim_state * BW_RESTRICT        state,
		float                                  x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(bw_is_finite(coeffs->max_inc));

	const float y = bw_minf(x, state->y_z1 + coeffs->max_inc);
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_slew_lim_process1_down(
		const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		bw_slew_lim_state * BW_RESTRICT        state,
		float                                  x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(bw_is_finite(coeffs->max_dec));

	const float y = bw_maxf(x, state->y_z1 - coeffs->max_dec);
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_slew_lim_process1_none(
	const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
	bw_slew_lim_state * BW_RESTRICT        state,
	float                                  x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	(void)coeffs;
	state->y_z1 = x;

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));

	return x;
}

static inline void bw_slew_lim_process(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		bw_slew_lim_state * BW_RESTRICT  state,
		const float *                    x,
		float *                          y,
		size_t                           n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));

	bw_slew_lim_update_coeffs_ctrl(coeffs);
	if (y != BW_NULL) {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t i = 0; i < n_samples; i++)
					y[i] = bw_slew_lim_process1(coeffs, state, x[i]);
			else
				for (size_t i = 0; i < n_samples; i++)
					y[i] = bw_slew_lim_process1_up(coeffs, state, x[i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t i = 0; i < n_samples; i++)
					y[i] = bw_slew_lim_process1_down(coeffs, state, x[i]);
			else {
				for (size_t i = 0; i < n_samples; i++)
					y[i] = x[i];
				state->y_z1 = x[n_samples - 1];
			}
		}
	} else {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t i = 0; i < n_samples; i++)
					bw_slew_lim_process1(coeffs, state, x[i]);
			else
				for (size_t i = 0; i < n_samples; i++)
					bw_slew_lim_process1_up(coeffs, state, x[i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t i = 0; i < n_samples; i++)
					bw_slew_lim_process1_down(coeffs, state, x[i]);
			else
				state->y_z1 = x[n_samples - 1];
		}
	}

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(y != BW_NULL ? bw_has_only_finite(y, n_samples) : 1);
}

static inline void bw_slew_lim_process_multi(
		bw_slew_lim_coeffs * BW_RESTRICT                    coeffs,
		bw_slew_lim_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                               x,
		float * const *                                     y,
		size_t                                              n_channels,
		size_t                                              n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x != BW_NULL);
#ifndef BW_NO_DEBUG
	if (y != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = i + 1; j < n_channels; j++)
				BW_ASSERT(y[i] == BW_NULL || y[j] == BW_NULL || y[i] != y[j]);
#endif

	bw_slew_lim_update_coeffs_ctrl(coeffs);
	if (y != BW_NULL) {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t j = 0; j < n_channels; j++)
					if (y[j] != BW_NULL)
						for (size_t i = 0; i < n_samples; i++)
							y[j][i] = bw_slew_lim_process1(coeffs, state[j], x[j][i]);
					else
						for (size_t i = 0; i < n_samples; i++)
							bw_slew_lim_process1(coeffs, state[j], x[j][i]);
			else
				for (size_t j = 0; j < n_channels; j++)
					for (size_t i = 0; i < n_samples; i++)
						if (y[j] != BW_NULL)
							y[j][i] = bw_slew_lim_process1_up(coeffs, state[j], x[j][i]);
						else
							bw_slew_lim_process1_up(coeffs, state[j], x[j][i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t j = 0; j < n_channels; j++)
					if (y[j] != BW_NULL)
						for (size_t i = 0; i < n_samples; i++)
							y[j][i] = bw_slew_lim_process1_down(coeffs, state[j], x[j][i]);
					else
						for (size_t i = 0; i < n_samples; i++)
							bw_slew_lim_process1_down(coeffs, state[j], x[j][i]);
			else
				for (size_t j = 0; j < n_channels; j++) {
					BW_ASSERT(state[j] != BW_NULL);
					BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state[j]));
					BW_ASSERT(x[j] != BW_NULL);
					BW_ASSERT_DEEP(bw_has_only_finite(x[j], n_samples));

					if (y[j] != BW_NULL)
						for (size_t i = 0; i < n_samples; i++)
							y[j][i] = x[j][i];
					state[j]->y_z1 = x[j][n_samples - 1];

					BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state[j]));
					BW_ASSERT_DEEP(y[j] != BW_NULL ? bw_has_only_finite(y[j], n_samples) : 1);
				}
		}
	} else {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t j = 0; j < n_channels; j++)
					for (size_t i = 0; i < n_samples; i++)
						bw_slew_lim_process1(coeffs, state[j], x[j][i]);
			else
				for (size_t j = 0; j < n_channels; j++)
					for (size_t i = 0; i < n_samples; i++)
						bw_slew_lim_process1_up(coeffs, state[j], x[j][i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t j = 0; j < n_channels; j++)
					for (size_t i = 0; i < n_samples; i++)
						bw_slew_lim_process1_down(coeffs, state[j], x[j][i]);
			else
				for (size_t j = 0; j < n_channels; j++) {
					BW_ASSERT(state[j] != BW_NULL);
					BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state[j]));
					BW_ASSERT(x[j] != BW_NULL);
					BW_ASSERT_DEEP(bw_has_only_finite(x[j], n_samples));

					state[j]->y_z1 = x[j][n_samples - 1];

					BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(coeffs, state[j]));
				}
		}
	}

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs);
}

static inline void bw_slew_lim_set_max_rate(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	bw_slew_lim_set_max_rate_up(coeffs, value);
	bw_slew_lim_set_max_rate_down(coeffs, value);

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_init);
}

static inline void bw_slew_lim_set_max_rate_up(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	coeffs->max_rate_up = value;

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_init);
}

static inline void bw_slew_lim_set_max_rate_down(
		bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	coeffs->max_rate_down = value;

	BW_ASSERT_DEEP(bw_slew_lim_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_slew_lim_coeffs_state_init);
}

static inline float bw_slew_lim_get_y_z1(
		const bw_slew_lim_state * BW_RESTRICT state) {
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_slew_lim_state_is_valid(BW_NULL, state));

	return state->y_z1;
}

static inline char bw_slew_lim_coeffs_is_valid(
		const bw_slew_lim_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_slew_lim_coeffs"))
		return 0;
	if (coeffs->state < bw_slew_lim_coeffs_state_init || coeffs->state > bw_slew_lim_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (bw_is_nan(coeffs->max_rate_up) || coeffs->max_rate_up < 0.f)
		return 0;
	if (bw_is_nan(coeffs->max_rate_down) || coeffs->max_rate_down < 0.f)
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_slew_lim_coeffs_state_set_sample_rate) {
		if (!bw_is_finite(coeffs->T) || coeffs->T <= 0.f)
			return 0;
	}

	if (coeffs->state >= bw_slew_lim_coeffs_state_reset_coeffs) {
		if (bw_is_nan(coeffs->max_inc) || coeffs->max_inc < 0.f)
			return 0;
		if (bw_is_nan(coeffs->max_dec) || coeffs->max_dec < 0.f)
			return 0;
	}
#endif

	return 1;
}

static inline char bw_slew_lim_state_is_valid(
		const bw_slew_lim_coeffs * BW_RESTRICT coeffs,
		const bw_slew_lim_state * BW_RESTRICT  state) {
	BW_ASSERT(state != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_slew_lim_state"))
		return 0;

	if (coeffs != BW_NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	(void)coeffs;

	return bw_is_finite(state->y_z1);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::SlewLim
 *  ```>>> */
template<size_t N_CHANNELS>
class SlewLim {
public:
	SlewLim();

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

	void setMaxRate(
		float value);

	void setMaxRateUp(
		float value);

	void setMaxRateDown(
		float value);

	float getYZ1(
		size_t channel);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_slew_lim_coeffs		coeffs;
	bw_slew_lim_state		states[N_CHANNELS];
	bw_slew_lim_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline SlewLim<N_CHANNELS>::SlewLim() {
	bw_slew_lim_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_slew_lim_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_slew_lim_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_slew_lim_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_slew_lim_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_slew_lim_reset_coeffs(&coeffs);
	bw_slew_lim_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_slew_lim_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setMaxRate(
		float value) {
	bw_slew_lim_set_max_rate(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setMaxRateUp(
		float value) {
	bw_slew_lim_set_max_rate_up(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setMaxRateDown(
		float value) {
	bw_slew_lim_set_max_rate_down(&coeffs, value);
}

template<size_t N_CHANNELS>
inline float SlewLim<N_CHANNELS>::getYZ1(
		size_t channel) {
	return bw_slew_lim_get_y_z1(states + channel);
}

}
#endif

#endif
