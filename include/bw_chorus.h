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
 *  requires {{{
 *    bw_buf bw_comb bw_common bw_delay bw_gain bw_math bw_one_pole bw_osc_sin
 *    bw_phase_gen
 *  }}}
 *  description {{{
 *    Chorus / vibrato / flanger with variable rate and amount.
 *
 *    It outputs a mix of the dry input signal with itself going through a
 *    modulated delay and an optional feedback, as explained in
 *
 *    J. Dattorro, "Effect Design, Part 2: Delay-Line Modulation and Chorus",
 *    J. Audio Eng. Soc., vol. 45, no. 10, pp. 764-788, October 1997.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added initial value argument in
 *              <code>bw_chorus_reset_state()</code>.</li>
 *          <li>Added <code>bw_chorus_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_chorus_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *          <li><code>bw_chorus_process()</code> and
 *              <code>bw_chorus_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implmenetation.</li>
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
 *          <li>Added <code>bw_chorus_process_multi()</code>.</li>
 *          <li>Updated mem_req/set API.</li>
 *          <li>Now properly setting feedforward delay on reset.</li>
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

#ifndef BW_CHORUS_H
#define BW_CHORUS_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_chorus_coeffs
 *  ```>>> */
typedef struct bw_chorus_coeffs bw_chorus_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_chorus_state
 *  ```>>> */
typedef struct bw_chorus_state bw_chorus_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_chorus_init()
 *  ```>>> */
static inline void bw_chorus_init(
	bw_chorus_coeffs * BW_RESTRICT coeffs,
	float                          max_delay);
/*! <<<```
 *    Initializes input parameter values in `coeffs` using `max_delay` (s) as
 *    the maximum delay time.
 *
 *    `max_delay` must be finite and non-negative.
 *
 *    #### bw_chorus_set_sample_rate()
 *  ```>>> */
static inline void bw_chorus_set_sample_rate(
	bw_chorus_coeffs * BW_RESTRICT coeffs,
	float                          sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_chorus_mem_req()
 *  ```>>> */
static inline size_t bw_chorus_mem_req(
	const bw_chorus_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the size, in bytes, of contiguous memory to be supplied to
 *    `bw_chorus_mem_set()` using `coeffs`.
 *
 *    #### bw_chorus_mem_set()
 *  ```>>> */
static inline void bw_chorus_mem_set(
	const bw_chorus_coeffs * BW_RESTRICT coeffs,
	bw_chorus_state * BW_RESTRICT        state,
	void * BW_RESTRICT                   mem);
/*! <<<```
 *    Associates the contiguous memory block `mem` to the given `state` using
 *    `coeffs`.
 *
 *    #### bw_chorus_reset_coeffs()
 *  ```>>> */
static inline void bw_chorus_reset_coeffs(
	bw_chorus_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_chorus_reset_state()
 *  ```>>> */
static inline float bw_chorus_reset_state(
	const bw_chorus_coeffs * BW_RESTRICT coeffs,
	bw_chorus_state * BW_RESTRICT        state,
	float                                x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    If parameter `coeff_fb`  has value `-1.f` or `1.f`, then `x_0` must be
 *    `0.f`.
 *
 *    #### bw_chorus_reset_state_multi()
 *  ```>>> */
static inline void bw_chorus_reset_state_multi(
	const bw_chorus_coeffs * BW_RESTRICT              coeffs,
	bw_chorus_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                     x_0,
	float *                                           y_0,
	size_t                                            n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `NULL`.
 *
 *    If parameter `coeff_fb`  has value `-1.f` or `1.f`, then `x_0` must only
 *    contain `0.f`.
 *
 *    #### bw_chorus_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_chorus_update_coeffs_ctrl(
	bw_chorus_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_chorus_update_coeffs_audio()
 *  ```>>> */
static inline void bw_chorus_update_coeffs_audio(
	bw_chorus_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_chorus_process1()
 *  ```>>> */
static inline float bw_chorus_process1(
	const bw_chorus_coeffs * BW_RESTRICT coeffs,
	bw_chorus_state * BW_RESTRICT        state,
	float                                x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_chorus_process()
 *  ```>>> */
static inline void bw_chorus_process(
	bw_chorus_coeffs * BW_RESTRICT coeffs,
	bw_chorus_state * BW_RESTRICT  state,
	const float *                  x,
	float *                        y,
	size_t                         n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_chorus_process_multi()
 *  ```>>> */
static inline void bw_chorus_process_multi(
	bw_chorus_coeffs * BW_RESTRICT                    coeffs,
	bw_chorus_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                             x,
	float * const *                                   y,
	size_t                                            n_channels,
	size_t                                            n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_chorus_set_rate()
 *  ```>>> */
static inline void bw_chorus_set_rate(
	bw_chorus_coeffs * BW_RESTRICT coeffs,
	float                          value);
/*! <<<```
 *    Sets the modulation rate `value` (Hz) in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_chorus_set_delay()
 *  ```>>> */
static inline void bw_chorus_set_delay(
	bw_chorus_coeffs * BW_RESTRICT coeffs,
	float                          value);
/*! <<<```
 *    Sets the center delay `value` (s) in `coeffs`.
 *
 *    The actual instantaneous delay is internally limited to avoid buffer
 *    overflows.
 *
 *    `value` must be finite.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_chorus_set_amount()
 *  ```>>> */
static inline void bw_chorus_set_amount(
	bw_chorus_coeffs * BW_RESTRICT coeffs,
	float                          value);
/*! <<<```
 *    Sets the delay modulation amount (semi-amplitude) `value` (s) in `coeffs`.
 *
 *    The actual instantaneous delay is internally limited to avoid buffer
 *    overflows.
 *
 *    `value` must be finite.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_chorus_set_coeff_x()
 *  ```>>> */
static inline void bw_chorus_set_coeff_x(
	bw_chorus_coeffs * BW_RESTRICT coeffs,
	float                          value);
/*! <<<```
 *    Sets the input coefficient `value` in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_chorus_set_coeff_mod()
 *  ```>>> */
static inline void bw_chorus_set_coeff_mod(
	bw_chorus_coeffs * BW_RESTRICT coeffs,
	float                          value);
/*! <<<```
 *    Sets the modulated branch coefficient `value` in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_chorus_set_coeff_fb()
 *  ```>>> */
static inline void bw_chorus_set_coeff_fb(
	bw_chorus_coeffs * BW_RESTRICT coeffs,
	float                          value);
/*! <<<```
 *    Sets the feedback branch coefficient `value` in `coeffs`.
 *
 *    Valid range: [`-1.f`, `1.f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_chorus_coeffs_is_valid()
 *  ```>>> */
static inline char bw_chorus_coeffs_is_valid(
	const bw_chorus_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_chorus_coeffs`.
 *
 *    #### bw_chorus_state_is_valid()
 *  ```>>> */
static inline char bw_chorus_state_is_valid(
	const bw_chorus_coeffs * BW_RESTRICT coeffs,
	const bw_chorus_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_chorus_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_phase_gen.h>
#include <bw_osc_sin.h>
#include <bw_comb.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_chorus_coeffs_state {
	bw_chorus_coeffs_state_invalid,
	bw_chorus_coeffs_state_init,
	bw_chorus_coeffs_state_set_sample_rate,
	bw_chorus_coeffs_state_reset_coeffs
};
#endif

#ifdef BW_DEBUG_DEEP
enum bw_chorus_state_state {
	bw_chorus_state_state_invalid,
	bw_chorus_state_state_mem_set,
	bw_chorus_state_state_reset_state
};
#endif

struct bw_chorus_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_chorus_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_phase_gen_coeffs		phase_gen_coeffs;
	bw_phase_gen_state		phase_gen_state;
	bw_comb_coeffs			comb_coeffs;

	// Parameters
	float				delay;
	float				amount;
};

struct bw_chorus_state {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_chorus_state_state	state;
	uint32_t			coeffs_reset_id;
#endif

	// Sub-components
	bw_comb_state			comb_state;
};

static inline void bw_chorus_init(
		bw_chorus_coeffs * BW_RESTRICT coeffs,
		float                          max_delay) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT(bw_is_finite(max_delay));
	BW_ASSERT(max_delay >= 0.f);

	bw_phase_gen_init(&coeffs->phase_gen_coeffs);
	bw_comb_init(&coeffs->comb_coeffs, max_delay);
	coeffs->delay = 0.f;
	coeffs->amount = 0.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_chorus_coeffs");
	coeffs->state = bw_chorus_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_chorus_coeffs_state_init);
}

static inline void bw_chorus_set_sample_rate(
		bw_chorus_coeffs * BW_RESTRICT coeffs,
		float                          sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_phase_gen_set_sample_rate(&coeffs->phase_gen_coeffs, sample_rate);
	bw_comb_set_sample_rate(&coeffs->comb_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_chorus_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_chorus_coeffs_state_set_sample_rate);
}

static inline size_t bw_chorus_mem_req(
		const bw_chorus_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_set_sample_rate);

	return bw_comb_mem_req(&coeffs->comb_coeffs);
}

static inline void bw_chorus_mem_set(
		const bw_chorus_coeffs * BW_RESTRICT coeffs,
		bw_chorus_state * BW_RESTRICT        state,
		void * BW_RESTRICT                   mem) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_set_sample_rate);
	BW_ASSERT(state != NULL);
	BW_ASSERT(mem != NULL);

	bw_comb_mem_set(&coeffs->comb_coeffs, &state->comb_state, mem);

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_chorus_state");
	state->state = bw_chorus_state_state_mem_set;
#endif
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_set_sample_rate);
	BW_ASSERT_DEEP(bw_chorus_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state == bw_chorus_state_state_mem_set);
}

static inline void bw_chorus_reset_coeffs(
		bw_chorus_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_set_sample_rate);

	bw_phase_gen_reset_coeffs(&coeffs->phase_gen_coeffs);
	float p, pi;
	bw_phase_gen_reset_state(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state, 0.f, &p, &pi);
	const float mod = coeffs->delay + coeffs->amount * bw_osc_sin_process1(p);
	bw_comb_set_delay_ff(&coeffs->comb_coeffs, mod);
	bw_comb_reset_coeffs(&coeffs->comb_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_chorus_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_chorus_coeffs_state_reset_coeffs);
}

