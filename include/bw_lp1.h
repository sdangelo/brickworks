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
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    First-order lowpass filter (6 dB/oct) with unitary DC gain.
 *
 *    This is better suited to filtering actual audio than
 *    [bw_one_pole](bw_one_pole).
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Limited actual prewarping frequency to prevent instability.</li>
 *          <li>Added <code>bw_lp1_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_lp1_reset_state()</code> returns the initial output
 *              value.</li>
 *          <li><code>bw_lp1_process()</code> and
 *              <code>bw_lp1_process_multi()</code> now use <code>size_t</code>
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
 *          <li>Added <code>bw_lp1_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_lp1_reset_state()</code>.</li>
 *          <li>Fixed unused parameter warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_LP1_H
#define BW_LP1_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_lp1_coeffs
 *  ```>>> */
typedef struct bw_lp1_coeffs bw_lp1_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_lp1_state
 *  ```>>> */
typedef struct bw_lp1_state bw_lp1_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_lp1_init()
 *  ```>>> */
static inline void bw_lp1_init(
	bw_lp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_lp1_set_sample_rate()
 *  ```>>> */
static inline void bw_lp1_set_sample_rate(
	bw_lp1_coeffs * BW_RESTRICT coeffs,
	float                       sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_lp1_reset_coeffs()
 *  ```>>> */
static inline void bw_lp1_reset_coeffs(
	bw_lp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_lp1_reset_state()
 *  ```>>> */
static inline float bw_lp1_reset_state(
	const bw_lp1_coeffs * BW_RESTRICT coeffs,
	bw_lp1_state * BW_RESTRICT        state,
	float                             x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_lp1_reset_state_multi()
 *  ```>>> */
static inline void bw_lp1_reset_state_multi(
	const bw_lp1_coeffs * BW_RESTRICT              coeffs,
	bw_lp1_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                  x_0,
	float *                                        y_0,
	size_t                                         n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `NULL`.
 *
 *    #### bw_lp1_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_lp1_update_coeffs_ctrl(
	bw_lp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_lp1_update_coeffs_audio()
 *  ```>>> */
static inline void bw_lp1_update_coeffs_audio(
	bw_lp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_lp1_process1()
 *  ```>>> */
static inline float bw_lp1_process1(
	const bw_lp1_coeffs * BW_RESTRICT coeffs,
	bw_lp1_state * BW_RESTRICT        state,
	float                             x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_lp1_process()
 *  ```>>> */
static inline void bw_lp1_process(
	bw_lp1_coeffs * BW_RESTRICT coeffs,
	bw_lp1_state * BW_RESTRICT  state,
	const float *               x,
	float *                     y,
	size_t                      n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_lp1_process_multi()
 *  ```>>> */
static inline void bw_lp1_process_multi(
	bw_lp1_coeffs * BW_RESTRICT                    coeffs,
	bw_lp1_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                          x,
	float * const *                                y,
	size_t                                         n_channels,
	size_t                                         n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_lp1_set_cutoff()
 *  ```>>> */
static inline void bw_lp1_set_cutoff(
	bw_lp1_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Valid range: [`1e-6f`, `1e12f`].
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_lp1_set_prewarp_at_cutoff()
 *  ```>>> */
static inline void bw_lp1_set_prewarp_at_cutoff(
	bw_lp1_coeffs * BW_RESTRICT coeffs,
	char                        value);
/*! <<<```
 *    Sets whether bilinear transform prewarping frequency should match the
 *    cutoff frequency (non-`0`) or not (`0`).
 *
 *    Default value: non-`0` (on).
 *
 *    #### bw_lp1_set_prewarp_freq()
 *  ```>>> */
static inline void bw_lp1_set_prewarp_freq(
	bw_lp1_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the prewarping frequency `value` (Hz) in `coeffs`.
 *
 *    Only used when the prewarp\_at\_cutoff parameter is off and however
 *    internally limited to avoid instability.
 *
 *    Valid range: [`1e-6f`, `1e12f`].
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_lp1_coeffs_is_valid()
 *  ```>>> */
static inline char bw_lp1_coeffs_is_valid(
	const bw_lp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_lp1_coeffs`.
 *
 *    #### bw_lp1_state_is_valid()
 *  ```>>> */
static inline char bw_lp1_state_is_valid(
	const bw_lp1_coeffs * BW_RESTRICT coeffs,
	const bw_lp1_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_lp1_state`.
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
enum bw_lp1_coeffs_state {
	bw_lp1_coeffs_state_invalid,
	bw_lp1_coeffs_state_init,
	bw_lp1_coeffs_state_set_sample_rate,
	bw_lp1_coeffs_state_reset_coeffs
};
#endif

struct bw_lp1_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_lp1_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_one_pole_coeffs		smooth_coeffs;
	bw_one_pole_state		smooth_cutoff_state;
	bw_one_pole_state		smooth_prewarp_freq_state;

	// Coefficients
	float				t_k;

	float				t;
	float				X_x;
	float				X_X_z1;
	float				y_X;

	// Parameters
	float				cutoff;
	float				prewarp_k;
	float				prewarp_freq;
};

struct bw_lp1_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// States
	float		y_z1;
	float		X_z1;
};

static inline void bw_lp1_init(
		bw_lp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->cutoff = 1e3f;
	coeffs->prewarp_k = 1.f;
	coeffs->prewarp_freq = 1e3f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_lp1_coeffs");
	coeffs->state = bw_lp1_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_lp1_coeffs_state_init);
}

static inline void bw_lp1_set_sample_rate(
		bw_lp1_coeffs * BW_RESTRICT coeffs,
		float                       sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->t_k = 3.141592653589793f / sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_lp1_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_lp1_coeffs_state_set_sample_rate);
}

static inline void bw_lp1_do_update_coeffs(
		bw_lp1_coeffs * BW_RESTRICT coeffs,
		char                        force) {
	const float prewarp_freq = coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq);
	float prewarp_freq_cur = bw_one_pole_get_y_z1(&coeffs->smooth_prewarp_freq_state);
	float cutoff_cur = bw_one_pole_get_y_z1(&coeffs->smooth_cutoff_state);
	const char prewarp_freq_changed = force || prewarp_freq != prewarp_freq_cur;
	const char cutoff_changed = force || coeffs->cutoff != cutoff_cur;
	if (prewarp_freq_changed || cutoff_changed) {
		if (prewarp_freq_changed) {
			prewarp_freq_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, prewarp_freq);
			coeffs->t = bw_tanf(bw_minf(coeffs->t_k * prewarp_freq_cur, 1.567654734141306f)); // max = 0.499 * fs
		}
		if (cutoff_changed) {
			cutoff_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
			coeffs->y_X = bw_rcpf(cutoff_cur);
		}
		const float k = cutoff_cur * bw_rcpf(cutoff_cur * coeffs->t + prewarp_freq_cur);
		coeffs->X_x = k * prewarp_freq_cur;
		coeffs->X_X_z1 = k * coeffs->t;
	}
}

static inline void bw_lp1_reset_coeffs(
		bw_lp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_set_sample_rate);

	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq));
	bw_lp1_do_update_coeffs(coeffs, 1);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_lp1_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_lp1_coeffs_state_reset_coeffs);
}

static inline float bw_lp1_reset_state(
		const bw_lp1_coeffs * BW_RESTRICT coeffs,
		bw_lp1_state * BW_RESTRICT        state,
		float                             x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(bw_is_finite(x_0));

	(void)coeffs;
	const float y = x_0;
	state->y_z1 = x_0;
	state->X_z1 = 0.f;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_lp1_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_lp1_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_lp1_reset_state_multi(
		const bw_lp1_coeffs * BW_RESTRICT              coeffs,
		bw_lp1_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                  x_0,
		float *                                        y_0,
		size_t                                         n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_lp1_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_lp1_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_lp1_update_coeffs_ctrl(
		bw_lp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_lp1_update_coeffs_audio(
		bw_lp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);

	bw_lp1_do_update_coeffs(coeffs, 0);

	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
}

static inline float bw_lp1_process1(
		const bw_lp1_coeffs * BW_RESTRICT coeffs,
		bw_lp1_state * BW_RESTRICT        state,
		float                             x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_lp1_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	const float X = coeffs->X_x * (x - state->y_z1) - coeffs->X_X_z1 * state->X_z1;
	const float y = x - coeffs->y_X * X;
	state->y_z1 = y;
	state->X_z1 = X;

	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_lp1_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_lp1_process(
		bw_lp1_coeffs * BW_RESTRICT coeffs,
		bw_lp1_state * BW_RESTRICT  state,
		const float *               x,
		float *                     y,
		size_t                      n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_lp1_state_is_valid(coeffs, state));
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != NULL);

	for (size_t i = 0; i < n_samples; i++) {
		bw_lp1_update_coeffs_audio(coeffs);
		y[i] = bw_lp1_process1(coeffs, state, x[i]);
	}

	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_lp1_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_lp1_process_multi(
		bw_lp1_coeffs * BW_RESTRICT                    coeffs,
		bw_lp1_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                          x,
		float * const *                                y,
		size_t                                         n_channels,
		size_t                                         n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
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

	for (size_t i = 0; i < n_samples; i++) {
		bw_lp1_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_lp1_process1(coeffs, state[j], x[j][i]);
	}

	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_reset_coeffs);
}

static inline void bw_lp1_set_cutoff(
		bw_lp1_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e12f);

	coeffs->cutoff = value;

	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_init);
}

static inline void bw_lp1_set_prewarp_at_cutoff(
		bw_lp1_coeffs * BW_RESTRICT coeffs,
		char                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_init);

	coeffs->prewarp_k = value ? 1.f : 0.f;

	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_init);
}

static inline void bw_lp1_set_prewarp_freq(
		bw_lp1_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e12f);

	coeffs->prewarp_freq = value;

	BW_ASSERT_DEEP(bw_lp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_lp1_coeffs_state_init);
}

static inline char bw_lp1_coeffs_is_valid(
		const bw_lp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_lp1_coeffs"))
		return 0;
	if (coeffs->state < bw_lp1_coeffs_state_init || coeffs->state > bw_lp1_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->cutoff) || coeffs->cutoff < 1e-6f || coeffs->cutoff > 1e12f)
		return 0;
	if (!bw_is_finite(coeffs->prewarp_k) || (coeffs->prewarp_k != 0.f && coeffs->prewarp_k != 1.f))
		return 0;
	if (!bw_is_finite(coeffs->prewarp_freq) || coeffs->prewarp_freq < 1e-6f || coeffs->prewarp_freq > 1e12f)
		return 0;

	if (!bw_one_pole_coeffs_is_valid(&coeffs->smooth_coeffs))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_lp1_coeffs_state_set_sample_rate) {
		if (!bw_is_finite(coeffs->t_k) || coeffs->t_k <= 0.f)
			return 0;
	}

	if (coeffs->state >= bw_lp1_coeffs_state_reset_coeffs) {
		if (!bw_is_finite(coeffs->t) || coeffs->t <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->y_X) || coeffs->y_X <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->X_x) || coeffs->X_x <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->X_X_z1) || coeffs->X_X_z1 <= 0.f)
			return 0;

		if (!bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state))
			return 0;
		if (!bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state))
			return 0;
	}
#endif

	return 1;
}

static inline char bw_lp1_state_is_valid(
		const bw_lp1_coeffs * BW_RESTRICT coeffs,
		const bw_lp1_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_lp1_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	(void)coeffs;

	return bw_is_finite(state->y_z1) && bw_is_finite(state->X_z1);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::LP1
 *  ```>>> */
template<size_t N_CHANNELS>
class LP1 {
public:
	LP1();

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

	void setCutoff(
		float value);

	void setPrewarpAtCutoff(
		bool value);

	void setPrewarpFreq(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_lp1_coeffs			coeffs;
	bw_lp1_state			states[N_CHANNELS];
	bw_lp1_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline LP1<N_CHANNELS>::LP1() {
	bw_lp1_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_lp1_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_lp1_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_lp1_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_lp1_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_lp1_reset_coeffs(&coeffs);
	bw_lp1_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_lp1_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::setCutoff(
		float value) {
	bw_lp1_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::setPrewarpAtCutoff(
		bool value) {
	bw_lp1_set_prewarp_at_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void LP1<N_CHANNELS>::setPrewarpFreq(
		float value) {
	bw_lp1_set_prewarp_freq(&coeffs, value);
}

}
#endif

#endif
