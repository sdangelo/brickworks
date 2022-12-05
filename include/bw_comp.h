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
 *
 * File author: Stefano D'Angelo
 */

/*!
 *  module_type {{{ dsp }}}
 *  version {{{ 0.2.0 }}}
 *  requires {{{ bw_config bw_common bw_one_pole bw_math }}}
 *  description {{{
 *    Feedforward compressor/limiter with independent sidechain input.
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
 *    ### bw_comp_state
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
 *    These function process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_comp_process1()` assumes that gain compensation is disabled;
 *     * `bw_comp_process1_comp()` assumes that gain compensation is enabled.
 *
 *    ...
 *
 *    #### bw_comp_process()
 *  ```>>> */
static inline void bw_comp_process(bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state, const float *x, const float *x_sc, float y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    ...
 *
 *    #### bw_comp_set_bias()
 *  ```>>> */
static inline void bw_comp_set_thresh(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
static inline void bw_comp_set_ratio(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
static inline void bw_comp_set_attack_tau(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
static inline void bw_comp_set_release_tau(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
static inline void bw_comp_set_gain_dB(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the input bias `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comp_set_gain()
 *  ```>>> */
//static inline void bw_comp_set_gain(bw_comp_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the gain `value` in `coeffs`.
 *
 *    Do not set it to `0.f`, obviously.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_comp_set_gain_compensation()
 *  ```>>> */
//static inline void bw_comp_set_gain_compensation(bw_comp_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the output should be divided by gain (`value` non-`0`) or not
 *    (`0`).
 *
 *    Default value: `1` (on).
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_comp_coeffs {
	// Sub-components
	bw_env_follow_coeffs	env_follow_coeffs;
	bw_vol_coeffs		vol_coeffs;
	
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
	bw_vol_init(&coeffs->vol_coeffs);
	coeffs->thresh = 1.f;
	coeffs->ratio = 1.f;
}

static inline void bw_comp_set_sample_rate(bw_comp_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_env_follow_set_sample_rate(&coeffs->env_follow_coeffs, sample_rate);
	bw_vol_set_sample_rate(&coeffs->vol_coeffs, sample_rate);
}

static inline void bw_comp_reset_coeffs(bw_comp_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_reset_coeffs(&coeffs->env_follow_coeffs);
	bw_vol_reset_coeffs(&coeffs->vol_coeffs);
}

static inline void bw_comp_reset_state(const bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state) {
	bw_env_follow_reset_state(&coeffs->env_follow_coeffs, &state->env_follow_state);
}

static inline void bw_comp_update_coeffs_ctrl(bw_comp_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_ctrl(&coeffs->env_follow_coeffs);
	bw_vol_update_coeffs_ctrl(&coeffs->vol_coeffs);
	coeffs->kc = 1.f - coeffs->ratio;
}

static inline void bw_comp_update_coeffs_audio(bw_comp_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_audio(&coeffs->env_follow_coeffs);
	bw_vol_update_coeffs_audio(&coeffs->vol_coeffs);
}

static inline float bw_comp_process1(const bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state, float x, float x_sc) {
	const float env = bw_env_follow_process1(&coeffs->env_follow_coeffs, &coeffs->env_follow_state, x_sc);
	return env > coeffs->thresh ? coeffs->kc * bw_log2f_3(coeffs->thresh / env) * x : x;
}

static inline void bw_comp_process(bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state, const float *x, const float *x_sc, float y, int n_samples) {
	bw_comp_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_comp_update_coeffs_audio(coeffs);
		y[i] = bw_comp_process1(coeffs, state, x[i], x_sc[i]);
	}
}

static inline void bw_comp_set_thresh(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->thresh = value;
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

static inline void bw_comp_set_gain_dB(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	bw_vol_set_volume_dB(&coeffs->vol_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
