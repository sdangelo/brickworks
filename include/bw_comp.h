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
 *  version {{{ 0.5.0 }}}
 *  requires {{{
 *    bw_config bw_common bw_env_follow bw_one_pole bw_math bw_gain
 *  }}}
 *  description {{{
 *    Feedforward compressor/limiter with independent sidechain input.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Fixed typo in <code>bw_comp_set_ratio()</code>
 *              documentation.</li>
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

#ifndef _BW_COMP_H
#define _BW_COMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_comp_coeffs
 *  ```>>> */
typedef struct _bw_comp_coeffs bw_comp_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_comp_state
 *  ```>>> */
typedef struct _bw_comp_state bw_comp_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_comp_init()
 *  ```>>> */
static inline void bw_comp_init(bw_comp_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_comp_set_sample_rate()
 *  ```>>> */
static inline void bw_comp_set_sample_rate(bw_comp_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_comp_reset_coeffs()
 *  ```>>> */
static inline void bw_comp_reset_coeffs(bw_comp_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_comp_reset_state()
 *  ```>>> */
static inline void bw_comp_reset_state(const bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_comp_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_comp_update_coeffs_ctrl(bw_comp_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_comp_update_coeffs_audio()
 *  ```>>> */
static inline void bw_comp_update_coeffs_audio(bw_comp_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_comp_process1()
 *  ```>>> */
static inline float bw_comp_process1(const bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state, float x, float x_sc);
/*! <<<```
 *    Processes one input sample `x` and the corresponding sidechain input
 *    sample `x_sc` using `coeffs`, while using and updating `state`. Returns
 *    the corresponding output sample.
 *
 *    #### bw_comp_process()
 *  ```>>> */
static inline void bw_comp_process(bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state, const float *x, const float *x_sc, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and the first
 *    `n_samples` of the sidechain input buffer `x_sc`, and fills the first
 *    `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_comp_set_thresh_lin()
 *  ```>>> */
static inline void bw_comp_set_thresh_lin(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the threshold `value` (linear) in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_comp_set_thresh_dBFS()
 *  ```>>> */
static inline void bw_comp_set_thresh_dBFS(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the threshold `value` (dBFS) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comp_set_ratio()
 *  ```>>> */
static inline void bw_comp_set_ratio(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the compression ratio `value` in `coeffs`.
 *
 *    `value` is actually the slope of the gain curve above the threshold,
 *    hence `1.f` means no compression and `0.f` is a hard limit.
 *
 *    Valid range: [`0.f`, `1.f`].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_comp_set_attack_tau()
 *  ```>>> */
static inline void bw_comp_set_attack_tau(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the attack time constant `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comp_set_release_tau()
 *  ```>>> */
static inline void bw_comp_set_release_tau(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the release time constant `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comp_set_gain_lin()
 *  ```>>> */
static inline void bw_comp_set_gain_lin(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the output makeup gain `value` (linear ratio) in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_comp_set_gain_dB()
 *  ```>>> */
static inline void bw_comp_set_gain_dB(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the output makeup gain `value` (dB) in `coeffs`.
 *
 *    Default value: `0.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_env_follow.h>
#include <bw_gain.h>
#include <bw_one_pole.h>

struct _bw_comp_coeffs {
	// Sub-components
	bw_env_follow_coeffs	env_follow_coeffs;
	bw_gain_coeffs		gain_coeffs;
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_thresh_state;
	bw_one_pole_state	smooth_ratio_state;
	
	// Coefficients
	float			kc;

	// Parameters
	float			thresh;
	float			ratio;
};

struct _bw_comp_state {
	bw_env_follow_state	env_follow_state;
};

static inline void bw_comp_init(bw_comp_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_init(&coeffs->env_follow_coeffs);
	bw_gain_init(&coeffs->gain_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->thresh = 1.f;
	coeffs->ratio = 1.f;
}

static inline void bw_comp_set_sample_rate(bw_comp_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_env_follow_set_sample_rate(&coeffs->env_follow_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_coeffs, sample_rate);
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void bw_comp_reset_coeffs(bw_comp_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_reset_coeffs(&coeffs->env_follow_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_coeffs);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_thresh_state, coeffs->thresh);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_ratio_state, coeffs->ratio);
}

static inline void bw_comp_reset_state(const bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state) {
	bw_env_follow_reset_state(&coeffs->env_follow_coeffs, &state->env_follow_state);
}

static inline void bw_comp_update_coeffs_ctrl(bw_comp_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_ctrl(&coeffs->env_follow_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_coeffs);
}

static inline void bw_comp_update_coeffs_audio(bw_comp_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_audio(&coeffs->env_follow_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_coeffs);
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_thresh_state, coeffs->thresh);
	coeffs->kc = 1.f - bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_ratio_state, coeffs->ratio);
}

static inline float bw_comp_process1(const bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state, float x, float x_sc) {
	const float env = bw_env_follow_process1(&coeffs->env_follow_coeffs, &state->env_follow_state, x_sc);
	const float thresh = bw_one_pole_get_y_z1(&coeffs->smooth_thresh_state);
	const float y = env > thresh ? bw_pow2f_3(coeffs->kc * bw_log2f_3(thresh * bw_rcpf_2(env))) * x : x;
	return bw_gain_process1(&coeffs->gain_coeffs, y);
}

static inline void bw_comp_process(bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state, const float *x, const float *x_sc, float *y, int n_samples) {
	bw_comp_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_comp_update_coeffs_audio(coeffs);
		y[i] = bw_comp_process1(coeffs, state, x[i], x_sc[i]);
	}
}

static inline void bw_comp_set_thresh_lin(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->thresh = value;
}

static inline void bw_comp_set_thresh_dBFS(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->thresh = bw_dB2linf_3(value);
}

static inline void bw_comp_set_ratio(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->ratio = value;
}

static inline void bw_comp_set_attack_tau(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	bw_env_follow_set_attack_tau(&coeffs->env_follow_coeffs, value);
}

static inline void bw_comp_set_release_tau(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	bw_env_follow_set_release_tau(&coeffs->env_follow_coeffs, value);
}

static inline void bw_comp_set_gain_lin(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_coeffs, value);
}

static inline void bw_comp_set_gain_dB(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_dB(&coeffs->gain_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
