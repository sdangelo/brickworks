/*
 * Brickworks
 *
 * Copyright (C) 2023, 2024 Orastron Srl unipersonale
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
 *  version {{{ 1.1.1 }}}
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    Antialiased hard clipper with parametric bias and gain
 *    (compensation) and output bias removal.
 *
 *    In other words this implements (approximately)
 *
 *    > y(n) = clip(gain \* x(n) + bias, -1, 1) - clip(bias, -1, 1)
 *
 *    with antialiasing and optionally dividing the output by gain.
 *
 *    As a side effect, antialiasing causes attenuation at higher frequencies
 *    (about 3 dB at 0.5 × Nyquist frequency and rapidly increasing at higher
 *    frequencies).
 * 
 *    The antialiasing technique used here is described in
 *
 *    J. D. Parker, V. Zavalishin, and E. Le Bivic, "Reducing the Aliasing of
 *    Nonlinear Waveshaping Using Continuous-Time Convolution", Proc. 19th Intl.
 *    Conf. Digital Audio Effects (DAFx-16), pp. 137-144, Brno, Czech Republic,
 *    September 2016.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.1.1</strong>:
 *        <ul>
 *          <li>Added debugging check in <code>bw_clip_process_multi()</code> to
 *              ensure that buffers used for both input and output appear at the
 *              same channel indices.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.1.0</strong>:
 *        <ul>
 *          <li>Now using <code>BW_NULL</code> and
 *              <code>BW_CXX_NO_ARRAY</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Changed default value for gain compensation to off.</li>
 *          <li>Added initial input value to
 *              <code>bw_clip_reset_state()</code>.</li>
 *          <li>Added <code>bw_clip_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_clip_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_clip_process()</code> and
 *              <code>bw_clip_process_multi()</code> now use <code>size_t</code>
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
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_CLIP_H
#define BW_CLIP_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_clip_coeffs
 *  ```>>> */
typedef struct bw_clip_coeffs bw_clip_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_clip_state
 *  ```>>> */
typedef struct bw_clip_state bw_clip_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_clip_init()
 *  ```>>> */
static inline void bw_clip_init(
	bw_clip_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_clip_set_sample_rate()
 *  ```>>> */
static inline void bw_clip_set_sample_rate(
	bw_clip_coeffs * BW_RESTRICT coeffs,
	float                        sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_clip_reset_coeffs()
 *  ```>>> */
static inline void bw_clip_reset_coeffs(
	bw_clip_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_clip_reset_state()
 *  ```>>> */
static inline float bw_clip_reset_state(
	const bw_clip_coeffs * BW_RESTRICT coeffs,
	bw_clip_state * BW_RESTRICT        state,
	float                              x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_clip_reset_state_multi()
 *  ```>>> */
static inline void bw_clip_reset_state_multi(
	const bw_clip_coeffs * BW_RESTRICT              coeffs,
	bw_clip_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                   x_0,
	float *                                         y_0,
	size_t                                          n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `BW_NULL`.
 *
 *    #### bw_clip_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_clip_update_coeffs_ctrl(
	bw_clip_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_clip_update_coeffs_audio()
 *  ```>>> */
static inline void bw_clip_update_coeffs_audio(
	bw_clip_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_clip_process1()
 *  ```>>> */
static inline float bw_clip_process1(
	const bw_clip_coeffs * BW_RESTRICT coeffs,
	bw_clip_state * BW_RESTRICT        state,
	float                              x);

static inline float bw_clip_process1_comp(
	const bw_clip_coeffs * BW_RESTRICT coeffs,
	bw_clip_state * BW_RESTRICT        state,
	float                              x);
/*! <<<```
 *    These function process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_clip_process1()` assumes that gain compensation is disabled;
 *     * `bw_clip_process1_comp()` assumes that gain compensation is enabled.
 *
 *    The actual gain compensation parameter value is ignored.
 *
 *    #### bw_clip_process()
 *  ```>>> */
static inline void bw_clip_process(
	bw_clip_coeffs * BW_RESTRICT coeffs,
	bw_clip_state * BW_RESTRICT  state,
	const float *                x,
	float *                      y,
	size_t                       n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_clip_process_multi()
 *  ```>>> */
static inline void bw_clip_process_multi(
	bw_clip_coeffs * BW_RESTRICT                    coeffs,
	bw_clip_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_clip_set_bias()
 *  ```>>> */
static inline void bw_clip_set_bias(
	bw_clip_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the input bias `value` in `coeffs`.
 *
 *    Valid range: [`-1e12f`, `1e12f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_clip_set_gain()
 *  ```>>> */
static inline void bw_clip_set_gain(
	bw_clip_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the gain `value` in `coeffs`.
 *
 *    Valid range: [`1e-12f`, `1e12f`].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_clip_set_gain_compensation()
 *  ```>>> */
static inline void bw_clip_set_gain_compensation(
	bw_clip_coeffs * BW_RESTRICT coeffs,
	char                         value);
/*! <<<```
 *    Sets whether the output should be divided by gain (`value` non-`0`) or not
 *    (`0`).
 *
 *    Default value: `0` (off).
 *
 *    #### bw_clip_coeffs_is_valid()
 *  ```>>> */
static inline char bw_clip_coeffs_is_valid(
	const bw_clip_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_clip_coeffs`.
 *
 *    #### bw_clip_state_is_valid()
 *  ```>>> */
static inline char bw_clip_state_is_valid(
	const bw_clip_coeffs * BW_RESTRICT coeffs,
	const bw_clip_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `BW_NULL` extra cross-checks might be performed
 *    (`state` is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_clip_state`.
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
enum bw_clip_coeffs_state {
	bw_clip_coeffs_state_invalid,
	bw_clip_coeffs_state_init,
	bw_clip_coeffs_state_set_sample_rate,
	bw_clip_coeffs_state_reset_coeffs
};
#endif

struct bw_clip_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_clip_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_one_pole_coeffs		smooth_coeffs;
	bw_one_pole_state		smooth_bias_state;
	bw_one_pole_state		smooth_gain_state;
	
	// Coefficients
	float				bias_dc;
	float				inv_gain;

	// Parameters
	float				bias;
	float				gain;
	char				gain_compensation;
};

struct bw_clip_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// States
	float		x_z1;
	float		F_z1;
};

static inline void bw_clip_init(
		bw_clip_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->bias = 0.f;
	coeffs->gain = 1.f;
	coeffs->gain_compensation = 0;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_clip_coeffs");
	coeffs->state = bw_clip_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_clip_coeffs_state_init);
}

static inline void bw_clip_set_sample_rate(
		bw_clip_coeffs * BW_RESTRICT coeffs,
		float                        sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_clip_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_clip_coeffs_state_set_sample_rate);
}

static inline void bw_clip_do_update_coeffs(
		bw_clip_coeffs * BW_RESTRICT coeffs,
		char                         force) {
	float bias_cur = bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	if (force || coeffs->bias != bias_cur) {
		bias_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_bias_state, coeffs->bias);
		coeffs->bias_dc = bw_clipf(bias_cur, -1.f, 1.f);
	}
	float gain_cur = bw_one_pole_get_y_z1(&coeffs->smooth_gain_state);
	if (force || coeffs->gain != gain_cur) {
		gain_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_gain_state, coeffs->gain);
		coeffs->inv_gain = bw_rcpf(gain_cur);
	}
}

static inline void bw_clip_reset_coeffs(
		bw_clip_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_set_sample_rate);

	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_bias_state, coeffs->bias);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_gain_state, coeffs->gain);
	bw_clip_do_update_coeffs(coeffs, 1);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_clip_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_clip_coeffs_state_reset_coeffs);
}

