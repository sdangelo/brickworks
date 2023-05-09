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
 *  version {{{ 0.5.0 }}}
 *  requires {{{ bw_config bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    State variable filter (2nd order, 12 dB/oct) model with separated lowpass,
 *    bandpass, and highpass outputs.
 *  }}}
 *  changelog {{{
     <ul>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Fixed prewarping-related stability bug.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_svf_reset_state()</code>.</li>
 *          <li>Fixed unused parameter warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Strenghtened algorithm for modulation.</li>
 *          <li>Added prewarping control parameters (prewarp_at_cutoff and
 *              prewarp_freq).</li>
 *          <li>Added <code>BW_RESTRICT</code> to
 *              <code>bw_svf_process1()</code>.</li>
 *          <li>Fixed typo in <code>bw_svf_set_Q()</code> documentation.</li>
 *        </ul>
 *      </li>
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
 *    #### bw_svf_state
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
static inline void bw_svf_reset_state(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x0`.
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
static inline void bw_svf_process1(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x, float *BW_RESTRICT y_lp, float *BW_RESTRICT y_bp, float *BW_RESTRICT y_hp);
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
 *    Sets the quality factor to the given `value` in `coeffs`.
 *
 *    `value` must be equal or bigger than `0.5f`.
 *
 *    Default value: `0.5f`.
 * 
 *    #### bw_svf_set_prewarp_at_cutoff()
 *  ```>>> */
static inline void bw_svf_set_prewarp_at_cutoff(bw_svf_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether bilinear transform prewarping frequency should match the
 *    cutoff frequency (non-`0`) or not (`0`).
 *
 *    Default value: non-`0` (on).
 *
 *    #### bw_svf_set_prewarp_freq()
 *  ```>>> */
static inline void bw_svf_set_prewarp_freq(bw_svf_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the prewarping frequency `value` (Hz) in `coeffs`.
 *
 *    Only used when the prewarp\_at\_cutoff parameter is off.
 *
 *    Default value: `1e3f`.
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
	bw_one_pole_state	smooth_prewarp_freq_state;
	
	// Coefficients
	float			t_k;

	float			prewarp_k;
	float			t;
	float			kf;
	float			kbl;
	float			k;
	float			hp_hb;
	float			hp_x;

	// Parameters
	float			cutoff;
	float			Q;
	float			prewarp_freq;
};

struct _bw_svf_state {
	float	hp_z1;
	float	lp_z1;
	float	bp_z1;
	float	cutoff_z1;
};

static inline void bw_svf_init(bw_svf_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->cutoff = 1e3f;
	coeffs->Q = 0.5f;
	coeffs->prewarp_freq = 1e3f;
	coeffs->prewarp_k = 1.f;
}

static inline void bw_svf_set_sample_rate(bw_svf_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->t_k = 3.141592653589793f / sample_rate;
}

static inline void _bw_svf_do_update_coeffs(bw_svf_coeffs *BW_RESTRICT coeffs, char force) {
	const float prewarp_freq = coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq);
	float cutoff_cur = bw_one_pole_get_y_z1(&coeffs->smooth_cutoff_state);
	float prewarp_freq_cur = bw_one_pole_get_y_z1(&coeffs->smooth_prewarp_freq_state);
	float Q_cur = bw_one_pole_get_y_z1(&coeffs->smooth_Q_state);
	const char cutoff_changed = force || coeffs->cutoff != cutoff_cur;
	const char prewarp_freq_changed = force || prewarp_freq != prewarp_freq_cur;
	const char Q_changed = force || coeffs->Q != Q_cur;
	if (cutoff_changed || prewarp_freq_changed || Q_changed) {
		if (cutoff_changed || prewarp_freq_changed) {
			if (cutoff_changed)
				cutoff_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
			if (prewarp_freq_changed) {
				prewarp_freq_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, prewarp_freq);
				coeffs->t = bw_tanf_3(coeffs->t_k * prewarp_freq_cur);
				coeffs->kf = coeffs->t * bw_rcpf_2(prewarp_freq_cur);
			}
			coeffs->kbl = coeffs->kf * cutoff_cur;
		}
		if (Q_changed) {
			Q_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_Q_state, coeffs->Q);
			coeffs->k = bw_rcpf_2(Q_cur);
		}
		coeffs->hp_hb = coeffs->k + coeffs->kbl;
		coeffs->hp_x = bw_rcpf_2(1.f + coeffs->kbl * coeffs->hp_hb);
	}
}

static inline void bw_svf_reset_coeffs(bw_svf_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_Q_state, coeffs->Q);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq));
	_bw_svf_do_update_coeffs(coeffs, 1);
}

static inline void bw_svf_reset_state(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x0) {
	state->hp_z1 = 0.f;
	state->lp_z1 = x0;
	state->bp_z1 = 0.f;
	state->cutoff_z1 = coeffs->cutoff;
}

static inline void bw_svf_update_coeffs_ctrl(bw_svf_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline void bw_svf_update_coeffs_audio(bw_svf_coeffs *BW_RESTRICT coeffs) {
	_bw_svf_do_update_coeffs(coeffs, 0);
}

static inline void bw_svf_process1(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x, float *BW_RESTRICT y_lp, float *BW_RESTRICT y_bp, float *BW_RESTRICT y_hp) {
	const float kk = coeffs->kf * state->cutoff_z1;
	const float lp_xz1 = state->lp_z1 - kk * state->bp_z1;
	const float bp_xz1 = state->bp_z1 - kk * state->hp_z1;
	*y_hp = coeffs->hp_x * (x + coeffs->hp_hb * bp_xz1 - lp_xz1);
	*y_bp = bp_xz1 - coeffs->kbl * *y_hp;
	*y_lp = lp_xz1 - coeffs->kbl * *y_bp;
	state->hp_z1 = *y_hp;
	state->lp_z1 = *y_lp;
	state->bp_z1 = *y_bp;
	state->cutoff_z1 = bw_one_pole_get_y_z1(&coeffs->smooth_cutoff_state);
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

static inline void bw_svf_set_prewarp_at_cutoff(bw_svf_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->prewarp_k = value ? 1.f : 0.f;
}

static inline void bw_svf_set_prewarp_freq(bw_svf_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->prewarp_freq = value;
}

#ifdef __cplusplus
}
#endif

#endif
