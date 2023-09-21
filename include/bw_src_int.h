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
 *  requires {{{ bw_common bw_math }}}
 *  description {{{
 *    Integer-ratio IIR sample rate converter.
 *
 *    The multi-rate filtering approach was inspired by
 *
 *    M. Holters and J.Parker, "A Combined Model for a Bucket Brigade Device and
 *    its Input and Output Filters", 21st Intl. Conf. Digital Audio Effects
 *    (DAFx-18), Aveiro, Portugal, September 2018.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_src_int_reset_state_multi()</code> and updated
 *              C++ API in this regard.</li>
 *          <li>Now <code>bw_src_int_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_src_int_lim_process()</code> and
 *              <code>bw_src_int_lim_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Fixed frequency response and improved speed.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Clarified that the same buffer cannot be used for both input and
 *              output.</li>
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
 *          <li>Added <code>bw_src_int_process_multi()</code>.</li>
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

#ifndef BW_SRC_INT_H
#define BW_SRC_INT_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_src_int_coeffs
 *  ```>>> */
typedef struct bw_src_int_coeffs bw_src_int_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_src_int_state
 *  ```>>> */
typedef struct bw_src_int_state bw_src_int_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_src_int_init()
 *  ```>>> */
static inline void bw_src_int_init(
	bw_src_int_coeffs * BW_RESTRICT coeffs,
	int                             ratio);
/*! <<<```
 *    Initializes `coeffs` using the given resampling `ratio`.
 *
 *    If `ratio` is positive, then the sample rate of the output signal will be
 *    `ratio` times the sample rate of the input signal, otherwise, if it is
 *    negative, then the sample rate of the output signal will be equal to the
 *    sample rate of the input signal divided by `-ratio`.
 *
 *    `ratio` must not be `0`.
 *
 *    #### bw_src_int_reset_state()
 *  ```>>> */
static inline float bw_src_int_reset_state(
	const bw_src_int_coeffs * BW_RESTRICT coeffs,
	bw_src_int_state * BW_RESTRICT        state,
	float                                 x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_src_int_reset_state_multi()
 *  ```>>> */
static inline void bw_src_int_reset_state_multi(
	const bw_src_int_coeffs * BW_RESTRICT              coeffs,
	bw_src_int_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                      x_0,
	float *                                            y_0,
	size_t                                             n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `NULL`.
 *
 *    #### bw_src_int_process()
 *  ```>>> */
static inline size_t bw_src_int_process(
	const bw_src_int_coeffs * BW_RESTRICT coeffs,
	bw_src_int_state * BW_RESTRICT        state,
	const float * BW_RESTRICT             x,
	float * BW_RESTRICT                   y,
	size_t                                n_in_samples);
/*! <<<```
 *    Processes the first `n_in_samples` of the input buffer `x` and fills the
 *    output buffer `y` using `coeffs`, while using and updating `state`.
 *
 *    The number of generated output samples will be `ratio` times
 *    `n_in_samples` if `ratio` is positive, otherwise at most `n_in_samples`
 *    divided by `-ratio` and then rounded towards positive infinity.
 *
 *    `x` and `y` must point to different buffers.
 *
 *    Returns the number of generated output samples.
 *
 *    #### bw_src_int_process_multi()
 *  ```>>> */
static inline void bw_src_int_process_multi(
	const bw_src_int_coeffs * BW_RESTRICT              coeffs,
	bw_src_int_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * BW_RESTRICT const * BW_RESTRICT      x,
	float * BW_RESTRICT const * BW_RESTRICT            y,
	size_t                                             n_channels,
	size_t                                             n_in_samples,
	size_t * BW_RESTRICT                               n_out_samples);
/*! <<<```
 *    Processes the first `n_in_samples` of the `n_channels` input buffers `x`
 *    and fills the `n_channels` output buffers `y` using `coeffs`, while using
 *    and updating each of the `n_channels` `state`s.
 *
 *    The number of generated output samples in each output buffer will be
 *    `ratio` times `n_in_samples` if `ratio` is positive, otherwise at most
 *    `n_in_samples` divided by `-ratio` and then rounded towards positive
 *    infinity.
 *
 *    A given buffer cannot be used both as an input and output buffer.
 *
 *    `n_out_samples` is filled with the number of generated output samples for
 *    each output buffer, if not `NULL`.
 *
 *    #### bw_src_int_coeffs_is_valid()
 *  ```>>> */
static inline char bw_src_int_coeffs_is_valid(
	const bw_src_int_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_src_int_coeffs`.
 *
 *    #### bw_src_int_state_is_valid()
 *  ```>>> */
static inline char bw_src_int_state_is_valid(
	const bw_src_int_coeffs * BW_RESTRICT coeffs,
	const bw_src_int_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_src_int_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bw_src_int_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	uint32_t			reset_id;
#endif

	// Coefficients
	int				ratio;
	float				b0;
	float				ma1;
	float				ma2;
	float				ma3;
	float				ma4;
};

struct bw_src_int_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// States
	int		i;
	float		z1;
	float		z2;
	float		z3;
	float		z4;
};

static inline void bw_src_int_init(
		bw_src_int_coeffs * BW_RESTRICT coeffs,
		int ratio) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT(ratio != 0);

	coeffs->ratio = ratio;
	// 4th-degree Butterworth with cutoff at ratio * Nyquist, using bilinear transform w/ prewarping
	const float fc = (float)(ratio >= 0 ? ratio : -ratio);
	const float T = bw_tanf(1.570796326794896f / fc);
	const float T2 = T * T;
	const float k = 1.f / (T * (T * (T * (T + 2.613125929752753f) + 3.414213562373095f) + 2.613125929752753f) + 1.f);
	coeffs->b0 = k * T2 * T2;
	coeffs->ma1 = k * (T * (T2 * (-5.226251859505504f - 4.f * T) + 5.226251859505504f) + 4.f);
	coeffs->ma2 = k * ((6.82842712474619f - 6.f * T2) * T2 - 6.f);
	coeffs->ma3 = k * (T * (T2 * (5.226251859505504f - 4.f * T) - 5.226251859505504f) + 4.f);
	coeffs->ma4 = k * (T * (T * ((2.613125929752753f - T) * T - 3.414213562373095f) + 2.613125929752753f) - 1.f);

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_src_int_coeffs");
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_src_int_coeffs_is_valid(coeffs));
}

static inline float bw_src_int_reset_state(
		const bw_src_int_coeffs * BW_RESTRICT coeffs,
		bw_src_int_state * BW_RESTRICT        state,
		float                                 x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_src_int_coeffs_is_valid(coeffs));
	BW_ASSERT(state != NULL);
	BW_ASSERT(bw_is_finite(x_0));

	if (coeffs->ratio < 0) {
		// DF-II
		state->z1 = x_0 / (1.f - coeffs->ma1 - coeffs->ma2 - coeffs->ma3 - coeffs->ma4);
		state->z2 = state->z1;
		state->z3 = state->z2;
		state->z4 = state->z3;
		state->i = 0;
	} else {
		// TDF-II
		const float k = 4.f * coeffs->b0;
		state->z4 = (coeffs->b0 + coeffs->ma4) * x_0;
		state->z3 = (k + coeffs->ma3) * x_0 + state->z4;
		state->z2 = (6.f * coeffs->b0 + coeffs->ma2) * x_0 + state->z3;
		state->z1 = (k + coeffs->ma1) * x_0 + state->z2;
	}
	const float y = x_0;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_src_int_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_src_int_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(bw_src_int_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_src_int_reset_state_multi(
		const bw_src_int_coeffs * BW_RESTRICT              coeffs,
		bw_src_int_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                      x_0,
		float *                                            y_0,
		size_t                                             n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_src_int_coeffs_is_valid(coeffs));
	BW_ASSERT(state != NULL);
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_src_int_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_src_int_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_src_int_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline size_t bw_src_int_process(
		const bw_src_int_coeffs * BW_RESTRICT coeffs,
		bw_src_int_state * BW_RESTRICT        state,
		const float * BW_RESTRICT             x,
		float * BW_RESTRICT                   y,
		size_t                                n_in_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_src_int_coeffs_is_valid(coeffs));
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_src_int_state_is_valid(coeffs, state));
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_in_samples));
	BW_ASSERT(y != NULL);
	BW_ASSERT(x != y);

	size_t n = 0;
	if (coeffs->ratio < 0) {
		for (size_t i = 0; i < n_in_samples; i++) {
			// DF-II
			const float z0 = x[i] + coeffs->ma1 * state->z1 + coeffs->ma2 * state->z2 + coeffs->ma3 * state->z3 + coeffs->ma4 * state->z4;
			if (!state->i) {
				state->i = -coeffs->ratio;
				y[n] = coeffs->b0 * (z0 + state->z4 + 4.f * (state->z1 + state->z3) + 6.f * state->z2);
				n++;
			}
			state->i--;
			state->z4 = state->z3;
			state->z3 = state->z2;
			state->z2 = state->z1;
			state->z1 = z0;
		}
	} else {
		for (size_t i = 0; i < n_in_samples; i++) {
			// TDF-II
			const float in = coeffs->ratio * x[i];
			const float v0 = coeffs->b0 * in;
			const float v1 = 4.f * v0;
			const float v2 = 6.f * v0;
			float o = v0 + state->z1;
			state->z1 = v1 + coeffs->ma1 * o + state->z2;
			state->z2 = v2 + coeffs->ma2 * o + state->z3;
			state->z3 = v1 + coeffs->ma3 * o + state->z4;
			state->z4 = v0 + coeffs->ma4 * o;
			y[n] = o;
			n++;
			for (int j = 1; j < coeffs->ratio; j++) {
				o = state->z1;
				state->z1 = coeffs->ma1 * o + state->z2;
				state->z2 = coeffs->ma2 * o + state->z3;
				state->z3 = coeffs->ma3 * o + state->z4;
				state->z4 = coeffs->ma4 * o;
				y[n] = o;
				n++;
			}
		}
	}

	BW_ASSERT_DEEP(bw_src_int_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(bw_src_int_state_is_valid(coeffs, state));
	BW_ASSERT(n <= (coeffs->ratio > 0 ? coeffs->ratio * n_in_samples : n_in_samples / (-coeffs->ratio)) + 1);

	return n;
}

static inline void bw_src_int_process_multi(
		const bw_src_int_coeffs * BW_RESTRICT              coeffs,
		bw_src_int_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * BW_RESTRICT const * BW_RESTRICT      x,
		float * BW_RESTRICT const * BW_RESTRICT            y,
		size_t                                             n_channels,
		size_t                                             n_in_samples,
		size_t * BW_RESTRICT                               n_out_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_src_int_coeffs_is_valid(coeffs));
	BW_ASSERT(state != NULL);
	BW_ASSERT(x != NULL);
	BW_ASSERT(y != NULL);
	BW_ASSERT((void*)x != (void*)y);

	if (n_out_samples != NULL)
		for (size_t i = 0; i < n_channels; i++)
			n_out_samples[i] = bw_src_int_process(coeffs, state[i], x[i], y[i], n_in_samples);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_src_int_process(coeffs, state[i], x[i], y[i], n_in_samples);

	BW_ASSERT_DEEP(bw_src_int_coeffs_is_valid(coeffs));
}

static inline char bw_src_int_coeffs_is_valid(
		const bw_src_int_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_src_int_coeffs"))
		return 0;
#endif

	return coeffs->ratio != 0
		&& bw_is_finite(coeffs->b0)
		&& bw_is_finite(coeffs->ma1)
		&& bw_is_finite(coeffs->ma2)
		&& bw_is_finite(coeffs->ma3)
		&& bw_is_finite(coeffs->ma4);
}

static inline char bw_src_int_state_is_valid(
		const bw_src_int_coeffs * BW_RESTRICT coeffs,
		const bw_src_int_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_src_int_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	if (coeffs)
		if (coeffs->ratio < 0 && (state->i < 0 || state->i >= -coeffs->ratio))
			return 0;

	return bw_is_finite(state->z1)
		&& bw_is_finite(state->z2)
		&& bw_is_finite(state->z3)
		&& bw_is_finite(state->z4);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::SRCInt
 *  ```>>> */
template<size_t N_CHANNELS>
class SRCInt {
public:
	SRCInt(
		int ratio);

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
		const float * BW_RESTRICT const * BW_RESTRICT x,
		float * const * BW_RESTRICT                   y,
		size_t                                        nInSamples,
		size_t * BW_RESTRICT                          nOutSamples = nullptr);

	void process(
		std::array<const float * BW_RESTRICT, N_CHANNELS> x,
		std::array<float * BW_RESTRICT, N_CHANNELS>       y,
		size_t                                            nInSamples,
		std::array<size_t, N_CHANNELS> * BW_RESTRICT      nOutSamples = nullptr);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_src_int_coeffs		coeffs;
	bw_src_int_state		states[N_CHANNELS];
	bw_src_int_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline SRCInt<N_CHANNELS>::SRCInt(
		int ratio) {
	bw_src_int_init(&coeffs, ratio);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void SRCInt<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_src_int_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_src_int_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void SRCInt<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void SRCInt<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_src_int_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void SRCInt<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void SRCInt<N_CHANNELS>::process(
		const float * BW_RESTRICT const * BW_RESTRICT x,
		float * const * BW_RESTRICT                   y,
		size_t                                        nInSamples,
		size_t * BW_RESTRICT                          nOutSamples) {
	bw_src_int_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nInSamples, nOutSamples);
}

template<size_t N_CHANNELS>
inline void SRCInt<N_CHANNELS>::process(
		std::array<const float * BW_RESTRICT, N_CHANNELS> x,
		std::array<float * BW_RESTRICT, N_CHANNELS>       y,
		size_t                                            nInSamples,
		std::array<size_t, N_CHANNELS> * BW_RESTRICT      nOutSamples) {
	process(x.data(), y.data(), nInSamples, nOutSamples ? nOutSamples->data() : nullptr);
}

}
#endif

#endif
