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
 *  requires {{{ bw_common bw_env_follow bw_math bw_one_pole }}}
 *  description {{{
 *    Digital peak programme meter with adjustable integration time constant.
 *
 *    An integration time constant of `0.f` results in true-peak metering, while
 *    values between `0.f` and `0.006f` give quasi-peak metering behavior.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Enforced minimum output value <code>-600.f</code>.</li>
 *          <li>Added initial input value to
 *              <code>bw_ppm_reset_state()</code>.</li>
 *          <li>Added <code>bw_ppm_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_ppm_reset_state()</code> returns the initial output
 *              value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_ppm_process()</code> and
 *              <code>bw_ppm_process_multi()</code> now use <code>size_t</code>
 *              to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Removed dependency on bw_config.</li>
 *          <li>Fixed bug by forcing <code>-INFINITY</code> output when signal
 *              level is below -600 dB.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_ppm_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
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

#ifndef BW_PPM_H
#define BW_PPM_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_ppm_coeffs
 *  ```>>> */
typedef struct bw_ppm_coeffs bw_ppm_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_ppm_state
 *  ```>>> */
typedef struct bw_ppm_state bw_ppm_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_ppm_init()
 *  ```>>> */
static inline void bw_ppm_init(
	bw_ppm_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_ppm_set_sample_rate()
 *  ```>>> */
static inline void bw_ppm_set_sample_rate(
	bw_ppm_coeffs * BW_RESTRICT coeffs,
	float                       sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_ppm_reset_coeffs()
 *  ```>>> */
static inline void bw_ppm_reset_coeffs(
	bw_ppm_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_ppm_reset_state()
 *  ```>>> */
static inline float bw_ppm_reset_state(
	const bw_ppm_coeffs * BW_RESTRICT coeffs,
	bw_ppm_state * BW_RESTRICT        state,
	float                             x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_ppm_reset_state_multi()
 *  ```>>> */
static inline void bw_ppm_reset_state_multi(
	const bw_ppm_coeffs * BW_RESTRICT              coeffs,
	bw_ppm_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_ppm_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_ppm_update_coeffs_ctrl(
	bw_ppm_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ppm_update_coeffs_audio()
 *  ```>>> */
static inline void bw_ppm_update_coeffs_audio(
	bw_ppm_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ppm_process1()
 *  ```>>> */
static inline float bw_ppm_process1(
	const bw_ppm_coeffs * BW_RESTRICT coeffs,
	bw_ppm_state * BW_RESTRICT        state,
	float                             x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample value in dBSF (minimum
 *    `-600.f`).
 *
 *    #### bw_ppm_process()
 *  ```>>> */
static inline void bw_ppm_process(
	bw_ppm_coeffs * BW_RESTRICT coeffs,
	bw_ppm_state * BW_RESTRICT  state,
	const float *               x,
	float *                     y,
	size_t                      n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    Output sample values are in dBFS (minimum `-600.f`).
 *
 *    `y` may be `NULL`.
 *
 *    #### bw_ppm_process_multi()
 *  ```>>> */
static inline void bw_ppm_process_multi(
	bw_ppm_coeffs * BW_RESTRICT                    coeffs,
	bw_ppm_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    Output sample values are in dBFS (minimum `-600.f`).
 *
 *    `y` or any element of `y` may be `NULL`.
 *
 *    #### bw_ppm_set_integration_time()
 *  ```>>> */
static inline void bw_ppm_set_integration_tau(
	bw_ppm_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the upgoing (integration) time constant in `coeffs` to `value` (s).
 *
 *    `value` must be non-negative.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_ppm_get_y_z1()
 *  ```>>> */
static inline float bw_ppm_get_y_z1(
	const bw_ppm_state * BW_RESTRICT state);
/*! <<<```
 *    Returns the last output sample as stored in `state`.
 *
 *    #### bw_ppm_coeffs_is_valid()
 *  ```>>> */
static inline char bw_ppm_coeffs_is_valid(
	const bw_ppm_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_ppm_coeffs`.
 *
 *    #### bw_ppm_state_is_valid()
 *  ```>>> */
static inline char bw_ppm_state_is_valid(
	const bw_ppm_coeffs * BW_RESTRICT coeffs,
	const bw_ppm_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_ppm_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_env_follow.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_ppm_coeffs_state {
	bw_ppm_coeffs_state_invalid,
	bw_ppm_coeffs_state_init,
	bw_ppm_coeffs_state_set_sample_rate,
	bw_ppm_coeffs_state_reset_coeffs
};
#endif

struct bw_ppm_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_ppm_coeffs_state	state;
	uint32_t			reset_id;
#endif
	// Sub-components
	bw_env_follow_coeffs		env_follow_coeffs;
};

struct bw_ppm_state {
#ifdef BW_DEBUG_DEEP
	uint32_t		hash;
	uint32_t		coeffs_reset_id;
#endif

	// Sub-components
	bw_env_follow_state	env_follow_state;

	// States
	float			y_z1;
};

static inline void bw_ppm_init(
		bw_ppm_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	bw_env_follow_init(&coeffs->env_follow_coeffs);
	bw_env_follow_set_release_tau(&coeffs->env_follow_coeffs, 0.738300619235528f);

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_ppm_coeffs");
	coeffs->state = bw_ppm_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_ppm_coeffs_state_init);
}

static inline void bw_ppm_set_sample_rate(
		bw_ppm_coeffs * BW_RESTRICT coeffs,
		float                       sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_env_follow_set_sample_rate(&coeffs->env_follow_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_ppm_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_ppm_coeffs_state_set_sample_rate);
}

static inline void bw_ppm_reset_coeffs(
		bw_ppm_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_set_sample_rate);

	bw_env_follow_reset_coeffs(&coeffs->env_follow_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_ppm_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_ppm_coeffs_state_reset_coeffs);
}

static inline float bw_ppm_reset_state(
		const bw_ppm_coeffs * BW_RESTRICT coeffs,
		bw_ppm_state * BW_RESTRICT        state,
		float                             x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT(bw_is_finite(x_0));

	const float yl = bw_env_follow_reset_state(&coeffs->env_follow_coeffs, &state->env_follow_state, x_0);
	const float y = yl >= 1e-30f ? bw_lin2dBf(yl) : -600.f;
	state->y_z1 = y;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_ppm_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_ppm_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_ppm_reset_state_multi(
		const bw_ppm_coeffs * BW_RESTRICT              coeffs,
		bw_ppm_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                  x_0,
		float *                                        y_0,
		size_t                                         n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_ppm_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_ppm_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_ppm_update_coeffs_ctrl(
		bw_ppm_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);

	bw_env_follow_update_coeffs_ctrl(&coeffs->env_follow_coeffs);

	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
}

static inline void bw_ppm_update_coeffs_audio(
		bw_ppm_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);

	bw_env_follow_update_coeffs_audio(&coeffs->env_follow_coeffs);

	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
}

static inline float bw_ppm_process1(
		const bw_ppm_coeffs * BW_RESTRICT coeffs,
		bw_ppm_state * BW_RESTRICT        state,
		float                             x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_ppm_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	const float yl = bw_env_follow_process1(&coeffs->env_follow_coeffs, &state->env_follow_state, x);
	const float y = yl >= 1e-30f ? bw_lin2dBf(yl) : -600.f; // -600 dB is quiet enough
	state->y_z1 = y;

	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_ppm_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_ppm_process(
		bw_ppm_coeffs * BW_RESTRICT coeffs,
		bw_ppm_state * BW_RESTRICT  state,
		const float *               x,
		float *                     y,
		size_t                      n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_ppm_state_is_valid(coeffs, state));
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));

	bw_ppm_update_coeffs_ctrl(coeffs);
	if (y != NULL)
		for (size_t i = 0; i < n_samples; i++) {
			bw_ppm_update_coeffs_audio(coeffs);
			y[i] = bw_ppm_process1(coeffs, state, x[i]);
		}
	else
		for (size_t i = 0; i < n_samples; i++) {
			bw_ppm_update_coeffs_audio(coeffs);
			bw_ppm_process1(coeffs, state, x[i]);
		}

	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_ppm_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(y != NULL ? bw_has_only_finite(y, n_samples) : 1);
}

static inline void bw_ppm_process_multi(
		bw_ppm_coeffs * BW_RESTRICT                    coeffs,
		bw_ppm_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                          x,
		float * const *                                y,
		size_t                                         n_channels,
		size_t                                         n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x != NULL);
#ifndef BW_NO_DEBUG
	if (y != NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = i + 1; j < n_channels; j++)
				BW_ASSERT(y[i] != y[j]);
#endif

	bw_ppm_update_coeffs_ctrl(coeffs);
	if (y != NULL)
		for (size_t i = 0; i < n_samples; i++) {
			bw_ppm_update_coeffs_audio(coeffs);
			for (size_t j = 0; j < n_channels; j++) {
				const float v = bw_ppm_process1(coeffs, state[j], x[j][i]);
				if (y[j] != NULL)
					y[j][i] = v;
			}
		}
	else
		for (size_t i = 0; i < n_samples; i++) {
			bw_ppm_update_coeffs_audio(coeffs);
			for (size_t j = 0; j < n_channels; j++)
				bw_ppm_process1(coeffs, state[j], x[j][i]);
		}

	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_reset_coeffs);
}

static inline void bw_ppm_set_integration_tau(
		bw_ppm_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	bw_env_follow_set_attack_tau(&coeffs->env_follow_coeffs, value);

	BW_ASSERT_DEEP(bw_ppm_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ppm_coeffs_state_init);
}

static inline float bw_ppm_get_y_z1(const bw_ppm_state *BW_RESTRICT state) {
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_ppm_state_is_valid(NULL, state));

	return state->y_z1;
}

static inline char bw_ppm_coeffs_is_valid(
		const bw_ppm_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_ppm_coeffs"))
		return 0;
	if (coeffs->state < bw_ppm_coeffs_state_init || coeffs->state > bw_ppm_coeffs_state_reset_coeffs)
		return 0;
#endif

	return bw_env_follow_coeffs_is_valid(&coeffs->env_follow_coeffs);
}

static inline char bw_ppm_state_is_valid(
		const bw_ppm_coeffs * BW_RESTRICT coeffs,
		const bw_ppm_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_ppm_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	if (!bw_is_finite(state->y_z1) || state->y_z1 < -600.f)
		return 0.f;

	return bw_env_follow_state_is_valid(coeffs ? &coeffs->env_follow_coeffs : NULL, &state->env_follow_state);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::PPM
 *  ```>>> */
template<size_t N_CHANNELS>
class PPM {
public:
	PPM();

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

	void setIntegrationTau(
		float value);

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
	bw_ppm_coeffs			coeffs;
	bw_ppm_state			states[N_CHANNELS];
	bw_ppm_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline PPM<N_CHANNELS>::PPM() {
	bw_ppm_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void PPM<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_ppm_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void PPM<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_ppm_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_ppm_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_ppm_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void PPM<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void PPM<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_ppm_reset_coeffs(&coeffs);
	bw_ppm_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void PPM<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void PPM<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_ppm_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void PPM<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void PPM<N_CHANNELS>::setIntegrationTau(
		float value) {
	bw_ppm_set_integration_tau(&coeffs, value);
}

template<size_t N_CHANNELS>
inline float PPM<N_CHANNELS>::getYZ1(
		size_t channel) {
	return bw_ppm_get_y_z1(states + channel);
}

}
#endif

#endif
