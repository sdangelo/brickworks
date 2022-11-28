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
 *  requires {{{ bw_config bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    Envelope follower made of a full-wave rectifier followed by
 *    [bw_one_pole](bw_one_pole) one-pole filter (6 dB/oct).
 *
 *    Unlike other dsp modules, you can directly access the parameters of the
 *    internal one-pole filter via the `bw_env_follow_get_one_pole()` function.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>Refactored API.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.1.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_ENV_FOLLOW_H
#define _BW_ENV_FOLLOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_env_follow_coeffs
 *  ```>>> */
typedef struct _bw_env_follow_coeffs bw_env_follow_coeffs;
/*! <<<```
 *    Coefficients.
 *
 *    ### bw_env_follow_state
 *  >>> */
typedef struct _bw_env_follow_state bw_env_follow_state;
/*! <<<```
 *    State.
 *
 *    #### bw_env_follow_init()
 *  ```>>> */
static inline void bw_env_follow_init(bw_env_follow_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes `coeffs`.
 *
 *    #### bw_env_follow_set_sample_rate()
 *  ```>>> */
static inline void bw_env_follow_set_sample_rate(bw_env_follow_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `coeffs`.
 *
 *    #### bw_one_pole_reset_state()
 *  ```>>> */
static inline void bw_env_follow_reset_state(const bw_env_follow_coeffs *BW_RESTRICT coeffs, bw_env_follow_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to the initial state using the given `coeffs`.
 *  >>> */

static inline void bw_env_follow_reset_coeffs(bw_env_follow_coeffs *BW_RESTRICT coeffs);

static inline void bw_env_follow_update_coeffs_ctrl(bw_env_follow_coeffs *BW_RESTRICT coeffs);
static inline void bw_env_follow_update_coeffs_audio(bw_env_follow_coeffs *BW_RESTRICT coeffs);

static inline float bw_env_follow_process1(const bw_env_follow_coeffs *BW_RESTRICT coeffs, bw_env_follow_state *BW_RESTRICT state, float x);

/*! ...
 *    #### bw_env_follow_process()
 *  ```>>> */
static inline void bw_env_follow_process(bw_env_follow_coeffs *BW_RESTRICT coeffs, bw_env_follow_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *  }}} */

static inline void bw_env_follow_set_attack_tau(bw_env_follow_coeffs *BW_RESTRICT coeffs, float value);
static inline void bw_env_follow_set_release_tau(bw_env_follow_coeffs *BW_RESTRICT coeffs, float value);

static inline float bw_env_follow_get_y_z1(const bw_env_follow_state *BW_RESTRICT state);

/*** Implementation ***/

#include <bw_math.h>
#include <bw_one_pole.h>

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

struct _bw_env_follow_coeffs {
	// Sub-components
	bw_one_pole_coeffs	one_pole_coeffs;
};

struct _bw_env_follow_state {
	bw_one_pole_state	one_pole_state;
};

static inline void bw_env_follow_init(bw_env_follow_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->one_pole_coeffs);
}

static inline void bw_env_follow_set_sample_rate(bw_env_follow_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->one_pole_coeffs, sample_rate);
}

static inline void bw_env_follow_reset_coeffs(bw_env_follow_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_coeffs(&coeffs->one_pole_coeffs);
}

static inline void bw_env_follow_reset_state(const bw_env_follow_coeffs *BW_RESTRICT coeffs, bw_env_follow_state *BW_RESTRICT state) {
	bw_one_pole_reset_state(&coeffs->one_pole_coeffs, &state->one_pole_state, 0.f);
}

static inline void bw_env_follow_update_coeffs_ctrl(bw_env_follow_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_update_coeffs_ctrl(&coeffs->one_pole_coeffs);
}

static inline void bw_env_follow_update_coeffs_audio(bw_env_follow_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_update_coeffs_audio(&coeffs->one_pole_coeffs);
}

static inline float bw_env_follow_process1(const bw_env_follow_coeffs *BW_RESTRICT coeffs, bw_env_follow_state *BW_RESTRICT state, float x) {
	x = bw_absf(x);
	return bw_one_pole_process1_asym(&coeffs->one_pole_coeffs, &state->one_pole_state, x);
}

static inline void bw_env_follow_process(bw_env_follow_coeffs *BW_RESTRICT coeffs, bw_env_follow_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_env_follow_update_coeffs_ctrl(coeffs);
	if (y)
		for (int i = 0; i < n_samples; i++) {
			bw_env_follow_update_coeffs_audio(coeffs);
			y[i] = bw_env_follow_process1(coeffs, state, x[i]);
		}
	else
		for (int i = 0; i < n_samples; i++) {
			bw_env_follow_update_coeffs_audio(coeffs);
			bw_env_follow_process1(coeffs, state, x[i]);
		}
}

static inline void bw_env_follow_set_attack_tau(bw_env_follow_coeffs *BW_RESTRICT coeffs, float value) {
	bw_one_pole_set_tau_up(&coeffs->one_pole_coeffs, value);
}

static inline void bw_env_follow_set_release_tau(bw_env_follow_coeffs *BW_RESTRICT coeffs, float value) {
	bw_one_pole_set_tau_down(&coeffs->one_pole_coeffs, value);
}

static inline float bw_env_follow_get_y_z1(const bw_env_follow_state *BW_RESTRICT state) {
	return bw_one_pole_get_y_z1(&state->one_pole_state);
}

#ifdef __cplusplus
}
#endif

#endif
