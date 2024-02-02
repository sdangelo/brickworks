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
 *  version {{{ 1.1.0 }}}
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    Ring modulator with variable modulation amount.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.1.0</strong>:
 *        <ul>
 *          <li>Now using <code>BW_NULL</code> and
 *              <code>BW_CXX_NO_ARRAY</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Module renamed as bw_ring_mod.</li>
 *          <li><code>bw_ring_mod_process()</code> and
 *              <code>bw_ring_mod_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Fixed inverted-polarity modulation (for real this time).</li>
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
 *          <li>Added <code>bw_ringmod_process_multi()</code>.</li>
 *          <li>Fixed inverted-polarity modulation.</li>
 *          <li>"modulator signal" -> "modulation signal" in documentation.</li>
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

#ifndef BW_RINGMOD_H
#define BW_RINGMOD_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_ring_mod_coeffs
 *  ```>>> */
typedef struct bw_ring_mod_coeffs bw_ring_mod_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_ring_mod_init()
 *  ```>>> */
static inline void bw_ring_mod_init(
	bw_ring_mod_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_ring_mod_set_sample_rate()
 *  ```>>> */
static inline void bw_ring_mod_set_sample_rate(
	bw_ring_mod_coeffs * BW_RESTRICT coeffs,
	float                            sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_ring_mod_reset_coeffs()
 *  ```>>> */
static inline void bw_ring_mod_reset_coeffs(
	bw_ring_mod_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_ring_mod_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_ring_mod_update_coeffs_ctrl(
	bw_ring_mod_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ring_mod_update_coeffs_audio()
 *  ```>>> */
static inline void bw_ring_mod_update_coeffs_audio(
	bw_ring_mod_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ring_mod_process1()
 *  ```>>> */
static inline float bw_ring_mod_process1(
	const bw_ring_mod_coeffs * BW_RESTRICT coeffs,
	float                                  x_mod,
	float                                  x_car);
/*! <<<```
 *    Processes one modulation input sample `x_mod` and one carrier input sample
 *    `x_car` using `coeffs` and returns the corresponding output sample.
 *
 *    #### bw_ring_mod_process()
 *  ```>>> */
static inline void bw_ring_mod_process(
	bw_ring_mod_coeffs * BW_RESTRICT coeffs,
	const float *                    x_mod,
	const float *                    x_car,
	float *                          y,
	size_t                           n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the modulation input buffer `x_mod` and
 *    of the carrier input buffer `x_car` and fills the first `n_samples` of the
 *    output buffer `y`, while using and updating `coeffs` (control and audio
 *    rate).
 *
 *    #### bw_ring_mod_process_multi()
 *  ```>>> */
static inline void bw_ring_mod_process_multi(
	bw_ring_mod_coeffs * BW_RESTRICT coeffs,
	const float * const *            x_mod,
	const float * const *            x_car,
	float * const *                  y,
	size_t                           n_channels,
	size_t                           n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` modulation input
 *    buffers `x_mod` and of the `n_channels` carrier input buffers `x_car`, and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating the common `coeffs` (control and audio rate).
 *
 *    #### bw_ring_mod_set_amount()
 *  ```>>> */
static inline void bw_ring_mod_set_amount(
	bw_ring_mod_coeffs * BW_RESTRICT coeffs,
	float                            value);
/*! <<<```
 *    Sets the modulation amount parameter to the given `value` (`0.f` = no
 *    modulation, `1.f` = full modulation, `-1.f` = full modulation with
 *    inverted polarity) in `coeffs`.
 *
 *    Valid range: [`-1.f` (full modulation with inverted polarity),
 *    `1.f` (full modulation)].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_ring_mod_coeffs_is_valid()
 *  ```>>> */
static inline char bw_ring_mod_coeffs_is_valid(
	const bw_ring_mod_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_ring_mod_coeffs`.
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
enum bw_ring_mod_coeffs_state {
	bw_ring_mod_coeffs_state_invalid,
	bw_ring_mod_coeffs_state_init,
	bw_ring_mod_coeffs_state_set_sample_rate,
	bw_ring_mod_coeffs_state_reset_coeffs
};
#endif

struct bw_ring_mod_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_ring_mod_coeffs_state	state;
#endif

	// Sub-components
	bw_one_pole_coeffs		smooth_coeffs;
	bw_one_pole_state		smooth_state;

	// Parameters
	float				mod_amount;
};

static inline void bw_ring_mod_init(
		bw_ring_mod_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->mod_amount = 1.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_ring_mod_coeffs");
	coeffs->state = bw_ring_mod_coeffs_state_init;
#endif
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_ring_mod_coeffs_state_init);
}

static inline void bw_ring_mod_set_sample_rate(
		bw_ring_mod_coeffs * BW_RESTRICT coeffs,
		float                            sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_ring_mod_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_ring_mod_coeffs_state_set_sample_rate);
}

static inline void bw_ring_mod_reset_coeffs(
		bw_ring_mod_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_set_sample_rate);

	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->mod_amount);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_ring_mod_coeffs_state_reset_coeffs;