static inline float bw_chorus_reset_state(
		const bw_chorus_coeffs * BW_RESTRICT coeffs,
		bw_chorus_state * BW_RESTRICT        state,
		float                                x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_chorus_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_chorus_state_state_mem_set);
	BW_ASSERT(bw_is_finite(x_0));

	const float y = bw_comb_reset_state(&coeffs->comb_coeffs, &state->comb_state, x_0);

#ifdef BW_DEBUG_DEEP
	state->state = bw_chorus_state_state_reset_state;
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_chorus_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_chorus_state_state_reset_state);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_chorus_reset_state_multi(
		const bw_chorus_coeffs * BW_RESTRICT              coeffs,
		bw_chorus_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                     x_0,
		float *                                           y_0,
		size_t                                            n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_chorus_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_chorus_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_chorus_update_coeffs_ctrl(
		bw_chorus_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);

	bw_phase_gen_update_coeffs_ctrl(&coeffs->phase_gen_coeffs);
	bw_comb_update_coeffs_ctrl(&coeffs->comb_coeffs);

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
}

static inline void bw_chorus_update_coeffs_audio(
		bw_chorus_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);

	bw_phase_gen_update_coeffs_audio(&coeffs->phase_gen_coeffs);
	float p, pi;
	bw_phase_gen_process1(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state, &p, &pi);
	const float mod = coeffs->delay + coeffs->amount * bw_osc_sin_process1(p);
	bw_comb_set_delay_ff(&coeffs->comb_coeffs, mod);
	bw_comb_update_coeffs_ctrl(&coeffs->comb_coeffs);
	bw_comb_update_coeffs_audio(&coeffs->comb_coeffs);

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
}

