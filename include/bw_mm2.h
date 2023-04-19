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
 *  requires {{{ bw_config bw_common bw_gain bw_math bw_one_pole bw_svf }}}
 *  description {{{
 *    Second-order multimode filter.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_mm2_reset_state()</code>.</li>
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

#ifndef _BW_MM2_H
#define _BW_MM2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_mm2_coeffs
 *  ```>>> */
typedef struct _bw_mm2_coeffs bw_mm2_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_mm2_state
 *  ```>>> */
typedef struct _bw_mm2_state bw_mm2_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_mm2_init()
 *  ```>>> */
static inline void bw_mm2_init(bw_mm2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_mm2_set_sample_rate()
 *  ```>>> */
static inline void bw_mm2_set_sample_rate(bw_mm2_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_mm2_reset_coeffs()
 *  ```>>> */
static inline void bw_mm2_reset_coeffs(bw_mm2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_mm2_reset_state()
 *  ```>>> */
static inline void bw_mm2_reset_state(const bw_mm2_coeffs *BW_RESTRICT coeffs, bw_mm2_state *BW_RESTRICT state, float x0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x0`.
 *
 *    #### bw_mm2_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_mm2_update_coeffs_ctrl(bw_mm2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_mm2_update_coeffs_audio()
 *  ```>>> */
static inline void bw_mm2_update_coeffs_audio(bw_mm2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_mm2_process1()
 *  ```>>> */
static inline float bw_mm2_process1(const bw_mm2_coeffs *BW_RESTRICT coeffs, bw_mm2_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_mm2_process()
 *  ```>>> */
static inline void bw_mm2_process(bw_mm2_coeffs *BW_RESTRICT coeffs, bw_mm2_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_mm2_set_cutoff()
 *  ```>>> */
static inline void bw_mm2_set_cutoff(bw_mm2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_mm2_set_Q()
 *  ```>>> */
static inline void bw_mm2_set_Q(bw_mm2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the quality factor to the given `value` in `coeffs`.
 *
 *    `value` must be equal or bigger than `0.5f`.
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_mm2_set_prewarp_at_cutoff()
 *  ```>>> */
static inline void bw_mm2_set_prewarp_at_cutoff(bw_mm2_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether bilinear transform prewarping frequency should match the
 *    cutoff frequency (non-`0`) or not (`0`).
 *
 *    Default value: non-`0` (on).
 *
 *    #### bw_mm2_set_prewarp_freq()
 *  ```>>> */
static inline void bw_mm2_set_prewarp_freq(bw_mm2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the prewarping frequency `value` (Hz) in `coeffs`.
 *
 *    Only used when the prewarp\_at\_cutoff parameter is off.
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_mm2_set_coeff_x()
 *  ```>>> */
static inline void bw_mm2_set_coeff_x(bw_mm2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the input mode coefficient `value` in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_mm2_set_coeff_lp()
 *  ```>>> */
static inline void bw_mm2_set_coeff_lp(bw_mm2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the lowpass mode coefficient `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_mm2_set_coeff_bp()
 *  ```>>> */
static inline void bw_mm2_set_coeff_bp(bw_mm2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the bandpass mode coefficient `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_mm2_set_coeff_hp()
 *  ```>>> */
static inline void bw_mm2_set_coeff_hp(bw_mm2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the highpass mode coefficient `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_svf.h>
#include <bw_gain.h>

struct _bw_mm2_coeffs {
	// Sub-components
	bw_svf_coeffs	svf_coeffs;
	bw_gain_coeffs	gain_x_coeffs;
	bw_gain_coeffs	gain_lp_coeffs;
	bw_gain_coeffs	gain_bp_coeffs;
	bw_gain_coeffs	gain_hp_coeffs;
};

struct _bw_mm2_state {
	bw_svf_state	svf_state;
};

static inline void bw_mm2_init(bw_mm2_coeffs *BW_RESTRICT coeffs) {
	bw_svf_init(&coeffs->svf_coeffs);
	bw_gain_init(&coeffs->gain_x_coeffs);
	bw_gain_init(&coeffs->gain_lp_coeffs);
	bw_gain_init(&coeffs->gain_bp_coeffs);
	bw_gain_init(&coeffs->gain_hp_coeffs);
	bw_gain_set_smooth_tau(&coeffs->gain_x_coeffs, 0.005f);
	bw_gain_set_smooth_tau(&coeffs->gain_lp_coeffs, 0.005f);
	bw_gain_set_smooth_tau(&coeffs->gain_bp_coeffs, 0.005f);
	bw_gain_set_smooth_tau(&coeffs->gain_hp_coeffs, 0.005f);
	bw_gain_set_gain_lin(&coeffs->gain_x_coeffs, 1.f);
	bw_gain_set_gain_lin(&coeffs->gain_lp_coeffs, 0.f);
	bw_gain_set_gain_lin(&coeffs->gain_bp_coeffs, 0.f);
	bw_gain_set_gain_lin(&coeffs->gain_hp_coeffs, 0.f);
}

static inline void bw_mm2_set_sample_rate(bw_mm2_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_svf_set_sample_rate(&coeffs->svf_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_x_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_lp_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_bp_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_hp_coeffs, sample_rate);
}

static inline void bw_mm2_reset_coeffs(bw_mm2_coeffs *BW_RESTRICT coeffs) {
	bw_svf_reset_coeffs(&coeffs->svf_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_x_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_lp_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_bp_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_hp_coeffs);
}

static inline void bw_mm2_reset_state(const bw_mm2_coeffs *BW_RESTRICT coeffs, bw_mm2_state *BW_RESTRICT state, float x0) {
	bw_svf_reset_state(&coeffs->svf_coeffs, &state->svf_state, x0);
}

static inline void bw_mm2_update_coeffs_ctrl(bw_mm2_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_ctrl(&coeffs->svf_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_x_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_lp_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_bp_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_hp_coeffs);
}

static inline void bw_mm2_update_coeffs_audio(bw_mm2_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_audio(&coeffs->svf_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_x_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_lp_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_bp_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_hp_coeffs);
}

static inline float bw_mm2_process1(const bw_mm2_coeffs *BW_RESTRICT coeffs, bw_mm2_state *BW_RESTRICT state, float x) {
	float lp, bp, hp;
	bw_svf_process1(&coeffs->svf_coeffs, &state->svf_state, x, &lp, &bp, &hp);
	const float vx = bw_gain_process1(&coeffs->gain_x_coeffs, x);
	const float vlp = bw_gain_process1(&coeffs->gain_lp_coeffs, lp);
	const float vbp = bw_gain_process1(&coeffs->gain_bp_coeffs, bp);
	const float vhp = bw_gain_process1(&coeffs->gain_hp_coeffs, hp);
	return vx + vlp + vbp + vhp;
}

static inline void bw_mm2_process(bw_mm2_coeffs *BW_RESTRICT coeffs, bw_mm2_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_mm2_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_mm2_update_coeffs_audio(coeffs);
		y[i] = bw_mm2_process1(coeffs, state, x[i]);
	}
}

static inline void bw_mm2_set_cutoff(bw_mm2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_cutoff(&coeffs->svf_coeffs, value);
}

static inline void bw_mm2_set_Q(bw_mm2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_Q(&coeffs->svf_coeffs, value);
}

static inline void bw_mm2_set_prewarp_at_cutoff(bw_mm2_coeffs *BW_RESTRICT coeffs, char value) {
	bw_svf_set_prewarp_at_cutoff(&coeffs->svf_coeffs, value);
}

static inline void bw_mm2_set_prewarp_freq(bw_mm2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_prewarp_freq(&coeffs->svf_coeffs, value);
}

static inline void bw_mm2_set_coeff_x(bw_mm2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_x_coeffs, value);
}

static inline void bw_mm2_set_coeff_lp(bw_mm2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_lp_coeffs, value);
}

static inline void bw_mm2_set_coeff_bp(bw_mm2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_bp_coeffs, -value);
}

static inline void bw_mm2_set_coeff_hp(bw_mm2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_hp_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
