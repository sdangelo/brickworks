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
 *  requires {{{ bw_common bw_gain bw_math bw_one_pole }}}
 *  description {{{
 *    Dry/wet mixer.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Module renamed as bw_dry_wet.</li>
 *          <li><code>bw_dry_wet_process()</code> and
 *              <code>bw_dry_wet_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
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
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_DRY_WET_H
#define BW_DRY_WET_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_dry_wet_coeffs
 *  ```>>> */
typedef struct bw_dry_wet_coeffs bw_dry_wet_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_dry_wet_init()
 *  ```>>> */
static inline void bw_dry_wet_init(
	bw_dry_wet_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_dry_wet_set_sample_rate()
 *  ```>>> */
static inline void bw_dry_wet_set_sample_rate(
	bw_dry_wet_coeffs * BW_RESTRICT coeffs,
	float                           sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_dry_wet_reset_coeffs()
 *  ```>>> */
static inline void bw_dry_wet_reset_coeffs(
	bw_dry_wet_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_dry_wet_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_dry_wet_update_coeffs_ctrl(
	bw_dry_wet_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_dry_wet_update_coeffs_audio()
 *  ```>>> */
static inline void bw_dry_wet_update_coeffs_audio(
	bw_dry_wet_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_dry_wet_process1()
 *  ```>>> */
static inline float bw_dry_wet_process1(
	const bw_dry_wet_coeffs * BW_RESTRICT coeffs,
	float                                 x_dry,
	float                                 x_wet);
/*! <<<```
 *    Processes one dry input sample `x_dry` and one wet input sample `x_wet`
 *    using `coeffs` and returns the corresponding output sample.
 *
 *    #### bw_dry_wet_process()
 *  ```>>> */
static inline void bw_dry_wet_process(
	bw_dry_wet_coeffs * BW_RESTRICT coeffs,
	const float *                   x_dry,
	const float *                   x_wet,
	float *                         y,
	size_t                          n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the dry input buffer `x_dry` and of the
 *    wet input buffer `x_wet` and fills the first `n_samples` of the output
 *    buffer `y`, while using and updating `coeffs` (control and audio rate).
 *
 *    #### bw_dry_wet_process_multi()
 *  ```>>> */
static inline void bw_dry_wet_process_multi(
	bw_dry_wet_coeffs * BW_RESTRICT coeffs,
	const float * const *           x_dry,
	const float * const *           x_wet,
	float * const *                 y,
	size_t                          n_channels,
	size_t                          n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` dry input buffers
 *    `x_dry` and of the `n_channels` wet input buffers `x_wet`, and fills the
 *    first `n_samples` of the `n_channels` output buffers `y`, while using and
 *    updating the common `coeffs` (control and audio rate).
 *
 *    #### bw_dry_wet_set_wet()
 *  ```>>> */
static inline void bw_dry_wet_set_wet(
	bw_dry_wet_coeffs * BW_RESTRICT coeffs,
	float                           value);
/*! <<<```
 *    Sets the wet gain parameter to the given `value` (linear gain) in `coeffs`.
 *
 *    Valid range: [`0.f`, `1.f`].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_dry_wet_set_smooth_tau()
 *  ```>>> */
static inline void bw_dry_wet_set_smooth_tau(
	bw_dry_wet_coeffs * BW_RESTRICT coeffs,
	float                           value);
/*! <<<```
 *    Sets the smoothing time constant `value` (s) in `coeffs`.
 *
 *    `value` must be non-negative.
 *
 *    Default value: `0.05f`.
 *
 *    #### bw_dry_wet_coeffs_is_valid()
 *  ```>>> */
static inline char bw_dry_wet_coeffs_is_valid(
	const bw_dry_wet_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_dry_wet_coeffs`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_gain.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_dry_wet_coeffs_state {
	bw_dry_wet_coeffs_state_invalid,
	bw_dry_wet_coeffs_state_init,
	bw_dry_wet_coeffs_state_set_sample_rate,
	bw_dry_wet_coeffs_state_reset_coeffs
};
#endif

struct bw_dry_wet_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_dry_wet_coeffs_state	state;
#endif

	// Sub-components
	bw_gain_coeffs			gain_coeffs;
};

static inline void bw_dry_wet_init(
		bw_dry_wet_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	bw_gain_init(&coeffs->gain_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_dry_wet_coeffs");
	coeffs->state = bw_dry_wet_coeffs_state_init;
#endif
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_dry_wet_coeffs_state_init);
}

static inline void bw_dry_wet_set_sample_rate(
		bw_dry_wet_coeffs * BW_RESTRICT coeffs,
		float                           sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_gain_set_sample_rate(&coeffs->gain_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_dry_wet_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_dry_wet_coeffs_state_set_sample_rate);
}

static inline void bw_dry_wet_reset_coeffs(
		bw_dry_wet_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_set_sample_rate);

	bw_gain_reset_coeffs(&coeffs->gain_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_dry_wet_coeffs_state_reset_coeffs;
#endif
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_dry_wet_coeffs_state_reset_coeffs);
}

static inline void bw_dry_wet_update_coeffs_ctrl(
		bw_dry_wet_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);

	bw_gain_update_coeffs_ctrl(&coeffs->gain_coeffs);

	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);
}

static inline void bw_dry_wet_update_coeffs_audio(
		bw_dry_wet_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);

	bw_gain_update_coeffs_audio(&coeffs->gain_coeffs);

	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);
}

static inline float bw_dry_wet_process1(
		const bw_dry_wet_coeffs * BW_RESTRICT coeffs,
		float                                 x_dry,
		float                                 x_wet) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(x_dry));
	BW_ASSERT(bw_is_finite(x_wet));

	const float y = bw_gain_process1(&coeffs->gain_coeffs, x_wet - x_dry) + x_dry;

	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_dry_wet_process(
		bw_dry_wet_coeffs * BW_RESTRICT coeffs,
		const float *                   x_dry,
		const float *                   x_wet,
		float *                         y,
		size_t                          n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);
	BW_ASSERT(x_dry != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x_dry, n_samples));
	BW_ASSERT(x_wet != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x_wet, n_samples));
	BW_ASSERT(y != NULL);

	bw_dry_wet_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_dry_wet_update_coeffs_audio(coeffs);
		y[i] = bw_dry_wet_process1(coeffs, x_dry[i], x_wet[i]);
	}

	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_dry_wet_process_multi(
		bw_dry_wet_coeffs * BW_RESTRICT coeffs,
		const float * const *           x_dry,
		const float * const *           x_wet,
		float * const *                 y,
		size_t                          n_channels,
		size_t                          n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);
	BW_ASSERT(x_dry != NULL);
	BW_ASSERT(x_wet != NULL);
	BW_ASSERT(y != NULL);

	bw_dry_wet_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_dry_wet_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_dry_wet_process1(coeffs, x_dry[j][i], x_wet[j][i]);
	}

	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_reset_coeffs);
}

