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
 *    tanh()-based saturation.
 *
 *    ...
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_SATUR_H
#define _BW_SATUR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_satur_coeffs
 *  ```>>> */
typedef struct _bw_satur_coeffs bw_satur_coeffs;
/*! <<<```
 *    Coefficients.
 *
 *    ### bw_satur_state
 *  ```>>> */
typedef struct _bw_satur_state bw_satur_state;
/*! <<<```
 *    State.
 *
 *    #### bw_satur_init()
 *  ```>>> */
static inline void bw_satur_init(bw_satur_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes `coeffs`.
 *
 *    #### bw_satur_set_sample_rate()
 *  ```>>> */
static inline void bw_satur_set_sample_rate(bw_satur_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `coeffs`.
 *
 *    #### bw_satur_reset_state()
 *  ```>>> */
static inline void bw_satur_reset_state(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to the initial state using the given `coeffs`.
 *  >>> */

static inline void bw_satur_reset_coeffs(bw_satur_coeffs *BW_RESTRICT coeffs);

static inline void bw_satur_update_coeffs_ctrl(bw_satur_coeffs *BW_RESTRICT coeffs);
static inline void bw_satur_update_coeffs_audio(bw_satur_coeffs *BW_RESTRICT coeffs);

static inline float bw_satur_process1(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, float x);

static inline float bw_satur_process1_comp(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, float x);

/*! ...
 *    #### bw_satur_process()
 *  ```>>> */
static inline void bw_satur_process(bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_satur_set_bias()
 *  ```>>> */
static inline void bw_satur_set_bias(bw_satur_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    ...
 *
 *    Default value: `0.f`.
 *  >>> */

/*! ...
 *    #### bw_satur_set_bias()
 *  ```>>> */
static inline void bw_satur_set_gain(bw_satur_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    ...
 *
 *    Default value: `1.f`.
 *  >>> */
 
/*! ...
 *    #### bw_satur_set_bias()
 *  ```>>> */
static inline void bw_satur_set_gain_compensation(bw_satur_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    ...
 *
 *    Default value: `1`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_satur_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_bias_state;
	bw_one_pole_state	smooth_gain_state;
	
	// Coefficients
	float			bias_dc;
	float			inv_gain;

	// Parameters
	float			bias;
	float			gain;
	char			gain_compensation;
};

struct _bw_satur_state {
	float	x_z1;
	float	F_z1;
};

static inline float _bw_satur_tanhf_3(float x) {
	const float xm = bw_clipf(x, -2.115287308554551f, 2.115287308554551f);
	const float axm = bw_absf(xm);
	return xm * axm * (0.01218073260037716f * axm - 0.2750231331124371f) + xm;
}

static inline void bw_satur_init(bw_satur_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->bias = 0.f;
	coeffs->gain = 1.f;
	coeffs->gain_compensation = 1;
}

static inline void bw_satur_set_sample_rate(bw_satur_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void _bw_satur_do_update_coeffs(bw_satur_coeffs *BW_RESTRICT coeffs, char force) {
	float bias_cur = bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	if (force || coeffs->bias != bias_cur) {
		bias_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_bias_state, coeffs->bias);
		coeffs->bias_dc = _bw_satur_tanhf_3(bias_cur);
	}
	float gain_cur = bw_one_pole_get_y_z1(&coeffs->smooth_gain_state);
	if (force || coeffs->gain != gain_cur) {
		gain_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_gain_state, coeffs->gain);
		coeffs->inv_gain = bw_rcpf_2(gain_cur);
	}
}

static inline void bw_satur_reset_coeffs(bw_satur_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_bias_state, coeffs->bias);
	_bw_satur_do_update_coeffs(coeffs, 1);
}

static inline void bw_satur_reset_state(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state) {
	state->F_z1 = 0.f;
	state->x_z1 = 0.f;
}

static inline void bw_satur_update_coeffs_ctrl(bw_satur_coeffs *BW_RESTRICT coeffs) {
}

static inline void bw_satur_update_coeffs_audio(bw_satur_coeffs *BW_RESTRICT coeffs) {
	_bw_satur_do_update_coeffs(coeffs, 0);
}

static inline float bw_satur_process1(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, float x) {
	x = bw_one_pole_get_y_z1(&coeffs->smooth_gain_state) * x + bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	const float ax = bw_absf(x);
	const float F = ax >= 2.115287308554551f ? ax - 0.6847736211329452f : ax * ax * ((0.00304518315009429f * ax - 0.09167437770414569f) * ax + 0.5f);
	const float d = x - state->x_z1;
	const float y = d * d < 1e-6f ? _bw_satur_tanhf_3(0.5f * (x + state->x_z1)) : (F - state->F_z1) * bw_rcpf_2(d);
	state->x_z1 = x;
	state->F_z1 = F;
	return y - coeffs->bias_dc;
}

static inline float bw_satur_process1_comp(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, float x) {
	float y = bw_satur_process1(coeffs, state, x);
	return coeffs->inv_gain * y;
}

static inline void bw_satur_process(bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	if (coeffs->gain_compensation)
		for (int i = 0; i < n_samples; i++) {
			bw_satur_update_coeffs_audio(coeffs);
			y[i] = bw_satur_process1_comp(coeffs, state, x[i]);
		}
	else
		for (int i = 0; i < n_samples; i++) {
			bw_satur_update_coeffs_audio(coeffs);
			y[i] = bw_satur_process1(coeffs, state, x[i]);
		}
}

static inline void bw_satur_set_bias(bw_satur_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->bias = value;
}

static inline void bw_satur_set_gain(bw_satur_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->gain = value;
}

static inline void bw_satur_set_gain_compensation(bw_satur_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->gain_compensation = value;
}

#ifdef __cplusplus
}
#endif

#endif
