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
 *    bw_ap1 bw_common bw_lp1 bw_math bw_one_pole bw_osc_sin bw_phase_gen
 *  }}}
 *  description {{{
 *    Phaser containing 4 1st-order allpass filters modulated by a sinusoidal
 *    LFO.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_phaser_reset_state()</code>.</li>
 *          <li>Added <code>bw_phaser_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_phaser_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_phaser_process()</code> and
 *              <code>bw_phaser_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed internal limits on cutoff frequency.</li>
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
 *          <li>Added <code>bw_phaser_process_multi()</code>.</li>
 *          <li>Now properly setting allpass cutoff on reset.</li>
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

#ifndef BW_PHASER_H
#define BW_PHASER_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_phaser_coeffs
 *  ```>>> */
typedef struct bw_phaser_coeffs bw_phaser_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_phaser_state
 *  ```>>> */
typedef struct bw_phaser_state bw_phaser_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_phaser_init()
 *  ```>>> */
static inline void bw_phaser_init(
	bw_phaser_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_phaser_set_sample_rate()
 *  ```>>> */
static inline void bw_phaser_set_sample_rate(
	bw_phaser_coeffs * BW_RESTRICT coeffs,
	float                          sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_phaser_reset_coeffs()
 *  ```>>> */
static inline void bw_phaser_reset_coeffs(
	bw_phaser_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_phaser_reset_state()
 *  ```>>> */
static inline float bw_phaser_reset_state(
	const bw_phaser_coeffs * BW_RESTRICT coeffs,
	bw_phaser_state * BW_RESTRICT        state,
	float                                x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_phaser_reset_state_multi()
 *  ```>>> */
static inline void bw_phaser_reset_state_multi(
	const bw_phaser_coeffs * BW_RESTRICT              coeffs,
	bw_phaser_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_phaser_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_phaser_update_coeffs_ctrl(
	bw_phaser_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_phaser_update_coeffs_audio()
 *  ```>>> */
static inline void bw_phaser_update_coeffs_audio(
	bw_phaser_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_phaser_process1()
 *  ```>>> */
static inline float bw_phaser_process1(
	const bw_phaser_coeffs * BW_RESTRICT coeffs,
	bw_phaser_state * BW_RESTRICT        state,
	float                                x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_phaser_process()
 *  ```>>> */
static inline void bw_phaser_process(
	bw_phaser_coeffs * BW_RESTRICT coeffs,
	bw_phaser_state * BW_RESTRICT  state,
	const float *                  x,
	float *                        y,
	size_t                         n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_phaser_process_multi()
 *  ```>>> */
static inline void bw_phaser_process_multi(
	bw_phaser_coeffs * BW_RESTRICT                    coeffs,
	bw_phaser_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_phaser_set_rate()
 *  ```>>> */
static inline void bw_phaser_set_rate(
	bw_phaser_coeffs * BW_RESTRICT coeffs,
	float                          value);
/*! <<<```
 *    Sets the modulation rate `value` (Hz) in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_phaser_set_center()
 *  ```>>> */
static inline void bw_phaser_set_center(
	bw_phaser_coeffs * BW_RESTRICT coeffs,
	float                          value);
/*! <<<```
 *    Sets the center frequency `value` (Hz) in `coeffs`.
 *
 *    Valid range: [`1e-6f`, `1e12f`].
 *
 *    By the time `bw_phaser_reset_coeffs()`, `bw_phaser_update_coeffs_ctrl()`,
 *    `bw_phaser_update_coeffs_audio()`, `bw_phaser_process1()`,
 *    `bw_phaser_process()`, or `bw_phaser_process_multi()` is called,
 *    `center * bw_pow2f(amount)` must be in [`1e-6f`, `1e12f`].
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_phaser_set_amount()
 *  ```>>> */
static inline void bw_phaser_set_amount(
	bw_phaser_coeffs * BW_RESTRICT coeffs,
	float                          value);
/*! <<<```
 *    Sets the modulation amount `value` (octaves) in `coeffs`.
 *
 *    `value` must be finite and non-negative.
 *
 *    By the time `bw_phaser_reset_coeffs()`, `bw_phaser_update_coeffs_ctrl()`,
 *    `bw_phaser_update_coeffs_audio()`, `bw_phaser_process1()`,
 *    `bw_phaser_process()`, or `bw_phaser_process_multi()` is called,
 *    `center * bw_pow2f(amount)` must be in [`1e-6f`, `1e12f`].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_phaser_coeffs_is_valid()
 *  ```>>> */
static inline char bw_phaser_coeffs_is_valid(
	const bw_phaser_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_phaser_coeffs`.
 *
 *    #### bw_phaser_state_is_valid()
 *  ```>>> */
static inline char bw_phaser_state_is_valid(
	const bw_phaser_coeffs * BW_RESTRICT coeffs,
	const bw_phaser_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_phaser_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_phase_gen.h>
#include <bw_osc_sin.h>
#include <bw_ap1.h>
#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_phaser_coeffs_state {
	bw_phaser_coeffs_state_invalid,
	bw_phaser_coeffs_state_init,
	bw_phaser_coeffs_state_set_sample_rate,
	bw_phaser_coeffs_state_reset_coeffs
};
#endif

struct bw_phaser_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_phaser_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_phase_gen_coeffs		phase_gen_coeffs;
	bw_phase_gen_state		phase_gen_state;
	bw_ap1_coeffs			ap1_coeffs;

	// Parameters
	float				center;
	float				amount;
};

struct bw_phaser_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// Sub-components
	bw_ap1_state	ap1_state[4];
};

static inline void bw_phaser_init(
		bw_phaser_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	bw_phase_gen_init(&coeffs->phase_gen_coeffs);
	bw_ap1_init(&coeffs->ap1_coeffs);
	coeffs->center = 1e3f;
	coeffs->amount = 1.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_phaser_coeffs");
	coeffs->state = bw_phaser_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_phaser_coeffs_state_init);
}

static inline void bw_phaser_set_sample_rate(
		bw_phaser_coeffs * BW_RESTRICT coeffs,
		float                          sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_phase_gen_set_sample_rate(&coeffs->phase_gen_coeffs, sample_rate);
	bw_ap1_set_sample_rate(&coeffs->ap1_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_phaser_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_phaser_coeffs_state_set_sample_rate);
}

static inline void bw_phaser_reset_coeffs(
		bw_phaser_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_set_sample_rate);
	BW_ASSERT_DEEP(coeffs->center * bw_pow2f(coeffs->amount) >= 1e-6f && coeffs->center * bw_pow2f(coeffs->amount) <= 1e12f);

	bw_phase_gen_reset_coeffs(&coeffs->phase_gen_coeffs);
	float p, inc;
	bw_phase_gen_reset_state(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state, 0.f, &p, &inc);
	bw_ap1_set_cutoff(&coeffs->ap1_coeffs, coeffs->center);
	bw_ap1_reset_coeffs(&coeffs->ap1_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_phaser_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_phaser_coeffs_state_reset_coeffs);
}

static inline float bw_phaser_reset_state(
		const bw_phaser_coeffs * BW_RESTRICT coeffs,
		bw_phaser_state * BW_RESTRICT        state,
		float                                x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(bw_is_finite(x_0));

	float y = bw_ap1_reset_state(&coeffs->ap1_coeffs, &state->ap1_state[0], x_0);
	y = bw_ap1_reset_state(&coeffs->ap1_coeffs, &state->ap1_state[1], y);
	y = bw_ap1_reset_state(&coeffs->ap1_coeffs, &state->ap1_state[2], y);
	y = x_0 + bw_ap1_reset_state(&coeffs->ap1_coeffs, &state->ap1_state[3], y);

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_phaser_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_phaser_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_phaser_reset_state_multi(
		const bw_phaser_coeffs * BW_RESTRICT              coeffs,
		bw_phaser_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                     x_0,
		float *                                           y_0,
		size_t                                            n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_phaser_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_phaser_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_phaser_update_coeffs_ctrl(
		bw_phaser_coeffs *BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->center * bw_pow2f(coeffs->amount) >= 1e-6f && coeffs->center * bw_pow2f(coeffs->amount) <= 1e12f);

	bw_phase_gen_update_coeffs_ctrl(&coeffs->phase_gen_coeffs);

	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
}

static inline void bw_phaser_update_coeffs_audio(
		bw_phaser_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->center * bw_pow2f(coeffs->amount) >= 1e-6f && coeffs->center * bw_pow2f(coeffs->amount) <= 1e12f);

	bw_phase_gen_update_coeffs_audio(&coeffs->phase_gen_coeffs);
	float p, pi;
	bw_phase_gen_process1(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state, &p, &pi);
	const float m = coeffs->amount * bw_osc_sin_process1(p);
	bw_ap1_set_cutoff(&coeffs->ap1_coeffs, coeffs->center * bw_pow2f(m));
	bw_ap1_update_coeffs_ctrl(&coeffs->ap1_coeffs);
	bw_ap1_update_coeffs_audio(&coeffs->ap1_coeffs);

	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
}

static inline float bw_phaser_process1(
		const bw_phaser_coeffs * BW_RESTRICT coeffs,
		bw_phaser_state * BW_RESTRICT        state,
		float                                x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->center * bw_pow2f(coeffs->amount) >= 1e-6f && coeffs->center * bw_pow2f(coeffs->amount) <= 1e12f);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_phaser_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	float y = bw_ap1_process1(&coeffs->ap1_coeffs, &state->ap1_state[0], x);
	y = bw_ap1_process1(&coeffs->ap1_coeffs, &state->ap1_state[1], y);
	y = bw_ap1_process1(&coeffs->ap1_coeffs, &state->ap1_state[2], y);
	y = x + bw_ap1_process1(&coeffs->ap1_coeffs, &state->ap1_state[3], y);

	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_phaser_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_phaser_process(
		bw_phaser_coeffs * BW_RESTRICT coeffs,
		bw_phaser_state * BW_RESTRICT  state,
		const float *                  x,
		float *                        y,
		size_t                         n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->center * bw_pow2f(coeffs->amount) >= 1e-6f && coeffs->center * bw_pow2f(coeffs->amount) <= 1e12f);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_phaser_state_is_valid(coeffs, state));
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != NULL);

	bw_phaser_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_phaser_update_coeffs_audio(coeffs);
		y[i] = bw_phaser_process1(coeffs, state, x[i]);
	}

	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_phaser_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_phaser_process_multi(
		bw_phaser_coeffs * BW_RESTRICT                    coeffs,
		bw_phaser_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                             x,
		float * const *                                   y,
		size_t                                            n_channels,
		size_t                                            n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->center * bw_pow2f(coeffs->amount) >= 1e-6f && coeffs->center * bw_pow2f(coeffs->amount) <= 1e12f);
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

	bw_phaser_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_phaser_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_phaser_process1(coeffs, state[j], x[j][i]);
	}

	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_reset_coeffs);
}

