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
 *  requires {{{ bw_config bw_common bw_math }}}
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
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_SRC_INT_H
#define _BW_SRC_INT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_src_int_coeffs
 *  ```>>> */
typedef struct _bw_src_int_coeffs bw_src_int_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    ### bw_src_int_state
 *  ```>>> */
typedef struct _bw_src_int_state bw_src_int_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_src_int_init()
 *  ```>>> */
static inline void bw_src_int_init(bw_src_int_coeffs *BW_RESTRICT coeffs, int ratio);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *    XXX
 *
 *    #### bw_src_int_reset_state()
 *  ```>>> */
static inline void bw_src_int_reset_state(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state *BW_RESTRICT state, float x0);
/*! <<<```
 *    XXX
 *
 *    #### bw_src_int_process()
 *  ```>>> */
static inline int bw_src_int_process(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state *BW_RESTRICT state, const float *x, float *y, int n_in_samples);
/*! <<<```
 *    XXX
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

struct _bw_src_int_coeffs {
	int	ratio;
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

struct _bw_src_int_state {
	int	i;
	float	z1;
	float	z2;
	float	z3;
	float	z4;
};

static inline void bw_src_int_init(bw_src_int_coeffs *BW_RESTRICT coeffs, int ratio) {
	coeffs->ratio = ratio;
	// TODO: better filter, perhaps optimzied coefficients
	ratio = ratio >= 0 ? ratio : -ratio;
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

static inline void bw_src_int_reset_state(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state *BW_RESTRICT state, float x0) {
	if (coeffs->ratio < 0) {
		// DF-II
		state->z1 = x0 / (1.f + coeffs->a1 + coeffs->a2 + coeffs->a3 + coeffs->a4);
		state->z2 = state->z1;
		state->z3 = state->z2;
		state->z4 = state->z3;
		state->i = 1;
	} else {
		// TDF-II
		state->z4 = (coeffs->b4 - coeffs->a4) * x0;
		state->z3 = (coeffs->b3 - coeffs->a3) * x0 + state->z4;
		state->z2 = (coeffs->b2 - coeffs->a2) * x0 + state->z3;
		state->z1 = (coeffs->b1 - coeffs->a1) * x0 + state->z2;
	}
}

static inline int bw_src_int_process(const bw_src_int_coeffs *BW_RESTRICT coeffs, bw_src_int_state *BW_RESTRICT state, const float *x, float *y, int n_in_samples) {
	int n = 0;
	if (coeffs->ratio < 0) {
		for (int i = 0; i < n_in_samples; i++) {
			// DF-II
			const float z0 = x[i] - coeffs->a1 * state->z1 - coeffs->a2 * state->z2 - coeffs->a3 * state->z3 - coeffs->a4 * state->z4;
			state->i--;
			if (!state->i) {
				state->i = -coeffs->ratio;
				y[n] = coeffs->b0 * z0 + coeffs->b1 * state->z1 + coeffs->b2 * state->z2 + coeffs->b3 * state->z3 + coeffs->b4 * state->z4;
				n++;
			}
			state->z4 = state->z3;
			state->z3 = state->z2;
			state->z2 = state->z1;
			state->z1 = z0;
		}
	} else {
		for (int i = 0; i < n_in_samples; i++) {
			// TDF-II
			const float v0 = coeffs->b0 * x[i];
			const float v1 = coeffs->b1 * x[i];
			const float v2 = coeffs->b2 * x[i];
			const float v3 = coeffs->b3 * x[i];
			const float v4 = coeffs->b4 * x[i];
			for (int j = 0; j < coeffs->ratio; j++) {
				y[n] = v0 + state->z1;
				state->z1 = v1 - coeffs->a1 * y[n] + state->z2;
				state->z2 = v2 - coeffs->a2 * y[n] + state->z3;
				state->z3 = v3 - coeffs->a3 * y[n] + state->z4;
				state->z4 = v4 - coeffs->a4 * y[n];
				n++;
			}
		}
	}
	return n;
}

#ifdef __cplusplus
}
#endif

#endif
