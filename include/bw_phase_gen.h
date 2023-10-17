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
 *  version {{{ 1.0.1 }}}
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    Phase generator with portamento and exponential frequency modulation.
 *
 *    It outputs a normalized phase signal (range [`0.f`, `1.f`]).
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.1</strong>:
 *        <ul>
 *          <li>Fixed rouding bug when frequency is tiny and negative.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_phase_gen_reset_state()</code>.</li>
 *          <li>Added <code>bw_phase_gen_reset_state_multi()</code> and updated
 *              C++ API in this regard.</li>
 *          <li>Now <code>bw_phase_gen_reset_state()</code> returns the initial
 *              output values.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_phase_gen_process()</code> and
 *              <code>bw_phase_gen_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Fixed smoothing coefficients update in
 *              <code>bw_phase_gen_reset_coeffs()</code>.</li>
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
 *          <li>Added <code>bw_phase_gen_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Fixed unused parameter warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Added <code>BW_RESTRICT</code> to
 *              <code>bw_phase_gen_process1*()</code>.</li>
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

#ifndef BW_PHASE_GEN_H
#define BW_PHASE_GEN_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_phase_gen_coeffs
 *  ```>>> */
typedef struct bw_phase_gen_coeffs bw_phase_gen_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_phase_gen_state
 *  ```>>> */
typedef struct bw_phase_gen_state bw_phase_gen_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_phase_gen_init()
 *  ```>>> */
static inline void bw_phase_gen_init(
	bw_phase_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_phase_gen_set_sample_rate()
 *  ```>>> */
