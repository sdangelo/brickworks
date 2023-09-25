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
 *    Aribtrary-ratio IIR sample rate converter.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_src_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_src_reset_state()</code> returns the initial output
 *              value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_src_process()</code> and
 *              <code>bw_src_process_multi()</code> now use <code>size_t</code>
 *              to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header and fixed C++ API.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Removed useless computation when upsampling.</li>
 *          <li>Clarified when the same buffers cannot be used for both input
 *              and output.</li>
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
 *          <li>Added <code>bw_src_process_multi()</code>.</li>
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

#ifndef BW_SRC_H
#define BW_SRC_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_src_coeffs
 *  ```>>> */
typedef struct bw_src_coeffs bw_src_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_src_state
 *  ```>>> */
typedef struct bw_src_state bw_src_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_src_init()
 *  ```>>> */
static inline void bw_src_init(
	bw_src_coeffs * BW_RESTRICT coeffs,
	float                       ratio);
/*! <<<```
 *    Initializes `coeffs` using the given resampling `ratio`.
 *
 *    `ratio` must be positive and determines the sample rate of the output
 *    signal, which will be equal to `ratio` times the sample rate of the input
 *    signal.
 *
 *    #### bw_src_reset_state()
 *  ```>>> */
static inline float bw_src_reset_state(
	const bw_src_coeffs * BW_RESTRICT coeffs,
	bw_src_state * BW_RESTRICT        state,
	float                             x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_src_reset_state_multi()
 *  ```>>> */
static inline void bw_src_reset_state_multi(
	const bw_src_coeffs * BW_RESTRICT              coeffs,
	bw_src_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_src_process()
 *  ```>>> */
static inline void bw_src_process(
	const bw_src_coeffs * BW_RESTRICT coeffs,
	bw_src_state * BW_RESTRICT        state,
	const float * BW_RESTRICT         x,
	float * BW_RESTRICT               y,
	size_t * BW_RESTRICT              n_in_samples,
	size_t * BW_RESTRICT              n_out_samples);
/*! <<<```
 *    Processes at most the first `n_in_samples` of the input buffer `x` and
 *    fills the output buffer `y` with at most `n_out_samples` using `coeffs`,
 *    while using and updating `state`.
 *
 *    After the call `n_in_samples` and `n_out_samples` will contain the actual
 *    number of consumed input samples and generated output samples,
 *    respectively.
 *
 *    `x` and `y` must point to different buffers and also `n_in_samples`. Also,
 *    `n_in_samples` and `n_out_samples` must be different.
 *
 *    #### bw_src_process_multi()
 *  ```>>> */
static inline void bw_src_process_multi(
	const bw_src_coeffs * BW_RESTRICT              coeffs,
	bw_src_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * BW_RESTRICT const * BW_RESTRICT  x,
	float * BW_RESTRICT const * BW_RESTRICT        y,
	size_t                                         n_channels,
	size_t * BW_RESTRICT                           n_in_samples,
	size_t * BW_RESTRICT                           n_out_samples);
/*! <<<```
 *    Processes at most the first `n_in_samples[i]` of each input buffer `x[i]`
 *    and fills the corresponding output buffer `y[i]` with at most
 *    `n_out_samples[i]` using `coeffs`, while using and updating each
 *    `state[i]`.
 *
 *    After the call each element in `n_in_samples` and `n_out_samples` will
 *    contain the actual number of consumed input samples and generated output
 *    samples, respectively, for each of the `n_channels` input/output buffer
 *    couples.
 *
 *    A given buffer cannot be used both as an input and output buffer. Also,
 *    `n_in_samples` and `n_out_samples` must point to non-overlapping memory
 *    areas.
 *
 *    #### bw_src_coeffs_is_valid()
 *  ```>>> */
static inline char bw_src_coeffs_is_valid(
	const bw_src_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_src_coeffs`.
 *
 *    #### bw_src_state_is_valid()
 *  ```>>> */
static inline char bw_src_state_is_valid(
	const bw_src_coeffs * BW_RESTRICT coeffs,
	const bw_src_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_src_state`.
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

struct bw_src_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	reset_id;
#endif

	// Coefficients
	float		k;
	float		b0;
	float		ma1;
	float		ma2;
	float		ma3;
	float		ma4;
};

struct bw_src_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// States
	float		i;
	float		z1;
	float		z2;
	float		z3;
	float		z4;
	float		xz1;
	float		xz2;
	float		xz3;
};

