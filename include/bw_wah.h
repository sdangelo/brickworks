/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
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
 *  requires {{{ bw_common bw_math bw_one_pole bw_svf }}}
 *  description {{{
 *    Wah effect.
 *
 *    This is a simple digital effect, not an emulation of any analog circuit.
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
 *          <li>Added <code>bw_wah_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Now specifying <code>0.f</code> as initial input value for
 *              <code>bw_svf_reset_state()</code>.</li>
 *          <li>Fixed documentation for <code>bw_wah_state</code>,
 *              <code>bw_wah_reset_state()</code>, and
 *              <code>bw_wah_process()</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Use bandpass SVF output rather than lowpass.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_WAH_H
#define _BW_WAH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_wah_coeffs
 *  ```>>> */
typedef struct _bw_wah_coeffs bw_wah_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_wah_state
 *  ```>>> */
typedef struct _bw_wah_state bw_wah_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_wah_init()
 *  ```>>> */
static inline void bw_wah_init(bw_wah_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_wah_set_sample_rate()
 *  ```>>> */
static inline void bw_wah_set_sample_rate(bw_wah_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_wah_reset_coeffs()
 *  ```>>> */
static inline void bw_wah_reset_coeffs(bw_wah_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_wah_reset_state()
 *  ```>>> */
static inline void bw_wah_reset_state(const bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_wah_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_wah_update_coeffs_ctrl(bw_wah_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_wah_update_coeffs_audio()
 *  ```>>> */
static inline void bw_wah_update_coeffs_audio(bw_wah_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_wah_process1()
 *  ```>>> */
static inline float bw_wah_process1(const bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_wah_process()
 *  ```>>> */
static inline void bw_wah_process(bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_wah_process_multi()
 *  ```>>> */
static inline void bw_wah_process_multi(bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_wah_set_wah()
 *  ```>>> */
static inline void bw_wah_set_wah(bw_wah_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the wah pedal position to the given `value` in [`0.f` (low cutoff),
 *    `1.f` (high cutoff)] in `coeffs`.
 *
 *    Default value: `0.5f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_svf.h>

struct _bw_wah_coeffs {
	// Sub-components
	bw_svf_coeffs	svf_coeffs;
};

struct _bw_wah_state {
	// Sub-components
	bw_svf_state	svf_state;
};

static inline void bw_wah_init(bw_wah_coeffs *BW_RESTRICT coeffs) {
	bw_svf_init(&coeffs->svf_coeffs);
	bw_wah_set_wah(coeffs, 0.5f);
	bw_svf_set_Q(&coeffs->svf_coeffs, 9.f);
}

static inline void bw_wah_set_sample_rate(bw_wah_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_svf_set_sample_rate(&coeffs->svf_coeffs, sample_rate);
}

static inline void bw_wah_reset_coeffs(bw_wah_coeffs *BW_RESTRICT coeffs) {
	bw_svf_reset_coeffs(&coeffs->svf_coeffs);
}

static inline void bw_wah_reset_state(const bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state) {
	bw_svf_reset_state(&coeffs->svf_coeffs, &state->svf_state, 0.f);
}

static inline void bw_wah_update_coeffs_ctrl(bw_wah_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_ctrl(&coeffs->svf_coeffs);
}

static inline void bw_wah_update_coeffs_audio(bw_wah_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_audio(&coeffs->svf_coeffs);
}

static inline float bw_wah_process1(const bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state, float x) {
	float v_lp, v_hp, v_bp;
	bw_svf_process1(&coeffs->svf_coeffs, &state->svf_state, x, &v_lp, &v_bp, &v_hp);
	return v_bp;
}

static inline void bw_wah_process(bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_wah_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_wah_update_coeffs_audio(coeffs);
		y[i] = bw_wah_process1(coeffs, state, x[i]);
	}
}

static inline void bw_wah_process_multi(bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples) {
	bw_wah_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_wah_update_coeffs_audio(coeffs);
		for (int j = 0; j < n_channels; j++)
			y[j][i] = bw_wah_process1(coeffs, state[j], x[j][i]);
	}
}

static inline void bw_wah_set_wah(bw_wah_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_cutoff(&coeffs->svf_coeffs, 400.f + (2e3f - 400.f) * value * value * value);
}

#ifdef __cplusplus
}
#endif

#endif