static inline float bw_chorus_process1(
		const bw_chorus_coeffs * BW_RESTRICT coeffs,
		bw_chorus_state * BW_RESTRICT        state,
		float                                x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_chorus_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_chorus_state_state_reset_state);
	BW_ASSERT(bw_is_finite(x));

	const float y = bw_comb_process1(&coeffs->comb_coeffs, &state->comb_state, x);

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_chorus_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_chorus_state_state_reset_state);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_chorus_process(
		bw_chorus_coeffs * BW_RESTRICT coeffs,
		bw_chorus_state * BW_RESTRICT  state,
		const float *                  x,
		float *                        y,
		size_t                         n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_chorus_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_chorus_state_state_reset_state);
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != NULL);

	bw_chorus_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_chorus_update_coeffs_audio(coeffs);
		y[i] = bw_chorus_process1(coeffs, state, x[i]);
	}

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_chorus_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_chorus_state_state_reset_state);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_chorus_process_multi(
		bw_chorus_coeffs * BW_RESTRICT                    coeffs,
		bw_chorus_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                             x,
		float * const *                                   y,
		size_t                                            n_channels,
		size_t                                            n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
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

	bw_chorus_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_chorus_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_chorus_process1(coeffs, state[j], x[j][i]);
	}

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_reset_coeffs);
}

