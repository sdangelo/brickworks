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
 *  requires {{{ bw_config bw_common bw_math bw_one_pole bw_svf }}}
 *  description {{{
 *    Second-order notch filter with unitary gain at DC and asymptotically as
 *    frequency increases, and null gain at cutoff frequency.
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

#ifndef _BW_NOTCH2_H
#define _BW_NOTCH2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_notch2_coeffs
 *  ```>>> */
typedef struct _bw_notch2_coeffs bw_notch2_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_notch2_state
 *  ```>>> */
typedef struct _bw_notch2_state bw_notch2_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_notch2_init()
 *  ```>>> */
static inline void bw_notch2_init(bw_notch2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_notch2_set_sample_rate()
 *  ```>>> */
static inline void bw_notch2_set_sample_rate(bw_notch2_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_notch2_reset_coeffs()
 *  ```>>> */
static inline void bw_notch2_reset_coeffs(bw_notch2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_notch2_reset_state()
 *  ```>>> */
static inline void bw_notch2_reset_state(const bw_notch2_coeffs *BW_RESTRICT coeffs, bw_notch2_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_notch2_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_notch2_update_coeffs_ctrl(bw_notch2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_notch2_update_coeffs_audio()
 *  ```>>> */
static inline void bw_notch2_update_coeffs_audio(bw_notch2_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_notch2_process1()
 *  ```>>> */
static inline float bw_notch2_process1(const bw_notch2_coeffs *BW_RESTRICT coeffs, bw_notch2_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_notch2_process()
 *  ```>>> */
static inline void bw_notch2_process(bw_notch2_coeffs *BW_RESTRICT coeffs, bw_notch2_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_notch2_set_cutoff()
 *  ```>>> */
static inline void bw_notch2_set_cutoff(bw_notch2_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_notch2_set_Q()
 *  ```>>> */
static inline void bw_notch2_set_Q(bw_svf_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the quality factor to the given `value` in `coeffs`.
 *
 *    `value` must be equal or bigger than `0.5f`.
 *
 *    Default value: `0.5f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_svf.h>

struct _bw_notch2_coeffs {
	// Sub-components
	bw_svf_coeffs	svf_coeffs;
};

struct _bw_notch2_state {
	bw_svf_state	svf_state;
};

static inline void bw_notch2_init(bw_notch2_coeffs *BW_RESTRICT coeffs) {
	bw_svf_init(&coeffs->svf_coeffs);
}

static inline void bw_notch2_set_sample_rate(bw_notch2_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_svf_set_sample_rate(&coeffs->svf_coeffs, sample_rate);
}

static inline void bw_notch2_reset_coeffs(bw_notch2_coeffs *BW_RESTRICT coeffs) {
	bw_svf_reset_coeffs(&coeffs->svf_coeffs);
}

static inline void bw_notch2_reset_state(const bw_notch2_coeffs *BW_RESTRICT coeffs, bw_notch2_state *BW_RESTRICT state) {
	bw_svf_reset_state(&coeffs->svf_coeffs, &state->svf_state);
}

static inline void bw_notch2_update_coeffs_ctrl(bw_notch2_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_ctrl(&coeffs->svf_coeffs);
}

static inline void bw_notch2_update_coeffs_audio(bw_notch2_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_audio(&coeffs->svf_coeffs);
}

static inline float bw_notch2_process1(const bw_notch2_coeffs *BW_RESTRICT coeffs, bw_notch2_state *BW_RESTRICT state, float x) {
	float lp, bp, hp;
	bw_svf_process1(&coeffs->svf_coeffs, &state->svf_state, x, &lp, &bp, &hp);
	return lp + hp;
}

static inline void bw_notch2_process(bw_notch2_coeffs *BW_RESTRICT coeffs, bw_notch2_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_notch2_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_notch2_update_coeffs_audio(coeffs);
		y[i] = bw_notch2_process1(coeffs, state, x[i]);
	}
}

static inline void bw_notch2_set_cutoff(bw_notch2_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_cutoff(&coeffs->svf_coeffs, value);
}

static inline void bw_notch2_set_Q(bw_svf_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_Q(&coeffs->svf_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
