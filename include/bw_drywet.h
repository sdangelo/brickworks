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
 *  version {{{ 1.0.0 }}}
 *  requires {{{ bw_common bw_gain bw_math bw_one_pole }}}
 *  description {{{
 *    Dry/wet mixer.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_DRYWET_H
#define _BW_DRYWET_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_drywet_coeffs
 *  ```>>> */
typedef struct _bw_drywet_coeffs bw_drywet_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_drywet_init()
 *  ```>>> */
static inline void bw_drywet_init(bw_drywet_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_drywet_set_sample_rate()
 *  ```>>> */
static inline void bw_drywet_set_sample_rate(bw_drywet_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_drywet_reset_coeffs()
 *  ```>>> */
static inline void bw_drywet_reset_coeffs(bw_drywet_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_drywet_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_drywet_update_coeffs_ctrl(bw_drywet_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_drywet_update_coeffs_audio()
 *  ```>>> */
static inline void bw_drywet_update_coeffs_audio(bw_drywet_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_drywet_process1()
 *  ```>>> */
static inline float bw_drywet_process1(const bw_drywet_coeffs *BW_RESTRICT coeffs, float x_dry, float x_wet);
/*! <<<```
 *    Processes one dry input sample `x_dry` and one wet input sample `x_wet`
 *    using `coeffs` and returns the corresponding output sample.
 *
 *    #### bw_drywet_process()
 *  ```>>> */
static inline void bw_drywet_process(bw_drywet_coeffs *BW_RESTRICT coeffs, const float *x_dry, const float *x_wet, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the dry input buffer `x_dry` and of the
 *    wet input buffer `x_wet` and fills the first `n_samples` of the output
 *    buffer `y`, while using and updating `coeffs` (control and audio rate).
 *
 *    #### bw_drywet_process_multi()
 *  ```>>> */
static inline void bw_drywet_process_multi(bw_drywet_coeffs *BW_RESTRICT coeffs, const float **x_dry, const float **x_wet, float **y, int n_channels, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` dry input buffers
 *    `x_dry` and of the `n_channels` wet input buffers `x_wet`, and fills the
 *    first `n_samples` of the `n_channels` output buffers `y`, while using and
 *    updating the common `coeffs` (control and audio rate).
 *
 *    #### bw_drywet_set_wet()
 *  ```>>> */
static inline void bw_drywet_set_wet(bw_drywet_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the wet gain parameter to the given `value` (linear gain) in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_drywet_set_smooth_tau()
 *  ```>>> */
static inline void bw_drywet_set_smooth_tau(bw_drywet_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the smoothing time constant `value` (s) in `coeffs`.
 *
 *    Default value: `0.05f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_gain.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _bw_drywet_coeffs {
	// Sub-components
	bw_gain_coeffs	gain_coeffs;
};

static inline void bw_drywet_init(bw_drywet_coeffs *BW_RESTRICT coeffs) {
	bw_gain_init(&coeffs->gain_coeffs);
}

static inline void bw_drywet_set_sample_rate(bw_drywet_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_gain_set_sample_rate(&coeffs->gain_coeffs, sample_rate);
}

static inline void bw_drywet_reset_coeffs(bw_drywet_coeffs *BW_RESTRICT coeffs) {
	bw_gain_reset_coeffs(&coeffs->gain_coeffs);
}

static inline void bw_drywet_update_coeffs_ctrl(bw_drywet_coeffs *BW_RESTRICT coeffs) {
	bw_gain_update_coeffs_ctrl(&coeffs->gain_coeffs);
}

static inline void bw_drywet_update_coeffs_audio(bw_drywet_coeffs *BW_RESTRICT coeffs) {
	bw_gain_update_coeffs_audio(&coeffs->gain_coeffs);
}

static inline float bw_drywet_process1(const bw_drywet_coeffs *BW_RESTRICT coeffs, float x_dry, float x_wet) {
	return bw_gain_get_gain(&coeffs->gain_coeffs) * (x_wet - x_dry) + x_dry;
}

static inline void bw_drywet_process(bw_drywet_coeffs *BW_RESTRICT coeffs, const float *x_dry, const float *x_wet, float *y, int n_samples) {
	bw_drywet_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_drywet_update_coeffs_audio(coeffs);
		y[i] = bw_drywet_process1(coeffs, x_dry[i], x_wet[i]);
	}
}

static inline void bw_drywet_process_multi(bw_drywet_coeffs *BW_RESTRICT coeffs, const float **x_dry, const float **x_wet, float **y, int n_channels, int n_samples) {
	bw_drywet_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_drywet_update_coeffs_audio(coeffs);
		for (int j = 0; j < n_channels; j++)
			y[j][i] = bw_drywet_process1(coeffs, x_dry[j][i], x_wet[j][i]);
	}
}

static inline void bw_drywet_set_wet(bw_drywet_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_coeffs, value);
}

static inline void bw_drywet_set_smooth_tau(bw_drywet_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_smooth_tau(&coeffs->gain_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