#endif
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_ring_mod_coeffs_state_reset_coeffs);
}

static inline void bw_ring_mod_update_coeffs_ctrl(
		bw_ring_mod_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_ring_mod_update_coeffs_audio(
		bw_ring_mod_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs);

	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->mod_amount);

	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs);
}

static inline float bw_ring_mod_process1(
		const bw_ring_mod_coeffs * BW_RESTRICT coeffs,
		float                                  x_mod,
		float                                  x_car) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(x_mod));
	BW_ASSERT(bw_is_finite(x_car));

	const float k = bw_one_pole_get_y_z1(&coeffs->smooth_state);
	const float y = (k * x_car - bw_absf(k)) * x_mod + x_mod;

	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_ring_mod_process(
		bw_ring_mod_coeffs * BW_RESTRICT coeffs,
		const float *                    x_mod,
		const float *                    x_car,
		float *                          y,
		size_t                           n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs);
	BW_ASSERT(x_mod != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x_mod, n_samples));
	BW_ASSERT(x_car != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x_car, n_samples));
	BW_ASSERT(y != BW_NULL);

	for (size_t i = 0; i < n_samples; i++) {
		bw_ring_mod_update_coeffs_audio(coeffs);
		y[i] = bw_ring_mod_process1(coeffs, x_mod[i], x_car[i]);
	}

	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_ring_mod_process_multi(
		bw_ring_mod_coeffs * BW_RESTRICT coeffs,
		const float * const *            x_mod,
		const float * const *            x_car,
		float * const *                  y,
		size_t                           n_channels,
		size_t                           n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs);
	BW_ASSERT(x_mod != BW_NULL);
	BW_ASSERT(x_car != BW_NULL);
	BW_ASSERT(y != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(y[i] != y[j]);
#endif

	for (size_t i = 0; i < n_samples; i++) {
		bw_ring_mod_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_ring_mod_process1(coeffs, x_mod[j][i], x_car[j][i]);
	}

	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs);
}

static inline void bw_ring_mod_set_amount(
		bw_ring_mod_coeffs * BW_RESTRICT coeffs,
		float                            value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= -1.f && value <= 1.f);

	coeffs->mod_amount = value;

	BW_ASSERT_DEEP(bw_ring_mod_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_ring_mod_coeffs_state_init);
}

static inline char bw_ring_mod_coeffs_is_valid(
		const bw_ring_mod_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_ring_mod_coeffs"))
		return 0;
	if (coeffs->state < bw_ring_mod_coeffs_state_init || coeffs->state > bw_ring_mod_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->mod_amount) || coeffs->mod_amount < -1.f || coeffs->mod_amount > 1.f)
		return 0;

	if (!bw_one_pole_coeffs_is_valid(&coeffs->smooth_coeffs))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_ring_mod_coeffs_state_reset_coeffs && !bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_state))
			return 0;
#endif

	return 1;
}

#ifdef __cplusplus
}

#ifndef BW_CXX_NO_ARRAY
# include <array>
#endif

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::RingMod
 *  ```>>> */
template<size_t N_CHANNELS>
class RingMod {
public:
	RingMod();

	void setSampleRate(
		float sampleRate);

	void reset();

	void process(
		const float * const * xMod,
		const float * const * xCar,
		float * const *       y,
		size_t                nSamples);

#ifndef BW_CXX_NO_ARRAY
	void process(
		std::array<const float *, N_CHANNELS> xMod,
		std::array<const float *, N_CHANNELS> xCar,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);
#endif

	void setAmount(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_ring_mod_coeffs	coeffs;
};

template<size_t N_CHANNELS>
inline RingMod<N_CHANNELS>::RingMod() {
	bw_ring_mod_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_ring_mod_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::reset() {
	bw_ring_mod_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::process(
		const float * const * xMod,
		const float * const * xCar,
		float * const *       y,
		size_t                nSamples) {
	bw_ring_mod_process_multi(&coeffs, xMod, xCar, y, N_CHANNELS, nSamples);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> xMod,
		std::array<const float *, N_CHANNELS> xCar,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(xMod.data(), xCar.data(), y.data(), nSamples);
}
#endif

template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::setAmount(
		float value) {
	bw_ring_mod_set_amount(&coeffs, value);
}

}
#endif

#endif
