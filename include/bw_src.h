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
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
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

#ifndef _BW_SRC_H
#define _BW_SRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_src_coeffs
 *  ```>>> */
typedef struct _bw_src_coeffs bw_src_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_src_state
 *  ```>>> */
typedef struct _bw_src_state bw_src_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_src_init()
 *  ```>>> */
static inline void bw_src_init(bw_src_coeffs *BW_RESTRICT coeffs, float ratio);
/*! <<<```
 *    Initializes `coeffs` using the given resampling `ratio`.
 *
 *    `ratio` must be positive and determines the sample rate of the output
 *    signal, which will be equal to `ratio` times the sample rate of the input
 *    signal.
 *
 *    #### bw_src_reset_state()
 *  ```>>> */
static inline void bw_src_reset_state(const bw_src_coeffs *BW_RESTRICT coeffs, bw_src_state *BW_RESTRICT state, float x0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x0`.
 *
 *    #### bw_src_process()
 *  ```>>> */
static inline void bw_src_process(const bw_src_coeffs *BW_RESTRICT coeffs, bw_src_state *BW_RESTRICT state, const float *x, float *y, int *n_in_samples, int *n_out_samples);
/*! <<<```
 *    Processes at most the first `n_in_samples` of the input buffer `x` and
 *    fills the output buffer `y` with at most `n_out_samples` using `coeffs`,
 *    while using and updating `state`.
 *
 *    After the call `n_in_samples` and `n_out_samples` will contain the actual
 *    number of consumed input samples and generated output samples,
 *    respectively.
 *
 *    #### bw_src_process_multi()
 *  ```>>> */
static inline void bw_src_process_multi(const bw_src_coeffs *BW_RESTRICT coeffs, bw_src_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int **n_in_samples, int **n_out_samples);
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
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

struct _bw_src_coeffs {
	float	k;
	float	a1;
	float	a2;
	float	a3;
	float	a4;
	float	b0;
	float	b1;
	float	b2;
};

struct _bw_src_state {
	float	i;
	float	z1;
	float	z2;
	float	z3;
	float	z4;
	float	xz1;
	float	xz2;
	float	xz3;
};

static inline void bw_src_init(bw_src_coeffs *BW_RESTRICT coeffs, float ratio) {
	coeffs->k = ratio >= 1.f ? 1.f / ratio : -1.f / ratio;
	// 4th-degree Butterworth with cutoff at ratio * Nyquist, using bilinear transform w/ prewarping
	const float fc = bw_minf(ratio >= 1.f ? 1.f / ratio : ratio, 0.9f);
	const float T = bw_tanf(1.570796326794896f * fc);
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

static inline void bw_src_reset_state(const bw_src_coeffs *BW_RESTRICT coeffs, bw_src_state *BW_RESTRICT state, float x0) {
	if (coeffs->k < 0) {
		// DF-II
		state->z1 = x0 / (1.f + coeffs->a1 + coeffs->a2 + coeffs->a3 + coeffs->a4);
		state->z2 = state->z1;
		state->z3 = state->z2;
		state->z4 = state->z3;
	} else {
		// TDF-II
		state->z4 = (coeffs->b0 - coeffs->a4) * x0;
		state->z3 = (coeffs->b1 - coeffs->a3) * x0 + state->z4;
		state->z2 = (coeffs->b2 - coeffs->a2) * x0 + state->z3;
		state->z1 = (coeffs->b1 - coeffs->a1) * x0 + state->z2;
	}
	state->i = 0.f;
	state->xz1 = x0;
	state->xz2 = x0;
	state->xz3 = x0;
}

static inline void bw_src_process(const bw_src_coeffs *BW_RESTRICT coeffs, bw_src_state *BW_RESTRICT state, const float *x, float *y, int *n_in_samples, int *n_out_samples) {
	int i = 0;
	int j = 0;
	if (coeffs->k < 0) {
		while (i < *n_in_samples && j < *n_out_samples) {
			// DF-II
			const float z0 = x[i] - coeffs->a1 * state->z1 - coeffs->a2 * state->z2 - coeffs->a3 * state->z3 - coeffs->a4 * state->z4;
			const float o = coeffs->b0 * (z0 + state->z4) + coeffs->b1 * (state->z1 + state->z3) + coeffs->b2 * state->z2;
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
				const float v1 = coeffs->b1 * o;
				const float v2 = coeffs->b2 * o;
				y[j] = v0 + state->z1;
				state->z1 = v1 - coeffs->a1 * y[j] + state->z2;
				state->z2 = coeffs->b2 * o - coeffs->a2 * y[j] + state->z3;
				state->z3 = v1 - coeffs->a3 * y[j] + state->z4;
				state->z4 = v0 - coeffs->a4 * y[j];
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
}

static inline void bw_src_process_multi(const bw_src_coeffs *BW_RESTRICT coeffs, bw_src_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int **n_in_samples, int **n_out_samples) {
	for (int i = 0; i < n_channels; i++)
		bw_src_process(coeffs, state[i], x[i], y[i], n_in_samples[i], n_out_samples[i]);
}

#ifdef __cplusplus
}
#endif

#endif
