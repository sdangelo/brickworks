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
 *  requires {{{ bw_config bw_common bw_slew_lim bw_math }}}
 *  description {{{
 *    Volume control for an arbitrary number of channels.
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
 *    Coefficients.
 *
 *    #### bw_vol_init()
 *  ```>>> */
static inline void bw_vol_init(bw_vol_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes `coeffs`.
 *
 *    #### bw_vol_set_sample_rate()
 *  ```>>> */
static inline void bw_vol_set_sample_rate(bw_vol_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `coeffs`.
 *
 *  >>> */

static inline void bw_vol_reset_coeffs(bw_vol_coeffs *BW_RESTRICT coeffs);

static inline void bw_vol_update_coeffs_ctrl(bw_vol_coeffs *BW_RESTRICT coeffs);
static inline void bw_vol_update_coeffs_audio(bw_vol_coeffs *BW_RESTRICT coeffs);

static inline float bw_vol_process1(const bw_vol_coeffs *BW_RESTRICT coeffs, float x);

/*! ...
 *    #### bw_vol_process()
 *  ```>>> */
static inline void bw_vol_process(bw_vol_coeffs *BW_RESTRICT coeffs, const float *x, float *y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from each of the
 *    `n_channels` input buffers and fills the corresponding `n_samples` samples
 *    in each of the `n_channels` output buffers.
 *
 *    `x` is an array of `n_channels` input buffers and similarly `y` is an
 *    array of `n_channels` output buffers.
 *  >>> */

/*! ...
 *    #### bw_vol_set_volume()
 *  ```>>> */
static inline void bw_vol_set_volume(bw_vol_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the volume parameter to the given `value` (range [`0.f`, `1.f`]) for
 *    the given `instance`.
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
#include <bw_slew_lim.h>

struct _bw_vol_coeffs {
	// Sub-components
	bw_slew_lim_coeffs	smooth_coeffs;
	bw_slew_lim_state	smooth_state;
	
	// Coefficients
	float	k;

	// Parameters
	float	volume;
};

static inline void bw_vol_init(bw_vol_coeffs *BW_RESTRICT coeffs) {
	bw_slew_lim_init(&coeffs->smooth_coeffs);
	bw_slew_lim_set_max_rate(&coeffs->smooth_coeffs, 1.f / 0.2f);
	coeffs->volume = 1.f;
}

static inline void bw_vol_set_sample_rate(bw_vol_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_slew_lim_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_slew_lim_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void bw_vol_reset_coeffs(bw_vol_coeffs *BW_RESTRICT coeffs) {
	bw_slew_lim_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->volume);
}

static inline void bw_vol_update_coeffs_ctrl(bw_vol_coeffs *BW_RESTRICT coeffs) {
}

static inline void bw_vol_update_coeffs_audio(bw_vol_coeffs *BW_RESTRICT coeffs) {
	// tracking parameter changes is more trouble than it's worth
	float v = bw_slew_lim_process1(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->volume);
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
