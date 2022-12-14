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
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

/*!
 *  module_type {{{ dsp }}}
 *  version {{{ 0.3.0 }}}
 *  requires {{{ bw_config bw_common bw_lp1 bw_math bw_one_pole }}}
 *  description {{{
 *    First-order allpass filter (90° shift at cutoff, approaching 180° shift
 *    at high frequencies) with unitary gain.
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

#ifndef _BW_AP1_H
#define _BW_AP1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_ap1_coeffs
 *  ```>>> */
typedef struct _bw_ap1_coeffs bw_ap1_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_ap1_state
 *  ```>>> */
typedef struct _bw_ap1_state bw_ap1_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_ap1_init()
 *  ```>>> */
static inline void bw_ap1_init(bw_ap1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_ap1_set_sample_rate()
 *  ```>>> */
static inline void bw_ap1_set_sample_rate(bw_ap1_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_ap1_reset_coeffs()
 *  ```>>> */
static inline void bw_ap1_reset_coeffs(bw_ap1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_ap1_reset_state()
 *  ```>>> */
static inline void bw_ap1_reset_state(const bw_ap1_coeffs *BW_RESTRICT coeffs, bw_ap1_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_ap1_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_ap1_update_coeffs_ctrl(bw_ap1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ap1_update_coeffs_audio()
 *  ```>>> */
static inline void bw_ap1_update_coeffs_audio(bw_ap1_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ap1_process1()
 *  ```>>> */
static inline float bw_ap1_process1(const bw_ap1_coeffs *BW_RESTRICT coeffs, bw_ap1_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_ap1_process()
 *  ```>>> */
static inline void bw_ap1_process(bw_ap1_coeffs *BW_RESTRICT coeffs, bw_ap1_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_ap1_set_cutoff()
 *  ```>>> */
static inline void bw_ap1_set_cutoff(bw_ap1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_lp1.h>

struct _bw_ap1_coeffs {
	// Sub-components
	bw_lp1_coeffs	lp1_coeffs;
};

struct _bw_ap1_state {
	bw_lp1_state	lp1_state;
};

static inline void bw_ap1_init(bw_ap1_coeffs *BW_RESTRICT coeffs) {
	bw_lp1_init(&coeffs->lp1_coeffs);
}

static inline void bw_ap1_set_sample_rate(bw_ap1_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_lp1_set_sample_rate(&coeffs->lp1_coeffs, sample_rate);
}

static inline void bw_ap1_reset_coeffs(bw_ap1_coeffs *BW_RESTRICT coeffs) {
	bw_lp1_reset_coeffs(&coeffs->lp1_coeffs);
}

static inline void bw_ap1_reset_state(const bw_ap1_coeffs *BW_RESTRICT coeffs, bw_ap1_state *BW_RESTRICT state) {
	bw_lp1_reset_state(&coeffs->lp1_coeffs, &state->lp1_state);
}

static inline void bw_ap1_update_coeffs_ctrl(bw_ap1_coeffs *BW_RESTRICT coeffs) {
	bw_lp1_update_coeffs_ctrl(&coeffs->lp1_coeffs);
}

static inline void bw_ap1_update_coeffs_audio(bw_ap1_coeffs *BW_RESTRICT coeffs) {
	bw_lp1_update_coeffs_audio(&coeffs->lp1_coeffs);
}

static inline float bw_ap1_process1(const bw_ap1_coeffs *BW_RESTRICT coeffs, bw_ap1_state *BW_RESTRICT state, float x) {
	const float lp = bw_lp1_process1(&coeffs->lp1_coeffs, &state->lp1_state, x);
	return x - lp - lp;
}

static inline void bw_ap1_process(bw_ap1_coeffs *BW_RESTRICT coeffs, bw_ap1_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_ap1_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_ap1_update_coeffs_audio(coeffs);
		y[i] = bw_ap1_process1(coeffs, state, x[i]);
	}
}

static inline void bw_ap1_set_cutoff(bw_ap1_coeffs *BW_RESTRICT coeffs, float value) {
	bw_lp1_set_cutoff(&coeffs->lp1_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
