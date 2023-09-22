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
 *    bw_buf bw_common bw_delay bw_gain bw_math bw_one_pole
 *  }}}
 *  description {{{
 *    Comb filter / delay effect with feedforward and feedback paths.
 *
 *    Essentially an implementation of the structure described in
 *
 *    J. Dattorro, "Effect Design, Part 2: Delay-Line Modulation and Chorus",
 *    J. Audio Eng. Soc., vol. 45, no. 10, pp. 764-788, October 1997.
 *
 *    Unlike in the original paper, the feedback signal is not subtracted but
 *    rather added to the input.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added initial value argument in
 *              <code>bw_chorus_reset_state()</code>.</li>
 *          <li>Added <code>bw_comb_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_comb_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *          <li><code>bw_comb_process()</code> and
 *              <code>bw_comb_process_multi()</code> now use <code>size_t</code>
 *              to count samples and channels.</li>
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
 *          <li>Added <code>bw_comb_process_multi()</code>.</li>
 *          <li>Updated mem_req/set API.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_COMB_H
#define BW_COMB_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_comb_coeffs
 *  ```>>> */
typedef struct bw_comb_coeffs bw_comb_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_comb_state
 *  ```>>> */
typedef struct bw_comb_state bw_comb_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_comb_init()
 *  ```>>> */
static inline void bw_comb_init(
	bw_comb_coeffs * BW_RESTRICT coeffs,
	float                        max_delay);
/*! <<<```
 *    Initializes input parameter values in `coeffs` using `max_delay` (s) as
 *    the maximum delay time.
 *
 *    `max_delay` must be finite and non-negative.
 *
 *    #### bw_comb_set_sample_rate()
 *  ```>>> */
static inline void bw_comb_set_sample_rate(
	bw_comb_coeffs * BW_RESTRICT coeffs,
	float                        sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_comb_mem_req()
 *  ```>>> */
static inline size_t bw_comb_mem_req(
	const bw_comb_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the size, in bytes, of contiguous memory to be supplied to
 *    `bw_comb_mem_set()` using `coeffs`.
 *
 *    #### bw_comb_mem_set()
 *  ```>>> */
static inline void bw_comb_mem_set(
	const bw_comb_coeffs * BW_RESTRICT coeffs,
	bw_comb_state * BW_RESTRICT        state,
	void * BW_RESTRICT                 mem);
/*! <<<```
 *    Associates the contiguous memory block `mem` to the given `state` using
 *    `coeffs`.
 *
 *    #### bw_comb_reset_coeffs()
 *  ```>>> */
static inline void bw_comb_reset_coeffs(
	bw_comb_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_comb_reset_state()
 *  ```>>> */
static inline float bw_comb_reset_state(
	const bw_comb_coeffs * BW_RESTRICT coeffs,
	bw_comb_state * BW_RESTRICT        state,
	float                              x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    If parameter `fb`  has value `-1.f` or `1.f`, then `x_0` must be `0.f`.
 *
 *    #### bw_comb_reset_state_multi()
 *  ```>>> */
static inline void bw_comb_reset_state_multi(
	const bw_comb_coeffs * BW_RESTRICT              coeffs,
	bw_comb_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                   x_0,
	float *                                         y_0,
	size_t                                          n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `NULL`.
 *
 *    If parameter `fb`  has value `-1.f` or `1.f`, then `x_0` must only contain
 *    `0.f`.
 *
 *    #### bw_comb_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_comb_update_coeffs_ctrl(
	bw_comb_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_comb_update_coeffs_audio()
 *  ```>>> */
static inline void bw_comb_update_coeffs_audio(
	bw_comb_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_comb_process1()
 *  ```>>> */
static inline float bw_comb_process1(
	const bw_comb_coeffs * BW_RESTRICT coeffs,
	bw_comb_state * BW_RESTRICT        state,
	float                              x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_comb_process()
 *  ```>>> */
static inline void bw_comb_process(
	bw_comb_coeffs * BW_RESTRICT coeffs,
	bw_comb_state * BW_RESTRICT  state,
	const float *                x,
	float *                      y,
	size_t                       n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_comb_process_multi()
 *  ```>>> */
static inline void bw_comb_process_multi(
	bw_comb_coeffs * BW_RESTRICT                    coeffs,
	bw_comb_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                           x,
	float * const *                                 y,
	size_t                                          n_channels,
	size_t                                          n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_comb_set_delay_ff()
 *  ```>>> */
static inline void bw_comb_set_delay_ff(
	bw_comb_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the feedforward delay time `value` (s) in `coeffs`.
 *
 *    This parameter is internally limited to avoid buffer overflows.
 *
 *    `value` must be finite.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comb_set_delay_fb()
 *  ```>>> */
static inline void bw_comb_set_delay_fb(
	bw_comb_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the feedback delay time `value` (s) in `coeffs`.
 *
 *    This parameter is internally limited to avoid buffer overflows.
 *
 *    `value` must be finite.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comb_set_coeff_blend()
 *  ```>>> */
static inline void bw_comb_set_coeff_blend(
	bw_comb_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the blend coefficient `value` in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_comb_set_coeff_ff()
 *  ```>>> */
static inline void bw_comb_set_coeff_ff(
	bw_comb_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the feedforward coefficient `value` in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comb_set_coeff_fb()
 *  ```>>> */
static inline void bw_comb_set_coeff_fb(
	bw_comb_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the feedback coefficient `value` in `coeffs`.
 *
 *    Valid range: [`-1.f`, `1.f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comb_coeffs_is_valid()
 *  ```>>> */
static inline char bw_comb_coeffs_is_valid(
	const bw_comb_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_comb_coeffs`.
 *
 *    #### bw_comb_state_is_valid()
 *  ```>>> */
static inline char bw_comb_state_is_valid(
	const bw_comb_coeffs * BW_RESTRICT coeffs,
	const bw_comb_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_comb_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_delay.h>
#include <bw_gain.h>
#include <bw_one_pole.h>
#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_comb_coeffs_state {
	bw_comb_coeffs_state_invalid,
	bw_comb_coeffs_state_init,
	bw_comb_coeffs_state_set_sample_rate,
	bw_comb_coeffs_state_reset_coeffs
};
#endif

#ifdef BW_DEBUG_DEEP
enum bw_comb_state_state {
	bw_comb_state_state_invalid,
	bw_comb_state_state_mem_set,
	bw_comb_state_state_reset_state
};
#endif

struct bw_comb_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_comb_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_delay_coeffs			delay_coeffs;
	bw_gain_coeffs			blend_coeffs;
	bw_gain_coeffs			ff_coeffs;
	bw_gain_coeffs			fb_coeffs;
	bw_one_pole_coeffs		smooth_coeffs;
	bw_one_pole_state		smooth_delay_ff_state;
	bw_one_pole_state		smooth_delay_fb_state;

	// Coefficients
	float				fs;

	size_t				dffi;
	float				dfff;
	size_t				dfbi;
	float				dfbf;

	// Parameters
	float				delay_ff;
	float				delay_fb;
};

struct bw_comb_state {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_comb_state_state	state;
	uint32_t			coeffs_reset_id;
#endif

	// Sub-components
	bw_delay_state			delay_state;
};

static inline void bw_comb_init(
		bw_comb_coeffs * BW_RESTRICT coeffs,
		float                        max_delay) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT(bw_is_finite(max_delay));
	BW_ASSERT(max_delay >= 0.f);

	bw_delay_init(&coeffs->delay_coeffs, max_delay);
	bw_gain_init(&coeffs->blend_coeffs);
	bw_gain_init(&coeffs->ff_coeffs);
	bw_gain_init(&coeffs->fb_coeffs);
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-6f);
	bw_gain_set_gain_lin(&coeffs->ff_coeffs, 0.f);
	bw_gain_set_gain_lin(&coeffs->fb_coeffs, 0.f);
	coeffs->delay_ff = 0.f;
	coeffs->delay_fb = 0.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_comb_coeffs");
	coeffs->state = bw_comb_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_comb_coeffs_state_init);
}

static inline void bw_comb_set_sample_rate(
		bw_comb_coeffs * BW_RESTRICT coeffs,
		float                        sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_delay_set_sample_rate(&coeffs->delay_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->blend_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->ff_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->fb_coeffs, sample_rate);
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->fs = sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_comb_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_comb_coeffs_state_set_sample_rate);
}

static inline size_t bw_comb_mem_req(
		const bw_comb_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_set_sample_rate);

	return bw_delay_mem_req(&coeffs->delay_coeffs);
}

static inline void bw_comb_mem_set(
		const bw_comb_coeffs * BW_RESTRICT coeffs,
		bw_comb_state * BW_RESTRICT        state,
		void * BW_RESTRICT                 mem) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_set_sample_rate);
	BW_ASSERT(state != NULL);
	BW_ASSERT(mem != NULL);

	bw_delay_mem_set(&coeffs->delay_coeffs, &state->delay_state, mem);

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_comb_state");
	state->state = bw_comb_state_state_mem_set;
#endif
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_set_sample_rate);
	BW_ASSERT_DEEP(bw_comb_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state == bw_comb_state_state_mem_set);
}

static inline void bw_comb_do_update_coeffs(
		bw_comb_coeffs * BW_RESTRICT coeffs,
		char                         force) {
	float delay_ff_cur = bw_one_pole_get_y_z1(&coeffs->smooth_delay_ff_state);
	float delay_fb_cur = bw_one_pole_get_y_z1(&coeffs->smooth_delay_fb_state);
	if (force || delay_ff_cur != coeffs->delay_ff) {
		delay_ff_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_delay_ff_state, coeffs->delay_ff);
		const size_t len = bw_delay_get_length(&coeffs->delay_coeffs);
		const float dff = bw_maxf(coeffs->fs * delay_ff_cur, 0.f);
		float dffif;
		bw_intfracf(dff, &dffif, &coeffs->dfff);
		coeffs->dffi = (size_t)dffif;
		if (coeffs->dffi >= len) {
			coeffs->dffi = len;
			coeffs->dfff = 0.f;
		}
	}
	if (force || delay_fb_cur != coeffs->delay_fb) {
		delay_fb_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_delay_fb_state, coeffs->delay_fb);
		const size_t len = bw_delay_get_length(&coeffs->delay_coeffs);
		const float dfb = bw_maxf(coeffs->fs * delay_fb_cur, 1.f) - 1.f;
		float dfbif;
		bw_intfracf(dfb, &dfbif, &coeffs->dfbf);
		coeffs->dfbi = (size_t)dfbif;
		if (coeffs->dfbi >= len) {
			coeffs->dfbi = len;
			coeffs->dfbf = 0.f;
		}
	}
}

static inline void bw_comb_reset_coeffs(
		bw_comb_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_set_sample_rate);

	bw_delay_reset_coeffs(&coeffs->delay_coeffs);
	bw_gain_reset_coeffs(&coeffs->blend_coeffs);
	bw_gain_reset_coeffs(&coeffs->ff_coeffs);
	bw_gain_reset_coeffs(&coeffs->fb_coeffs);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_delay_ff_state, coeffs->delay_ff);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_delay_fb_state, coeffs->delay_fb);
	bw_comb_do_update_coeffs(coeffs, 1);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_comb_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_comb_coeffs_state_reset_coeffs);
}