static inline void bw_phaser_set_rate(
		bw_phaser_coeffs * BW_RESTRICT coeffs,
		float                          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	bw_phase_gen_set_frequency(&coeffs->phase_gen_coeffs, value);

	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_init);
}

static inline void bw_phaser_set_center(
		bw_phaser_coeffs * BW_RESTRICT coeffs,
		float                          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e12f);

	coeffs->center = value;

	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_init);
}

static inline void bw_phaser_set_amount(
		bw_phaser_coeffs * BW_RESTRICT coeffs,
		float                          value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f);

	coeffs->amount = value;

	BW_ASSERT_DEEP(bw_phaser_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phaser_coeffs_state_init);
}

#include <stdio.h>
static inline char bw_phaser_coeffs_is_valid(
		const bw_phaser_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_phaser_coeffs"))
		return 0;
	if (coeffs->state < bw_phaser_coeffs_state_init || coeffs->state > bw_phaser_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->center) || coeffs->center < 1e-6f || coeffs->center > 1e12f)
		return 0;
	if (!bw_is_finite(coeffs->amount) || coeffs->amount < 0.f)
		return 0;

	if (!bw_phase_gen_coeffs_is_valid(&coeffs->phase_gen_coeffs))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_phaser_coeffs_state_reset_coeffs && !bw_phase_gen_state_is_valid(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state))
		return 0;
