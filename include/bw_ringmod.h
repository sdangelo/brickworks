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
 *  version {{{ 0.4.0 }}}
 *  requires {{{ bw_common bw_config bw_math bw_one_pole }}}
 *  description {{{
 *    Ring modulator with variable modulation amount.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_RINGMOD_H
#define _BW_RINGMOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_ringmod_coeffs
 *  ```>>> */
typedef struct _bw_ringmod_coeffs bw_ringmod_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_ringmod_init()
 *  ```>>> */
static inline void bw_ringmod_init(bw_ringmod_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_ringmod_set_sample_rate()
 *  ```>>> */
static inline void bw_ringmod_set_sample_rate(bw_ringmod_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_ringmod_reset_coeffs()
 *  ```>>> */
static inline void bw_ringmod_reset_coeffs(bw_ringmod_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_ringmod_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_ringmod_update_coeffs_ctrl(bw_ringmod_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ringmod_update_coeffs_audio()
 *  ```>>> */
static inline void bw_ringmod_update_coeffs_audio(bw_ringmod_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ringmod_process1()
 *  ```>>> */
static inline float bw_ringmod_process1(const bw_ringmod_coeffs *BW_RESTRICT coeffs, float x_mod, float x_car);
/*! <<<```
 *    Processes one modulator input sample `x_mod` and one carrier input sample
 *    `x_car` using `coeffs` and returns the corresponding output sample.
 *
 *    #### bw_ringmod_process()
 *  ```>>> */
static inline void bw_ringmod_process(bw_ringmod_coeffs *BW_RESTRICT coeffs, const float *x_mod, const float *x_car, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the modulator input buffer `x_mod` and
 *    of the carrier input buffer `x_car` and fills the first `n_samples` of the
 *    output buffer `y`, while using and updating `coeffs` (control and audio
 *    rate).
 *
 *    #### bw_ringmod_set_amount()
 *  ```>>> */
static inline void bw_ringmod_set_amount(bw_ringmod_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the modulation amount parameter to the given `value` (`0.f` = no
 *    modulation, `1.f` = full modulation, `-1.f` = full modulation with
 *    inverted polarity) in `coeffs`.
 *
 *    Default value: `1.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_ringmod_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_state;

	// Parameters
	float			mod_amount;
};

static inline void bw_ringmod_init(bw_ringmod_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->mod_amount = 1.f;
}

static inline void bw_ringmod_set_sample_rate(bw_ringmod_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void bw_ringmod_reset_coeffs(bw_ringmod_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->mod_amount);
}

static inline void bw_ringmod_update_coeffs_ctrl(bw_ringmod_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline void bw_ringmod_update_coeffs_audio(bw_ringmod_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->mod_amount);
}

static inline float bw_ringmod_process1(const bw_ringmod_coeffs *BW_RESTRICT coeffs, float x_mod, float x_car) {
	return bw_one_pole_get_y_z1(&coeffs->smooth_state) * (x_car * x_mod - x_mod) + x_mod;
}

static inline void bw_ringmod_process(bw_ringmod_coeffs *BW_RESTRICT coeffs, const float *x_mod, const float *x_car, float *y, int n_samples) {
	for (int i = 0; i < n_samples; i++) {
		bw_ringmod_update_coeffs_audio(coeffs);
		y[i] = bw_ringmod_process1(coeffs, x_mod[i], x_car[i]);
	}
}

static inline void bw_ringmod_set_amount(bw_ringmod_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->mod_amount = value;
}

#ifdef __cplusplus
}
#endif

#endif