static inline float bw_comb_reset_state(
		const bw_comb_coeffs * BW_RESTRICT coeffs,
		bw_comb_state * BW_RESTRICT        state,
		float                              x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_comb_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_comb_state_state_mem_set);
	BW_ASSERT(bw_is_finite(x_0));
	BW_ASSERT(bw_gain_get_gain_cur(&coeffs->fb_coeffs) == -1.f || bw_gain_get_gain_cur(&coeffs->fb_coeffs) == 1.f ? x_0 == 0.f : 1);

	const float fb = bw_gain_get_gain_cur(&coeffs->fb_coeffs);
	float y;
	if (fb == -1.f || fb == 1.f) {
		bw_delay_reset_state(&coeffs->delay_coeffs, &state->delay_state, 0.f);
		y = 0.f;
	} else {
		const float v = x_0 / (1.f - fb);
		bw_delay_reset_state(&coeffs->delay_coeffs, &state->delay_state, v);
		y = (bw_gain_get_gain_cur(&coeffs->ff_coeffs) + bw_gain_get_gain_cur(&coeffs->blend_coeffs)) * v;
	}

#ifdef BW_DEBUG_DEEP
	state->state = bw_comb_state_state_reset_state;
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_comb_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_comb_state_state_reset_state);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_comb_reset_state_multi(
		const bw_comb_coeffs * BW_RESTRICT              coeffs,
		bw_comb_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                   x_0,
		float *                                         y_0,
		size_t                                          n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_comb_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_comb_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_comb_update_coeffs_ctrl(
		bw_comb_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);

	bw_gain_update_coeffs_ctrl(&coeffs->blend_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->ff_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->fb_coeffs);

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
}

