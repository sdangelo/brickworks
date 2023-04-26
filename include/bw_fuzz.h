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
 *  version {{{ 0.5.0 }}}
 *  requires {{{
 *    bw_config bw_common bw_gain bw_hp1 bw_hs1 bw_lp1 bw_math bw_mm1
 *    bw_one_pole bw_satur bw_svf
 *  }}}
 *  description {{{
 *    Fuzz effect.
 *
 *    Loosely inspired to the "smiling" fuzz pedal.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_FUZZ_H
#define _BW_FUZZ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_fuzz_coeffs
 *  ```>>> */
typedef struct _bw_fuzz_coeffs bw_fuzz_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_fuzz_state
 *  ```>>> */
typedef struct _bw_fuzz_state bw_fuzz_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_fuzz_init()
 *  ```>>> */
static inline void bw_fuzz_init(bw_fuzz_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_fuzz_set_sample_rate()
 *  ```>>> */
static inline void bw_fuzz_set_sample_rate(bw_fuzz_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_fuzz_reset_coeffs()
 *  ```>>> */
static inline void bw_fuzz_reset_coeffs(bw_fuzz_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_fuzz_reset_state()
 *  ```>>> */
static inline void bw_fuzz_reset_state(const bw_fuzz_coeffs *BW_RESTRICT coeffs, bw_fuzz_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_fuzz_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_fuzz_update_coeffs_ctrl(bw_fuzz_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_fuzz_update_coeffs_audio()
 *  ```>>> */
static inline void bw_fuzz_update_coeffs_audio(bw_fuzz_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_fuzz_process1()
 *  ```>>> */
static inline float bw_fuzz_process1(const bw_fuzz_coeffs *BW_RESTRICT coeffs, bw_fuzz_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_fuzz_process()
 *  ```>>> */
static inline void bw_fuzz_process(bw_fuzz_coeffs *BW_RESTRICT coeffs, bw_fuzz_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_fuzz_set_fuzz()
 *  ```>>> */
static inline void bw_fuzz_set_fuzz(bw_fuzz_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the fuzz (input gain, approximately) to the given `value` in [`0.f`,
 *    `1.f`] in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_fuzz_set_volume()
 *  ```>>> */
static inline void bw_fuzz_set_volume(bw_fuzz_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the volume (output gain) to the given `value` in [`0.f`, `1.f`] in
 *    `coeffs`.
 *
 *    Default value: `1.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_svf.h>
#include <bw_hs1.h>
#include <bw_satur.h>
#include <bw_hp1.h>
#include <bw_gain.h>

struct _bw_fuzz_coeffs {
	// Sub-components
	bw_svf_coeffs	svf_coeffs;
	bw_hs1_coeffs	hs1_coeffs;
	bw_satur_coeffs	satur_coeffs;
	bw_hp1_coeffs	hp1_coeffs;
	bw_gain_coeffs	gain_coeffs;
};

struct _bw_fuzz_state {
	// Sub-components
	bw_svf_state	svf_state;
	bw_hs1_state	hs1_state;
	bw_satur_state	satur_state;
	bw_hp1_state	hp1_state;
};

static inline void bw_fuzz_init(bw_fuzz_coeffs *BW_RESTRICT coeffs) {
	bw_svf_init(&coeffs->svf_coeffs);
	bw_hs1_init(&coeffs->hs1_coeffs);
	bw_satur_init(&coeffs->satur_coeffs);
	bw_hp1_init(&coeffs->hp1_coeffs);
	bw_gain_init(&coeffs->gain_coeffs);
	bw_svf_set_cutoff(&coeffs->svf_coeffs, 5e3f);
	bw_svf_set_Q(&coeffs->svf_coeffs, 0.87f);
	bw_hs1_set_cutoff(&coeffs->hs1_coeffs, 50.f);
	bw_satur_set_bias(&coeffs->satur_coeffs, 0.11f);
	bw_satur_set_gain_compensation(&coeffs->satur_coeffs, 0);
	bw_hp1_set_cutoff(&coeffs->hp1_coeffs, 30.f);
}

static inline void bw_fuzz_set_sample_rate(bw_fuzz_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_svf_set_sample_rate(&coeffs->svf_coeffs, sample_rate);
	bw_hs1_set_sample_rate(&coeffs->hs1_coeffs, sample_rate);
	bw_satur_set_sample_rate(&coeffs->satur_coeffs, sample_rate);
	bw_hp1_set_sample_rate(&coeffs->hp1_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_coeffs, sample_rate);
	bw_svf_reset_coeffs(&coeffs->svf_coeffs);
	bw_satur_reset_coeffs(&coeffs->satur_coeffs);
	bw_hp1_reset_coeffs(&coeffs->hp1_coeffs);
}

static inline void bw_fuzz_reset_coeffs(bw_fuzz_coeffs *BW_RESTRICT coeffs) {
	bw_hs1_reset_coeffs(&coeffs->hs1_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_coeffs);
}

static inline void bw_fuzz_reset_state(const bw_fuzz_coeffs *BW_RESTRICT coeffs, bw_fuzz_state *BW_RESTRICT state) {
	bw_svf_reset_state(&coeffs->svf_coeffs, &state->svf_state, 0.f);
	bw_hs1_reset_state(&coeffs->hs1_coeffs, &state->hs1_state, 0.f);
	bw_satur_reset_state(&coeffs->satur_coeffs, &state->satur_state);
	bw_hp1_reset_state(&coeffs->hp1_coeffs, &state->hp1_state, 0.f);
}

static inline void bw_fuzz_update_coeffs_ctrl(bw_fuzz_coeffs *BW_RESTRICT coeffs) {
	bw_hs1_update_coeffs_ctrl(&coeffs->hs1_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_coeffs);
}

static inline void bw_fuzz_update_coeffs_audio(bw_fuzz_coeffs *BW_RESTRICT coeffs) {
	bw_hs1_update_coeffs_audio(&coeffs->hs1_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_coeffs);
}

static inline float bw_fuzz_process1(const bw_fuzz_coeffs *BW_RESTRICT coeffs, bw_fuzz_state *BW_RESTRICT state, float x) {
	float v_lp, v_hp, v_bp;
	bw_svf_process1(&coeffs->svf_coeffs, &state->svf_state, x, &v_lp, &v_bp, &v_hp);
	float y = bw_hs1_process1(&coeffs->hs1_coeffs, &state->hs1_state, v_lp);
	y = bw_satur_process1(&coeffs->satur_coeffs, &state->satur_state, y);
	y = bw_hp1_process1(&coeffs->hp1_coeffs, &state->hp1_state, y);
	return bw_gain_process1(&coeffs->gain_coeffs, y);
}

static inline void bw_fuzz_process(bw_fuzz_coeffs *BW_RESTRICT coeffs, bw_fuzz_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_fuzz_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_fuzz_update_coeffs_audio(coeffs);
		y[i] = bw_fuzz_process1(coeffs, state, x[i]);
	}
}

static inline void bw_fuzz_set_fuzz(bw_fuzz_coeffs *BW_RESTRICT coeffs, float value) {
	bw_hs1_set_high_gain_dB(&coeffs->hs1_coeffs, 35.f * value);
}

static inline void bw_fuzz_set_volume(bw_fuzz_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_coeffs, value * value * value);
}

#ifdef __cplusplus
}
#endif

#endif
