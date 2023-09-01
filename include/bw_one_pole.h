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
 *  version {{{ 1.0.0 }}}
 *  requires {{{ bw_common bw_math }}}
 *  description {{{
 *    One-pole (6 dB/oct) lowpass filter with unitary DC gain, separate attack
 *    and decay time constants, and sticky target-reach threshold.
 *
 *    This is better suited to implement smoothing than [bw_lp1](bw_lp1).
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_one_pole_reset_state_multi()</code> and updated
 *              C++ API in this regard.</li>
 *          <li>Now <code>bw_one_pole_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overladed C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Now using backward Euler rather than impulse invariant
 *              method.</li>
 *          <li>Clearly specified parameter validity ranges.</li>
 *          <li>Added more debugging code and added `coeffs` argument to
 *              <code>bw_one_pole_state_is_valid()</code>.</li>
 *          <li>Added pragmas to silence bogus GCC uninitialized variable
 *              warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li><code>bw_one_pole_process()</code> and
 *              <code>bw_one_pole_process_multi()</code> now use
 *              <code>BW_SIZE_T</code> to count samples and channels.</li>
 *          <li>Added debugging code.</li>
 *          <li>Removed dependency on bw_config.</li>
 *          <li>Fixed bug when setting very high cutoff values.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_one_pole_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
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

#ifndef BW_ONE_POLE_H
#define BW_ONE_POLE_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*** Public API ***/

/*! api {{{
 *    #### bw_one_pole_coeffs
 *  ```>>> */
typedef struct bw_one_pole_coeffs bw_one_pole_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_one_pole_state
 *  ```>>> */
typedef struct bw_one_pole_state bw_one_pole_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_one_pole_sticky_mode
 *  ```>>> */
typedef enum {
	bw_one_pole_sticky_mode_abs,
	bw_one_pole_sticky_mode_rel
} bw_one_pole_sticky_mode;
/*! <<<```
 *    Distance metrics for sticky behavior:
 *     * `bw_one_pole_sticky_mode_abs`: absolute difference (|`out` - `in`|);
 *     * `bw_one_pole_sticky_mode_rel`: relative difference with respect to
 *       input (|`out` - `in`| / |`in`|).
 *
 *    #### bw_one_pole_init()
 *  ```>>> */
static inline void bw_one_pole_init(
	bw_one_pole_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_one_pole_set_sample_rate()
 *  ```>>> */
static inline void bw_one_pole_set_sample_rate(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	float                            sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_one_pole_reset_coeffs()
 *  ```>>> */
static inline void bw_one_pole_reset_coeffs(
	bw_one_pole_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_one_pole_reset_state()
 *  ```>>> */
static inline float bw_one_pole_reset_state(
	const bw_one_pole_coeffs * BW_RESTRICT coeffs,
	bw_one_pole_state * BW_RESTRICT        state,
	float                                  x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x_0`.
 *
 *    Returns the corresponding quiescent/initial output value.
 *
 *    #### bw_one_pole_reset_state_multi()
 *  ```>>> */
static inline void bw_one_pole_reset_state_multi(
	const bw_one_pole_coeffs * BW_RESTRICT              coeffs,
	bw_one_pole_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                       x_0,
	float *                                             y_0,
	size_t                                              n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding quiescent/initial input value in the
 *    `x_0` array.
 *
 *    The corresponding quiescent/initial output values are written into the
 *    `y_0` array, if not `NULL`.
 *
 *    #### bw_one_pole_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_one_pole_update_coeffs_ctrl(
	bw_one_pole_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_one_pole_update_coeffs_audio()
 *  ```>>> */
static inline void bw_one_pole_update_coeffs_audio(
	bw_one_pole_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_one_pole_process1\*()
 *  ```>>> */
static inline float bw_one_pole_process1(
	const bw_one_pole_coeffs * BW_RESTRICT coeffs,
	bw_one_pole_state * BW_RESTRICT        state,
	float                                  x);

static inline float bw_one_pole_process1_sticky_abs(
	const bw_one_pole_coeffs * BW_RESTRICT coeffs,
	bw_one_pole_state * BW_RESTRICT        state,
	float                                  x);

static inline float bw_one_pole_process1_sticky_rel(
	const bw_one_pole_coeffs * BW_RESTRICT coeffs,
	bw_one_pole_state * BW_RESTRICT        state,
	float                                  x);

static inline float bw_one_pole_process1_asym(
	const bw_one_pole_coeffs * BW_RESTRICT coeffs,
	bw_one_pole_state * BW_RESTRICT        state,
	float                                  x);

static inline float bw_one_pole_process1_asym_sticky_abs(
	const bw_one_pole_coeffs * BW_RESTRICT coeffs,
	bw_one_pole_state * BW_RESTRICT        state,
	float                                  x);

static inline float bw_one_pole_process1_asym_sticky_rel(
	const bw_one_pole_coeffs * BW_RESTRICT coeffs,
	bw_one_pole_state * BW_RESTRICT        state,
	float                                  x);
/*! <<<```
 *    These functions process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_one_pole_process1()` assumes that upgoing and downgoing cutoff/tau
 *       are equal and the target-reach threshold is `0.f`;
 *     * `bw_one_pole_process1_sticky_abs()` assumes that upgoing and downgoing
 *       cutoff/tau are equal, that the target-reach threshold is not `0.f`,
 *       and that the distance metric for sticky behavior is set to
 *       `bw_one_pole_sticky_mode_abs`;
 *     * `bw_one_pole_process1_sticky_rel()` assumes that upgoing and downgoing
 *       cutoff/tau are equal, that the target-reach threshold is not `0.f`,
 *       and that the distance metric for sticky behavior is set to
 *       `bw_one_pole_sticky_mode_rel`;
 *     * `bw_one_pole_process1_asym()` assumes that upgoing and downgoing
 *       cutoff/tau are different and the target-reach threshold is `0.f`;
 *     * `bw_one_pole_process1_asym_sticky_abs()` assumes that upgoing and
 *       downgoing cutoff/tau are different, that the target-reach threshold is
 *       not `0.f`, and that the distance metric for sticky behavior is set to
 *       `bw_one_pole_sticky_mode_abs`;
 *     * `bw_one_pole_process1_asym_sticky_rel()` assumes that upgoing and
 *       downgoing cutoff/tau are different, that the target-reach threshold is
 *       not `0.f`, and that the distance metric for sticky behavior is set to
 *       `bw_one_pole_sticky_mode_rel`.
 *
 *    #### bw_one_pole_process()
 *  ```>>> */
static inline void bw_one_pole_process(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	bw_one_pole_state * BW_RESTRICT  state,
	const float *                    x,
	float *                          y,
	size_t                           n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    `y` may be `NULL`.
 *
 *    #### bw_one_pole_process_multi()
 *  ```>>> */
static inline void bw_one_pole_process_multi(
	bw_one_pole_coeffs * BW_RESTRICT                    coeffs,
	bw_one_pole_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    `y` or any element of `y` may be `NULL`.
 *
 *    #### bw_one_pole_set_cutoff()
 *  ```>>> */
static inline void bw_one_pole_set_cutoff(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets both the upgoing (attack) and downgoing (decay) cutoff frequency to
 *    the given `value` (Hz) in `coeffs`.
 *
 *    This is equivalent to calling both `bw_one_pole_set_cutoff_up()` and
 *    `bw_one_pole_set_cutoff_down()` with same `coeffs` and `value` or calling
 *    `bw_one_pole_set_tau()` with same `coeffs` and
 *    value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    `value` must be non-negative.
 *
 *    Default value: `INFINITY`.
 *
 *    #### bw_one_pole_set_cutoff_up()
 *  ```>>> */
static inline void bw_one_pole_set_cutoff_up(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets the upgoing (attack) cutoff frequency to the given `value` (Hz) in
 *    `coeffs`.
 *
 *    This is equivalent to calling `bw_one_pole_set_tau_up()` with same
 *    `coeffs` and value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    `value` must be non-negative.
 *
 *    Default value: `INFINITY`.
 *
 *    #### bw_one_pole_set_cutoff_down()
 *  ```>>> */
static inline void bw_one_pole_set_cutoff_down(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets the downgoing (attack) cutoff frequency to the given `value` (Hz) in
 *    `coeffs`.
 *
 *    This is equivalent to calling `bw_one_pole_set_tau_down()` with same
 *    `coeffs` and value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    `value` must be non-negative.
 *
 *    Default value: `INFINITY`.
 *
 *    #### bw_one_pole_set_tau()
 *  ```>>> */
static inline void bw_one_pole_set_tau(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets both the upgoing (attack) and downgoing (decay) time constant to the
 *    given `value` (s) in `coeffs`.
 *
 *    This is equivalent to calling both `bw_one_pole_set_tau_up()` and
 *    `bw_one_pole_set_tau_down()` with same `coeffs` and `value` or calling
 *    `bw_one_pole_set_cutoff()` with same `coeffs` and
 *    value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    `value` must be non-negative.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_one_pole_set_tau_up()
 *  ```>>> */
static inline void bw_one_pole_set_tau_up(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets the upgoing (attack) time constant to the given `value` (s) in
 *    `coeffs`.
 *
 *    This is equivalent to calling `bw_one_pole_set_cutoff_up()` with same
 *    `coeffs` and value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    `value` must be non-negative.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_one_pole_set_tau_down()
 *  ```>>> */
static inline void bw_one_pole_set_tau_down(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets the downgoing (decay) time constant to the given `value` (s) in
 *    `coeffs`.
 *
 *    This is equivalent to calling `bw_one_pole_set_cutoff_down()` with same
 *    `coeffs` and value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    `value` must be non-negative.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_one_pole_set_sticky_thresh()
 *  ```>>> */
static inline void bw_one_pole_set_sticky_thresh(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets the target-reach threshold specified by `value` in `coeffs`.
 *
 *    When the difference between the output and the input would fall under such
 *    threshold according to the current distance metric (see
 *    `bw_one_pole_set_sticky_mode()`), the output is forcefully set to be equal
 *    to the input value.
 *
 *    Valid range: [`0.f`, `1e18f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_one_pole_set_sticky_mode()
 *  ```>>> */
static inline void bw_one_pole_set_sticky_mode(
	bw_one_pole_coeffs * BW_RESTRICT coeffs,
	bw_one_pole_sticky_mode          value);
/*! <<<```
 *    Sets the current distance metric for sticky behavior to `value` in
 *    `coeffs`.
 *
 *    Default value: `bw_one_pole_sticky_mode_abs`.
 *
 *    #### bw_one_pole_get_y_z1()
 *  ```>>> */
static inline float bw_one_pole_get_y_z1(
	const bw_one_pole_state * BW_RESTRICT state);
/*! <<<```
 *    Returns the last output sample as stored in `state`.
 *
 *    #### bw_one_pole_coeffs_is_valid()
 *  ```>>> */
static inline char bw_one_pole_coeffs_is_valid(
	const bw_one_pole_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_one_pole_coeffs`.
 *
 *    #### bw_one_pole_state_is_valid()
 *  ```>>> */
static inline char bw_one_pole_state_is_valid(
	const bw_one_pole_coeffs * BW_RESTRICT coeffs,
	const bw_one_pole_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_one_pole_state`.
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
enum bw_one_pole_coeffs_state {
	bw_one_pole_coeffs_state_invalid,
	bw_one_pole_coeffs_state_init,
	bw_one_pole_coeffs_state_set_sample_rate,
	bw_one_pole_coeffs_state_reset_coeffs
};
#endif

struct bw_one_pole_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_one_pole_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Coefficients
	float				fs_2pi;

	float				mA1u;
	float				mA1d;
	float				st2;

	// Parameters
	float				cutoff_up;
	float				cutoff_down;
	float				sticky_thresh;
	bw_one_pole_sticky_mode		sticky_mode;
	int				param_changed;
};

struct bw_one_pole_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	float		y_z1;
};

#define BW_ONE_POLE_PARAM_CUTOFF_UP	1
#define BW_ONE_POLE_PARAM_CUTOFF_DOWN	(1<<1)
#define BW_ONE_POLE_PARAM_STICKY_THRESH	(1<<2)

static inline void bw_one_pole_init(
		bw_one_pole_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	coeffs->cutoff_up = INFINITY;
	coeffs->cutoff_down = INFINITY;
	coeffs->sticky_thresh = 0.f;
	coeffs->sticky_mode = bw_one_pole_sticky_mode_abs;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_one_pole_coeffs");
	coeffs->state = bw_one_pole_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_one_pole_coeffs_state_init);
}

static inline void bw_one_pole_set_sample_rate(
		bw_one_pole_coeffs * BW_RESTRICT coeffs,
		float                            sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	coeffs->fs_2pi = 0.15915494309189535f * sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_one_pole_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_one_pole_coeffs_state_set_sample_rate);
}

static inline void bw_one_pole_do_update_coeffs_ctrl(
		bw_one_pole_coeffs * BW_RESTRICT coeffs) {
	if (coeffs->param_changed) {
		if (coeffs->param_changed & BW_ONE_POLE_PARAM_CUTOFF_UP)
			coeffs->mA1u = coeffs->cutoff_up > 1.591549430918953e8f ? 0.f : coeffs->fs_2pi * bw_rcpf(coeffs->fs_2pi + coeffs->cutoff_up);
			// tau < 1 ns is instantaneous for any practical purpose
		if (coeffs->param_changed & BW_ONE_POLE_PARAM_CUTOFF_DOWN)
			coeffs->mA1d = coeffs->cutoff_down > 1.591549430918953e8f ? 0.f : coeffs->fs_2pi * bw_rcpf(coeffs->fs_2pi + coeffs->cutoff_down);
			// as before
		if (coeffs->param_changed & BW_ONE_POLE_PARAM_STICKY_THRESH)
			coeffs->st2 = coeffs->sticky_thresh * coeffs->sticky_thresh;
		coeffs->param_changed = 0;
	}
}

static inline void bw_one_pole_reset_coeffs(
		bw_one_pole_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_set_sample_rate);

	coeffs->param_changed = ~0;
	bw_one_pole_do_update_coeffs_ctrl(coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_one_pole_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_one_pole_coeffs_state_reset_coeffs);
}

static inline float bw_one_pole_reset_state(
		const bw_one_pole_coeffs * BW_RESTRICT coeffs,
		bw_one_pole_state * BW_RESTRICT        state,
		float                                  x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(bw_is_finite(x_0));

	(void)coeffs;
	const float y = x_0;
	state->y_z1 = x_0;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_one_pole_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_one_pole_reset_state_multi(
		const bw_one_pole_coeffs * BW_RESTRICT              coeffs,
		bw_one_pole_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                       x_0,
		float *                                             y_0,
		size_t                                              n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_one_pole_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_one_pole_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_one_pole_update_coeffs_ctrl(
		bw_one_pole_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);

	bw_one_pole_do_update_coeffs_ctrl(coeffs);

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
}

static inline void bw_one_pole_update_coeffs_audio(
		bw_one_pole_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline float bw_one_pole_process1(
		const bw_one_pole_coeffs * BW_RESTRICT coeffs,
		bw_one_pole_state * BW_RESTRICT        state,
		float                                  x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT_DEEP(coeffs->mA1u == coeffs->mA1d && coeffs->st2 == 0.f);

	const float y = x + coeffs->mA1u * (state->y_z1 - x);
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_one_pole_process1_sticky_abs(
		const bw_one_pole_coeffs * BW_RESTRICT coeffs,
		bw_one_pole_state * BW_RESTRICT        state,
		float                                  x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT_DEEP(coeffs->mA1u == coeffs->mA1d && coeffs->st2 != 0.f && coeffs->sticky_mode == bw_one_pole_sticky_mode_abs);

	float y = x + coeffs->mA1u * (state->y_z1 - x);
	const float d = y - x;
	if (d * d <= coeffs->st2)
		y = x;
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_one_pole_process1_sticky_rel(
		const bw_one_pole_coeffs * BW_RESTRICT coeffs,
		bw_one_pole_state * BW_RESTRICT        state,
		float                                  x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT_DEEP(coeffs->mA1u == coeffs->mA1d && coeffs->st2 != 0.f && coeffs->sticky_mode == bw_one_pole_sticky_mode_rel);

	float y = x + coeffs->mA1u * (state->y_z1 - x);
	const float d = y - x;
	if (d * d <= coeffs->st2 * x * x)
		y = x;
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_one_pole_process1_asym(
		const bw_one_pole_coeffs * BW_RESTRICT coeffs,
		bw_one_pole_state * BW_RESTRICT        state,
		float                                  x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT_DEEP(coeffs->mA1u != coeffs->mA1d && coeffs->st2 == 0.f);

	const float y = x + (x >= state->y_z1 ? coeffs->mA1u : coeffs->mA1d) * (state->y_z1 - x);
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_one_pole_process1_asym_sticky_abs(
		const bw_one_pole_coeffs * BW_RESTRICT coeffs,
		bw_one_pole_state * BW_RESTRICT        state,
		float                                  x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT_DEEP(coeffs->mA1u != coeffs->mA1d && coeffs->st2 != 0.f && coeffs->sticky_mode == bw_one_pole_sticky_mode_abs);

	float y = x + (x >= state->y_z1 ? coeffs->mA1u : coeffs->mA1d) * (state->y_z1 - x);
	const float d = y - x;
	if (d * d <= coeffs->st2)
		y = x;
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_one_pole_process1_asym_sticky_rel(
		const bw_one_pole_coeffs * BW_RESTRICT coeffs,
		bw_one_pole_state * BW_RESTRICT        state,
		float                                  x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT_DEEP(coeffs->mA1u != coeffs->mA1d && coeffs->st2 != 0.f && coeffs->sticky_mode == bw_one_pole_sticky_mode_rel);

	float y = x + (x >= state->y_z1 ? coeffs->mA1u : coeffs->mA1d) * (state->y_z1 - x);
	const float d = y - x;
	if (d * d <= coeffs->st2 * x * x)
		y = x;
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_one_pole_process(
		bw_one_pole_coeffs * BW_RESTRICT coeffs,
		bw_one_pole_state * BW_RESTRICT  state,
		const float *                    x,
		float *                          y,
		size_t                           n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));

	bw_one_pole_update_coeffs_ctrl(coeffs);
	if (y != NULL) {
		if (coeffs->mA1u != coeffs->mA1d) {
			if (coeffs->st2 != 0.f) {
				if (coeffs->sticky_mode == bw_one_pole_sticky_mode_abs)
					for (size_t i = 0; i < n_samples; i++)
						y[i] = bw_one_pole_process1_asym_sticky_abs(coeffs, state, x[i]);
				else
					for (size_t i = 0; i < n_samples; i++)
						y[i] = bw_one_pole_process1_asym_sticky_rel(coeffs, state, x[i]);
			}
			else {
				for (size_t i = 0; i < n_samples; i++)
					y[i] = bw_one_pole_process1_asym(coeffs, state, x[i]);
			}
		}
		else {
			if (coeffs->st2 != 0.f) {
				if (coeffs->sticky_mode == bw_one_pole_sticky_mode_abs)
					for (size_t i = 0; i < n_samples; i++)
						y[i] = bw_one_pole_process1_sticky_abs(coeffs, state, x[i]);
				else
					for (size_t i = 0; i < n_samples; i++)
						y[i] = bw_one_pole_process1_sticky_rel(coeffs, state, x[i]);
			}
			else {
				for (size_t i = 0; i < n_samples; i++)
					y[i] = bw_one_pole_process1(coeffs, state, x[i]);
			}
		}
	} else {
		if (coeffs->mA1u != coeffs->mA1d) {
			if (coeffs->st2 != 0.f) {
				if (coeffs->sticky_mode == bw_one_pole_sticky_mode_abs)
					for (size_t i = 0; i < n_samples; i++)
						bw_one_pole_process1_asym_sticky_abs(coeffs, state, x[i]);
				else
					for (size_t i = 0; i < n_samples; i++)
						bw_one_pole_process1_asym_sticky_rel(coeffs, state, x[i]);
			}
			else {
				for (size_t i = 0; i < n_samples; i++)
					bw_one_pole_process1_asym(coeffs, state, x[i]);
			}
		}
		else {
			if (coeffs->st2 != 0.f) {
				if (coeffs->sticky_mode == bw_one_pole_sticky_mode_abs)
					for (size_t i = 0; i < n_samples; i++)
						bw_one_pole_process1_sticky_abs(coeffs, state, x[i]);
				else
					for (size_t i = 0; i < n_samples; i++)
						bw_one_pole_process1_sticky_rel(coeffs, state, x[i]);
			}
			else {
				for (size_t i = 0; i < n_samples; i++)
					bw_one_pole_process1(coeffs, state, x[i]);
			}
		}
	}

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(y != NULL ? bw_has_only_finite(y, n_samples) : 1);
}

static inline void bw_one_pole_process_multi(
		bw_one_pole_coeffs * BW_RESTRICT                    coeffs,
		bw_one_pole_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                               x,
		float * const *                                     y,
		size_t                                              n_channels,
		size_t                                              n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(x != NULL);

	bw_one_pole_update_coeffs_ctrl(coeffs);
	if (y != NULL) {
		if (coeffs->mA1u != coeffs->mA1d) {
			if (coeffs->st2 != 0.f) {
				if (coeffs->sticky_mode == bw_one_pole_sticky_mode_abs)
					for (size_t j = 0; j < n_channels; j++)
						if (y[j] != NULL)
							for (size_t i = 0; i < n_samples; i++)
								y[j][i] = bw_one_pole_process1_asym_sticky_abs(coeffs, state[j], x[j][i]);
						else
							for (size_t i = 0; i < n_samples; i++)
								bw_one_pole_process1_asym_sticky_abs(coeffs, state[j], x[j][i]);

				else
					for (size_t j = 0; j < n_channels; j++)
						if (y[j] != NULL)
							for (size_t i = 0; i < n_samples; i++)
								y[j][i] = bw_one_pole_process1_asym_sticky_rel(coeffs, state[j], x[j][i]);
						else
							for (size_t i = 0; i < n_samples; i++)
								bw_one_pole_process1_asym_sticky_rel(coeffs, state[j], x[j][i]);
			}
			else {
				for (size_t j = 0; j < n_channels; j++)
					if (y[j] != NULL)
						for (size_t i = 0; i < n_samples; i++)
							y[j][i] = bw_one_pole_process1_asym(coeffs, state[j], x[j][i]);
					else
						for (size_t i = 0; i < n_samples; i++)
							bw_one_pole_process1_asym(coeffs, state[j], x[j][i]);
			}
		}
		else {
			if (coeffs->st2 != 0.f) {
				if (coeffs->sticky_mode == bw_one_pole_sticky_mode_abs)
					for (size_t j = 0; j < n_channels; j++)
						if (y[j] != NULL)
							for (size_t i = 0; i < n_samples; i++)
								y[j][i] = bw_one_pole_process1_sticky_abs(coeffs, state[j], x[j][i]);
						else
							for (size_t i = 0; i < n_samples; i++)
								bw_one_pole_process1_sticky_abs(coeffs, state[j], x[j][i]);
				else
					for (size_t j = 0; j < n_channels; j++)
						if (y[j] != NULL)
							for (size_t i = 0; i < n_samples; i++)
								y[j][i] = bw_one_pole_process1_sticky_rel(coeffs, state[j], x[j][i]);
						else
							for (size_t i = 0; i < n_samples; i++)
								bw_one_pole_process1_sticky_rel(coeffs, state[j], x[j][i]);
			}
			else {
				for (size_t j = 0; j < n_channels; j++)
					if (y[j] != NULL)
						for (size_t i = 0; i < n_samples; i++)
							y[j][i] = bw_one_pole_process1(coeffs, state[j], x[j][i]);
					else
						for (size_t i = 0; i < n_samples; i++)
							bw_one_pole_process1(coeffs, state[j], x[j][i]);
			}
		}
	} else {
		if (coeffs->mA1u != coeffs->mA1d) {
			if (coeffs->st2 != 0.f) {
				if (coeffs->sticky_mode == bw_one_pole_sticky_mode_abs)
					for (size_t i = 0; i < n_samples; i++)
						for (size_t j = 0; j < n_channels; j++)
							bw_one_pole_process1_asym_sticky_abs(coeffs, state[j], x[j][i]);
				else
					for (size_t i = 0; i < n_samples; i++)
						for (size_t j = 0; j < n_channels; j++)
							bw_one_pole_process1_asym_sticky_rel(coeffs, state[j], x[j][i]);
			}
			else {
				for (size_t i = 0; i < n_samples; i++)
					for (size_t j = 0; j < n_channels; j++)
						bw_one_pole_process1_asym(coeffs, state[j], x[j][i]);
			}
		}
		else {
			if (coeffs->st2 != 0.f) {
				if (coeffs->sticky_mode == bw_one_pole_sticky_mode_abs)
					for (size_t i = 0; i < n_samples; i++)
						for (size_t j = 0; j < n_channels; j++)
							bw_one_pole_process1_sticky_abs(coeffs, state[j], x[j][i]);
				else
					for (size_t i = 0; i < n_samples; i++)
						for (size_t j = 0; j < n_channels; j++)
							bw_one_pole_process1_sticky_rel(coeffs, state[j], x[j][i]);
			}
			else {
				for (size_t i = 0; i < n_samples; i++)
					for (size_t j = 0; j < n_channels; j++)
						bw_one_pole_process1(coeffs, state[j], x[j][i]);
			}
		}
	}

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs);
}

static inline void bw_one_pole_set_cutoff(bw_one_pole_coeffs *BW_RESTRICT coeffs, float value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	bw_one_pole_set_cutoff_up(coeffs, value);
	bw_one_pole_set_cutoff_down(coeffs, value);

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
}

static inline void bw_one_pole_set_cutoff_up(
		bw_one_pole_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	if (coeffs->cutoff_up != value) {
		coeffs->cutoff_up = value;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
		coeffs->param_changed |= BW_ONE_POLE_PARAM_CUTOFF_UP;
#pragma GCC diagnostic pop
	}

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
}

static inline void bw_one_pole_set_cutoff_down(
		bw_one_pole_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	if (coeffs->cutoff_down != value) {
		coeffs->cutoff_down = value;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
		coeffs->param_changed |= BW_ONE_POLE_PARAM_CUTOFF_DOWN;
#pragma GCC diagnostic pop
	}

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
}

static inline void bw_one_pole_set_tau(
		bw_one_pole_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	bw_one_pole_set_tau_up(coeffs, value);
	bw_one_pole_set_tau_down(coeffs, value);

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
}

static inline void bw_one_pole_set_tau_up(
		bw_one_pole_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	bw_one_pole_set_cutoff_up(coeffs, value < 1e-9f ? INFINITY : 0.1591549430918953f * bw_rcpf(value));
	// tau < 1 ns is instantaneous for any practical purpose

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
}

static inline void bw_one_pole_set_tau_down(
		bw_one_pole_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	bw_one_pole_set_cutoff_down(coeffs, value < 1e-9f ? INFINITY : 0.1591549430918953f * bw_rcpf(value));
	// as before

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
}

static inline void bw_one_pole_set_sticky_thresh(
		bw_one_pole_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f && value <= 1e18f);

	if (coeffs->sticky_thresh != value) {
		coeffs->sticky_thresh = value;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
		coeffs->param_changed |= BW_ONE_POLE_PARAM_STICKY_THRESH;
#pragma GCC diagnostic pop
	}

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
}

static inline void bw_one_pole_set_sticky_mode(
		bw_one_pole_coeffs * BW_RESTRICT coeffs,
		bw_one_pole_sticky_mode          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
	BW_ASSERT(value == bw_one_pole_sticky_mode_abs || value == bw_one_pole_sticky_mode_rel);

	coeffs->sticky_mode = value;

	BW_ASSERT_DEEP(bw_one_pole_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_one_pole_coeffs_state_init);
}

static inline float bw_one_pole_get_y_z1(
		const bw_one_pole_state * BW_RESTRICT state) {
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_one_pole_state_is_valid(NULL, state));

	return state->y_z1;
}

static inline char bw_one_pole_coeffs_is_valid(
		const bw_one_pole_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_one_pole_coeffs"))
		return 0;
	if (coeffs->state < bw_one_pole_coeffs_state_init || coeffs->state > bw_one_pole_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (coeffs->cutoff_up < 0.f)
		return 0;
	if (coeffs->cutoff_down < 0.f)
		return 0;
	if (coeffs->sticky_thresh < 0.f || coeffs->sticky_thresh > 1e18f)
		return 0;
	if (coeffs->sticky_mode != bw_one_pole_sticky_mode_abs && coeffs->sticky_mode != bw_one_pole_sticky_mode_rel)
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_one_pole_coeffs_state_set_sample_rate) {
		if (!bw_is_finite(coeffs->fs_2pi) || coeffs->fs_2pi <= 0.f)
			return 0;
	}

	if (coeffs->state >= bw_one_pole_coeffs_state_reset_coeffs) {
		if (!bw_is_finite(coeffs->mA1u) || coeffs->mA1u < 0.f || coeffs->mA1u > 1.f)
			return 0;
		if (!bw_is_finite(coeffs->mA1d) || coeffs->mA1d < 0.f || coeffs->mA1d > 1.f)
			return 0;
		if (!bw_is_finite(coeffs->st2) || coeffs->st2 < 0.f)
			return 0;
	}
#endif

	return 1;
}

static inline char bw_one_pole_state_is_valid(
		const bw_one_pole_coeffs * BW_RESTRICT coeffs,
		const bw_one_pole_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_one_pole_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	(void)coeffs;

	return bw_is_finite(state->y_z1);
}

#undef BW_ONE_POLE_PARAM_CUTOFF_UP
#undef BW_ONE_POLE_PARAM_CUTOFF_DOWN
#undef BW_ONE_POLE_PARAM_STICKY_THRESH

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::OnePole
 *  ```>>> */
template<size_t N_CHANNELS>
class OnePole {
public:
	OnePole();

	void setSampleRate(
		float sampleRate);

	void reset(
		float   x0 = 0.f,
		float * y0 = nullptr);

	void reset(
		float                          x0,
		std::array<float, N_CHANNELS>& y0);

	void reset(
		const float * x0,
		float *       y0 = nullptr);

	void reset(
		std::array<float, N_CHANNELS>  x0,
		std::array<float, N_CHANNELS>& y0);

	void process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples);

	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);

	void setCutoff(
		float value);

	void setCutoffUp(
		float value);

	void setCutoffDown(
		float value);

	void setTau(
		float value);

	void setTauUp(
		float value);

	void setTauDown(
		float value);

	void setStickyThresh(
		float value);

	void setStickyMode(
		bw_one_pole_sticky_mode value);

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
	bw_one_pole_coeffs		coeffs;
	bw_one_pole_state		states[N_CHANNELS];
	bw_one_pole_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline OnePole<N_CHANNELS>::OnePole() {
	bw_one_pole_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_one_pole_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::reset(
		float   x0,
		float * y0) {
	bw_one_pole_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_one_pole_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_one_pole_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::reset(
		float                          x0,
		std::array<float, N_CHANNELS>& y0) {
	reset(x0, y0.data());
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_one_pole_reset_coeffs(&coeffs);
	bw_one_pole_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>  x0,
		std::array<float, N_CHANNELS>& y0) {
	reset(x0.data(), y0.data());
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_one_pole_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::setCutoff(
		float value) {
	bw_one_pole_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::setCutoffUp(
		float value) {
	bw_one_pole_set_cutoff_up(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::setCutoffDown(
		float value) {
	bw_one_pole_set_cutoff_down(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::setTau(
		float value) {
	bw_one_pole_set_tau(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::setTauUp(
		float value) {
	bw_one_pole_set_tau_up(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::setTauDown(
		float value) {
	bw_one_pole_set_tau_down(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::setStickyThresh(
		float value) {
	bw_one_pole_set_sticky_thresh(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void OnePole<N_CHANNELS>::setStickyMode(
		bw_one_pole_sticky_mode value) {
	bw_one_pole_set_sticky_mode(&coeffs, value);
}

template<size_t N_CHANNELS>
inline float OnePole<N_CHANNELS>::getYZ1(
		size_t channel) {
	return bw_one_pole_get_y_z1(states + channel);
}

}
#endif

#endif