static inline void bw_comb_update_coeffs_audio(
		bw_comb_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);

	bw_gain_update_coeffs_audio(&coeffs->blend_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->ff_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->fb_coeffs);
	bw_comb_do_update_coeffs(coeffs, 0);

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
}

static inline float bw_comb_process1(
		const bw_comb_coeffs * BW_RESTRICT coeffs,
		bw_comb_state * BW_RESTRICT        state,
		float                              x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_comb_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_comb_state_state_reset_state);
	BW_ASSERT(bw_is_finite(x));

	const float fb = bw_delay_read(&coeffs->delay_coeffs, &state->delay_state, coeffs->dfbi, coeffs->dfbf);
	const float v = x + bw_gain_process1(&coeffs->fb_coeffs, fb);
	bw_delay_write(&coeffs->delay_coeffs, &state->delay_state, v);
	const float ff = bw_delay_read(&coeffs->delay_coeffs, &state->delay_state, coeffs->dffi, coeffs->dfff);
	const float y = bw_gain_process1(&coeffs->blend_coeffs, v) + bw_gain_process1(&coeffs->ff_coeffs, ff);

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_comb_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_comb_state_state_reset_state);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_comb_process(
		bw_comb_coeffs * BW_RESTRICT coeffs,
		bw_comb_state * BW_RESTRICT  state,
		const float *                x,
		float *                      y,
		size_t                       n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_comb_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_comb_state_state_reset_state);
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != NULL);

	bw_comb_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_comb_update_coeffs_audio(coeffs);
		y[i] = bw_comb_process1(coeffs, state, x[i]);
	}

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_comb_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(state->state >= bw_comb_state_state_reset_state);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_comb_process_multi(
		bw_comb_coeffs * BW_RESTRICT                    coeffs,
		bw_comb_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                           x,
		float * const *                                 y,
		size_t                                          n_channels,
		size_t                                          n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(x != NULL);
	BW_ASSERT(y != NULL);

	bw_comb_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_comb_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_comb_process1(coeffs, state[j], x[j][i]);
	}

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_reset_coeffs);
}

