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
 *  requires {{{ bw_common bw_lp1 bw_math bw_one_pole }}}
 *  description {{{
 *    First-order highpass filter (6 dB/oct) with gain asymptotically
 *    approaching unity as frequency increases.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_hp1_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_hp1_reset_state()</code> returns the initial output
 *              value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li>Added prewarp_at_cutoff and prewarp_freq parameters.</li>
 *          <li><code>bw_hp1_process()</code> and
 *              <code>bw_hp1_process_multi()</code> now use <code>size_t</code>
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
 *          <li>Added <code>bw_hp1_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_hp1_reset_state()</code>.</li>
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

#ifndef BW_HP1_H
#define BW_HP1_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_hp1_coeffs
 *  ```>>> */
typedef struct bw_hp1_coeffs bw_hp1_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_hp1_state
 *  ```>>> */
typedef struct bw_hp1_state bw_hp1_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_hp1_init()
 *  ```>>> */
static inline void bw_hp1_init(
	bw_hp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_hp1_set_sample_rate()
 *  ```>>> */
static inline void bw_hp1_set_sample_rate(
	bw_hp1_coeffs * BW_RESTRICT coeffs,
	float                       sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_hp1_reset_coeffs()
 *  ```>>> */
static inline void bw_hp1_reset_coeffs(
	bw_hp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_hp1_reset_state()
 *  ```>>> */
static inline float bw_hp1_reset_state(
	const bw_hp1_coeffs * BW_RESTRICT coeffs,
	bw_hp1_state * BW_RESTRICT        state,
	float                             x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_hp1_reset_state_multi()
 *  ```>>> */
static inline void bw_hp1_reset_state_multi(
	const bw_hp1_coeffs * BW_RESTRICT              coeffs,
	bw_hp1_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_hp1_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_hp1_update_coeffs_ctrl(
	bw_hp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_hp1_update_coeffs_audio()
 *  ```>>> */
static inline void bw_hp1_update_coeffs_audio(
	bw_hp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_hp1_process1()
 *  ```>>> */
static inline float bw_hp1_process1(
	const bw_hp1_coeffs * BW_RESTRICT coeffs,
	bw_hp1_state * BW_RESTRICT        state,
	float                             x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_hp1_process()
 *  ```>>> */
static inline void bw_hp1_process(
	bw_hp1_coeffs * BW_RESTRICT coeffs,
	bw_hp1_state * BW_RESTRICT  state,
	const float *               x,
	float *                     y,
	size_t                      n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_hp1_process_multi()
 *  ```>>> */
static inline void bw_hp1_process_multi(
	bw_hp1_coeffs * BW_RESTRICT                    coeffs,
	bw_hp1_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_hp1_set_cutoff()
 *  ```>>> */
static inline void bw_hp1_set_cutoff(
	bw_hp1_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Valid range: [`1e-6f`, `1e12f`].
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_hp1_set_prewarp_at_cutoff()
 *  ```>>> */
static inline void bw_hp1_set_prewarp_at_cutoff(
	bw_hp1_coeffs * BW_RESTRICT coeffs,
	char                        value);
/*! <<<```
 *    Sets whether bilinear transform prewarping frequency should match the
 *    cutoff frequency (non-`0`) or not (`0`).
 *
 *    Default value: non-`0` (on).
 *
 *    #### bw_hp1_set_prewarp_freq()
 *  ```>>> */
static inline void bw_hp1_set_prewarp_freq(
	bw_hp1_coeffs * BW_RESTRICT coeffs,
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
 *    #### bw_hp1_coeffs_is_valid()
 *  ```>>> */
static inline char bw_hp1_coeffs_is_valid(
	const bw_hp1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_hp1_coeffs`.
 *
 *    #### bw_hp1_state_is_valid()
 *  ```>>> */
static inline char bw_hp1_state_is_valid(
	const bw_hp1_coeffs * BW_RESTRICT coeffs,
	const bw_hp1_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_hp1_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_lp1.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_hp1_coeffs_state {
	bw_hp1_coeffs_state_invalid,
	bw_hp1_coeffs_state_init,
	bw_hp1_coeffs_state_set_sample_rate,
	bw_hp1_coeffs_state_reset_coeffs
};
#endif

struct bw_hp1_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_hp1_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_lp1_coeffs			lp1_coeffs;
};

struct bw_hp1_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// Sub-components
	bw_lp1_state	lp1_state;
};

static inline void bw_hp1_init(
		bw_hp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	bw_lp1_init(&coeffs->lp1_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_hp1_coeffs");
	coeffs->state = bw_hp1_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_hp1_coeffs_state_init);
}

static inline void bw_hp1_set_sample_rate(
		bw_hp1_coeffs * BW_RESTRICT coeffs,
		float                       sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_lp1_set_sample_rate(&coeffs->lp1_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_hp1_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_hp1_coeffs_state_set_sample_rate);
}

static inline void bw_hp1_reset_coeffs(
		bw_hp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_set_sample_rate);

	bw_lp1_reset_coeffs(&coeffs->lp1_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_hp1_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_hp1_coeffs_state_reset_coeffs);
}

static inline float bw_hp1_reset_state(
		const bw_hp1_coeffs * BW_RESTRICT coeffs,
		bw_hp1_state * BW_RESTRICT        state,
		float                             x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(bw_is_finite(x_0));

	const float lp = bw_lp1_reset_state(&coeffs->lp1_coeffs, &state->lp1_state, x_0);
	const float y = x_0 - lp;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_hp1_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_hp1_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_hp1_reset_state_multi(
		const bw_hp1_coeffs * BW_RESTRICT              coeffs,
		bw_hp1_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                  x_0,
		float *                                        y_0,
		size_t                                         n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(x_0 != NULL);

	for (size_t i = 0; i < n_channels; i++)
		bw_hp1_reset_state(coeffs, state[i], x_0[i]);
	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = 0.f;

	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_hp1_update_coeffs_ctrl(
		bw_hp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);

	bw_lp1_update_coeffs_ctrl(&coeffs->lp1_coeffs);

	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
}

static inline void bw_hp1_update_coeffs_audio(
		bw_hp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);

	bw_lp1_update_coeffs_audio(&coeffs->lp1_coeffs);

	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
}

static inline float bw_hp1_process1(
		const bw_hp1_coeffs * BW_RESTRICT coeffs,
		bw_hp1_state * BW_RESTRICT        state,
		float                             x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_hp1_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	const float lp = bw_lp1_process1(&coeffs->lp1_coeffs, &state->lp1_state, x);
	const float y = x - lp;

	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_hp1_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_hp1_process(
		bw_hp1_coeffs * BW_RESTRICT coeffs,
		bw_hp1_state * BW_RESTRICT  state,
		const float *               x,
		float *                     y,
		size_t                      n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_hp1_state_is_valid(coeffs, state));
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != NULL);

	bw_hp1_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_hp1_update_coeffs_audio(coeffs);
		y[i] = bw_hp1_process1(coeffs, state, x[i]);
	}

	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_hp1_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_hp1_process_multi(
		bw_hp1_coeffs * BW_RESTRICT                    coeffs,
		bw_hp1_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                          x,
		float * const *                                y,
		size_t                                         n_channels,
		size_t                                         n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(x != NULL);
	BW_ASSERT(y != NULL);

	bw_hp1_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_hp1_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_hp1_process1(coeffs, state[j], x[j][i]);
	}

	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_reset_coeffs);
}

static inline void bw_hp1_set_cutoff(
		bw_hp1_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e12f);

	bw_lp1_set_cutoff(&coeffs->lp1_coeffs, value);

	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_init);
}

static inline void bw_hp1_set_prewarp_at_cutoff(
		bw_hp1_coeffs * BW_RESTRICT coeffs,
		char                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_init);

	bw_lp1_set_prewarp_at_cutoff(&coeffs->lp1_coeffs, value);

	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_init);
}

