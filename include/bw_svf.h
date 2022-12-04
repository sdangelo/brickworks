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
 *    State variable filter (2nd order, 12 dB/oct) model with separated lowpass,
 *    bandpass, and highpass outputs.
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

#ifndef _BW_SVF_H
#define _BW_SVF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_svf_coeffs
 *  ```>>> */
typedef struct _bw_svf_coeffs bw_svf_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    ### bw_svf_state
 *  ```>>> */
typedef struct _bw_svf_state bw_svf_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_svf_init()
 *  ```>>> */
static inline void bw_svf_init(bw_svf_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_svf_set_sample_rate()
 *  ```>>> */
static inline void bw_svf_set_sample_rate(bw_svf_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_svf_reset_coeffs()
 *  ```>>> */
static inline void bw_svf_reset_coeffs(bw_svf_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_svf_reset_state()
 *  ```>>> */
static inline void bw_svf_reset_state(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_svf_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_svf_update_coeffs_ctrl(bw_svf_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_svf_update_coeffs_audio()
 *  ```>>> */
static inline void bw_svf_update_coeffs_audio(bw_svf_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_svf_process1()
 *  ```>>> */
static inline void bw_svf_process1(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x, float *y_lp, float *y_bp, float *y_hp);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. The lowpass, bandpass, and highpass output samples are put into
 *    `y_lp`, `y_bp`, and `y_hp` respectively.
 *
 *    #### bw_svf_process()
 *  ```>>> */
static inline void bw_svf_process(bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, const float *x, float *y_lp, float *y_bp, float *y_hp, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffers `y_lp` (lowpass), `y_bp`
 *    (bandpass), and `y_hp` (highpass), if they are not `NULL`, while using and
 *    updating both `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_svf_set_cutoff()
 *  ```>>> */
static inline void bw_svf_set_cutoff(bw_svf_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency to the given `value` (Hz) in `coeffs`.
 *
 *    `value` must be positive and smaller than the Nyquist frequency (half the
 *    sample rate).
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_svf_set_Q()
 *  ```>>> */
static inline void bw_svf_set_Q(bw_svf_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the quality factor to the given `value` (Hz) in `coeffs`.
 *
 *    `value` must be equal or bigger than `0.5f`.
 *
 *    Default value: `0.5f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_svf_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_cutoff_state;
	bw_one_pole_state	smooth_Q_state;
	
	// Coefficients
	float			t_k;

	float			t;
	float			k;
	float			hp_hp_z1;
	float			hp_bp_z1;
	float			hp_x;

	// Parameters
	float			cutoff;
	float			Q;
};

struct _bw_svf_state {
	float	hp_z1;
	float	lp_z1;
	float	bp_z1;
};

static inline void bw_svf_init(bw_svf_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->cutoff = 1e3f;
	coeffs->Q = 0.5f;
}

static inline void bw_svf_set_sample_rate(bw_svf_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->t_k = 3.141592653589793f / sample_rate;
}

static inline void _bw_svf_do_update_coeffs(bw_svf_coeffs *BW_RESTRICT coeffs, char force) {
	float cutoff_cur = bw_one_pole_get_y_z1(&coeffs->smooth_cutoff_state);
	float Q_cur = bw_one_pole_get_y_z1(&coeffs->smooth_Q_state);
	const char cutoff_changed = force || coeffs->cutoff != cutoff_cur;
	const char Q_changed = force || coeffs->Q != Q_cur;
	if (cutoff_changed || Q_changed) {
		if (cutoff_changed) {
			cutoff_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
			coeffs->t = bw_tanf_3(coeffs->t_k * cutoff_cur);
		}
		if (Q_changed) {
			Q_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_Q_state, coeffs->Q);
			coeffs->k = bw_rcpf_2(Q_cur);
		}
		const float kpt = coeffs->k + coeffs->t;
		coeffs->hp_hp_z1 = coeffs->t * kpt;
		coeffs->hp_bp_z1 = coeffs->t + kpt;
		coeffs->hp_x = bw_rcpf_2(1.f + coeffs->hp_hp_z1);
	}
}

static inline void bw_svf_reset_coeffs(bw_svf_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_Q_state, coeffs->Q);
	_bw_svf_do_update_coeffs(coeffs, 1);
}

static inline void bw_svf_reset_state(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state) {
	state->hp_z1 = 0.f;
	state->lp_z1 = 0.f;
	state->bp_z1 = 0.f;
}

static inline void bw_svf_update_coeffs_ctrl(bw_svf_coeffs *BW_RESTRICT coeffs) {
}

static inline void bw_svf_update_coeffs_audio(bw_svf_coeffs *BW_RESTRICT coeffs) {
	_bw_svf_do_update_coeffs(coeffs, 0);
}

static inline void bw_svf_process1(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x, float *y_lp, float *y_bp, float *y_hp) {
	*y_hp = coeffs->hp_x * (x - state->lp_z1 + coeffs->hp_bp_z1 * state->bp_z1 - coeffs->hp_hp_z1 * state->hp_z1);
	*y_bp = state->bp_z1 - coeffs->t * (*y_hp + state->hp_z1);
	*y_lp = state->lp_z1 - coeffs->t * (*y_bp + state->bp_z1);

	state->hp_z1 = *y_hp;
	state->lp_z1 = *y_lp;
	state->bp_z1 = *y_bp;
}

static inline void bw_svf_process(bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, const float *x, float *y_lp, float *y_bp, float *y_hp, int n_samples) {
	if (y_lp != NULL) {
		if (y_bp != NULL) {
			if (y_hp != NULL) {
				for (int i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					bw_svf_process1(coeffs, state, x[i], y_lp + i, y_bp + i, y_hp + i);
				}
			} else {
				for (int i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, y_bp + i, y_hp + i);
				}
			}
		} else {
			if (y_hp != NULL) {
				for (int i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_bp;
					bw_svf_process1(coeffs, state, x[i], y_lp + i, &v_bp, y_hp + i);
				}
			} else {
				for (int i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_bp, v_hp;
					bw_svf_process1(coeffs, state, x[i], y_lp + i, &v_bp, &v_hp);
				}
			}
		}
	} else {
		if (y_bp != NULL) {
			if (y_hp != NULL) {
				for (int i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, y_bp + i, y_hp + i);
				}
			} else {
				for (int i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp, v_hp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, y_bp + i, &v_hp);
				}
			}
		} else {
			if (y_hp != NULL) {
				for (int i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp, v_bp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, &v_bp, y_hp + i);
				}
			} else {
				for (int i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp, v_bp, v_hp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, &v_bp, &v_hp);
				}
			}
		}
	}
}

static inline void bw_svf_set_cutoff(bw_svf_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->cutoff = value;
}

static inline void bw_svf_set_Q(bw_svf_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->Q = value;
}

#ifdef __cplusplus
}
#endif

#endif
