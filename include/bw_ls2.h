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
 *  requires {{{
 *    bw_config bw_common bw_gain bw_math bw_mm2 bw_one_pole bw_svf
 *  }}}
 *  description {{{
 *    Second-order low shelf filter (12 dB/oct) with gain asymptotically
 *    approaching unity as frequency increases.
 *
 *    The quality factor can be either directly controlled via the Q parameter
 *    or indirectly through the slope parameter, which indicates the "shelf
 *    slope" as defined in the "Cookbook formulae for audio EQ biquad filter
 *    coefficients" by Robert Bristow-Johnson. The use_slope parameter allows
 *    you to choose which parameterization to use.
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

#ifndef _BW_LS2_H
#define _BW_LS2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_ls2_coeffs
 *  ```>>> */
typedef struct _bw_ls2_coeffs bw_ls2_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_ls2_state
 *  ```>>> */
typedef struct _bw_ls2_state bw_ls2_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_ls2_init()
 *  ```>>> */
static inline void bw_ls2_init(bw_ls2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_ls2_set_sample_rate()
 *  ```>>> */
static inline void bw_ls2_set_sample_rate(bw_ls2_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_ls2_reset_coeffs()
 *  ```>>> */
static inline void bw_ls2_reset_coeffs(bw_ls2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_ls2_reset_state()
 *  ```>>> */
static inline void bw_ls2_reset_state(const bw_ls2_coeffs *BW_RESTRICT coeffs, bw_ls2_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_ls2_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_ls2_update_coeffs_ctrl(bw_ls2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ls2_update_coeffs_audio()
 *  ```>>> */
static inline void bw_ls2_update_coeffs_audio(bw_ls2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ls2_process1()
 *  ```>>> */
static inline float bw_ls2_process1(const bw_ls2_coeffs *BW_RESTRICT coeffs, bw_ls2_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_ls2_process()
 *  ```>>> */
static inline void bw_ls2_process(bw_ls2_coeffs *BW_RESTRICT coeffs, bw_ls2_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_ls2_set_cutoff()
 *  ```>>> */
static inline void bw_ls2_set_cutoff(bw_ls2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_ls2_set_Q()
 *  ```>>> */
static inline void bw_ls2_set_Q(bw_mm2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the quality factor to the given `value` in `coeffs`.
 *
 *    `value` must be equal or bigger than `0.5f`.
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_ls2_set_dc_gain_lin()
 *  ```>>> */
static inline void bw_ls2_set_dc_gain_lin(bw_ls2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the dc gain parameter to the given `value` (linear gain) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_ls2_set_dc_gain_dB()
 *  ```>>> */
static inline void bw_ls2_set_dc_gain_dB(bw_ls2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the dc gain parameter to the given `value` (dB) in `coeffs`.
 *
 *    Default value: `-INFINITY`.
 *
 *    #### bw_ls2_set_slope()
 *  ```>>> */
static inline void bw_ls2_set_slope(bw_ls2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the shelf slope `value` in `coeffs`.
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_ls2_set_use_slope()
 *  ```>>> */
static inline void bw_ls2_set_use_slope(bw_mm2_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the quality factor should be controlled via the slope
 *    parameter (`value` non-`0`) or via the Q parameter (`0`).
 *
 *    Default value: non-`0` (use slope parameter).
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_mm2.h>
#include <bw_math.h>

struct _bw_ls2_coeffs {
	// Sub-components
	bw_mm2_coeffs	mm2_coeffs;

	// Coefficients
	float		sg;
	float		isg;

	// Parameters
	float		gain;
	float		Q;
	float		slope;
	char		use_slope;
	int		param_changed;
};

struct _bw_ls2_state {
	bw_mm2_state	mm2_state;
};

#define _BW_LS2_PARAM_GAIN	1
#define _BW_LS2_PARAM_Q		(1<<1)
#define _BW_LS2_PARAM_SLOPE	(1<<1)

static inline void bw_ls2_init(bw_ls2_coeffs *BW_RESTRICT coeffs) {
	bw_mm2_init(&coeffs->mm2_coeffs);
	coeffs->gain = 1.f;
	coeffs->Q = 0.5f;
	coeffs->slope = 0.5f;
	coeffs->use_slope = 1;
}

static inline void bw_ls2_set_sample_rate(bw_ls2_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_mm2_set_sample_rate(&coeffs->mm2_coeffs, sample_rate);
}

static inline void _bw_ls2_update_mm2_params(bw_ls1_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->param_changed) {
		if (coeffs->param_changed & _BW_LS2_PARAM_GAIN) {
			coeffs->sg = bw_math_sqrtf_2(coeffs->gain);
			coeffs->isg = bw_rcpf_2(coeffs->sg);
			bw_mm2_set_coeff_x(&coeffs->mm2_coeffs, coeffs->sg);
			bw_mm2_set_coeff_lp(&coeffs->mm2_coeffs, 1.f - coeffs->isg);
			bw_mm2_set_coeff_hp(&coeffs->mm2_coeffs, 1.f - sg);
		}
		if (coeffs->use_slope) {
			if (coeffs->param_changed & _BW_LS2_PARAM_SLOPE) {
				const float k = coeffs->sg + coeffs->isg;
				bw_mm2_set_Q(&coeffs->mm2_coeffs, bw_sqrtf_2(coeffs->slope * bw_rcpf_2(coeffs->slope + coeffs->slope + k - k * coeffs->slope)));
			}
		}
		else {
			if (coeffs->param_changed & _BW_LS2_PARAM_Q)
				bw_mm2_set_Q(&coeffs->mm2_coeffs, coeffs->Q);
		}
		coeffs->param_changed = 0;
	}
}

static inline void bw_ls2_reset_coeffs(bw_ls2_coeffs *BW_RESTRICT coeffs) {
	coeffs->param_changed = ~0;
	_bw_ls2_update_mm2_params(coeffs);
	bw_mm2_reset_coeffs(&coeffs->mm2_coeffs);
}

static inline void bw_ls2_reset_state(const bw_ls2_coeffs *BW_RESTRICT coeffs, bw_ls2_state *BW_RESTRICT state) {
	bw_mm2_reset_state(&coeffs->mm2_coeffs, &state->mm2_state);
}

static inline void bw_ls2_update_coeffs_ctrl(bw_ls2_coeffs *BW_RESTRICT coeffs) {
	_bw_ls2_update_mm2_params(coeffs);
	bw_mm2_update_coeffs_ctrl(&coeffs->mm2_coeffs);
}

static inline void bw_ls2_update_coeffs_audio(bw_ls2_coeffs *BW_RESTRICT coeffs) {
	bw_mm2_update_coeffs_audio(&coeffs->mm2_coeffs);
}

static inline float bw_ls2_process1(const bw_ls2_coeffs *BW_RESTRICT coeffs, bw_ls2_state *BW_RESTRICT state, float x) {
	return bw_mm2_process1(&coeffs->mm2_coeffs, &state->mm2_state, x);
}

static inline void bw_ls2_process(bw_ls2_coeffs *BW_RESTRICT coeffs, bw_ls2_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_ls2_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_ls2_update_coeffs_audio(coeffs);
		y[i] = bw_ls2_process1(coeffs, state, x[i]);
	}
}

static inline void bw_ls2_set_cutoff(bw_ls2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_mm2_set_cutoff(&coeffs->mm2_coeffs, value);
}

static inline void bw_ls2_set_Q(bw_mm2_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->Q != value) {
		coeffs->Q = value;
		coeffs->param_changed |= _BW_LS2_PARAM_Q;
	}
}

static inline void bw_ls2_set_dc_gain_lin(bw_ls2_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->gain != value) {
		coeffs->gain = value;
		coeffs->param_changed |= _BW_LS2_PARAM_GAIN;
	}
}

static inline void bw_ls2_set_dc_gain_dB(bw_ls2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_ls2_set_dc_gain_lin(coeffs, bw_dB2linf_3(value));
}

static inline void bw_ls2_set_slope(bw_ls2_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->slope != value) {
		coeffs->slope = value;
		coeffs->param_changed |= _BW_LS2_PARAM_SLOPE;
	}
}

static inline void bw_ls2_set_use_slope(bw_mm2_coeffs *BW_RESTRICT coeffs, char value) {
	if ((coeffs->use_slope && !value) || (!coeffs->use_slope && value)) {
		coeffs->use_slope = value;
		coeffs->param_changed |= _BW_LS2_PARAM_Q | _BW_LS2_PARAM_SLOPE;
	}
}

#undef _BW_LS2_PARAM_GAIN
#undef _BW_LS2_PARAM_Q
#undef _BW_LS2_PARAM_S

#ifdef __cplusplus
}
#endif

#endif
