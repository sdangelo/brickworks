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
 *  requires {{{ bw_common bw_math bw_rand }}}
 *  description {{{
 *    Generator of white noise with uniform distribution.
 *
 *    This module has no internal state, rather its state is stored into a
 *    `uint64_t` value to which the API user supplies a pointer (as in
 *    [bw\_rand](bw_rand)).
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_noise_gen_reset_coeffs()</code>,
 *              <code>bw_noise_gen_update_coeffs_ctrl()</code>,
 *              and <code>bw_noise_gen_update_coeffs_audio()</code>.</li>
 *          <li><code>bw_noise_gen_process()</code> and
 *              <code>bw_noise_gen_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
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
 *          <li>Added <code>bw_noise_gen_process_multi()</code>.</li>
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

#ifndef BW_NOISE_GEN_H
#define BW_NOISE_GEN_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_noise_gen_coeffs
 *  ```>>> */
typedef struct bw_noise_gen_coeffs bw_noise_gen_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_noise_gen_init()
 *  ```>>> */
static inline void bw_noise_gen_init(
	bw_noise_gen_coeffs * BW_RESTRICT coeffs,
	uint64_t * BW_RESTRICT            state);
/*! <<<```
 *    Initializes input parameter values and sets the `state` pointer to obtain
 *    pseudo-random numbers in `coeffs`.
 *
 *    #### bw_noise_gen_set_sample_rate()
 *  ```>>> */
static inline void bw_noise_gen_set_sample_rate(
	bw_noise_gen_coeffs * BW_RESTRICT coeffs,
	float                             sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_noise_gen_reset_coeffs()
 *  ```>>> */
static inline void bw_noise_gen_reset_coeffs(
	bw_noise_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_noise_gen_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_noise_gen_update_coeffs_ctrl(
	bw_noise_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_noise_gen_update_coeffs_audio()
 *  ```>>> */
static inline void bw_noise_gen_update_coeffs_audio(
	bw_noise_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_noise_gen_process1\*()
 *  ```>>> */
static inline float bw_noise_gen_process1(
	const bw_noise_gen_coeffs * BW_RESTRICT coeffs);

static inline float bw_noise_gen_process1_scaling(
	const bw_noise_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    These functions generate and return one sample using `coeffs`, where:
 *     * `bw_noise_gen_process1()` assumes that sample rate scaling is disabled;
 *     * `bw_noise_gen_process1_scaling()` assumes that sample rate scaling is
 *       enabled.
 *
 *    Whether sample rate scaling is enabled or not is unchecked even for
 *    debugging purposes.
 *
 *    #### bw_noise_gen_process()
 *  ```>>> */
static inline void bw_noise_gen_process(
	bw_noise_gen_coeffs * BW_RESTRICT coeffs,
	float * BW_RESTRICT               y,
	size_t                            n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the output buffer `y` using
 *    `coeffs`.
 *
 *    #### bw_noise_gen_process_multi()
 *  ```>>> */
static inline void bw_noise_gen_process_multi(
	bw_noise_gen_coeffs * BW_RESTRICT       coeffs,
	float * BW_RESTRICT const * BW_RESTRICT y,
	size_t                                  n_channels,
	size_t                                  n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the `n_channels` output
 *    buffers `y` using `coeffs`.
 *
 *    #### bw_noise_gen_set_sample_rate_scaling()
 *  ```>>> */
static inline void bw_noise_gen_set_sample_rate_scaling(
	bw_noise_gen_coeffs * BW_RESTRICT coeffs,
	char                              value);
/*! <<<```
 *    Sets whether the output should be scaled (`value` non-`0`) or not (`0`)
 *    according to the sample rate in `coeffs`.
 *
 *    In order to maintain the same perceived loudness at different sample
 *    rates, a white noise signal with uniform distribution should be
 *    accordingly scaled. The 44100 Hz sample rate is used as a reference (that
 *    is, the scaling factor at that sample rate is `1.f`).
 *
 *    Default value: `0` (off).
 *
 *    #### bw_noise_gen_get_scaling_k()
 *  ```>>> */
static inline float bw_noise_gen_get_scaling_k(
	const bw_noise_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the sample rate scaling factor that is applied or would be applied
 *    if sample rate scaling were enabled, as stored in `coeffs`.
 *
 *    #### bw_noise_gen_coeffs_is_valid()
 *  ```>>> */
static inline char bw_noise_gen_coeffs_is_valid(
	const bw_noise_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_noise_gen_coeffs`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_rand.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_noise_gen_coeffs_state {
	bw_noise_gen_coeffs_state_invalid,
	bw_noise_gen_coeffs_state_init,
	bw_noise_gen_coeffs_state_set_sample_rate,
	bw_noise_gen_coeffs_state_reset_coeffs
};
#endif

struct bw_noise_gen_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_noise_gen_coeffs_state	state;
#endif

	// Coefficients
	float				scaling_k;

	// Parameters
	uint64_t * BW_RESTRICT		rand_state;
	char				sample_rate_scaling;
};

static inline void bw_noise_gen_init(
		bw_noise_gen_coeffs * BW_RESTRICT coeffs,
		uint64_t * BW_RESTRICT            state) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT(state != NULL);

	coeffs->rand_state = state;
	coeffs->sample_rate_scaling = 0;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_noise_gen_coeffs");
	coeffs->state = bw_noise_gen_coeffs_state_init;
#endif
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_noise_gen_coeffs_state_init);
}

static inline void bw_noise_gen_set_sample_rate(
		bw_noise_gen_coeffs * BW_RESTRICT coeffs,
		float                             sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	coeffs->scaling_k = 0.004761904761904762f * bw_sqrtf(sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_noise_gen_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_noise_gen_coeffs_state_set_sample_rate);
}

static inline void bw_noise_gen_reset_coeffs(
		bw_noise_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_set_sample_rate);

	(void)coeffs;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_noise_gen_coeffs_state_reset_coeffs;
#endif
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_noise_gen_coeffs_state_reset_coeffs);
}

static inline void bw_noise_gen_update_coeffs_ctrl(
		bw_noise_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_noise_gen_update_coeffs_audio(
		bw_noise_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline float bw_noise_gen_process1(
		const bw_noise_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);

	const float y = bw_randf(coeffs->rand_state);

	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline float bw_noise_gen_process1_scaling(
		const bw_noise_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);

	const float y = coeffs->scaling_k * bw_randf(coeffs->rand_state);

	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_noise_gen_process(
		bw_noise_gen_coeffs * BW_RESTRICT coeffs,
		float * BW_RESTRICT               y,
		size_t                            n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(y != NULL);

	if (coeffs->sample_rate_scaling)
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_noise_gen_process1(coeffs);
	else
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_noise_gen_process1_scaling(coeffs);

	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_noise_gen_process_multi(
		bw_noise_gen_coeffs * BW_RESTRICT       coeffs,
		float * BW_RESTRICT const * BW_RESTRICT y,
		size_t                                  n_channels,
		size_t                                  n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(y != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(y[i] != y[j]);
#endif

	for (size_t i = 0; i < n_channels; i++)
		bw_noise_gen_process(coeffs, y[i], n_samples);

	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_reset_coeffs);
}

static inline void bw_noise_gen_set_sample_rate_scaling(
		bw_noise_gen_coeffs * BW_RESTRICT coeffs,
		char                              value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_init);

	coeffs->sample_rate_scaling = value;

	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_init);
}

static inline float bw_noise_gen_get_scaling_k(
		const bw_noise_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_noise_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_noise_gen_coeffs_state_set_sample_rate);

	return coeffs->scaling_k;
}

static inline char bw_noise_gen_coeffs_is_valid(
		const bw_noise_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_noise_gen_coeffs"))
		return 0;
	if (coeffs->state < bw_noise_gen_coeffs_state_init || coeffs->state > bw_noise_gen_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (coeffs->rand_state == NULL)
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_noise_gen_coeffs_state_set_sample_rate && coeffs->scaling_k <= 0.f)
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
 *    ##### Brickworks::NoiseGen
 *  ```>>> */
template<size_t N_CHANNELS>
class NoiseGen {
public:
	NoiseGen(
		uint64_t * BW_RESTRICT state);

	void setSampleRate(
		float sampleRate);

	void reset();

	void process(
		float * BW_RESTRICT const * BW_RESTRICT y,
		size_t                                  nSamples);

	void process(
		std::array<float * BW_RESTRICT, N_CHANNELS> y,
		size_t                                      nSamples);

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
	bw_noise_gen_coeffs	coeffs;
};

template<size_t N_CHANNELS>
inline NoiseGen<N_CHANNELS>::NoiseGen(
		uint64_t * BW_RESTRICT state) {
	bw_noise_gen_init(&coeffs, state);
}

template<size_t N_CHANNELS>
inline void NoiseGen<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_noise_gen_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void NoiseGen<N_CHANNELS>::reset() {
	bw_noise_gen_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void NoiseGen<N_CHANNELS>::process(
		float *BW_RESTRICT const *BW_RESTRICT y,
		size_t                                nSamples) {
	bw_noise_gen_process_multi(&coeffs, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void NoiseGen<N_CHANNELS>::process(
		std::array<float * BW_RESTRICT, N_CHANNELS> y,
		size_t                                      nSamples) {
	process(y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void NoiseGen<N_CHANNELS>::setSampleRateScaling(
		bool value) {
	bw_noise_gen_set_sample_rate_scaling(&coeffs, value);
}

template<size_t N_CHANNELS>
inline float NoiseGen<N_CHANNELS>::getScalingK() {
	return bw_noise_gen_get_scaling_k(&coeffs);
}

}
#endif

#endif
