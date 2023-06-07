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
 *  requires {{{ bw_common bw_config bw_math bw_one_pole }}}
 *  description {{{
 *    Antialiased hard clipper with parametric bias and gain
 *    (compensation) and output bias removal.
 *
 *    In other words this implements (approximately)
 *
 *    > y(n) = clip(gain \* x(n) + bias, -1, 1) - clip(bias, -1, 1)
 *
 *    with antialiasing and optionally dividing the output by gain.
 *
 *    As a side effect, antialiasing causes attenuation at higher frequencies
 *    (about 3 dB at 0.5 × Nyquist frequency and rapidly increasing at higher
 *    frequencies).
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

#ifndef _BW_CLIP_H
#define _BW_CLIP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_clip_coeffs
 *  ```>>> */
typedef struct _bw_clip_coeffs bw_clip_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_clip_state
 *  ```>>> */
typedef struct _bw_clip_state bw_clip_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_clip_init()
 *  ```>>> */
static inline void bw_clip_init(bw_clip_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_clip_set_sample_rate()
 *  ```>>> */
static inline void bw_clip_set_sample_rate(bw_clip_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_clip_reset_coeffs()
 *  ```>>> */
static inline void bw_clip_reset_coeffs(bw_clip_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_clip_reset_state()
 *  ```>>> */
static inline void bw_clip_reset_state(const bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_clip_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_clip_update_coeffs_ctrl(bw_clip_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_clip_update_coeffs_audio()
 *  ```>>> */
static inline void bw_clip_update_coeffs_audio(bw_clip_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_clip_process1()
 *  ```>>> */
static inline float bw_clip_process1(const bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state *BW_RESTRICT state, float x);
static inline float bw_clip_process1_comp(const bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state *BW_RESTRICT state, float x);
/*! <<<```
 *    These function process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_clip_process1()` assumes that gain compensation is disabled;
 *     * `bw_clip_process1_comp()` assumes that gain compensation is enabled.
 *
 *    #### bw_clip_process()
 *  ```>>> */
static inline void bw_clip_process(bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_clip_process_multi()
 *  ```>>> */
static inline void bw_clip_process_multi(bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_clip_set_bias()
 *  ```>>> */
static inline void bw_clip_set_bias(bw_clip_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the input bias `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_clip_set_gain()
 *  ```>>> */
static inline void bw_clip_set_gain(bw_clip_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the gain `value` in `coeffs`.
 *
 *    Do not set it to `0.f`, obviously.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_clip_set_gain_compensation()
 *  ```>>> */
static inline void bw_clip_set_gain_compensation(bw_clip_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the output should be divided by gain (`value` non-`0`) or not
 *    (`0`).
 *
 *    Default value: `1` (on).
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_clip_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_bias_state;
	bw_one_pole_state	smooth_gain_state;
	
	// Coefficients
	float			bias_dc;
	float			inv_gain;

	// Parameters
	float			bias;
	float			gain;
	char			gain_compensation;
};

struct _bw_clip_state {
	float	x_z1;
	float	F_z1;
};

static inline void bw_clip_init(bw_clip_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->bias = 0.f;
	coeffs->gain = 1.f;
	coeffs->gain_compensation = 1;
}

static inline void bw_clip_set_sample_rate(bw_clip_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void _bw_clip_do_update_coeffs(bw_clip_coeffs *BW_RESTRICT coeffs, char force) {
	float bias_cur = bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	if (force || coeffs->bias != bias_cur) {
		bias_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_bias_state, coeffs->bias);
		coeffs->bias_dc = bw_clipf(bias_cur, -1.f, 1.f);
	}
	float gain_cur = bw_one_pole_get_y_z1(&coeffs->smooth_gain_state);
	if (force || coeffs->gain != gain_cur) {
		gain_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_gain_state, coeffs->gain);
		coeffs->inv_gain = bw_rcpf_2(gain_cur);
	}
}

static inline void bw_clip_reset_coeffs(bw_clip_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_bias_state, coeffs->bias);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_gain_state, coeffs->gain);
	_bw_clip_do_update_coeffs(coeffs, 1);
}

static inline void bw_clip_reset_state(const bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state *BW_RESTRICT state) {
	state->x_z1 = bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	const float a = bw_absf(state->x_z1);
	state->F_z1 = a > 1.f ? a - 0.5f : 0.5f * a * a;
}

static inline void bw_clip_update_coeffs_ctrl(bw_clip_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline void bw_clip_update_coeffs_audio(bw_clip_coeffs *BW_RESTRICT coeffs) {
	_bw_clip_do_update_coeffs(coeffs, 0);
}

static inline float bw_clip_process1(const bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state *BW_RESTRICT state, float x) {
	x = bw_one_pole_get_y_z1(&coeffs->smooth_gain_state) * x + bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	const float a = bw_absf(x);
	const float F = a > 1.f ? a - 0.5f : 0.5f * a * a;
	const float d = x - state->x_z1;
	const float y = d * d < 1e-6f ? bw_clipf(0.5f * (x + state->x_z1), -1.f, 1.f) : (F - state->F_z1) * bw_rcpf_2(d);
	state->x_z1 = x;
	state->F_z1 = F;
	return y - coeffs->bias_dc;
}

static inline float bw_clip_process1_comp(const bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state *BW_RESTRICT state, float x) {
	float y = bw_clip_process1(coeffs, state, x);
	return coeffs->inv_gain * y;
}

static inline void bw_clip_process(bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	if (coeffs->gain_compensation)
		for (int i = 0; i < n_samples; i++) {
			bw_clip_update_coeffs_audio(coeffs);
			y[i] = bw_clip_process1_comp(coeffs, state, x[i]);
		}
	else
		for (int i = 0; i < n_samples; i++) {
			bw_clip_update_coeffs_audio(coeffs);
			y[i] = bw_clip_process1(coeffs, state, x[i]);
		}
}

static inline void bw_clip_process_multi(bw_clip_coeffs *BW_RESTRICT coeffs, bw_clip_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples) {
	if (coeffs->gain_compensation)
		for (int i = 0; i < n_samples; i++) {
			bw_clip_update_coeffs_audio(coeffs);
			for (int j = 0; j < n_channels; j++)
				y[j][i] = bw_clip_process1_comp(coeffs, state[j], x[j][i]);
		}
	else
		for (int i = 0; i < n_samples; i++) {
			bw_clip_update_coeffs_audio(coeffs);
			for (int j = 0; j < n_channels; j++)
				y[j][i] = bw_clip_process1(coeffs, state[j], x[j][i]);
		}
}

static inline void bw_clip_set_bias(bw_clip_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->bias = value;
}

static inline void bw_clip_set_gain(bw_clip_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->gain = value;
}

static inline void bw_clip_set_gain_compensation(bw_clip_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->gain_compensation = value;
}

#ifdef __cplusplus
}
#endif

#endif
