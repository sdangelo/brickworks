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
 *  requires {{{ bw_common bw_gain bw_math bw_mm2 bw_one_pole bw_svf }}}
 *  description {{{
 *    Second-order peak filter with unitary gain at DC and asymptotically
 *    as frequency increases.
 *
 *    The quality factor of the underlying bandpass filter can be either
 *    directly controlled via the Q parameter or indirectly through the
 *    bandwidth parameter, which designates the distance in octaves between
 *    midpoint gain frequencies, i.e., frequencies with gain = peak gain / 2 in
 *    dB terms. The use_bandiwdth parameter allows you to choose which
 *    parameterization to use.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.1.1</strong>:
 *        <ul>
 *          <li>Added debugging check in <code>bw_peak_process_multi()</code> to
 *              ensure that buffers used for both input and output appear at the
 *              same channel indices.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.1.0</strong>:
 *        <ul>
 *          <li>Now using <code>BW_NULL</code> and
 *              <code>BW_CXX_NO_ARRAY</code>.</li>
 *          <li>Replaced GCC pragmas to suppress bogus uninitialized variable
 *              warnings with useless harmless statement.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_peak_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_peak_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li>Added prewarp_at_cutoff and prewarp_freq parameters.</li>
 *          <li><code>bw_peak_process()</code> and
 *              <code>bw_peak_process_multi()</code> now use <code>size_t</code>
 *              to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Added pragmas to silence bogus GCC uninitialized variable
 *              warnings.</li>
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
 *          <li>Added <code>bw_peak_process_multi()</code>.</li>
 *          <li>Fixed documentation for <code>bw_peak_set_peak_gain_lin()</code>
 *              and <code>bw_peak_set_gain_dB()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_peak_reset_state()</code>.</li>
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

#ifndef BW_PEAK_H
#define BW_PEAK_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_peak_coeffs
 *  ```>>> */
typedef struct bw_peak_coeffs bw_peak_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_peak_state
 *  ```>>> */
typedef struct bw_peak_state bw_peak_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_peak_init()
 *  ```>>> */
static inline void bw_peak_init(
	bw_peak_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_peak_set_sample_rate()
 *  ```>>> */
static inline void bw_peak_set_sample_rate(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	float                        sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_peak_reset_coeffs()
 *  ```>>> */
static inline void bw_peak_reset_coeffs(
	bw_peak_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_peak_reset_state()
 *  ```>>> */
static inline float bw_peak_reset_state(
	const bw_peak_coeffs * BW_RESTRICT coeffs,
	bw_peak_state * BW_RESTRICT        state,
	float                              x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_peak_reset_state_multi()
 *  ```>>> */
static inline void bw_peak_reset_state_multi(
	const bw_peak_coeffs * BW_RESTRICT              coeffs,
	bw_peak_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_peak_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_peak_update_coeffs_ctrl(
	bw_peak_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_peak_update_coeffs_audio()
 *  ```>>> */
static inline void bw_peak_update_coeffs_audio(
	bw_peak_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_peak_process1()
 *  ```>>> */
static inline float bw_peak_process1(
	const bw_peak_coeffs * BW_RESTRICT coeffs,
	bw_peak_state * BW_RESTRICT        state,
	float                              x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_peak_process()
 *  ```>>> */
static inline void bw_peak_process(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	bw_peak_state * BW_RESTRICT  state,
	const float *                x,
	float *                      y,
	size_t                       n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_peak_process_multi()
 *  ```>>> */
static inline void bw_peak_process_multi(
	bw_peak_coeffs * BW_RESTRICT                    coeffs,
	bw_peak_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_peak_set_cutoff()
 *  ```>>> */
static inline void bw_peak_set_cutoff(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Valid range: [`1e-6f`, `1e12f`].
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_peak_set_Q()
 *  ```>>> */
static inline void bw_peak_set_Q(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the quality factor to the given `value` in `coeffs`.
 *
 *    Valid range: [`1e-6f`, `1e6f`].
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_peak_set_prewarp_at_cutoff()
 *  ```>>> */
static inline void bw_peak_set_prewarp_at_cutoff(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	char                         value);
/*! <<<```
 *    Sets whether bilinear transform prewarping frequency should match the
 *    cutoff frequency (non-`0`) or not (`0`).
 *
 *    Default value: non-`0` (on).
 *
 *    #### bw_peak_set_prewarp_freq()
 *  ```>>> */
static inline void bw_peak_set_prewarp_freq(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	float                        value);
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
 *    #### bw_peak_set_peak_gain_lin()
 *  ```>>> */
static inline void bw_peak_set_peak_gain_lin(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the peak gain parameter to the given `value` (linear gain) in
 *    `coeffs`.
 *
 *    Valid range: [`1e-30f`, `1e30f`].
 *
 *    If actually using the bandwidth parameter to control Q,  by the time
 *    `bw_peak_reset_coeffs()`, `bw_peak_update_coeffs_ctrl()`,
 *    `bw_peak_update_coeffs_audio()`, `bw_peak_process1()`,
 *    `bw_peak_process()`, or `bw_peak_process_multi()` is called,
 *    `bw_sqrtf(bw_pow2f(bandwidth) * peak_gain) *
 *    bw_rcpf(bw_pow2f(bandwidth) - 1.f)` must be in [`1e-6f`, `1e6f`].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_peak_set_peak_gain_dB()
 *  ```>>> */
static inline void bw_peak_set_peak_gain_dB(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the peak gain parameter to the given `value` (dB) in `coeffs`.
 *
 *    Valid range: [`-600.f`, `600.f`].
 *
 *    If actually using the bandwidth parameter to control Q,  by the time
 *    `bw_peak_reset_coeffs()`, `bw_peak_update_coeffs_ctrl()`,
 *    `bw_peak_update_coeffs_audio()`, `bw_peak_process1()`,
 *    `bw_peak_process()`, or `bw_peak_process_multi()` is called,
 *    `bw_sqrtf(bw_pow2f(bandwidth) * peak_gain) *
 *    bw_rcpf(bw_pow2f(bandwidth) - 1.f)` must be in [`1e-6f`, `1e6f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_peak_set_bandiwdth()
 *  ```>>> */
static inline void bw_peak_set_bandwidth(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the bandwidth `value` (octaves) in `coeffs`.
 *
 *    Valid range: [`1e-6f`, `90.f`].
 *
 *    If actually using the bandwidth parameter to control Q,  by the time
 *    `bw_peak_reset_coeffs()`, `bw_peak_update_coeffs_ctrl()`,
 *    `bw_peak_update_coeffs_audio()`, `bw_peak_process1()`,
 *    `bw_peak_process()`, or `bw_peak_process_multi()` is called,
 *    `bw_sqrtf(bw_pow2f(bandwidth) * peak_gain) *
 *    bw_rcpf(bw_pow2f(bandwidth) - 1.f)` must be in [`1e-6f`, `1e6f`].
 *
 *    Default value: `2.543106606327224f`.
 *
 *    #### bw_peak_set_use_bandwidth()
 *  ```>>> */
static inline void bw_peak_set_use_bandwidth(
	bw_peak_coeffs * BW_RESTRICT coeffs,
	char                         value);
/*! <<<```
 *    Sets whether the quality factor should be controlled via the bandwidth
 *    parameter (`value` non-`0`) or via the Q parameter (`0`).
 *
 *    Default value: non-`0` (use bandwidth parameter).
 *
 *    #### bw_peak_coeffs_is_valid()
 *  ```>>> */
static inline char bw_peak_coeffs_is_valid(
	const bw_peak_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_peak_coeffs`.
 *
 *    #### bw_peak_state_is_valid()
 *  ```>>> */
static inline char bw_peak_state_is_valid(
	const bw_peak_coeffs * BW_RESTRICT coeffs,
	const bw_peak_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `BW_NULL` extra cross-checks might be performed
 *    (`state` is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_peak_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_mm2.h>
#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_peak_coeffs_state {
	bw_peak_coeffs_state_invalid,
	bw_peak_coeffs_state_init,
	bw_peak_coeffs_state_set_sample_rate,
	bw_peak_coeffs_state_reset_coeffs
};
#endif

struct bw_peak_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_peak_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_mm2_coeffs			mm2_coeffs;

	// Coefficients
	float				bw_k;

	// Parameters
	float				Q;
	float				peak_gain;
	float				bandwidth;
	char				use_bandwidth;
	int				param_changed;
};

struct bw_peak_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// Sub-components
	bw_mm2_state	mm2_state;
};

#define BW_PEAK_PARAM_Q		1
#define BW_PEAK_PARAM_PEAK_GAIN	(1<<1)
#define BW_PEAK_PARAM_BANDWIDTH	(1<<2)

static inline void bw_peak_init(
		bw_peak_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	bw_mm2_init(&coeffs->mm2_coeffs);
	coeffs->Q = 0.5f;
	coeffs->peak_gain = 1.f;
	coeffs->bandwidth = 2.543106606327224f;
	coeffs->use_bandwidth = 1;
	
	coeffs->param_changed = ~0; // useless, just to make compilers happy about uninitialized variables

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_peak_coeffs");
	coeffs->state = bw_peak_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_peak_coeffs_state_init);
}

static inline void bw_peak_set_sample_rate(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		float                        sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_mm2_set_sample_rate(&coeffs->mm2_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_peak_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_peak_coeffs_state_set_sample_rate);
}

static inline void bw_peak_update_mm2_params(
		bw_peak_coeffs * BW_RESTRICT coeffs) {
	if (coeffs->param_changed) {
		if (coeffs->use_bandwidth) {
			if (coeffs->param_changed & (BW_PEAK_PARAM_PEAK_GAIN | BW_PEAK_PARAM_BANDWIDTH)) {
				if (coeffs->param_changed & BW_PEAK_PARAM_BANDWIDTH)
					coeffs->bw_k = bw_pow2f(coeffs->bandwidth);
				const float Q = bw_sqrtf(coeffs->bw_k * coeffs->peak_gain) * bw_rcpf(coeffs->bw_k - 1.f);
				bw_mm2_set_Q(&coeffs->mm2_coeffs, Q);
				bw_mm2_set_coeff_bp(&coeffs->mm2_coeffs, (coeffs->peak_gain - 1.f) * bw_rcpf(Q));
			}
		} else {
			if (coeffs->param_changed & (BW_PEAK_PARAM_PEAK_GAIN | BW_PEAK_PARAM_Q)) {
				if (coeffs->param_changed & BW_PEAK_PARAM_Q)
					bw_mm2_set_Q(&coeffs->mm2_coeffs, coeffs->Q);
				bw_mm2_set_coeff_bp(&coeffs->mm2_coeffs, (coeffs->peak_gain - 1.f) * bw_rcpf(coeffs->Q));
			}
		}
		coeffs->param_changed = 0;
	}
}

static inline void bw_peak_reset_coeffs(
		bw_peak_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_set_sample_rate);
	BW_ASSERT_DEEP(coeffs->use_bandwidth
		? bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) >= 1e-6f
			&& bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) <= 1e6f
		: 1);

	coeffs->param_changed = ~0;
	bw_peak_update_mm2_params(coeffs);
	bw_mm2_reset_coeffs(&coeffs->mm2_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_peak_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_peak_coeffs_state_reset_coeffs);
}

static inline float bw_peak_reset_state(
		const bw_peak_coeffs * BW_RESTRICT coeffs,
		bw_peak_state * BW_RESTRICT        state,
		float                              x_0) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT(bw_is_finite(x_0));

	const float y = bw_mm2_reset_state(&coeffs->mm2_coeffs, &state->mm2_state, x_0);

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_peak_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_peak_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_peak_reset_state_multi(
		const bw_peak_coeffs * BW_RESTRICT              coeffs,
		bw_peak_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                   x_0,
		float *                                         y_0,
		size_t                                          n_channels) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != BW_NULL);

	if (y_0 != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_peak_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_peak_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != BW_NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_peak_update_coeffs_ctrl(
		bw_peak_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->use_bandwidth
		? bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) >= 1e-6f
			&& bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) <= 1e6f
		: 1);

	bw_peak_update_mm2_params(coeffs);
	bw_mm2_update_coeffs_ctrl(&coeffs->mm2_coeffs);

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
}

static inline void bw_peak_update_coeffs_audio(
		bw_peak_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->use_bandwidth
		? bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) >= 1e-6f
			&& bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) <= 1e6f
		: 1);

	bw_mm2_update_coeffs_audio(&coeffs->mm2_coeffs);

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
}

static inline float bw_peak_process1(
		const bw_peak_coeffs * BW_RESTRICT coeffs,
		bw_peak_state * BW_RESTRICT        state,
		float                              x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->use_bandwidth
		? bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) >= 1e-6f
			&& bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) <= 1e6f
		: 1);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	const float y = bw_mm2_process1(&coeffs->mm2_coeffs, &state->mm2_state, x);

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_peak_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_peak_process(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		bw_peak_state * BW_RESTRICT  state,
		const float *                x,
		float *                      y,
		size_t                       n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->use_bandwidth
		? bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) >= 1e-6f
			&& bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) <= 1e6f
		: 1);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_state_is_valid(coeffs, state));
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != BW_NULL);

	bw_peak_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_peak_update_coeffs_audio(coeffs);
		y[i] = bw_peak_process1(coeffs, state, x[i]);
	}

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_peak_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_peak_process_multi(
		bw_peak_coeffs * BW_RESTRICT                    coeffs,
		bw_peak_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                           x,
		float * const *                                 y,
		size_t                                          n_channels,
		size_t                                          n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(coeffs->use_bandwidth
		? bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) >= 1e-6f
			&& bw_sqrtf(bw_pow2f(coeffs->bandwidth) * coeffs->peak_gain) * bw_rcpf(bw_pow2f(coeffs->bandwidth) - 1.f) <= 1e6f
		: 1);
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

	bw_peak_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_peak_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_peak_process1(coeffs, state[j], x[j][i]);
	}

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_reset_coeffs);
}

static inline void bw_peak_set_cutoff(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e12f);

	bw_mm2_set_cutoff(&coeffs->mm2_coeffs, value);

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
}

static inline void bw_peak_set_Q(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e6f);

	if (coeffs->Q != value) {
		coeffs->Q = value;
		coeffs->param_changed |= BW_PEAK_PARAM_Q;
	}

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
}

static inline void bw_peak_set_prewarp_at_cutoff(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		char                        value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);

	bw_mm2_set_prewarp_at_cutoff(&coeffs->mm2_coeffs, value);

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
}

static inline void bw_peak_set_prewarp_freq(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e12f);

	bw_mm2_set_prewarp_freq(&coeffs->mm2_coeffs, value);

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
}

static inline void bw_peak_set_peak_gain_lin(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-30f && value <= 1e30f);

	if (coeffs->peak_gain != value) {
		coeffs->peak_gain = value;
		coeffs->param_changed |= BW_PEAK_PARAM_PEAK_GAIN;
	}

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
}

static inline void bw_peak_set_peak_gain_dB(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= -600.f && value <= 600.f);

	bw_peak_set_peak_gain_lin(coeffs, bw_dB2linf(value));

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
}

static inline void bw_peak_set_bandwidth(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 90.f);

	if (coeffs->bandwidth != value) {
		coeffs->bandwidth = value;
		coeffs->param_changed |= BW_PEAK_PARAM_BANDWIDTH;
	}

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
}

static inline void bw_peak_set_use_bandwidth(
		bw_peak_coeffs * BW_RESTRICT coeffs,
		char                         value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);

	if ((coeffs->use_bandwidth && !value) || (!coeffs->use_bandwidth && value)) {
		coeffs->use_bandwidth = value;
		coeffs->param_changed |= BW_PEAK_PARAM_Q | BW_PEAK_PARAM_BANDWIDTH;
	}

	BW_ASSERT_DEEP(bw_peak_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_peak_coeffs_state_init);
}

static inline char bw_peak_coeffs_is_valid(
		const bw_peak_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_peak_coeffs"))
		return 0;
	if (coeffs->state < bw_peak_coeffs_state_init || coeffs->state > bw_peak_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->Q) || coeffs->Q < 1e-6f || coeffs->Q > 1e6f)
		return 0;
	if (!bw_is_finite(coeffs->peak_gain) || coeffs->peak_gain < 1e-30f || coeffs->peak_gain > 1e30f)
		return 0;
	if (!bw_is_finite(coeffs->bandwidth) || coeffs->bandwidth < 1e-6f || coeffs->bandwidth > 90.f)
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_peak_coeffs_state_reset_coeffs) {
		if (!bw_is_finite(coeffs->bw_k) || coeffs->bw_k <= 0.f)
			return 0;
	}
#endif

	return bw_mm2_coeffs_is_valid(&coeffs->mm2_coeffs);
}

static inline char bw_peak_state_is_valid(
		const bw_peak_coeffs * BW_RESTRICT coeffs,
		const bw_peak_state * BW_RESTRICT  state) {
	BW_ASSERT(state != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_peak_state"))
		return 0;

	if (coeffs != BW_NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	return bw_mm2_state_is_valid(coeffs ? &coeffs->mm2_coeffs : BW_NULL, &state->mm2_state);
}

#undef BW_PEAK_PARAM_Q
#undef BW_PEAK_PARAM_PEAK_GAIN
#undef BW_PEAK_PARAM_BANDWIDTH

#ifdef __cplusplus
}

#ifndef BW_CXX_NO_ARRAY
# include <array>
#endif

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Peak
 *  ```>>> */
template<size_t N_CHANNELS>
class Peak {
public:
	Peak();

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

	void setCutoff(
		float value);

	void setQ(
		float value);

	void setPrewarpAtCutoff(
		bool value);

	void setPrewarpFreq(
		float value);

	void setPeakGainLin(
		float value);

	void setPeakGainDB(
		float value);

	void setBandwidth(
		float value);

	void setUseBandwidth(
		bool value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_peak_coeffs			coeffs;
	bw_peak_state			states[N_CHANNELS];
	bw_peak_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Peak<N_CHANNELS>::Peak() {
	bw_peak_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_peak_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_peak_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_peak_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_peak_reset_state(&coeffs, states + i, x0);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : y0);
}
#endif

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_peak_reset_coeffs(&coeffs);
	bw_peak_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}
#endif

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_peak_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}
#endif

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::setCutoff(
		float value) {
	bw_peak_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::setQ(
		float value) {
	bw_peak_set_Q(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::setPrewarpAtCutoff(
		bool value) {
	bw_peak_set_prewarp_at_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::setPrewarpFreq(
		float value) {
	bw_peak_set_prewarp_freq(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::setPeakGainLin(
		float value) {
	bw_peak_set_peak_gain_lin(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::setPeakGainDB(
		float value) {
	bw_peak_set_peak_gain_dB(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::setBandwidth(
		float value) {
	bw_peak_set_bandwidth(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Peak<N_CHANNELS>::setUseBandwidth(
		bool value) {
	bw_peak_set_use_bandwidth(&coeffs, value);
}

}
#endif

#endif