static inline float bw_clip_reset_state(
		const bw_clip_coeffs * BW_RESTRICT coeffs,
		bw_clip_state * BW_RESTRICT        state,
		float                              x_0) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT(bw_is_finite(x_0));

	const float x = bw_one_pole_get_y_z1(&coeffs->smooth_gain_state) * x_0 + bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	const float a = bw_absf(x);
	const float F = a > 1.f ? a - 0.5f : 0.5f * a * a;
	const float yb = bw_clipf(x, -1.f, 1.f);
	const float y = (coeffs->gain_compensation ? coeffs->inv_gain : 1.f) * (yb - coeffs->bias_dc);
	state->x_z1 = x;
	state->F_z1 = F;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_clip_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_clip_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_clip_reset_state_multi(
		const bw_clip_coeffs * BW_RESTRICT              coeffs,
		bw_clip_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                   x_0,
		float *                                         y_0,
		size_t                                          n_channels) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != BW_NULL);

	if (y_0 != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_clip_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_clip_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != BW_NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_clip_update_coeffs_ctrl(
		bw_clip_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_clip_update_coeffs_audio(
		bw_clip_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);

	bw_clip_do_update_coeffs(coeffs, 0);

	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
}

static inline float bw_clip_process1(
		const bw_clip_coeffs * BW_RESTRICT coeffs,
		bw_clip_state * BW_RESTRICT        state,
		float                              x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	x = bw_one_pole_get_y_z1(&coeffs->smooth_gain_state) * x + bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	const float a = bw_absf(x);
	const float F = a > 1.f ? a - 0.5f : 0.5f * a * a;
	const float d = x - state->x_z1;
	const float yb = d * d < 1e-6f ? bw_clipf(0.5f * (x + state->x_z1), -1.f, 1.f) : (F - state->F_z1) * bw_rcpf(d);
	const float y = yb - coeffs->bias_dc;
	state->x_z1 = x;
	state->F_z1 = F;

	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_clip_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_clip_process1_comp(
		const bw_clip_coeffs * BW_RESTRICT coeffs,
		bw_clip_state * BW_RESTRICT        state,
		float                              x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	const float y = coeffs->inv_gain * bw_clip_process1(coeffs, state, x);

	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_clip_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_clip_process(
		bw_clip_coeffs * BW_RESTRICT coeffs,
		bw_clip_state * BW_RESTRICT  state,
		const float *                x,
		float *                      y,
		size_t                       n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_state_is_valid(coeffs, state));
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != BW_NULL);

	if (coeffs->gain_compensation)
		for (size_t i = 0; i < n_samples; i++) {
			bw_clip_update_coeffs_audio(coeffs);
			y[i] = bw_clip_process1_comp(coeffs, state, x[i]);
		}
	else
		for (size_t i = 0; i < n_samples; i++) {
			bw_clip_update_coeffs_audio(coeffs);
			y[i] = bw_clip_process1(coeffs, state, x[i]);
		}

	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_clip_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_clip_process_multi(
		bw_clip_coeffs * BW_RESTRICT                    coeffs,
		bw_clip_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                           x,
		float * const *                                 y,
		size_t                                          n_channels,
		size_t                                          n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT(y != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(y[i] != y[j]);
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = 0; j < n_channels; j++)
			BW_ASSERT(i == j || x[i] != y[j]);
#endif

	if (coeffs->gain_compensation)
		for (size_t i = 0; i < n_samples; i++) {
			bw_clip_update_coeffs_audio(coeffs);
			for (size_t j = 0; j < n_channels; j++)
				y[j][i] = bw_clip_process1_comp(coeffs, state[j], x[j][i]);
		}
	else
		for (size_t i = 0; i < n_samples; i++) {
			bw_clip_update_coeffs_audio(coeffs);
			for (size_t j = 0; j < n_channels; j++)
				y[j][i] = bw_clip_process1(coeffs, state[j], x[j][i]);
		}

	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_reset_coeffs);
}

static inline void bw_clip_set_bias(
		bw_clip_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= -1e12f && value <= 1e12f);

	coeffs->bias = value;

	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_init);
}

