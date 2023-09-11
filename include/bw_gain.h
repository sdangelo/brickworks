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
 *    Gain.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_gain_get_gain_lin()</code>.</li>
 *          <li>Renamed <code>bw_gain_get_gain()</code> as
 *              <code>bw_gain_get_gain_cur()</code>.</li>
 *          <li>Simplified implementation to use less memory.</li>
 *          <li><code>bw_gain_process()</code> and
 *              <code>bw_gain_process_multi()</code> now use <code>size_t</code>
 *              to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
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
 *          <li>Added <code>bw_gain_process_multi()</code>.</li>
 *          <li>Added <code>bw_gain_get_gain()</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Renamed as bw_gain.</li>
 *          <li>Added new smooth_tau parameter.</li>
 *          <li>Changed gain parameter API to express values in linear gain
 *              and dB.</li>
 *          <li>Added C++ wrapper.</li>
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

#ifndef BW_GAIN_H
#define BW_GAIN_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_gain_coeffs
 *  ```>>> */
typedef struct bw_gain_coeffs bw_gain_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_gain_init()
 *  ```>>> */
static inline void bw_gain_init(
	bw_gain_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_gain_set_sample_rate()
 *  ```>>> */
static inline void bw_gain_set_sample_rate(
	bw_gain_coeffs * BW_RESTRICT coeffs,
	float                        sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_gain_reset_coeffs()
 *  ```>>> */
static inline void bw_gain_reset_coeffs(
	bw_gain_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_gain_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_gain_update_coeffs_ctrl(
	bw_gain_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_gain_update_coeffs_audio()
 *  ```>>> */
static inline void bw_gain_update_coeffs_audio(
	bw_gain_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_gain_process1()
 *  ```>>> */
static inline float bw_gain_process1(
	const bw_gain_coeffs * BW_RESTRICT coeffs,
	float                              x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs` and returns the
 *    corresponding output sample.
 *
 *    #### bw_gain_process()
 *  ```>>> */
static inline void bw_gain_process(
	bw_gain_coeffs * BW_RESTRICT coeffs,
	const float *                x,
	float *                      y,
	size_t                       n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating
 *    `coeffs` (control and audio rate).
 *
 *    #### bw_gain_process_multi()
 *  ```>>> */
static inline void bw_gain_process_multi(
	bw_gain_coeffs * BW_RESTRICT coeffs,
	const float * const *        x,
	float * const *              y,
	size_t                       n_channels,
	size_t                       n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating the common `coeffs` (control and audio rate).
 *
 *    #### bw_gain_set_gain_lin()
 *  ```>>> */
static inline void bw_gain_set_gain_lin(
	bw_gain_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the gain parameter to the given `value` (linear gain) in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_gain_set_gain_dB()
 *  ```>>> */
static inline void bw_gain_set_gain_dB(
	bw_gain_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the gain parameter to the given `value` (dB) in `coeffs`.
 *
 *    `value` must be less than or equal to `770.630f`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_gain_set_smooth_tau()
 *  ```>>> */
static inline void bw_gain_set_smooth_tau(
	bw_gain_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the smoothing time constant `value` (s) in `coeffs`.
 *
 *    `value` must be non-negative.
 *
 *    Default value: `0.05f`.
 *
 *    #### bw_gain_get_gain_lin()
 *  ```>>> */
static inline float bw_gain_get_gain_lin(
	const bw_gain_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the current gain parameter value (linear gain) in `coeffs`.
 *
 *    #### bw_gain_get_gain_cur()
 *  ```>>> */
static inline float bw_gain_get_gain_cur(
	const bw_gain_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the actual current gain coefficient (linear gain) in `coeffs`.
 *
 *    #### bw_gain_coeffs_is_valid()
 *  ```>>> */
static inline char bw_gain_coeffs_is_valid(
	const bw_gain_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_gain_coeffs`.
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
enum bw_gain_coeffs_state {
	bw_gain_coeffs_state_invalid,
	bw_gain_coeffs_state_init,
	bw_gain_coeffs_state_set_sample_rate,
	bw_gain_coeffs_state_reset_coeffs
};
#endif

struct bw_gain_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_gain_coeffs_state	state;
#endif

	// Sub-components
	bw_one_pole_coeffs		smooth_coeffs;
	bw_one_pole_state		smooth_state;

	// Parameters
	float				gain;
};

static inline void bw_gain_init(
		bw_gain_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->gain = 1.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_gain_coeffs");
	coeffs->state = bw_gain_coeffs_state_init;
#endif
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_gain_coeffs_state_init);
}

static inline void bw_gain_set_sample_rate(
		bw_gain_coeffs * BW_RESTRICT coeffs,
		float                        sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_gain_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_gain_coeffs_state_set_sample_rate);
}

static inline void bw_gain_reset_coeffs(
		bw_gain_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_set_sample_rate);

	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->gain);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_gain_coeffs_state_reset_coeffs;
#endif
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_gain_coeffs_state_reset_coeffs);
}

static inline void bw_gain_update_coeffs_ctrl(
		bw_gain_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);

	bw_one_pole_update_coeffs_ctrl(&coeffs->smooth_coeffs);

	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);
}