static inline void bw_phase_gen_set_sample_rate(
	bw_phase_gen_coeffs * BW_RESTRICT coeffs,
	float                             sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_phase_gen_reset_coeffs()
 *  ```>>> */
static inline void bw_phase_gen_reset_coeffs(
	bw_phase_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_phase_gen_reset_state()
 *  ```>>> */
static inline void bw_phase_gen_reset_state(
	const bw_phase_gen_coeffs * BW_RESTRICT coeffs,
	bw_phase_gen_state * BW_RESTRICT        state,
	float                                   phase_0,
	float * BW_RESTRICT                     y_0,
	float * BW_RESTRICT                     y_inc_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial phase value `phase_0`.
 *
 *    The corresponding initial output and phase increment values are put into
 *    `y_0` and `y_inc_0` respectively.
 *
 *    #### bw_phase_gen_reset_state_multi()
 *  ```>>> */
static inline void bw_phase_gen_reset_state_multi(
	const bw_phase_gen_coeffs * BW_RESTRICT              coeffs,
	bw_phase_gen_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                        phase_0,
	float *                                              y_0,
	float *                                              y_inc_0,
	size_t                                               n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial phase value in the `phase_0`
 *    array.
 *
 *    The corresponding initial output and phase increment values are put into
 *    the `y_0` and `y_inc_0` arrays, respectively, if they are not `NULL`.
 *
 *    #### bw_phase_gen_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_phase_gen_update_coeffs_ctrl(
	bw_phase_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_phase_gen_update_coeffs_audio()
 *  ```>>> */
static inline void bw_phase_gen_update_coeffs_audio(
	bw_phase_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_phase_gen_process1\*()
 *  ```>>> */
static inline void bw_phase_gen_process1(
	const bw_phase_gen_coeffs * BW_RESTRICT coeffs,
	bw_phase_gen_state * BW_RESTRICT        state,
	float * BW_RESTRICT                     y,
	float * BW_RESTRICT                     y_inc);

static inline void bw_phase_gen_process1_mod(
	const bw_phase_gen_coeffs * BW_RESTRICT coeffs,
	bw_phase_gen_state * BW_RESTRICT        state,
	float                                   x_mod,
	float * BW_RESTRICT                     y,
	float * BW_RESTRICT                     y_inc);
/*! <<<```
 *    These functions generate one output sample using `coeffs`, while using
 *    and updating `state`, putting its value in `y` and the corresponding phase
 *    increment value in `y_inc`.
 *
 *    In particular:
 *     * `bw_phase_gen_process1()` does not apply frequency modulation;
 *     * `bw_phase_gen_process1_mod()` applies exponential frequency modulation
 *       using `x_mod` as modulation input (scale `1.f`/octave).
 *
 *    #### bw_phase_gen_process()
 *  ```>>> */
static inline void bw_phase_gen_process(
	bw_phase_gen_coeffs * BW_RESTRICT coeffs,
	bw_phase_gen_state * BW_RESTRICT  state,
	const float *                     x_mod,
	float *                           y,
	float *                           y_inc,
	size_t                            n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the output buffer `y`, while
 *    using and updating both `coeffs` and `state` (control and audio rate).
 *
 *    If `x_mod` is not `NULL`, it is used as a source of exponential frequency
 *    modulation (scale `1.f`/octave).
 *
 *    If `y_inc` is not `NULL`, it is filled with phase increment values.
 *
 *    #### bw_phase_gen_process_multi()
 *  ```>>> */
static inline void bw_phase_gen_process_multi(
	bw_phase_gen_coeffs * BW_RESTRICT                    coeffs,
	bw_phase_gen_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                                x_mod,
	float * const *                                      y,
	float * const *                                      y_inc,
	size_t                                               n_channels,
	size_t                                               n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the `n_channels` output
 *    buffers `y`, while using and updating both the common `coeffs` and each of
 *    the `n_channels` `state`s (control and audio rate).
 *
 *    If `x_mod` and the channel-specific element are not `NULL`, this is used
 *    as a source of exponential frequency modulation (scale `1.f`/octave) for
 *    that channel.
 *
 *    If `y_inc` and the channel-specific element are not `NULL`, this is filled
 *    with phase increment values for that channel.
 *
 *    #### bw_phase_gen_set_frequency()
 *  ```>>> */
static inline void bw_phase_gen_set_frequency(
	bw_phase_gen_coeffs * BW_RESTRICT coeffs,
	float                             value);
/*! <<<```
 *    Sets the base frequency to `value` (Hz) in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_phase_gen_set_portamento_tau()
 *  ```>>> */
static inline void bw_phase_gen_set_portamento_tau(
	bw_phase_gen_coeffs * BW_RESTRICT coeffs,
	float                             value);
/*! <<<```
 *    Sets the portamento time constant `value` (s) in `coeffs`.
 *
 *    `value` must be non-negative.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_phase_gen_coeffs_is_valid()
 *  ```>>> */
static inline char bw_phase_gen_coeffs_is_valid(
	const bw_phase_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_phase_gen_coeffs`.
 *
 *    #### bw_phase_gen_state_is_valid()
 *  ```>>> */
static inline char bw_phase_gen_state_is_valid(
	const bw_phase_gen_coeffs * BW_RESTRICT coeffs,
	const bw_phase_gen_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_phase_gen_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_phase_gen_coeffs_state {
	bw_phase_gen_coeffs_state_invalid,
	bw_phase_gen_coeffs_state_init,
	bw_phase_gen_coeffs_state_set_sample_rate,
	bw_phase_gen_coeffs_state_reset_coeffs
};
#endif

struct bw_phase_gen_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_phase_gen_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_one_pole_coeffs		portamento_coeffs;
	bw_one_pole_state		portamento_state;

	// Coefficients
	float				T;

	float				portamento_target;

	// Parameters
	float				frequency;
	float				frequency_prev;
};

struct bw_phase_gen_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// States
	float		phase;
};

static inline void bw_phase_gen_init(
		bw_phase_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	bw_one_pole_init(&coeffs->portamento_coeffs);
	coeffs->frequency = 1.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_phase_gen_coeffs");
	coeffs->state = bw_phase_gen_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_phase_gen_coeffs_state_init);
}

static inline void bw_phase_gen_set_sample_rate(
		bw_phase_gen_coeffs * BW_RESTRICT coeffs,
		float                             sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_one_pole_set_sample_rate(&coeffs->portamento_coeffs, sample_rate);
	coeffs->T = 1.f / sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_phase_gen_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_phase_gen_coeffs_state_set_sample_rate);
}

static inline void bw_phase_gen_do_update_coeffs_ctrl(
		bw_phase_gen_coeffs * BW_RESTRICT coeffs, char force) {
	bw_one_pole_update_coeffs_ctrl(&coeffs->portamento_coeffs);
	if (force || coeffs->frequency != coeffs->frequency_prev) {
		coeffs->portamento_target = coeffs->T * coeffs->frequency;
		coeffs->frequency_prev = coeffs->frequency;
	}
}

static inline void bw_phase_gen_reset_coeffs(
		bw_phase_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_set_sample_rate);

	bw_one_pole_reset_coeffs(&coeffs->portamento_coeffs);
	bw_phase_gen_do_update_coeffs_ctrl(coeffs, 1);
	bw_one_pole_reset_state(&coeffs->portamento_coeffs, &coeffs->portamento_state, coeffs->portamento_target);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_phase_gen_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_phase_gen_coeffs_state_reset_coeffs);
}

static inline void bw_phase_gen_reset_state(
		const bw_phase_gen_coeffs * BW_RESTRICT coeffs,
		bw_phase_gen_state * BW_RESTRICT        state,
		float                                   phase_0,
		float * BW_RESTRICT                     y_0,
		float * BW_RESTRICT                     y_inc_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(bw_is_finite(phase_0));
	BW_ASSERT(y_0 != NULL);
	BW_ASSERT(y_inc_0 != NULL);
	BW_ASSERT(y_0 != y_inc_0);

	state->phase = phase_0;
	*y_inc_0 = bw_one_pole_get_y_z1(&coeffs->portamento_state);
	*y_0 = phase_0;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_phase_gen_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_phase_gen_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(*y_0));
	BW_ASSERT(bw_is_finite(*y_inc_0));
}

static inline void bw_phase_gen_reset_state_multi(
		const bw_phase_gen_coeffs * BW_RESTRICT              coeffs,
		bw_phase_gen_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                        phase_0,
		float *                                              y_0,
		float *                                              y_inc_0,
		size_t                                               n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(phase_0 != NULL);
	BW_ASSERT(y_0 != NULL && y_inc_0 != NULL ? y_0 != y_inc_0 : 1);

	if (y_0 != NULL) {
		if (y_inc_0 != NULL) {
			for (size_t i = 0; i < n_channels; i++)
				bw_phase_gen_reset_state(coeffs, state[i], phase_0[i], y_0 + i, y_inc_0 + i);
		} else {
			for (size_t i = 0; i < n_channels; i++) {
				float v_inc;
				bw_phase_gen_reset_state(coeffs, state[i], phase_0[i], y_0 + i, &v_inc);
			}
		}
	} else {
		if (y_inc_0 != NULL) {
			for (size_t i = 0; i < n_channels; i++) {
				float v;
				bw_phase_gen_reset_state(coeffs, state[i], phase_0[i], &v, y_inc_0 + i);
			}
		} else {
			for (size_t i = 0; i < n_channels; i++) {
				float v, v_inc;
				bw_phase_gen_reset_state(coeffs, state[i], phase_0[i], &v, &v_inc);
			}
		}
	}

	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
	BW_ASSERT_DEEP(y_inc_0 != NULL ? bw_has_only_finite(y_inc_0, n_channels) : 1);
}

static inline void bw_phase_gen_update_coeffs_ctrl(
		bw_phase_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);

	bw_phase_gen_do_update_coeffs_ctrl(coeffs, 0);

	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
}

static inline void bw_phase_gen_update_coeffs_audio(
		bw_phase_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);

	bw_one_pole_process1(&coeffs->portamento_coeffs, &coeffs->portamento_state, coeffs->portamento_target);

	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
}

static inline float bw_phase_gen_update_phase(
		bw_phase_gen_state * BW_RESTRICT state,
		float                            inc) {
	state->phase += inc + 1.f; // + 1.f solves rounding issues with tiny negative frequencies
	state->phase -= bw_floorf(state->phase);
	return state->phase;
}

static inline void bw_phase_gen_process1(
		const bw_phase_gen_coeffs * BW_RESTRICT coeffs,
		bw_phase_gen_state * BW_RESTRICT        state,
		float * BW_RESTRICT                     y,
		float * BW_RESTRICT                     y_inc) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_state_is_valid(coeffs, state));
	BW_ASSERT(y != NULL);
	BW_ASSERT(y_inc != NULL);
	BW_ASSERT(y != y_inc);

	*y_inc = bw_one_pole_get_y_z1(&coeffs->portamento_state);
	*y = bw_phase_gen_update_phase(state, *y_inc);

	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_phase_gen_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(*y));
	BW_ASSERT(*y >= 0.f && *y < 1.f);
	BW_ASSERT(bw_is_finite(*y_inc));
}

static inline void bw_phase_gen_process1_mod(
		const bw_phase_gen_coeffs * BW_RESTRICT coeffs,
		bw_phase_gen_state * BW_RESTRICT        state,
		float                                   x_mod,
		float * BW_RESTRICT                     y,
		float * BW_RESTRICT                     y_inc) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x_mod));
	BW_ASSERT(y != NULL);
	BW_ASSERT(y_inc != NULL);
	BW_ASSERT(y != y_inc);

	*y_inc = bw_one_pole_get_y_z1(&coeffs->portamento_state) * bw_pow2f(x_mod);
	*y = bw_phase_gen_update_phase(state, *y_inc);

	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_phase_gen_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(*y));
	BW_ASSERT(*y >= 0.f && *y < 1.f);
	BW_ASSERT(bw_is_finite(*y_inc));
}

static inline void bw_phase_gen_process(
		bw_phase_gen_coeffs * BW_RESTRICT coeffs,
		bw_phase_gen_state * BW_RESTRICT  state,
		const float *                     x_mod,
		float *                           y,
		float *                           y_inc,
		size_t                            n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(x_mod != NULL ? bw_has_only_finite(x_mod, n_samples) : 1);
	BW_ASSERT(y != NULL && y_inc != NULL ? y != y_inc : 1);

	bw_phase_gen_update_coeffs_ctrl(coeffs);
	if (y != NULL) {
		if (x_mod != NULL) {
			if (y_inc != NULL)
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					bw_phase_gen_process1_mod(coeffs, state, x_mod[i], y + i, y_inc + i);
				}
			else
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v_phase_inc;
					bw_phase_gen_process1_mod(coeffs, state, x_mod[i], y + i, &v_phase_inc);
				}
		} else {
			if (y_inc != NULL)
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					bw_phase_gen_process1(coeffs, state, y + i, y_inc + i);
				}
			else
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v_phase_inc;
					bw_phase_gen_process1(coeffs, state, y + i, &v_phase_inc);
				}
		}
	} else {
		if (x_mod != NULL) {
			if (y_inc != NULL)
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v;
					bw_phase_gen_process1_mod(coeffs, state, x_mod[i], &v, y_inc + i);
				}
			else
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v, v_phase_inc;
					bw_phase_gen_process1_mod(coeffs, state, x_mod[i], &v, &v_phase_inc);
				}
		} else {
			if (y_inc != NULL)
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v;
					bw_phase_gen_process1(coeffs, state, &v, y_inc + i);
				}
			else
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v, v_phase_inc;
					bw_phase_gen_process1(coeffs, state, &v, &v_phase_inc);
				}
		}
	}

	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_phase_gen_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(y != NULL ? bw_has_only_finite(y, n_samples) : 1);
	BW_ASSERT_DEEP(y_inc != NULL ? bw_has_only_finite(y_inc, n_samples) : 1);
}

static inline void bw_phase_gen_process_multi(
		bw_phase_gen_coeffs * BW_RESTRICT                    coeffs,
		bw_phase_gen_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                                x_mod,
		float * const *                                      y,
		float * const *                                      y_inc,
		size_t                                               n_channels,
		size_t                                               n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
	if (y != NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = i + 1; j < n_channels; j++)
				BW_ASSERT(y[i] != y[j]);
	if (y_inc != NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = i + 1; j < n_channels; j++)
				BW_ASSERT(y_inc[i] != y_inc[j]);
	if (y != NULL && y_inc != NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = 0; j < n_channels; j++)
				BW_ASSERT(y[i] != y_inc[j]);
#endif

	bw_phase_gen_update_coeffs_ctrl(coeffs);
	if (y != NULL) {
		if (x_mod != NULL) {
			if (y_inc != NULL)
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						if (x_mod[j])
							bw_phase_gen_process1_mod(coeffs, state[j], x_mod[j][i], &v, &v_phase_inc);
						else
							bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y[j])
							y[j][i] = v;
						if (y_inc[j])
							y_inc[j][i] = v_phase_inc;
					}
				}
			else
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						if (x_mod[j])
							bw_phase_gen_process1_mod(coeffs, state[j], x_mod[j][i], &v, &v_phase_inc);
						else
							bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y[j])
							y[j][i] = v;
					}
				}
		} else {
			if (y_inc != NULL)
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y[j])
							y[j][i] = v;
						if (y_inc[j])
							y_inc[j][i] = v_phase_inc;
					}
				}
			else
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y[j])
							y[j][i] = v;
					}
				}
		}
	} else {
		if (x_mod != NULL) {
			if (y_inc != NULL)
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						if (x_mod[j])
							bw_phase_gen_process1_mod(coeffs, state[j], x_mod[j][i], &v, &v_phase_inc);
						else
							bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y_inc[j])
							y_inc[j][i] = v_phase_inc;
					}
				}
			else
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						if (x_mod[j])
							bw_phase_gen_process1_mod(coeffs, state[j], x_mod[j][i], &v, &v_phase_inc);
						else
							bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
					}
				}
		} else {
			if (y_inc != NULL)
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y_inc[j])
							y_inc[j][i] = v_phase_inc;
					}
				}
			else
				for (size_t i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
					}
				}
		}
	}

	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs);
}

static inline void bw_phase_gen_set_frequency(
		bw_phase_gen_coeffs * BW_RESTRICT coeffs,
		float                             value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	coeffs->frequency = value;

	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_init);
}

static inline void bw_phase_gen_set_portamento_tau(
		bw_phase_gen_coeffs * BW_RESTRICT coeffs,
		float                             value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f);

	bw_one_pole_set_tau(&coeffs->portamento_coeffs, value);

	BW_ASSERT_DEEP(bw_phase_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_phase_gen_coeffs_state_init);
}

static inline char bw_phase_gen_coeffs_is_valid(
		const bw_phase_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_phase_gen_coeffs"))
		return 0;
	if (coeffs->state < bw_phase_gen_coeffs_state_init || coeffs->state > bw_phase_gen_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->frequency))
		return 0;

	if (!bw_one_pole_coeffs_is_valid(&coeffs->portamento_coeffs))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_phase_gen_coeffs_state_set_sample_rate) {
		if (!bw_is_finite(coeffs->T) || coeffs->T <= 0.f)
			return 0;
	}

	if (coeffs->state >= bw_phase_gen_coeffs_state_reset_coeffs) {
		if (!bw_is_finite(coeffs->portamento_target) || coeffs->portamento_target < 0.f)
			return 0;
		if (!bw_is_finite(coeffs->frequency_prev))
			return 0;

		if (!bw_one_pole_state_is_valid(&coeffs->portamento_coeffs, &coeffs->portamento_state))
			return 0;
	}
#endif

	return 1;
}

static inline char bw_phase_gen_state_is_valid(
		const bw_phase_gen_coeffs * BW_RESTRICT coeffs,
		const bw_phase_gen_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_phase_gen_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	(void)coeffs;

	return bw_is_finite(state->phase) && state->phase >= 0.f && state->phase < 1.f;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::PhaseGen
 *  ```>>> */
template<size_t N_CHANNELS>
class PhaseGen {
public:
	PhaseGen();

	void setSampleRate(
		float sampleRate);

	void reset(
		float               phase0 = 0.f,
		float * BW_RESTRICT y0 = nullptr,
		float * BW_RESTRICT yInc0 = nullptr);

	void reset(
		float                                       phase0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yInc0);

	void reset(
		const float * phase0,
		float *       y0 = nullptr,
		float *       yInc0 = nullptr);

	void reset(
		std::array<float, N_CHANNELS>               phase0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0 = nullptr,
		std::array<float, N_CHANNELS> * BW_RESTRICT yInc0 = nullptr);

	void process(
		const float * const * xMod,
		float * const *       y,
		float * const *       yInc,
		size_t                nSamples);

	void process(
		std::array<const float *, N_CHANNELS> xMod,
		std::array<float *, N_CHANNELS>       y,
		std::array<float *, N_CHANNELS>       yInc,
		size_t                                nSamples);

	void setFrequency(
		float value);

	void setPortamentoTau(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_phase_gen_coeffs			coeffs;
	bw_phase_gen_state			states[N_CHANNELS];
	bw_phase_gen_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline PhaseGen<N_CHANNELS>::PhaseGen() {
	bw_phase_gen_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void PhaseGen<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_phase_gen_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void PhaseGen<N_CHANNELS>::reset(
		float               phase0,
		float * BW_RESTRICT y0,
		float * BW_RESTRICT yInc0) {
	bw_phase_gen_reset_coeffs(&coeffs);
	if (y0 != nullptr) {
		if (yInc0 != nullptr) {
			for (size_t i = 0; i < N_CHANNELS; i++)
				bw_phase_gen_reset_state(&coeffs, states + i, phase0, y0 + i, yInc0 + i);
		} else {
			for (size_t i = 0; i < N_CHANNELS; i++) {
				float vInc;
				bw_phase_gen_reset_state(&coeffs, states + i, phase0, y0 + i, &vInc);
			}
		}
	} else {
		if (yInc0 != nullptr) {
			for (size_t i = 0; i < N_CHANNELS; i++) {
				float v;
				bw_phase_gen_reset_state(&coeffs, states + i, phase0, &v, yInc0 + i);
			}
		} else {
			for (size_t i = 0; i < N_CHANNELS; i++) {
				float v, vInc;
				bw_phase_gen_reset_state(&coeffs, states + i, phase0, &v, &vInc);
			}
		}
	}
}

template<size_t N_CHANNELS>
inline void PhaseGen<N_CHANNELS>::reset(
		float                                       phase0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yInc0) {
	reset(phase0, y0 != nullptr ? y0->data() : nullptr, yInc0 != nullptr ? yInc0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void PhaseGen<N_CHANNELS>::reset(
		const float * phase0,
		float *       y0,
		float *       yInc0) {
	bw_phase_gen_reset_coeffs(&coeffs);
	bw_phase_gen_reset_state_multi(&coeffs, statesP, phase0, y0, yInc0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void PhaseGen<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               phase0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yInc0) {
	reset(phase0.data(), y0 != nullptr ? y0->data() : nullptr, yInc0 != nullptr ? yInc0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void PhaseGen<N_CHANNELS>::process(
		const float * const * xMod,
		float * const *       y,
		float * const *       yInc,
		size_t                nSamples) {
	bw_phase_gen_process_multi(&coeffs, statesP, xMod, y, yInc, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void PhaseGen<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> xMod,
		std::array<float *, N_CHANNELS>       y,
		std::array<float *, N_CHANNELS>       yInc,
		size_t                                nSamples) {
	process(xMod.data(), y.data(), yInc.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void PhaseGen<N_CHANNELS>::setFrequency(
		float value) {
	bw_phase_gen_set_frequency(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void PhaseGen<N_CHANNELS>::setPortamentoTau(
		float value) {
	bw_phase_gen_set_portamento_tau(&coeffs, value);
}

}
#endif

#endif