static inline void bw_clip_set_gain(
		bw_clip_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-12f && value <= 1e12f);

	coeffs->gain = value;

	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_init);
}

static inline void bw_clip_set_gain_compensation(
		bw_clip_coeffs * BW_RESTRICT coeffs,
		char                         value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_init);

	coeffs->gain_compensation = value;

	BW_ASSERT_DEEP(bw_clip_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_clip_coeffs_state_init);
}

static inline char bw_clip_coeffs_is_valid(
		const bw_clip_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_clip_coeffs"))
		return 0;
	if (coeffs->state < bw_clip_coeffs_state_init || coeffs->state > bw_clip_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->bias) || coeffs->bias < -1e12f || coeffs->bias > 1e12f)
		return 0;
	if (!bw_is_finite(coeffs->gain) || coeffs->gain < 1e-12f || coeffs->gain > 1e12f)
		return 0;

	if (!bw_one_pole_coeffs_is_valid(&coeffs->smooth_coeffs))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_clip_coeffs_state_reset_coeffs) {
		if (!bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_bias_state))
			return 0;
		if (!bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_gain_state))
			return 0;

		if (!bw_is_finite(coeffs->bias_dc) || coeffs->bias_dc < -1.f || coeffs->bias_dc > 1.f)
			return 0;
		if (!bw_is_finite(coeffs->inv_gain) || coeffs->inv_gain <= 0.f)
			return 0;
	}
#endif

	return 1;
}

static inline char bw_clip_state_is_valid(
		const bw_clip_coeffs * BW_RESTRICT coeffs,
		const bw_clip_state * BW_RESTRICT  state) {
	BW_ASSERT(state != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_clip_state"))
		return 0;

	if (coeffs != BW_NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	(void)coeffs;

	return bw_is_finite(state->x_z1) && bw_is_finite(state->F_z1);
}

#ifdef __cplusplus
}

#ifndef BW_CXX_NO_ARRAY
# include <array>
#endif

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Clip
 *  ```>>> */
template<size_t N_CHANNELS>
class Clip {
public:
	Clip();

	void setSampleRate(
		float sampleRate);

	void reset(
		float               x0 = 0.f,
		float * BW_RESTRICT y0 = nullptr);

#ifndef BW_CXX_NO_ARRAY
	void reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0);
#endif

	void reset(
		const float * x0,
		float *       y0 = nullptr);

#ifndef BW_CXX_NO_ARRAY
	void reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0 = nullptr);
#endif

	void process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples);

#ifndef BW_CXX_NO_ARRAY
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);
#endif

	void setBias(
		float value);

	void setGain(
		float value);

	void setGainCompensation(
		bool value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_clip_coeffs			coeffs;
	bw_clip_state			states[N_CHANNELS];
	bw_clip_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Clip<N_CHANNELS>::Clip() {
	bw_clip_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_clip_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_clip_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_clip_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_clip_reset_state(&coeffs, states + i, x0);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}
#endif

template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_clip_reset_coeffs(&coeffs);
	bw_clip_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}
#endif

template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_clip_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}
#endif

template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::setBias(
		float value) {
	bw_clip_set_bias(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::setGain(
		float value) {
	bw_clip_set_gain(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Clip<N_CHANNELS>::setGainCompensation(
		bool value) {
	bw_clip_set_gain_compensation(&coeffs, value);
}

}
#endif

#endif