static inline void bw_src_init(
		bw_src_coeffs * BW_RESTRICT coeffs,
		float                       ratio) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT(ratio > 0.f);

	coeffs->k = ratio >= 1.f ? 1.f / ratio : -1.f / ratio;
	// 4th-degree Butterworth with cutoff at ratio * Nyquist, using bilinear transform w/ prewarping
	const float fc = bw_minf(ratio >= 1.f ? 1.f / ratio : ratio, 0.9f);
	const float T = bw_tanf(1.570796326794896f * fc);
	const float T2 = T * T;
	const float k = 1.f / (T * (T * (T * (T + 2.613125929752753f) + 3.414213562373095f) + 2.613125929752753f) + 1.f);
	coeffs->b0 = k * T2 * T2;
	coeffs->ma1 = k * (T * (T2 * (-5.226251859505504f - 4.f * T) + 5.226251859505504f) + 4.f);
	coeffs->ma2 = k * ((6.82842712474619f - 6.f * T2) * T2 - 6.f);
	coeffs->ma3 = k * (T * (T2 * (5.226251859505504f - 4.f * T) - 5.226251859505504f) + 4.f);
	coeffs->ma4 = k * (T * (T * ((2.613125929752753f - T) * T - 3.414213562373095f) + 2.613125929752753f) - 1.f);

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_src_coeffs");
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_src_coeffs_is_valid(coeffs));
}

static inline float bw_src_reset_state(
		const bw_src_coeffs * BW_RESTRICT coeffs,
		bw_src_state * BW_RESTRICT        state,
		float                             x_0) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_src_coeffs_is_valid(coeffs));
	BW_ASSERT(state != NULL);
	BW_ASSERT(bw_is_finite(x_0));

	if (coeffs->k < 0) {
		// DF-II
		state->z1 = x_0 / (1.f - coeffs->ma1 - coeffs->ma2 - coeffs->ma3 - coeffs->ma4);
		state->z2 = state->z1;
		state->z3 = state->z2;
		state->z4 = state->z3;
	} else {
		// TDF-II
		const float k = 4.f * coeffs->b0;
		state->z4 = (coeffs->b0 + coeffs->ma4) * x_0;
		state->z3 = (k + coeffs->ma3) * x_0 + state->z4;
		state->z2 = (6.f * coeffs->b0 + coeffs->ma2) * x_0 + state->z3;
		state->z1 = (k + coeffs->ma1) * x_0 + state->z2;
	}
	state->i = 0.f;
	state->xz1 = x_0;
	state->xz2 = x_0;
	state->xz3 = x_0;
	const float y = x_0;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_src_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_src_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(bw_src_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_src_reset_state_multi(
		const bw_src_coeffs * BW_RESTRICT              coeffs,
		bw_src_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                  x_0,
		float *                                        y_0,
		size_t                                         n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_src_coeffs_is_valid(coeffs));
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_src_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_src_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_src_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_src_process(
		const bw_src_coeffs * BW_RESTRICT coeffs,
		bw_src_state * BW_RESTRICT        state,
		const float * BW_RESTRICT         x,
		float * BW_RESTRICT               y,
		size_t * BW_RESTRICT              n_in_samples,
		size_t * BW_RESTRICT              n_out_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_src_coeffs_is_valid(coeffs));
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_src_state_is_valid(coeffs, state));
	BW_ASSERT(n_in_samples != NULL);
	BW_ASSERT(n_out_samples != NULL);
	BW_ASSERT(n_in_samples != n_out_samples);
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, *n_in_samples));
	BW_ASSERT(y != NULL);
	BW_ASSERT(x != y);

	size_t i = 0;
	size_t j = 0;
	if (coeffs->k < 0.f) {
		while (i < *n_in_samples && j < *n_out_samples) {
			// DF-II
			const float z0 = x[i] + coeffs->ma1 * state->z1 + coeffs->ma2 * state->z2 + coeffs->ma3 * state->z3 + coeffs->ma4 * state->z4;
			const float o = coeffs->b0 * (z0 + state->z4 + 4.f * (state->z1 + state->z3) + 6.f * state->z2);
			if (state->i >= 0.f) {
				// 3rd degree Lagrange interpolation + Horner's rule
				const float k1 = state->xz1 - state->xz2;
				const float k2 = 0.333333333333333f * (state->xz3 - o);
				const float k3 = o - k1;
				const float k4 = k3 - state->xz1;
				const float a = k2 - k4 - 0.5f * k4;
				const float b = k3 - k1 - 0.5f * (state->xz1 + state->xz3);
				const float c = 0.5f * (k1 + k2);
				y[j] = o + state->i * (a + state->i * (b + state->i * c));
				state->i += coeffs->k;
				j++;
			}
			state->z4 = state->z3;
			state->z3 = state->z2;
			state->z2 = state->z1;
			state->z1 = z0;
			state->xz3 = state->xz2;
			state->xz2 = state->xz1;
			state->xz1 = o;
			state->i += 1.f;
			i++;
		}
	} else {
		while (i < *n_in_samples && j < *n_out_samples) {
			while (state->i < 1.f && j < *n_out_samples) {
				// 3rd degree Lagrange interpolation + Horner's rule
				const float k1 = state->xz2 - state->xz1;
				const float k2 = 0.333333333333333f * (x[i] - state->xz3);
				const float k3 = state->xz3 - k1;
				const float k4 = state->xz2 - k3;
				const float a = k2 + k4 + 0.5f * k4;
				const float b = k3 - k1 - 0.5f * (x[i] + state->xz2);
				const float c = 0.5f * (k1 + k2);
				const float o = state->xz3 + state->i * (a + state->i * (b + state->i * c));
				// TDF-II
				const float v0 = coeffs->b0 * o;
				const float v1 = 4.f * v0;
				const float v2 = 6.f * v0;
				y[j] = v0 + state->z1;
				state->z1 = v1 + coeffs->ma1 * y[j] + state->z2;
				state->z2 = v2 + coeffs->ma2 * y[j] + state->z3;
				state->z3 = v1 + coeffs->ma3 * y[j] + state->z4;
				state->z4 = v0 + coeffs->ma4 * y[j];
				state->i += coeffs->k;
				j++;
			}
			if (state->i >= 1.f) {
				state->xz3 = state->xz2;
				state->xz2 = state->xz1;
				state->xz1 = x[i];
				state->i -= 1.f;
				i++;
			}
		}
	}
	*n_in_samples = i;
	*n_out_samples = j;

	BW_ASSERT_DEEP(bw_src_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(bw_src_state_is_valid(coeffs, state));
	BW_ASSERT(coeffs->k < 0.f ? *n_out_samples <= *n_in_samples : *n_out_samples >= *n_in_samples);
}

