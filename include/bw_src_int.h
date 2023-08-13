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
 *    Multi-rate filtering inspired by
 *
 *    M. Holters and J.Parker, "A Combined Model for a Bucket Brigade Device and
 *    its Input and Output Filters", 21st Intl. Conf. Digital Audio Effects
 *    (DAFx-18), Aveiro, Portugal, September 2018.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_src_int_lim_process()</code> and
 *              <code>bw_src_int_lim_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overladed C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
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
static inline void bw_src_int_init(bw_src_int_coeffs *BW_RESTRICT coeffs, int ratio);
/*! <<<```
 *    Initializes `coeffs` using the given resampling `ratio`.
 *
 *    If `ratio` is positive, then the sample rate of the output signal will be
 *    `ratio` times the sample rate of the input signal, otherwise, if it is
 *    negative, then the sample rate of the output signal will be equal to the
 *    sample rate of the input signal divided by `-ratio`. `ratio` *MUST NOT* be
 *    in [`-1`, `1`].
 *
 *    #### bw_src_int_reset_state()
 *  ```>>> */
static inline void bw_src_int_reset_state(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state *BW_RESTRICT state, float x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x_0`.
 *
 *    #### bw_src_int_process()
 *  ```>>> */
static inline size_t bw_src_int_process(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state *BW_RESTRICT state, const float *x, float *y, size_t n_in_samples);
/*! <<<```
 *    Processes the first `n_in_samples` of the input buffer `x` and fills the
 *    output buffer `y` using `coeffs`, while using and updating `state`.
 *
 *    The number of generated output samples will be `ratio` times
 *    `n_in_samples` if `ratio` is positive, otherwise at most `n_in_samples`
 *    divided by `-ratio` and then rounded towards positive infinity.
 *
 *    Returns the number of generated output samples.
 *
 *    #### bw_src_int_process_multi()
 *  ```>>> */
static inline void bw_src_int_process_multi(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state * const *BW_RESTRICT state, const float * const *x, float **y, size_t *n, size_t n_channels, size_t n_in_samples);
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
 *    `n` is filled with the number of generated output samples for each output
 *    buffer, if not `NULL`.
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
	int	ratio;
	float	a1;
	float	a2;
	float	a3;
	float	a4;
	float	b0;
	float	b1;
	float	b2;
};

struct bw_src_int_state {
	int	i;
	float	z1;
	float	z2;
	float	z3;
	float	z4;
};

static inline void bw_src_int_init(bw_src_int_coeffs *BW_RESTRICT coeffs, int ratio) {
	coeffs->ratio = ratio;
	// 4th-degree Butterworth with cutoff at ratio * Nyquist, using bilinear transform w/ prewarping
	const float fc = (float)(ratio >= 0 ? ratio : -ratio);
	const float T = bw_tanf(1.570796326794896f / fc);
	const float T2 = T * T;
	const float k = 1.f / (T * (T * (T * (T + 2.613125929752753f) + 3.414213562373095f) + 2.613125929752753f) + 1.f);
	coeffs->b0 = k * T2 * T2;
	coeffs->b1 = 4.f * coeffs->b0;
	coeffs->b2 = 6.f * coeffs->b0;
	coeffs->a1 = k * (T * (T2 * (4.f * T + 5.226251859505504f) - 5.226251859505504f) - 4.f);
	coeffs->a2 = k * (T2 * (6.f * T2 - 6.82842712474619f) + 6.f);
	coeffs->a3 = k * (T * (T2 * (4.f * T - 5.226251859505504f) + 5.226251859505504f) - 4.f);
	coeffs->a4 = k * (T * (T * (T * (T - 2.613125929752753f) + 3.414213562373095f) - 2.613125929752753f) + 1.f);
}

static inline void bw_src_int_reset_state(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state *BW_RESTRICT state, float x_0) {
	if (coeffs->ratio < 0) {
		// DF-II
		state->z1 = x_0 / (1.f + coeffs->a1 + coeffs->a2 + coeffs->a3 + coeffs->a4);
		state->z2 = state->z1;
		state->z3 = state->z2;
		state->z4 = state->z3;
		state->i = 0;
	} else {
		// TDF-II
		state->z4 = (coeffs->b0 - coeffs->a4) * x_0;
		state->z3 = (coeffs->b1 - coeffs->a3) * x_0 + state->z4;
		state->z2 = (coeffs->b2 - coeffs->a2) * x_0 + state->z3;
		state->z1 = (coeffs->b1 - coeffs->a1) * x_0 + state->z2;
	}
}

static inline size_t bw_src_int_process(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state *BW_RESTRICT state, const float *x, float *y, size_t n_in_samples) {
	size_t n = 0;
	if (coeffs->ratio < 0) {
		for (size_t i = 0; i < n_in_samples; i++) {
			// DF-II
			const float z0 = x[i] - coeffs->a1 * state->z1 - coeffs->a2 * state->z2 - coeffs->a3 * state->z3 - coeffs->a4 * state->z4;
			if (!state->i) {
				state->i = -coeffs->ratio;
				y[n] = coeffs->b0 * (z0 + state->z4) + coeffs->b1 * (state->z1 + state->z3) + coeffs->b2 * state->z2;
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
			const float v0 = coeffs->b0 * x[i];
			const float v1 = coeffs->b1 * x[i];
			const float v2 = coeffs->b2 * x[i];
			for (int j = 0; j < coeffs->ratio; j++) {
				y[n] = v0 + state->z1;
				state->z1 = v1 - coeffs->a1 * y[n] + state->z2;
				state->z2 = v2 - coeffs->a2 * y[n] + state->z3;
				state->z3 = v1 - coeffs->a3 * y[n] + state->z4;
				state->z4 = v0 - coeffs->a4 * y[n];
				n++;
			}
		}
	}
	return n;
}

static inline void bw_src_int_process_multi(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state * const *BW_RESTRICT state, const float * const *x, float **y, size_t *n, size_t n_channels, size_t n_in_samples) {
	if (n != NULL)
		for (size_t i = 0; i < n_channels; i++)
			n[i] = bw_src_int_process(coeffs, state[i], x[i], y[i], n_in_samples);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_src_int_process(coeffs, state[i], x[i], y[i], n_in_samples);
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
	SRCInt(int ratio);
	
	void reset(float x0 = 0.f);
	std::array<size_t, N_CHANNELS> process(
		const float * const *x,
		float **y,
		size_t nInSamples);
	std::array<size_t, N_CHANNELS> process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nInSamples);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_src_int_coeffs	 coeffs;
	bw_src_int_state	 states[N_CHANNELS];
	bw_src_int_state	*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline SRCInt<N_CHANNELS>::SRCInt(int ratio) {
	bw_src_int_init(&coeffs, ratio);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void SRCInt<N_CHANNELS>::reset(float x0) {
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_src_int_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline std::array<size_t, N_CHANNELS> SRCInt<N_CHANNELS>::process(
		const float * const *x,
		float **y,
		size_t nInSamples) {
	std::array<size_t, N_CHANNELS> ret;
	bw_src_int_process_multi(&coeffs, statesP, x, y, ret.data(), N_CHANNELS, nInSamples);
	return ret;
}

template<size_t N_CHANNELS>
inline std::array<size_t, N_CHANNELS> SRCInt<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nInSamples) {
	return process(x.data(), y.data(), nInSamples);
}

}
#endif

#endif
