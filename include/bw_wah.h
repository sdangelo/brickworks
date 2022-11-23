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
 *  requires {{{ bw_config bw_common bw_one_pole bw_math bw_svf }}}
 *  description {{{
 *    Wah effect.
 *
 *    This is a simple digital effect, not an emulation of any analog circuit.
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
 *    Coefficients.
 *
 *    ### bw_svf_state
 *  ```>>> */
typedef struct _bw_wah_state bw_wah_state;
/*! <<<```
 *    State.
 *
 *    #### bw_wah_init()
 *  ```>>> */
static inline void bw_wah_init(bw_wah_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes `coeffs`.
 *
 *    #### bw_wah_set_sample_rate()
 *  ```>>> */
static inline void bw_wah_set_sample_rate(bw_wah_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `coeffs`.
 *
 *    #### bw_wah_reset_state()
 *  ```>>> */
static inline void bw_wah_reset_state(const bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to the initial state using the given `coeffs`.
 *  >>> */

static inline void bw_wah_reset_coeffs(bw_wah_coeffs *BW_RESTRICT coeffs);

static inline void bw_wah_update_coeffs_ctrl(bw_wah_coeffs *BW_RESTRICT coeffs);
static inline void bw_wah_update_coeffs_audio(bw_wah_coeffs *BW_RESTRICT coeffs);

static inline float bw_wah_process1(const bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state, float x);

/*! ...
 *    #### bw_wah_process()
 *  ```>>> */
static inline void bw_wah_process(bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_wah_set_wah()
 *  ```>>> */
static inline void bw_wah_set_wah(bw_wah_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the wah pedal position to the given `value` in [`0.f` (low cutoff),
 *    `1.f` (high cutoff)].
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
	bw_svf_reset_state(&coeffs->svf_coeffs, &state->svf_state);
}

static inline void bw_wah_update_coeffs_ctrl(bw_wah_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_ctrl(&coeffs->svf_coeffs);
}

static inline void bw_wah_update_coeffs_audio(bw_wah_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_audio(&coeffs->svf_coeffs);
}

static inline float bw_wah_process1(const bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state, float x) {
	return bw_svf_process1_bp(&coeffs->svf_coeffs, &state->svf_state, x);
}

static inline void bw_wah_process(bw_wah_coeffs *BW_RESTRICT coeffs, bw_wah_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_wah_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_wah_update_coeffs_audio(coeffs);
		y[i] = bw_wah_process1(coeffs, state, x[i]);
	}
}

static inline void bw_wah_set_wah(bw_wah_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_cutoff(&coeffs->svf_coeffs, 400.f + (2e3f - 400.f) * value * value * value);
}

#ifdef __cplusplus
}
#endif

#endif