static inline void bw_src_process_multi(
		const bw_src_coeffs * BW_RESTRICT              coeffs,
		bw_src_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * BW_RESTRICT const * BW_RESTRICT  x,
		float * BW_RESTRICT const * BW_RESTRICT        y,
		size_t                                         n_channels,
		size_t * BW_RESTRICT                           n_in_samples,
		size_t * BW_RESTRICT                           n_out_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_src_coeffs_is_valid(coeffs));
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
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = 0; j < n_channels; j++)
			BW_ASSERT((void *)x[i] != (void *)y[j]);
#endif
	BW_ASSERT(n_in_samples != NULL);
	BW_ASSERT(n_out_samples != NULL);
	BW_ASSERT(n_in_samples != n_out_samples);

	for (size_t i = 0; i < n_channels; i++)
		bw_src_process(coeffs, state[i], x[i], y[i], n_in_samples + i, n_out_samples + i);

	BW_ASSERT_DEEP(bw_src_coeffs_is_valid(coeffs));
}

static inline char bw_src_coeffs_is_valid(
		const bw_src_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_src_coeffs"))
		return 0;
#endif

	return bw_is_finite(coeffs->k) && coeffs->k != 0.f
		&& bw_is_finite(coeffs->b0)
		&& bw_is_finite(coeffs->ma1)
		&& bw_is_finite(coeffs->ma2)
		&& bw_is_finite(coeffs->ma3)
		&& bw_is_finite(coeffs->ma4);
}

static inline char bw_src_state_is_valid(
		const bw_src_coeffs * BW_RESTRICT coeffs,
		const bw_src_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_src_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	(void)coeffs;

	return bw_is_finite(state->i)
		&& bw_is_finite(state->z1)
		&& bw_is_finite(state->z2)
		&& bw_is_finite(state->z3)
		&& bw_is_finite(state->z4)
		&& bw_is_finite(state->xz1)
		&& bw_is_finite(state->xz2)
		&& bw_is_finite(state->xz3);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::SRC
 *  ```>>> */
template<size_t N_CHANNELS>
class SRC {
public:
	SRC(
		float ratio);

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
		float * BW_RESTRICT const * BW_RESTRICT       y,
		size_t * BW_RESTRICT                          nInSamples,
		size_t * BW_RESTRICT                          nOutSamples);

	void process(
		std::array<const float * BW_RESTRICT, N_CHANNELS> x,
		std::array<float * BW_RESTRICT, N_CHANNELS>       y,
		std::array<size_t, N_CHANNELS> &                  nInSamples,
		std::array<size_t, N_CHANNELS> &                  nOutSamples);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_src_coeffs			coeffs;
	bw_src_state			states[N_CHANNELS];
	bw_src_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline SRC<N_CHANNELS>::SRC(
		float ratio) {
	bw_src_init(&coeffs, ratio);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void SRC<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_src_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_src_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void SRC<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void SRC<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_src_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void SRC<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void SRC<N_CHANNELS>::process(
		const float * BW_RESTRICT const * BW_RESTRICT x,
		float * BW_RESTRICT const * BW_RESTRICT       y,
		size_t * BW_RESTRICT                          nInSamples,
		size_t * BW_RESTRICT                          nOutSamples) {
	bw_src_process_multi(coeffs, statesP, x, y, N_CHANNELS, nInSamples, nOutSamples);
}

template<size_t N_CHANNELS>
inline void SRC<N_CHANNELS>::process(
		std::array<const float * BW_RESTRICT, N_CHANNELS> x,
		std::array<float * BW_RESTRICT, N_CHANNELS>       y,
		std::array<size_t, N_CHANNELS> &                  nInSamples,
		std::array<size_t, N_CHANNELS> &                  nOutSamples) {
	process(x.data(), y.data(), nInSamples.data(), nOutSamples.data());
}

}
#endif

#endif