static inline void bw_hp1_set_prewarp_freq(
		bw_hp1_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e12f);

	bw_lp1_set_prewarp_freq(&coeffs->lp1_coeffs, value);

	BW_ASSERT_DEEP(bw_hp1_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_hp1_coeffs_state_init);
}

static inline char bw_hp1_coeffs_is_valid(
		const bw_hp1_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_hp1_coeffs"))
		return 0;
	if (coeffs->state < bw_hp1_coeffs_state_init || coeffs->state > bw_hp1_coeffs_state_reset_coeffs)
		return 0;
#endif

	return bw_lp1_coeffs_is_valid(&coeffs->lp1_coeffs);
}

static inline char bw_hp1_state_is_valid(
		const bw_hp1_coeffs * BW_RESTRICT coeffs,
		const bw_hp1_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_hp1_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	return bw_lp1_state_is_valid(coeffs ? &coeffs->lp1_coeffs : NULL, &state->lp1_state);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::HP1
 *  ```>>> */
template<size_t N_CHANNELS>
class HP1 {
public:
	HP1();

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
	bw_hp1_coeffs			coeffs;
	bw_hp1_state			states[N_CHANNELS];
	bw_hp1_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline HP1<N_CHANNELS>::HP1() {
	bw_hp1_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_hp1_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_hp1_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_hp1_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_hp1_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : y0);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_hp1_reset_coeffs(&coeffs);
	bw_hp1_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_hp1_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::setCutoff(
		float value) {
	bw_hp1_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::setPrewarpAtCutoff(
		bool value) {
	bw_hp1_set_prewarp_at_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::setPrewarpFreq(
		float value) {
	bw_hp1_set_prewarp_freq(&coeffs, value);
}

}
#endif

#endif
