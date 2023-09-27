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
 *  requires {{{ bw_common }}}
 *  description {{{
 *    Pinking filter.
 *
 *    This is a
 *    <a href="https://en.wikipedia.org/wiki/Linear_time-invariant_system"
 *    target="_blank">linear time-invariant filter</a> approximately attenuating
 *    3 dB/oct from about 0.000046 × Nyquist frequency to about 0.9 × Nyquist
 *    frequency. It can be used to turn white noise into pink noise (hence the
 *    name).
 *
 *    The filter design is based on
 *
 *    P. P. La Pastina and S. D'Angelo, "Optimal Integer Order Approximation of
 *    Fractional Order Filters", Proc. 24th Intl. Conf. Digital Audio Effects
 *    (DAFx20in21), pp. 89-96, Vienna, Austria, September 2021.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_pink_filt_reset_coeffs()</code>,
 *              <code>bw_pink_filt_update_coeffs_ctrl()</code>, and
 *              <code>bw_pink_filt_update_coeffs_audio()</code>.</li>
 *          <li>Added initial input value to
 *              <code>bw_pink_filt_reset_state()</code>.</li>
 *          <li>Added <code>bw_pink_filt_reset_state_multi()</code> and updated
 *              C++ API in this regard.</li>
 *          <li>Now <code>bw_pink_filt_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_pink_filt_process()</code> and
 *              <code>bw_pink_filt_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Fixed type of coefficient in
 *              <code>bw_pink_filt_process1()</code>.</li>
 *          <li>Removed usage of reserved identifiers.</li>
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
 *          <li>Added <code>bw_pink_filt_process_multi()</code>.</li>
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
 *          <li>Fixed <code>bw_pink_filt_set_sample_rate_scaling()</code>
 *              definition.</li>
 *          <li>Added missing documentation for
 *              <code>bw_pink_filt_coeffs</code>.</li>
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

#ifndef BW_PINK_FILT_H
#define BW_PINK_FILT_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_pink_filt_coeffs
 *  ```>>> */
typedef struct bw_pink_filt_coeffs bw_pink_filt_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_pink_filt_state
 *  ```>>> */
typedef struct bw_pink_filt_state bw_pink_filt_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_pink_filt_init()
 *  ```>>> */
static inline void bw_pink_filt_init(
	bw_pink_filt_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_pink_filt_set_sample_rate()
 *  ```>>> */
static inline void bw_pink_filt_set_sample_rate(
	bw_pink_filt_coeffs * BW_RESTRICT coeffs,
	float                             sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_pink_filt_reset_coeffs()
 *  ```>>> */
static inline void bw_pink_filt_reset_coeffs(
	bw_pink_filt_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_pink_filt_reset_state()
 *  ```>>> */
static inline float bw_pink_filt_reset_state(
	const bw_pink_filt_coeffs * BW_RESTRICT coeffs,
	bw_pink_filt_state * BW_RESTRICT        state,
	float                                   x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_pink_filt_reset_state_multi()
 *  ```>>> */
static inline void bw_pink_filt_reset_state_multi(
	const bw_pink_filt_coeffs * BW_RESTRICT              coeffs,
	bw_pink_filt_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                        x_0,
	float *                                              y_0,
	size_t                                               n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `NULL`.
 *
 *    #### bw_pink_filt_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_pink_filt_update_coeffs_ctrl(
	bw_pink_filt_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_pink_filt_update_coeffs_audio()
 *  ```>>> */
static inline void bw_pink_filt_update_coeffs_audio(
	bw_pink_filt_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_pink_filt_process1()
 *  ```>>> */
static inline float bw_pink_filt_process1(
	const bw_pink_filt_coeffs * BW_RESTRICT coeffs,
	bw_pink_filt_state * BW_RESTRICT        state,
	float                                   x);

static inline float bw_pink_filt_process1_scaling(
	const bw_pink_filt_coeffs * BW_RESTRICT coeffs,
	bw_pink_filt_state * BW_RESTRICT        state,
	float                                   x);
/*! <<<```
 *    These function process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_pink_filt_process1()` assumes that sample rate scaling is disabled;
 *     * `bw_pink_filt_process1_scaling()` assumes that sample rate scaling is
 *       enabled.
 *
 *    Whether sample rate scaling is enabled or not is unchecked even for
 *    debugging purposes.
 *
 *    #### bw_pink_filt_process()
 *  ```>>> */
static inline void bw_pink_filt_process(
	bw_pink_filt_coeffs * BW_RESTRICT coeffs,
	bw_pink_filt_state * BW_RESTRICT  state,
	const float *                     x,
	float *                           y,
	size_t                            n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using `coeffs` and
 *    both using and updating `state`.
 * 
 *    #### bw_pink_filt_process_multi()
 *  ```>>> */
static inline void bw_pink_filt_process_multi(
	bw_pink_filt_coeffs * BW_RESTRICT                    coeffs,
	bw_pink_filt_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                                x,
	float * const *                                      y,
	size_t                                               n_channels,
	size_t                                               n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using the common `coeffs` and both using and updating each of the
 *    `n_channels` `state`s.
 *
 *    #### bw_pink_filt_set_sample_rate_scaling()
 *  ```>>> */
static inline void bw_pink_filt_set_sample_rate_scaling(
	bw_pink_filt_coeffs * BW_RESTRICT coeffs,
	char                              value);
/*! <<<```
 *    Sets whether the output should be scaled (`value` non-`0`) or not (`0`)
 *    according to the sample rate in `coeffs`.
 *
 *    In order to keep the magnitude response consistent at different sample
 *    rates, the output of this filter should be accordingly scaled. The 44100
 *    Hz sample rate is used as a reference (that is, the scaling factor at that
 *    sample rate is `1.f`).
 *
 *    Default value: `0` (off).
 *
 *    #### bw_pink_filt_get_scaling_k()
 *  ```>>> */
static inline float bw_pink_filt_get_scaling_k(
	const bw_pink_filt_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the sample rate scaling factor that is applied or would be applied
 *    if sample rate scaling were enabled, as stored in `coeffs`.
 *
 *    #### bw_pink_filt_coeffs_is_valid()
 *  ```>>> */
static inline char bw_pink_filt_coeffs_is_valid(
	const bw_pink_filt_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_pink_filt_coeffs`.
 *
 *    #### bw_pink_filt_state_is_valid()
 *  ```>>> */
static inline char bw_pink_filt_state_is_valid(
	const bw_pink_filt_coeffs * BW_RESTRICT coeffs,
	const bw_pink_filt_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_pink_filt_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_pink_filt_coeffs_state {
	bw_pink_filt_coeffs_state_invalid,
	bw_pink_filt_coeffs_state_init,
	bw_pink_filt_coeffs_state_set_sample_rate,
	bw_pink_filt_coeffs_state_reset_coeffs
};
#endif

struct bw_pink_filt_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_pink_filt_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Coefficients
	float				scaling_k;
	
	// Parameters
	char				sample_rate_scaling;
};

struct bw_pink_filt_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// States
	float		s1_z1;
	float		s2_z1;
	float		s3_z1;
	float		s4_z1;
};

static inline void bw_pink_filt_init(
		bw_pink_filt_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	coeffs->sample_rate_scaling = 0;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_pink_filt_coeffs");
	coeffs->state = bw_pink_filt_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_pink_filt_coeffs_state_init);
}

static inline void bw_pink_filt_set_sample_rate(
		bw_pink_filt_coeffs * BW_RESTRICT coeffs,
		float                             sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	coeffs->scaling_k = 210.f / bw_sqrtf(sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_pink_filt_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_pink_filt_coeffs_state_set_sample_rate);
}

static inline void bw_pink_filt_reset_coeffs(
		bw_pink_filt_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_set_sample_rate);

	(void)coeffs;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_pink_filt_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_pink_filt_coeffs_state_reset_coeffs);
}

static inline float bw_pink_filt_reset_state(
		const bw_pink_filt_coeffs * BW_RESTRICT coeffs,
		bw_pink_filt_state * BW_RESTRICT        state,
		float                                   x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(bw_is_finite(x_0));

	(void)coeffs;
	state->s1_z1 = x_0;
	state->s2_z1 = x_0;
	state->s3_z1 = x_0;
	state->s4_z1 = x_0;
	const float y = coeffs->sample_rate_scaling ? coeffs->scaling_k * x_0 : x_0;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_pink_filt_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_pink_filt_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_pink_filt_reset_state_multi(
		const bw_pink_filt_coeffs * BW_RESTRICT              coeffs,
		bw_pink_filt_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                        x_0,
		float *                                              y_0,
		size_t                                               n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_pink_filt_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_pink_filt_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_pink_filt_update_coeffs_ctrl(
		bw_pink_filt_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_pink_filt_update_coeffs_audio(
		bw_pink_filt_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline float bw_pink_filt_process1(
		const bw_pink_filt_coeffs * BW_RESTRICT coeffs,
		bw_pink_filt_state * BW_RESTRICT        state,
		float                                   x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	(void)coeffs;
	const float s1 = 0.320696754235142f * x + state->s1_z1;
	state->s1_z1 = 0.999760145116749f * s1 - 0.3204568993518913f * x;
	const float s2 = 0.2870206617007935f * s1 + state->s2_z1;
	state->s2_z1 = 0.9974135207366259f * s2 - 0.2844341824374191f * s1;
	const float s3 = 0.2962862885898576f * s2 + state->s3_z1;
	state->s3_z1 = 0.9687905029568185f * s3 - 0.265076791546676f * s2;
	const float s4 = 0.3882183163519794f * s3 + state->s4_z1;
	state->s4_z1 = 0.6573784623288251f * s4 - 0.04559677868080467f * s3;

	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_pink_filt_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(s4));

	return s4;
}

static inline float bw_pink_filt_process1_scaling(
		const bw_pink_filt_coeffs * BW_RESTRICT coeffs,
		bw_pink_filt_state * BW_RESTRICT        state,
		float                                   x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	const float y = coeffs->scaling_k * bw_pink_filt_process1(coeffs, state, x);

	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_pink_filt_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_pink_filt_process(
		bw_pink_filt_coeffs * BW_RESTRICT coeffs,
		bw_pink_filt_state * BW_RESTRICT  state,
		const float *                     x,
		float *                           y,
		size_t                            n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_state_is_valid(coeffs, state));
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != NULL);

	if (coeffs->sample_rate_scaling)
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_pink_filt_process1_scaling(coeffs, state, x[i]);
	else
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_pink_filt_process1(coeffs, state, x[i]);

	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_pink_filt_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_pink_filt_process_multi(
		bw_pink_filt_coeffs * BW_RESTRICT                    coeffs,
		bw_pink_filt_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                                x,
		float * const *                                      y,
		size_t                                               n_channels,
		size_t                                               n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
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

	for (size_t i = 0; i < n_channels; i++)
		bw_pink_filt_process(coeffs, state[i], x[i], y[i], n_samples);

	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_reset_coeffs);
}

static inline void bw_pink_filt_set_sample_rate_scaling(
		bw_pink_filt_coeffs * BW_RESTRICT coeffs,
		char                              value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_init);

	coeffs->sample_rate_scaling = value;

	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_init);
}

static inline float bw_pink_filt_get_scaling_k(
		const bw_pink_filt_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_pink_filt_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_pink_filt_coeffs_state_set_sample_rate);

	return coeffs->scaling_k;
}

static inline char bw_pink_filt_coeffs_is_valid(
		const bw_pink_filt_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_pink_filt_coeffs"))
		return 0;
	if (coeffs->state < bw_pink_filt_coeffs_state_init || coeffs->state > bw_pink_filt_coeffs_state_reset_coeffs)
		return 0;

	if (coeffs->state >= bw_pink_filt_coeffs_state_set_sample_rate && coeffs->scaling_k <= 0.f)
		return 0;
#endif

	(void)coeffs;

	return 1;
}

static inline char bw_pink_filt_state_is_valid(
		const bw_pink_filt_coeffs * BW_RESTRICT coeffs,
		const bw_pink_filt_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_pink_filt_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	(void)coeffs;

	return bw_is_finite(state->s1_z1) && bw_is_finite(state->s2_z1) && bw_is_finite(state->s3_z1) && bw_is_finite(state->s4_z1);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::PinkFilt
 *  ```>>> */
template<size_t N_CHANNELS>
class PinkFilt {
public:
	PinkFilt();

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

	void setSampleRateScaling(
		bool value);

	float getScalingK();
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_pink_filt_coeffs			coeffs;
	bw_pink_filt_state			states[N_CHANNELS];
	bw_pink_filt_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline PinkFilt<N_CHANNELS>::PinkFilt() {
	bw_pink_filt_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_pink_filt_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_pink_filt_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_pink_filt_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_pink_filt_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_pink_filt_reset_coeffs(&coeffs);
	bw_pink_filt_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_pink_filt_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::setSampleRateScaling(
		bool value) {
	bw_pink_filt_set_sample_rate_scaling(&coeffs, value);
}

template<size_t N_CHANNELS>
inline float PinkFilt<N_CHANNELS>::getScalingK() {
	return bw_pink_filt_get_scaling_k(&coeffs);
}

}
#endif

#endif
