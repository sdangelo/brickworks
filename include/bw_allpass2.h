/*
 * Brickworks
 *
 * Copyright (C) 2022 Orastron Srl unipersonale
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
 *  version {{{ 0.3.0 }}}
 *  requires {{{ bw_config bw_common bw_math }}}
 *  description {{{
 *    Second-order allpass filter.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_ALLPASS2_H
#define _BW_ALLPASS2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_allpass2_coeffs
 *  ```>>> */
typedef struct _bw_allpass2_coeffs bw_allpass2_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_allpass2_state
 *  ```>>> */
typedef struct _bw_allpass2_state bw_allpass2_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_allpass2_init()
 *  ```>>> */
static inline void bw_allpass2_init(bw_allpass2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_allpass2_set_sample_rate()
 *  ```>>> */
static inline void bw_allpass2_set_sample_rate(bw_allpass2_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_allpass2_reset_coeffs()
 *  ```>>> */
static inline void bw_allpass2_reset_coeffs(bw_allpass2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_allpass2_reset_state()
 *  ```>>> */
static inline void bw_allpass2_reset_state(const bw_allpass2_coeffs *BW_RESTRICT coeffs, bw_allpass2_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_allpass2_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_allpass2_update_coeffs_ctrl(bw_allpass2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_allpass2_update_coeffs_audio()
 *  ```>>> */
static inline void bw_allpass2_update_coeffs_audio(bw_allpass2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_allpass2_process1()
 *  ```>>> */
static inline float bw_allpass2_process1(const bw_allpass2_coeffs *BW_RESTRICT coeffs, bw_allpass2_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_allpass2_process()
 *  ```>>> */
static inline void bw_allpass2_process(bw_allpass2_coeffs *BW_RESTRICT coeffs, bw_allpass2_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_allpass2_set_cutoff()
 *  ```>>> */
static inline void bw_allpass2_set_cutoff(bw_allpass2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_allpass2_set_Q()
 *  ```>>> */
static inline void bw_allpass2_set_Q(bw_allpass2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the quality factor `value` in `coeffs`.
 *
 *    Default value: `0.5f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

struct _bw_allpass2_coeffs {
	// Coefficients
	float	t_k;

	float	t;
	float	kf;
	float	k;
	float	ks;
	float	X1_x;
	float	X2_X2_xz1_x1;
	float	X2_X1;
	float	X1_X2_xz1_x1;
	float	X1_VC1_xz1;

	// Parameters
	float	cutoff;
	float	Q;
	int	param_changed;
};

struct _bw_allpass2_state {
	float	VC1_z1;
	float	VC2_z1;
	float	X1_z1;
	float	X2_z1;
};

#define _BW_ALLPASS2_PARAM_CUTOFF	1
#define _BW_ALLPASS2_PARAM_Q		(1<<1)

static inline void bw_allpass2_init(bw_allpass2_coeffs *BW_RESTRICT coeffs) {
	coeffs->cutoff = 1e3f;
	coeffs->Q = 0.5f;
}

static inline void bw_allpass2_set_sample_rate(bw_allpass2_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->t_k = 3.141592653589793f / sample_rate;
}

static inline void bw_allpass2_reset_coeffs(bw_allpass2_coeffs *BW_RESTRICT coeffs) {
	coeffs->param_changed = ~0;
	bw_allpass2_update_coeffs_ctrl(coeffs);
}

static inline void bw_allpass2_reset_state(const bw_allpass2_coeffs *BW_RESTRICT coeffs, bw_allpass2_state *BW_RESTRICT state) {
	state->VC1_z1 = 0.f;
	state->VC2_z1 = 0.f;
	state->X1_z1 = 0.f;
	state->X2_z1 = 0.f;
}

static inline void bw_allpass2_update_coeffs_ctrl(bw_allpass2_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->param_changed) {
		if (coeffs->param_changed & _BW_ALLPASS2_PARAM_CUTOFF) {
			coeffs->t = bw_tanf_3(coeffs->t_k * coeffs->cutoff);
			coeffs->kf = coeffs->t * bw_rcpf_2(coeffs->cutoff);
		}
		if (coeffs->param_changed & _BW_ALLPASS2_PARAM_Q) {
			const float Q2 = coeffs->Q + coeffs->Q;
			const float Q2s = Q2 + Q2;
			coeffs->k = Q2 * bw_rcpf_2(1.f + bw_sqrtf_2(1.f + Q2s + Q2s));
			coeffs->ks = coeffs->k * coeffs->k;
			coeffs->X1_x = 1.f - coeffs->ks - coeffs->ks;
		}
		const float kt = coeffs->k * coeffs->t;
		const float kt1 = 1.f + kt;
		const float X1_k = coeffs->cutoff * bw_rcpf_2(coeffs->t * kt1 + coeffs->k * (1.f - kt - kt));
		coeffs->X2_X2_xz1_x1 = -coeffs->cutoff * coeffs->k * bw_rcpf_2(kt1);
		coeffs->X2_X1 = coeffs->kf * coeffs->X2_X2_xz1_x1;
		coeffs->X1_X2_xz1_x1 = 3.f * X1_k * coeffs->ks;
		coeffs->X1_VC1_xz1 = X1_k * kt1;
		coeffs->param_changed = 0;
	}
}

static inline void bw_allpass2_update_coeffs_audio(bw_allpass2_coeffs *BW_RESTRICT coeffs) {
}

static inline float bw_allpass2_process1(const bw_allpass2_coeffs *BW_RESTRICT coeffs, bw_allpass2_state *BW_RESTRICT state, float x) {
	const float VC2_xz1 = coeffs->kf * state->X2_z1 + state->VC2_z1;
	const float VC1_xz1 = coeffs->kf * state->X1_z1 + state->VC1_z1;
	const float X2_xz1_x1 = state->VC2_xz1 + state->VC1_xz1;
	const float X2_xz1 = coeffs->X2_X2_xz1_x1 * X2_xz1_x1;
	const float X1 = coeffs->X1_VC1_xz1 * (coeffs->X1_x * x - state->VC1_xz1) + coeffs->X1_X2_xz1_x1 * X2_xz1_x1;
	const float X2 = X2_xz1 + coeffs->X2_X1 * X1;
	const float VC1 = coeffs->kf * X1 + VC1_xz1;
	const float VC2 = coeffs->kf * X2 + VC2_xz1;
	const float y_x1 = VC1 + VC2;
	state->VC1_z1 = VC1;
	state->VC2_z1 = VC2;
	state->X1_z1 = X1;
	state->X2_z1 = X2;
	return x - (y_x1 + y_x1);
}

static inline void bw_allpass2_process(bw_allpass2_coeffs *BW_RESTRICT coeffs, bw_allpass2_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_allpass2_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_allpass2_process1(coeffs, state, x[i]);
}

static inline void bw_allpass2_set_cutoff(bw_allpass2_coeffs *BW_RESTRICT coeffs, float value) {
	if (value != coeffs->cutoff) {
		coeffs->cutoff = value;
		coeffs->param_changed |= _BW_ALLPASS2_PARAM_CUTOFF;
	}
}

static inline void bw_allpass2_set_Q(bw_allpass2_coeffs *BW_RESTRICT coeffs, float value) {
	if (value != coeffs->Q) {
		coeffs->Q = value;
		coeffs->param_changed |= _BW_ALLPASS2_PARAM_Q;
	}
}

#undef _BW_ALLPASS2_PARAM_CUTOFF
#undef _BW_ALLPASS2_PARAM_Q

#ifdef __cplusplus
}
#endif

#endif