static inline void bw_dry_wet_set_wet(
		bw_dry_wet_coeffs * BW_RESTRICT coeffs,
		float                           value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 1.f);

	bw_gain_set_gain_lin(&coeffs->gain_coeffs, value);

	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_init);
}

static inline void bw_dry_wet_set_smooth_tau(
		bw_dry_wet_coeffs * BW_RESTRICT coeffs,
		float                           value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f);

	bw_gain_set_smooth_tau(&coeffs->gain_coeffs, value);

	BW_ASSERT_DEEP(bw_dry_wet_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_dry_wet_coeffs_state_init);
}

static inline char bw_dry_wet_coeffs_is_valid(
		const bw_dry_wet_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_dry_wet_coeffs"))
		return 0;
	if (coeffs->state < bw_dry_wet_coeffs_state_init || coeffs->state > bw_dry_wet_coeffs_state_reset_coeffs)
		return 0;
#endif

	return bw_gain_coeffs_is_valid(&coeffs->gain_coeffs);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::DryWet
 *  ```>>> */
template<size_t N_CHANNELS>
class DryWet {
public:
	DryWet();

	void setSampleRate(
		float sampleRate);

	void reset();

	void process(
		const float * const * xDry,
		const float * const * xWet,
		float * const *       y,
		size_t                nSamples);

	void process(
		std::array<const float *, N_CHANNELS> xDry,
		std::array<const float *, N_CHANNELS> xWet,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);

	void setWet(
		float value);

	void setSmoothTau(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_dry_wet_coeffs	coeffs;
};

template<size_t N_CHANNELS>
inline DryWet<N_CHANNELS>::DryWet() {
	bw_dry_wet_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void DryWet<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_dry_wet_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void DryWet<N_CHANNELS>::reset() {
	bw_dry_wet_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void DryWet<N_CHANNELS>::process(
		const float * const * xDry,
		const float * const * xWet,
		float * const *       y,
		size_t                nSamples) {
	bw_dry_wet_process_multi(&coeffs, xDry, xWet, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void DryWet<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> xDry,
		std::array<const float *, N_CHANNELS> xWet,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(xDry.data(), xWet.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void DryWet<N_CHANNELS>::setWet(
		float value) {
	bw_dry_wet_set_wet(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void DryWet<N_CHANNELS>::setSmoothTau(
		float value) {
	bw_dry_wet_set_smooth_tau(&coeffs, value);
}

}
#endif

#endif