static inline void bw_comb_set_delay_ff(
		bw_comb_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	coeffs->delay_ff = value;

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
}

static inline void bw_comb_set_delay_fb(
		bw_comb_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	coeffs->delay_fb = value;

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
}

static inline void bw_comb_set_coeff_blend(
		bw_comb_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	bw_gain_set_gain_lin(&coeffs->blend_coeffs, value);

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
}

static inline void bw_comb_set_coeff_ff(
		bw_comb_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	bw_gain_set_gain_lin(&coeffs->ff_coeffs, value);

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
}

static inline void bw_comb_set_coeff_fb(
		bw_comb_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= -1.f && value <= 1.f);

	bw_gain_set_gain_lin(&coeffs->fb_coeffs, value);

	BW_ASSERT_DEEP(bw_comb_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comb_coeffs_state_init);
}

static inline char bw_comb_coeffs_is_valid(
		const bw_comb_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_comb_coeffs"))
		return 0;
	if (coeffs->state < bw_comb_coeffs_state_init || coeffs->state > bw_comb_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->delay_ff))
		return 0;
	if (!bw_is_finite(coeffs->delay_fb))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_comb_coeffs_state_set_sample_rate && (!bw_is_finite(coeffs->fs) || coeffs->fs <= 0.f))
		return 0;