#endif

	return bw_ap1_coeffs_is_valid(&coeffs->ap1_coeffs);
}

static inline char bw_phaser_state_is_valid(
		const bw_phaser_coeffs * BW_RESTRICT coeffs,
		const bw_phaser_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_phaser_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	return bw_ap1_state_is_valid(coeffs ? &coeffs->ap1_coeffs : NULL, &state->ap1_state[0])
		&& bw_ap1_state_is_valid(coeffs ? &coeffs->ap1_coeffs : NULL, &state->ap1_state[1])
		&& bw_ap1_state_is_valid(coeffs ? &coeffs->ap1_coeffs : NULL, &state->ap1_state[2])
		&& bw_ap1_state_is_valid(coeffs ? &coeffs->ap1_coeffs : NULL, &state->ap1_state[3]);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Phaser
 *  ```>>> */
template<size_t N_CHANNELS>
class Phaser {
public:
	Phaser();

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

	void setCenter(
		float value);

	void setAmount(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_phaser_coeffs		coeffs;
	bw_phaser_state			states[N_CHANNELS];
	bw_phaser_state	 * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Phaser<N_CHANNELS>::Phaser() {
	bw_phaser_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_phaser_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_phaser_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_phaser_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_phaser_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_phaser_reset_coeffs(&coeffs);
	bw_phaser_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_phaser_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::setRate(
		float value) {
	bw_phaser_set_rate(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::setCenter(
		float value) {
	bw_phaser_set_center(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Phaser<N_CHANNELS>::setAmount(
		float value) {
	bw_phaser_set_amount(&coeffs, value);
}

}
#endif

#endif
