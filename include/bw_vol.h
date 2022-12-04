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
 *    Volume control.
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

#ifndef _BW_VOL_H
#define _BW_VOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_vol_coeffs
 *  ```>>> */
typedef struct _bw_vol_coeffs bw_vol_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_vol_init()
 *  ```>>> */
static inline void bw_vol_init(bw_vol_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_vol_set_sample_rate()
 *  ```>>> */
static inline void bw_vol_set_sample_rate(bw_vol_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_vol_reset_coeffs()
 *  ```>>> */
static inline void bw_vol_reset_coeffs(bw_vol_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_vol_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_vol_update_coeffs_ctrl(bw_vol_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_vol_update_coeffs_audio()
 *  ```>>> */
static inline void bw_vol_update_coeffs_audio(bw_vol_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_vol_process1()
 *  ```>>> */
static inline float bw_vol_process1(const bw_vol_coeffs *BW_RESTRICT coeffs, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs` and returns the
 *    corresponding output sample.
 *
 *    #### bw_vol_process()
 *  ```>>> */
static inline void bw_vol_process(bw_vol_coeffs *BW_RESTRICT coeffs, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating
 *    `coeffs` (control and audio rate).
 *
 *    #### bw_vol_set_volume()
 *  ```>>> */
static inline void bw_vol_set_volume(bw_vol_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the volume parameter to the given `value` (range [`0.f`, `1.f`]) in
 *    `coeffs`.
 *
 *    This parameter is not linearly mapped, but the range extremes correspond
 *    to silence (gain = `0.f`) and bypass (gain = `1.f`).
 *
 *    Default value: `1.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_vol_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_state;
	
	// Coefficients
	float	k;

	// Parameters
	float	volume;
};

static inline void bw_vol_init(bw_vol_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->volume = 1.f;
}

static inline void bw_vol_set_sample_rate(bw_vol_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void bw_vol_reset_coeffs(bw_vol_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->volume);
}

static inline void bw_vol_update_coeffs_ctrl(bw_vol_coeffs *BW_RESTRICT coeffs) {
}

static inline void bw_vol_update_coeffs_audio(bw_vol_coeffs *BW_RESTRICT coeffs) {
	// tracking parameter changes is more trouble than it's worth
	float v = bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->volume);
	coeffs->k = v * v * v;
}

static inline float bw_vol_process1(const bw_vol_coeffs *BW_RESTRICT coeffs, float x) {
	return coeffs->k * x;
}

static inline void bw_vol_process(bw_vol_coeffs *BW_RESTRICT coeffs, const float *x, float *y, int n_samples) {
	for (int i = 0; i < n_samples; i++) {
		bw_vol_update_coeffs_audio(coeffs);
		y[i] = bw_vol_process1(coeffs, x[i]);
	}
}

static inline void bw_vol_set_volume(bw_vol_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->volume = value;
}

#ifdef __cplusplus
}
#endif

#endif
