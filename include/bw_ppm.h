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
 *  version {{{ 0.3.0 }}}
 *  requires {{{ bw_config bw_common bw_env_follow bw_math bw_one_pole }}}
 *  description {{{
 *    Digital peak programme meter with adjustable integration time constant.
 *
 *    An integration time constant of `0.f` results in true-peak metering, while
 *    values between `0.f` and `0.006f` give quasi-peak metering behavior.
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

#ifndef _BW_PPM_H
#define _BW_PPM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_ppm_coeffs
 *  ```>>> */
typedef struct _bw_ppm_coeffs bw_ppm_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_ppm_state
 *  ```>>> */
typedef struct _bw_ppm_state bw_ppm_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_ppm_init()
 *  ```>>> */
static inline void bw_ppm_init(bw_ppm_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_ppm_set_sample_rate()
 *  ```>>> */
static inline void bw_ppm_set_sample_rate(bw_ppm_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_ppm_reset_coeffs()
 *  ```>>> */
static inline void bw_ppm_reset_coeffs(bw_ppm_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_ppm_reset_state()
 *  ```>>> */
static inline void bw_ppm_reset_state(const bw_ppm_coeffs *BW_RESTRICT coeffs, bw_ppm_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_ppm_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_ppm_update_coeffs_ctrl(bw_ppm_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ppm_update_coeffs_audio()
 *  ```>>> */
static inline void bw_ppm_update_coeffs_audio(bw_ppm_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ppm_process1()
 *  ```>>> */
static inline float bw_ppm_process1(const bw_ppm_coeffs *BW_RESTRICT coeffs, bw_ppm_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_ppm_process()
 *  ```>>> */
static inline void bw_ppm_process(bw_ppm_coeffs *BW_RESTRICT coeffs, bw_ppm_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    Output sample values are in dBFS.
 *
 *    `y` may be `NULL`.
 *
 *    #### bw_ppm_set_integration_time()
 *  ```>>> */
static inline void bw_ppm_set_integration_tau(bw_ppm_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the upgoing (integration) time constant in `coeffs` to `value` (s).
 *
 *    Default value: `0.f`.
 *
 *    #### bw_ppm_get_y_z1()
 *  ```>>> */
static inline float bw_ppm_get_y_z1(const bw_ppm_state *BW_RESTRICT state);
/*! <<<```
 *    Returns the last output sample as stored in `state`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_env_follow.h>

struct _bw_ppm_coeffs {
	// Sub-components
	bw_env_follow_coeffs	env_follow_coeffs;
};

struct _bw_ppm_state {
	bw_env_follow_state	env_follow_state;
	float			y_z1;
};

static inline void bw_ppm_init(bw_ppm_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_init(&coeffs->env_follow_coeffs);
	bw_env_follow_set_release_tau(&coeffs->env_follow_coeffs, 0.738300619235528f);
}

static inline void bw_ppm_set_sample_rate(bw_ppm_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_env_follow_set_sample_rate(&coeffs->env_follow_coeffs, sample_rate);
}

static inline void bw_ppm_reset_coeffs(bw_ppm_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_reset_coeffs(&coeffs->env_follow_coeffs);
}

static inline void bw_ppm_reset_state(const bw_ppm_coeffs *BW_RESTRICT coeffs, bw_ppm_state *BW_RESTRICT state) {
	bw_env_follow_reset_state(&coeffs->env_follow_coeffs, &state->env_follow_state);
	state->y_z1 = -INFINITY;
}

static inline void bw_ppm_update_coeffs_ctrl(bw_ppm_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_ctrl(&coeffs->env_follow_coeffs);
}

static inline void bw_ppm_update_coeffs_audio(bw_ppm_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_audio(&coeffs->env_follow_coeffs);
}

static inline float bw_ppm_process1(const bw_ppm_coeffs *BW_RESTRICT coeffs, bw_ppm_state *BW_RESTRICT state, float x) {
	const float yl = bw_env_follow_process1(&coeffs->env_follow_coeffs, &state->env_follow_state, x);
	const float y = yl > 0.f ? bw_lin2dBf_3(yl) : -INFINITY;
	state->y_z1 = y;
	return y;
}

static inline void bw_ppm_process(bw_ppm_coeffs *BW_RESTRICT coeffs, bw_ppm_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_ppm_update_coeffs_ctrl(coeffs);
	if (y != NULL)
		for (int i = 0; i < n_samples; i++) {
			bw_ppm_update_coeffs_audio(coeffs);
			y[i] = bw_ppm_process1(coeffs, state, x[i]);
		}
	else
		for (int i = 0; i < n_samples; i++) {
			bw_ppm_update_coeffs_audio(coeffs);
			bw_ppm_process1(coeffs, state, x[i]);
		}
}

static inline void bw_ppm_set_integration_tau(bw_ppm_coeffs *BW_RESTRICT coeffs, float value) {
	bw_env_follow_set_attack_tau(&coeffs->env_follow_coeffs, value);
}

static inline float bw_ppm_get_y_z1(const bw_ppm_state *BW_RESTRICT state) {
	return state->y_z1;
}

#ifdef __cplusplus
}
#endif

#endif
