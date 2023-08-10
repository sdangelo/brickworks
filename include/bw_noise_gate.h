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
 *  requires {{{ bw_common bw_env_follow bw_math bw_one_pole }}}
 *  description {{{
 *    Noise gate with independent sidechain input.
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
 *          <li>Added <code>bw_noise_gate_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_NOISE_GATE_H
#define _BW_NOISE_GATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_noise_gate_coeffs
 *  ```>>> */
typedef struct _bw_noise_gate_coeffs bw_noise_gate_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_noise_gate_state
 *  ```>>> */
typedef struct _bw_noise_gate_state bw_noise_gate_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_noise_gate_init()
 *  ```>>> */
static inline void bw_noise_gate_init(bw_noise_gate_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_noise_gate_set_sample_rate()
 *  ```>>> */
static inline void bw_noise_gate_set_sample_rate(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_noise_gate_reset_coeffs()
 *  ```>>> */
static inline void bw_noise_gate_reset_coeffs(bw_noise_gate_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_noise_gate_reset_state()
 *  ```>>> */
static inline void bw_noise_gate_reset_state(const bw_noise_gate_coeffs *BW_RESTRICT coeffs, bw_noise_gate_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_noise_gate_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_noise_gate_update_coeffs_ctrl(bw_noise_gate_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_noise_gate_update_coeffs_audio()
 *  ```>>> */
static inline void bw_noise_gate_update_coeffs_audio(bw_noise_gate_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_noise_gate_process1()
 *  ```>>> */
static inline float bw_noise_gate_process1(const bw_noise_gate_coeffs *BW_RESTRICT coeffs, bw_noise_gate_state *BW_RESTRICT state, float x, float x_sc);
/*! <<<```
 *    Processes one input sample `x` and the corresponding sidechain input
 *    sample `x_sc` using `coeffs`, while using and updating `state`. Returns
 *    the corresponding output sample.
 *
 *    #### bw_noise_gate_process()
 *  ```>>> */
static inline void bw_noise_gate_process(bw_noise_gate_coeffs *BW_RESTRICT coeffs, bw_noise_gate_state *BW_RESTRICT state, const float *x, const float *x_sc, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and the first
 *    `n_samples` of the sidechain input buffer `x_sc`, and fills the first
 *    `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_noise_gate_process_multi()
 *  ```>>> */
static inline void bw_noise_gate_process_multi(bw_noise_gate_coeffs *BW_RESTRICT coeffs, bw_noise_gate_state **BW_RESTRICT state, const float **x, const float **x_sc, float **y, int n_channels, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    the first `n_samples` of the `n_channels` sidechain input buffers `x_sc`,
 *    and fills the first `n_samples` of the `n_channels` output buffers `y`,
 *    while using and updating both the common `coeffs` and each of the
 *    `n_channels` `state`s (control and audio rate).
 *
 *    #### bw_noise_gate_set_thresh_lin()
 *  ```>>> */
static inline void bw_noise_gate_set_thresh_lin(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the threshold `value` (linear) in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_noise_gate_set_thresh_dBFS()
 *  ```>>> */
static inline void bw_noise_gate_set_thresh_dBFS(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the threshold `value` (dBFS) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_noise_gate_set_ratio()
 *  ```>>> */
static inline void bw_noise_gate_set_ratio(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the copmpression ratio `value` in `coeffs`.
 *
 *    `value` is actually the slope of the gain curve below the threshold, hence
 *    `1.f` means no gating and `INIFINITY` leads to a hard gate.
 *
 *    Valid range: [`1.f`, `INFINITY`].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_noise_gate_set_attack_tau()
 *  ```>>> */
static inline void bw_noise_gate_set_attack_tau(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the attack time constant `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_noise_gate_set_release_tau()
 *  ```>>> */
static inline void bw_noise_gate_set_release_tau(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the release time constant `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_env_follow.h>
#include <bw_one_pole.h>

struct _bw_noise_gate_coeffs {
	// Sub-noise_gateonents
	bw_env_follow_coeffs	env_follow_coeffs;
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_thresh_state;
	bw_one_pole_state	smooth_ratio_state;
	
	// Coefficients
	float			kc;

	// Parameters
	float			thresh;
	float			ratio;
};

struct _bw_noise_gate_state {
	bw_env_follow_state	env_follow_state;
};

static inline void bw_noise_gate_init(bw_noise_gate_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_init(&coeffs->env_follow_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->thresh = 1.f;
	coeffs->ratio = 1.f;
}

static inline void bw_noise_gate_set_sample_rate(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_env_follow_set_sample_rate(&coeffs->env_follow_coeffs, sample_rate);
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void bw_noise_gate_reset_coeffs(bw_noise_gate_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_reset_coeffs(&coeffs->env_follow_coeffs);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_thresh_state, coeffs->thresh);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_ratio_state, coeffs->ratio > 1e12f ? 0.f : bw_rcpf(coeffs->ratio));
}

static inline void bw_noise_gate_reset_state(const bw_noise_gate_coeffs *BW_RESTRICT coeffs, bw_noise_gate_state *BW_RESTRICT state) {
	bw_env_follow_reset_state(&coeffs->env_follow_coeffs, &state->env_follow_state);
}

static inline void bw_noise_gate_update_coeffs_ctrl(bw_noise_gate_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_ctrl(&coeffs->env_follow_coeffs);
}

static inline void bw_noise_gate_update_coeffs_audio(bw_noise_gate_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_audio(&coeffs->env_follow_coeffs);
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_thresh_state, coeffs->thresh);
	const float rev_ratio = bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_ratio_state, coeffs->ratio > 1e12f ? 0.f : bw_rcpf(coeffs->ratio));
	coeffs->kc = rev_ratio < 1e-12f ? -INFINITY : 1.f - bw_rcpf(rev_ratio);
}

static inline float bw_noise_gate_process1(const bw_noise_gate_coeffs *BW_RESTRICT coeffs, bw_noise_gate_state *BW_RESTRICT state, float x, float x_sc) {
	const float env = bw_env_follow_process1(&coeffs->env_follow_coeffs, &state->env_follow_state, x_sc);
	const float thresh = bw_one_pole_get_y_z1(&coeffs->smooth_thresh_state);
	return env < thresh ? bw_pow2f_3(coeffs->kc * bw_log2f_3(thresh * bw_rcpf(env))) * x : x;
}

static inline void bw_noise_gate_process(bw_noise_gate_coeffs *BW_RESTRICT coeffs, bw_noise_gate_state *BW_RESTRICT state, const float *x, const float *x_sc, float *y, int n_samples) {
	bw_noise_gate_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_noise_gate_update_coeffs_audio(coeffs);
		y[i] = bw_noise_gate_process1(coeffs, state, x[i], x_sc[i]);
	}
}

static inline void bw_noise_gate_process_multi(bw_noise_gate_coeffs *BW_RESTRICT coeffs, bw_noise_gate_state **BW_RESTRICT state, const float **x, const float **x_sc, float **y, int n_channels, int n_samples) {
	bw_noise_gate_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_noise_gate_update_coeffs_audio(coeffs);
		for (int j = 0; j < n_channels; j++)
			y[j][i] = bw_noise_gate_process1(coeffs, state[j], x[j][i], x_sc[j][i]);
	}
}

static inline void bw_noise_gate_set_thresh_lin(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->thresh = value;
}

static inline void bw_noise_gate_set_thresh_dBFS(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->thresh = bw_dB2linf_3(value);
}

static inline void bw_noise_gate_set_ratio(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->ratio = value;
}

static inline void bw_noise_gate_set_attack_tau(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value) {
	bw_env_follow_set_attack_tau(&coeffs->env_follow_coeffs, value);
}

static inline void bw_noise_gate_set_release_tau(bw_noise_gate_coeffs *BW_RESTRICT coeffs, float value) {
	bw_env_follow_set_release_tau(&coeffs->env_follow_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
