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
 *  version {{{ 0.4.0 }}}
 *  requires {{{ bw_config bw_common }}}
 *  description {{{
 *    Aribtrary-ratio IIR sample rate converter.
 *  }}}
 *  changelog {{{
 *    <ul>
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
 *    ### bw_src_state
 *  ```>>> */
typedef struct _bw_src_state bw_src_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_src_init()
 *  ```>>> */
static inline void bw_src_init(bw_src_coeffs *BW_RESTRICT coeffs, float ratio);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *    XXX
 *
 *    #### bw_src_reset_state()
 *  ```>>> */
static inline void bw_src_reset_state(const bw_src_coeffs *BW_RESTRICT coeffs, bw_src_state *BW_RESTRICT state, float x0);
/*! <<<```
 *    XXX
 *
 *    #### bw_src_process()
 *  ```>>> */
static inline void bw_src_process(const bw_src_coeffs *BW_RESTRICT coeffs, bw_src_state *BW_RESTRICT state, const float *x, float *y, int *n_in_samples, int *n_out_samples);
/*! <<<```
 *    XXX
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

struct _bw_src_coeffs {
	float	k;
	float	a1;
	float	a2;
	float	a3;
	float	a4;
	float	b0;
	float	b1;
	float	b2;
	float	b3;
	float	b4;
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
	// TODO: better filter, perhaps optimzied coefficients
	ratio = ratio >= 1.f ? ratio : -coeffs->k;
	coeffs->a1 = (12.56637061435917f - 8.f * ratio) / (2.f * ratio + 3.141592653589793f);
	coeffs->a2 = (ratio * (24.f * ratio - 75.39822368615503f) + 59.21762640653615f) / (ratio * (4.f * ratio + 12.56637061435917f) + 9.869604401089358f);
	coeffs->a3 = (ratio * ((150.7964473723101f - 32.f * ratio) * ratio - 236.8705056261446f) + 124.0251067211993f) / (ratio * (ratio * (8.f * ratio + 37.69911184307752f) + 59.21762640653615f) + 31.00627668029982f);
	coeffs->a4 = (ratio * (ratio * (ratio * (16.f * ratio - 100.5309649148734f) + 236.8705056261446f) - 248.0502134423985f) + 97.40909103400242f) / (ratio * (ratio * (ratio * (16.f * ratio + 100.5309649148734f) + 236.8705056261446f) + 248.0502134423985f) + 97.40909103400242f);
	coeffs->b0 = 97.40909103400242f / (ratio * (ratio * (ratio * (16.f * ratio + 100.5309649148734f) + 236.8705056261446f) + 248.0502134423985f) + 97.40909103400242f);
	coeffs->b1 = 4.f * coeffs->b0;
	coeffs->b2 = 6.f * coeffs->b0;
	coeffs->b3 = coeffs->b1;
	coeffs->b4 = coeffs->b0;
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
		state->z4 = (coeffs->b4 - coeffs->a4) * x0;
		state->z3 = (coeffs->b3 - coeffs->a3) * x0 + state->z4;
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
			const float o = coeffs->b0 * z0 + coeffs->b1 * state->z1 + coeffs->b2 * state->z2 + coeffs->b3 * state->z3 + coeffs->b4 * state->z4;
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
			state->i += 1.f;
			state->z4 = state->z3;
			state->z3 = state->z2;
			state->z2 = state->z1;
			state->z1 = z0;
			state->xz3 = state->xz2;
			state->xz2 = state->xz1;
			state->xz1 = o;
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
				const float a = k2 + k4 + 0.5f;
				const float b = k3 - k1 - 0.5f * (x[i] + state->xz2);
				const float c = 0.5f * (k1 + k2);
				const float o = state->xz3 + state->i * (a + state->i * (b + state->i * c));
				// TDF-II
				y[j] = coeffs->b0 * o + state->z1;
				state->z1 = coeffs->b1 * o - coeffs->a1 * y[j] + state->z2;
				state->z2 = coeffs->b2 * o - coeffs->a2 * y[j] + state->z3;
				state->z3 = coeffs->b3 * o - coeffs->a3 * y[j] + state->z4;
				state->z4 = coeffs->b4 * o - coeffs->a4 * y[j];
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

#ifdef __cplusplus
}
#endif

#endif