#endif

	if (!bw_delay_coeffs_is_valid(&coeffs->delay_coeffs)
		|| !bw_one_pole_coeffs_is_valid(&coeffs->smooth_coeffs))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_comb_coeffs_state_reset_coeffs) {
		const size_t len = bw_delay_get_length(&coeffs->delay_coeffs);

		if (!bw_is_finite(coeffs->dfff) || coeffs->dfff < 0.f || coeffs->dfff >= 1.f)
			return 0;
		if (coeffs->dffi + coeffs->dfff > len)
			return 0;

		if (!bw_is_finite(coeffs->dfbf) || coeffs->dfbf < 0.f || coeffs->dfbf >= 1.f)
			return 0;
		if (coeffs->dfbi + coeffs->dfbf > len)
			return 0;

		if (!bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_delay_ff_state)
			|| !bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_delay_fb_state))
			return 0;
	}
#endif

	return bw_gain_coeffs_is_valid(&coeffs->blend_coeffs)
		&& bw_gain_coeffs_is_valid(&coeffs->ff_coeffs)
		&& bw_gain_coeffs_is_valid(&coeffs->fb_coeffs);
}

static inline char bw_comb_state_is_valid(
		const bw_comb_coeffs * BW_RESTRICT coeffs,
		const bw_comb_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_comb_state"))
		return 0;
	if (state->state < bw_comb_state_state_mem_set || state->state > bw_comb_state_state_reset_state)
		return 0;

	if (state->state >= bw_comb_state_state_reset_state && coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	return bw_delay_state_is_valid(coeffs ? &coeffs->delay_coeffs : NULL, &state->delay_state);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Comb
 *  ```>>> */
template<size_t N_CHANNELS>
class Comb {
public:
	Comb(
		float maxDelay = 1.f);

	~Comb();

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

	void setDelayFF(
		float value);

	void setDelayFB(
		float value);

	void setCoeffBlend(
		float value);

	void setCoeffFF(
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
	bw_comb_coeffs			coeffs;
	bw_comb_state			states[N_CHANNELS];
	bw_comb_state * BW_RESTRICT	statesP[N_CHANNELS];
	void * BW_RESTRICT		mem;
};

template<size_t N_CHANNELS>
inline Comb<N_CHANNELS>::Comb(
		float maxDelay) {
	bw_comb_init(&coeffs, maxDelay);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
	mem = nullptr;
}

template<size_t N_CHANNELS>
inline Comb<N_CHANNELS>::~Comb() {
	if (mem != nullptr)
		operator delete(mem);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_comb_set_sample_rate(&coeffs, sampleRate);
	size_t req = bw_comb_mem_req(&coeffs);
	if (mem != nullptr)
		operator delete(mem);
	mem = operator new(req * N_CHANNELS);
	void *m = mem;
	for (size_t i = 0; i < N_CHANNELS; i++, m = static_cast<char *>(m) + req)
		bw_comb_mem_set(&coeffs, states + i, m);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_comb_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_comb_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_comb_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_comb_reset_coeffs(&coeffs);
	bw_comb_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_comb_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::setDelayFF(
		float value) {
	bw_comb_set_delay_ff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::setDelayFB(
		float value) {
	bw_comb_set_delay_fb(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::setCoeffBlend(
		float value) {
	bw_comb_set_coeff_blend(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::setCoeffFF(
		float value) {
	bw_comb_set_coeff_ff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comb<N_CHANNELS>::setCoeffFB(
		float value) {
	bw_comb_set_coeff_fb(&coeffs, value);
}

}
#endif

#endif