static inline void bw_gain_update_coeffs_audio(
		bw_gain_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);

	bw_one_pole_update_coeffs_audio(&coeffs->smooth_coeffs);
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->gain);

	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);
}

static inline float bw_gain_process1(
		const bw_gain_coeffs * BW_RESTRICT coeffs,
		float                              x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(x));

	const float y = bw_one_pole_get_y_z1(&coeffs->smooth_state) * x;

	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_gain_process(
		bw_gain_coeffs * BW_RESTRICT coeffs,
		const float *                x,
		float *                      y,
		size_t                       n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != NULL);

	bw_gain_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_gain_update_coeffs_audio(coeffs);
		y[i] = bw_gain_process1(coeffs, x[i]);
	}

	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_gain_process_multi(
		bw_gain_coeffs * BW_RESTRICT coeffs,
		const float * const *        x,
		float * const *              y,
		size_t                       n_channels,
		size_t                       n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);
	BW_ASSERT(x != NULL);
	BW_ASSERT(y != NULL);

	bw_gain_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_gain_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_gain_process1(coeffs, x[j][i]);
	}

	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_reset_coeffs);
}

static inline void bw_gain_set_gain_lin(
		bw_gain_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));

	coeffs->gain = value;

	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_init);
}

static inline void bw_gain_set_gain_dB(
		bw_gain_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value <= 770.630f);

	coeffs->gain = bw_dB2linf(value);

	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_init);
}

static inline void bw_gain_set_smooth_tau(
		bw_gain_coeffs * BW_RESTRICT coeffs,
		float                        value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_init);
	BW_ASSERT(!bw_is_nan(value));
	BW_ASSERT(value >= 0.f);

	bw_one_pole_set_tau(&coeffs->smooth_coeffs, value);

	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_gain_coeffs_state_init);
}

static inline float bw_gain_get_gain_lin(
		const bw_gain_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));

	return coeffs->gain;
}

static inline float bw_gain_get_gain_cur(
		const bw_gain_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_gain_coeffs_is_valid(coeffs));

	return bw_one_pole_get_y_z1(&coeffs->smooth_state);
}

static inline char bw_gain_coeffs_is_valid(
		const bw_gain_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_gain_coeffs"))
		return 0;
	if (coeffs->state < bw_gain_coeffs_state_init || coeffs->state > bw_gain_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->gain))
		return 0;

	if (!bw_one_pole_coeffs_is_valid(&coeffs->smooth_coeffs))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_gain_coeffs_state_reset_coeffs && !bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_state))
		return 0;
#endif

	return 1;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Gain
 *  ```>>> */
template<size_t N_CHANNELS>
class Gain {
public:
	Gain();

	void setSampleRate(
		float sampleRate);

	void reset();

	void process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples);

	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);

	void setGainLin(
		float value);

	void setGainDB(
		float value);

	void setSmoothTau(
		float value);

	float getGainLin();

	float getGainCur();
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_gain_coeffs	 coeffs;
};

template<size_t N_CHANNELS>
inline Gain<N_CHANNELS>::Gain() {
	bw_gain_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void Gain<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_gain_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Gain<N_CHANNELS>::reset() {
	bw_gain_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void Gain<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_gain_process_multi(&coeffs, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Gain<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Gain<N_CHANNELS>::setGainLin(
		float value) {
	bw_gain_set_gain_lin(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Gain<N_CHANNELS>::setGainDB(
		float value) {
	bw_gain_set_gain_dB(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Gain<N_CHANNELS>::setSmoothTau(
		float value) {
	bw_gain_set_smooth_tau(&coeffs, value);
}

template<size_t N_CHANNELS>
inline float Gain<N_CHANNELS>::getGainLin() {
	return bw_gain_get_gain_lin(&coeffs);
}

template<size_t N_CHANNELS>
inline float Gain<N_CHANNELS>::getGainCur() {
	return bw_gain_get_gain_cur(&coeffs);
}

}
#endif

#endif
