/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
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
 *  requires {{{
 *    bw_config bw_common bw_gain bw_lp1 bw_math bw_mm1 bw_one_pole
 *  }}}
 *  description {{{
 *    First-order high shelf filter (6 dB/oct) with unitary DC gain.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_hs1_reset_state()</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_HS1_H
#define _BW_HS1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_hs1_coeffs
 *  ```>>> */
typedef struct _bw_hs1_coeffs bw_hs1_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_hs1_state
 *  ```>>> */
typedef struct _bw_hs1_state bw_hs1_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_hs1_init()
 *  ```>>> */
static inline void bw_hs1_init(bw_hs1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_hs1_set_sample_rate()
 *  ```>>> */
static inline void bw_hs1_set_sample_rate(bw_hs1_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_hs1_reset_coeffs()
 *  ```>>> */
static inline void bw_hs1_reset_coeffs(bw_hs1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_hs1_reset_state()
 *  ```>>> */
static inline void bw_hs1_reset_state(const bw_hs1_coeffs *BW_RESTRICT coeffs, bw_hs1_state *BW_RESTRICT state, float x0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x0`.
 *
 *    #### bw_hs1_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_hs1_update_coeffs_ctrl(bw_hs1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_hs1_update_coeffs_audio()
 *  ```>>> */
static inline void bw_hs1_update_coeffs_audio(bw_hs1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_hs1_process1()
 *  ```>>> */
static inline float bw_hs1_process1(const bw_hs1_coeffs *BW_RESTRICT coeffs, bw_hs1_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_hs1_process()
 *  ```>>> */
static inline void bw_hs1_process(bw_hs1_coeffs *BW_RESTRICT coeffs, bw_hs1_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_hs1_set_cutoff()
 *  ```>>> */
static inline void bw_hs1_set_cutoff(bw_hs1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_hs1_set_high_gain_lin()
 *  ```>>> */
static inline void bw_hs1_set_high_gain_lin(bw_hs1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the high-frequency gain parameter to the given `value` (linear gain)
 *    in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_hs1_set_high_gain_dB()
 *  ```>>> */
static inline void bw_hs1_set_high_gain_dB(bw_hs1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the high-frequency gain parameter to the given `value` (dB) in
 *    `coeffs`.
 *
 *    Default value: `-INFINITY`.
 *
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_mm1.h>
#include <bw_math.h>

struct _bw_hs1_coeffs {
	// Sub-components
	bw_mm1_coeffs	mm1_coeffs;

	// Parameters
	float		cutoff;
	float		high_gain;
	char		update;
};

struct _bw_hs1_state {
	bw_mm1_state	mm1_state;
};

static inline void bw_hs1_init(bw_hs1_coeffs *BW_RESTRICT coeffs) {
	bw_mm1_init(&coeffs->mm1_coeffs);
	bw_mm1_set_prewarp_at_cutoff(&coeffs->mm1_coeffs, 0);
	bw_mm1_set_coeff_x(&coeffs->mm1_coeffs, 0.f);
	bw_mm1_set_coeff_lp(&coeffs->mm1_coeffs, 1.f);
	coeffs->cutoff = 1e3f;
	coeffs->high_gain = 1.f;
}

static inline void bw_hs1_set_sample_rate(bw_hs1_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_mm1_set_sample_rate(&coeffs->mm1_coeffs, sample_rate);
}

static inline void _bw_hs1_update_mm1_params(bw_hs1_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->update) {
		bw_mm1_set_cutoff(&coeffs->mm1_coeffs, coeffs->cutoff * bw_sqrtf_2(coeffs->high_gain));
		bw_mm1_set_coeff_x(&coeffs->mm1_coeffs, coeffs->high_gain);
		bw_mm1_set_coeff_lp(&coeffs->mm1_coeffs, 1.f - coeffs->high_gain);
		bw_mm1_set_prewarp_freq(&coeffs->mm1_coeffs, coeffs->cutoff);
		coeffs->update = 0;
	}
}

static inline void bw_hs1_reset_coeffs(bw_hs1_coeffs *BW_RESTRICT coeffs) {
	coeffs->update = 1;
	_bw_hs1_update_mm1_params(coeffs);
	bw_mm1_reset_coeffs(&coeffs->mm1_coeffs);
}

static inline void bw_hs1_reset_state(const bw_hs1_coeffs *BW_RESTRICT coeffs, bw_hs1_state *BW_RESTRICT state, float x0) {
	bw_mm1_reset_state(&coeffs->mm1_coeffs, &state->mm1_state, x0);
}

static inline void bw_hs1_update_coeffs_ctrl(bw_hs1_coeffs *BW_RESTRICT coeffs) {
	_bw_hs1_update_mm1_params(coeffs);
	bw_mm1_update_coeffs_ctrl(&coeffs->mm1_coeffs);
}

static inline void bw_hs1_update_coeffs_audio(bw_hs1_coeffs *BW_RESTRICT coeffs) {
	bw_mm1_update_coeffs_audio(&coeffs->mm1_coeffs);
}

static inline float bw_hs1_process1(const bw_hs1_coeffs *BW_RESTRICT coeffs, bw_hs1_state *BW_RESTRICT state, float x) {
	return bw_mm1_process1(&coeffs->mm1_coeffs, &state->mm1_state, x);
}

static inline void bw_hs1_process(bw_hs1_coeffs *BW_RESTRICT coeffs, bw_hs1_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_hs1_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_hs1_update_coeffs_audio(coeffs);
		y[i] = bw_hs1_process1(coeffs, state, x[i]);
	}
}

static inline void bw_hs1_set_cutoff(bw_hs1_coeffs *BW_RESTRICT coeffs, float value) {
	if (value != coeffs->cutoff) {
		coeffs->cutoff = value;
		coeffs->update = 1;
	}
}

static inline void bw_hs1_set_high_gain_lin(bw_hs1_coeffs *BW_RESTRICT coeffs, float value) {
	if (value != coeffs->high_gain) {
		coeffs->high_gain = value;
		coeffs->update = 1;
	}
}

static inline void bw_hs1_set_high_gain_dB(bw_hs1_coeffs *BW_RESTRICT coeffs, float value) {
	bw_hs1_set_high_gain_lin(coeffs, bw_dB2linf_3(value));
}

#ifdef __cplusplus
}
#endif

#endif