static inline void bw_chorus_set_rate(
		bw_chorus_coeffs * BW_RESTRICT coeffs,
		float                          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	bw_phase_gen_set_frequency(&coeffs->phase_gen_coeffs, value);

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
}

static inline void bw_chorus_set_delay(
		bw_chorus_coeffs * BW_RESTRICT coeffs,
		float                          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	bw_comb_set_delay_fb(&coeffs->comb_coeffs, value);
	coeffs->delay = value;

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
}

static inline void bw_chorus_set_amount(
		bw_chorus_coeffs * BW_RESTRICT coeffs,
		float                          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	coeffs->amount = value;
}

static inline void bw_chorus_set_coeff_x(
		bw_chorus_coeffs * BW_RESTRICT coeffs,
		float                          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	bw_comb_set_coeff_blend(&coeffs->comb_coeffs, value);

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
}

static inline void bw_chorus_set_coeff_mod(
		bw_chorus_coeffs * BW_RESTRICT coeffs,
		float                          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	bw_comb_set_coeff_ff(&coeffs->comb_coeffs, value);

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
}

static inline void bw_chorus_set_coeff_fb(
		bw_chorus_coeffs * BW_RESTRICT coeffs,
		float                          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= -1.f && value <= 1.f);

	bw_comb_set_coeff_fb(&coeffs->comb_coeffs, value);

	BW_ASSERT_DEEP(bw_chorus_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_chorus_coeffs_state_init);
}

static inline char bw_chorus_coeffs_is_valid(
		const bw_chorus_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_chorus_coeffs"))
		return 0;
	if (coeffs->state < bw_chorus_coeffs_state_init || coeffs->state > bw_chorus_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->delay))
		return 0;
	if (!bw_is_finite(coeffs->amount))
		return 0;

	if (!bw_phase_gen_coeffs_is_valid(&coeffs->phase_gen_coeffs))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_chorus_coeffs_state_reset_coeffs && !bw_phase_gen_state_is_valid(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state))
		return 0;
#endif

	return bw_comb_coeffs_is_valid(&coeffs->comb_coeffs);
}

static inline char bw_chorus_state_is_valid(
		const bw_chorus_coeffs * BW_RESTRICT coeffs,
		const bw_chorus_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_chorus_state"))
		return 0;
	if (state->state < bw_chorus_state_state_mem_set || state->state > bw_chorus_state_state_reset_state)
		return 0;

	if (state->state >= bw_chorus_state_state_reset_state && coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	return bw_comb_state_is_valid(coeffs ? &coeffs->comb_coeffs : NULL, &state->comb_state);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Chorus
 *  ```>>> */
template<size_t N_CHANNELS>
class Chorus {
public:
	Chorus(
		float maxDelay = 0.01f);

	~Chorus();

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

	void setRate(
		float value);

	void setDelay(
		float value);

	void setAmount(
		float value);

	void setCoeffX(
		float value);

	void setCoeffMod(
		float value);

	void setCoeffFB(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_chorus_coeffs		coeffs;
	bw_chorus_state			states[N_CHANNELS];
	bw_chorus_state * BW_RESTRICT	statesP[N_CHANNELS];
	void * BW_RESTRICT		mem;
};

template<size_t N_CHANNELS>
inline Chorus<N_CHANNELS>::Chorus(
		float maxDelay) {
	bw_chorus_init(&coeffs, maxDelay);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
	mem = nullptr;
}

template<size_t N_CHANNELS>
inline Chorus<N_CHANNELS>::~Chorus() {
	if (mem != nullptr)
		operator delete(mem);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_chorus_set_sample_rate(&coeffs, sampleRate);
	size_t req = bw_chorus_mem_req(&coeffs);
	if (mem != nullptr)
		operator delete(mem);
	mem = operator new(req * N_CHANNELS);
	void *m = mem;
	for (size_t i = 0; i < N_CHANNELS; i++, m = static_cast<char *>(m) + req)
		bw_chorus_mem_set(&coeffs, states + i, m);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_chorus_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_chorus_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_chorus_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_chorus_reset_coeffs(&coeffs);
	bw_chorus_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_chorus_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setRate(
		float value) {
	bw_chorus_set_rate(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setDelay(
		float value) {
	bw_chorus_set_delay(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setAmount(
		float value) {
	bw_chorus_set_amount(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setCoeffX(
		float value) {
	bw_chorus_set_coeff_x(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setCoeffMod(
		float value) {
	bw_chorus_set_coeff_mod(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setCoeffFB(
		float value) {
	bw_chorus_set_coeff_fb(&coeffs, value);
}

}
#endif

#endif
