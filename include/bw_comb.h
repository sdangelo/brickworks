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
 *  version {{{ 0.4.0 }}}
 *  requires {{{
 *    bw_buf bw_config bw_common bw_delay bw_gain bw_math bw_one_pole
 *  }}}
 *  description {{{
 *    Comb filter / delay effect with feedforward and feedback paths.
 *
 *    Essentially an implementation of the structure described in
 *
 *    J. Dattorro, "Effect Design, Part 2: Delay-Line Modulation and Chorus",
 *    J. Audio Eng. Soc., vol. 45, no. 10, pp. 764-788, October 1997.
 *
 *    Unlike in the original paper, the feedback signal is not subtracted but
 *    rather added to the input. 
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.4.0</strong>:
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_COMB_H
#define _BW_COMB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_comb_coeffs
 *  ```>>> */
typedef struct _bw_comb_coeffs bw_comb_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    ### bw_comb_state
 *  ```>>> */
typedef struct _bw_comb_state bw_comb_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_comb_init()
 *  ```>>> */
static inline void bw_comb_init(bw_comb_coeffs *BW_RESTRICT coeffs, float max_delay);
/*! <<<```
 *    Initializes input parameter values in `coeffs` using `max_delay` (s) as
 *    the maximum delay time.
 *
 *    #### bw_comb_set_sample_rate()
 *  ```>>> */
static inline void bw_comb_set_sample_rate(bw_comb_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    ### bw_delay_mem_req()
 *  ```>>> */
static inline BW_SIZE_T bw_delay_mem_req(bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the size, in bytes, of contiguous memory to be supplied to
 *    `bw_delay_mem_set()` using `coeffs`.
 *
 *    ### bw_delay_mem_set()
 *  ```>>> */
static inline void bw_delay_mem_set(bw_delay_state *BW_RESTRICT state, void *mem);
/*! <<<```
 *    Associates the contiguous memory block `mem` to the given `state`.
 *
 *    #### bw_comb_reset_coeffs()
 *  ```>>> */
static inline void bw_comb_reset_coeffs(bw_comb_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_comb_reset_state()
 *  ```>>> */
static inline void bw_comb_reset_state(const bw_comb_coeffs *BW_RESTRICT coeffs, bw_comb_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_comb_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_comb_update_coeffs_ctrl(bw_comb_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_comb_update_coeffs_audio()
 *  ```>>> */
static inline void bw_comb_update_coeffs_audio(bw_comb_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_comb_process1()
 *  ```>>> */
static inline float bw_comb_process1(const bw_comb_coeffs *BW_RESTRICT coeffs, bw_comb_state *BW_RESTRICT state, float x, float x_mod);
/*! <<<```
 *    Processes one input sample `x` using `coeffs` and the feedforward delay
 *    time modulation term `x_mod` (s), while using and updating `state`.
 *    Returns the corresponding output sample.
 *
 *    #### bw_comb_process()
 *  ```>>> */
static inline void bw_comb_process(bw_comb_coeffs *BW_RESTRICT coeffs, bw_comb_state *BW_RESTRICT state, const float *x, const float *x_mod, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    `x_mod` can either be `NULL`, in which case the feedforward delay time is
 *    not modulated, or it points to a buffer whose first `n_samples` contain
 *    values (s) used for modulating the feedforward delay time.
 *
 *    #### bw_comb_set_delay_ff()
 *  ```>>> */
static inline void bw_comb_set_delay_ff(bw_comb_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the feedforward delay time `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comb_set_delay_fb()
 *  ```>>> */
static inline void bw_comb_set_delay_fb(bw_comb_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the feedback delay time `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comb_set_blend()
 *  ```>>> */
static inline void bw_mm1_set_coeff_blend(bw_mm1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the blend coefficient `value` in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_comb_set_coeff_ff()
 *  ```>>> */
static inline void bw_mm1_set_coeff_ff(bw_mm1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the feedforward coefficient `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comb_set_coeff_fb()
 *  ```>>> */
static inline void bw_mm1_set_coeff_fb(bw_mm1_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the feedback coefficient `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_delay.h>
#include <bw_gain.h>
#include <bw_math.h>

struct _bw_comb_coeffs {
	// Sub-components
	bw_delay_coeffs	delay_coeffs;
	bw_gain_coeffs	blend_coeffs;
	bw_gain_coeffs	ff_coeffs;
	bw_gain_coeffs	fb_coeffs;
	bw_one_pole	smooth_coeffs;
	bw_one_pole	smooth_delay_ff_state;
	bw_one_pole	smooth_delay_fb_state;

	// Coefficients
	float		fs;

	BW_SIZE_T	dfbi;
	float		dfbf;

	// Parameters
	float		delay_ff;
	float		delay_fb;
};

struct _bw_comb_state {
	// Sub-components
	bw_delay_state	delay_state;
};

static inline void bw_comb_init(bw_comb_coeffs *BW_RESTRICT coeffs, float max_delay) {
	bw_delay_init(&coeffs->delay_coeffs, max_delay);
	bw_gain_init(&coeffs->blend_coeffs);
	bw_gain_init(&coeffs->ff_coeffs);
	bw_gain_init(&coeffs->fb_coeffs);
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_gain_set_gain_lin(&coeffs->ff_coeffs, 0.f);
	bw_gain_set_gain_lin(&coeffs->fb_coeffs, 0.f);
	coeffs->delay_ff = 0.f;
	coeffs->delay_fb = 0.f;
}

static inline void bw_comb_set_sample_rate(bw_comb_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_delay_set_sample_rate(&coeffs->delay_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->blend_coeffs, sample_rate); 
	bw_gain_set_sample_rate(&coeffs->ff_coeffs, sample_rate); 
	bw_gain_set_sample_rate(&coeffs->fb_coeffs, sample_rate); 
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->fs = sample_rate;
}

static inline BW_SIZE_T bw_delay_mem_req(bw_delay_coeffs *BW_RESTRICT coeffs) {
	return bw_delay_mem_req(&coeffs->delay_coeffs);
}

static inline void bw_delay_mem_set(bw_delay_state *BW_RESTRICT state, void *mem) {
	bw_delay_mem_set(&state->delay_state, mem);
}

static inline void bw_comb_reset_coeffs(bw_comb_coeffs *BW_RESTRICT coeffs) {
	bw_delay_reset_coeffs(&coeffs->delay_coeffs);
	bw_gain_reset_coeffs(&coeffs->blend_coeffs);
	bw_gain_reset_coeffs(&coeffs->ff_coeffs);
	bw_gain_reset_coeffs(&coeffs->fb_coeffs);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_delay_ff_state, coeffs->fs * coeffs->delay_ff);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_delay_fb_state, coeffs->fs * coeffs->delay_fb);
}

static inline void bw_comb_reset_state(const bw_comb_coeffs *BW_RESTRICT coeffs, bw_comb_state *BW_RESTRICT state) {
	bw_delay_reset_state(&coeffs->delay_coeffs, &state->delay_state);
}

static inline void bw_comb_update_coeffs_ctrl(bw_comb_coeffs *BW_RESTRICT coeffs) {
	bw_gain_update_coeffs_ctrl(&coeffs->blend_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->ff_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->fb_coeffs);
}

static inline void bw_comb_update_coeffs_audio(bw_comb_coeffs *BW_RESTRICT coeffs) {
	bw_gain_update_coeffs_audio(&coeffs->blend_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->ff_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->fb_coeffs);
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_delay_ff_state, coeffs->fs * coeffs->delay_ff);
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_delay_fb_state, coeffs->fs * coeffs->delay_fb);
	const BW_SIZE_T len = bw_delay_get_length(&coeffs->delay_coeffs);
	const float dfb = bw_maxf(bw_one_pole_get_y_z1(&coeffs->smooth_delay_fb_state), 1.f) - 1.f;
	float dfbif, dfbf;
	bw_intfracf(dfb, &dfbif, &dfbf);
	BW_SIZE_T dfbi = (BW_SIZE_T)dfbif;
	if (dfbi >= len) {
		dfbi = len;
		dfbf = 0.f;
	}
	coeffs->dfbi = dfbi;
	coeffs->dfbf = dfbf;
}

static inline float bw_comb_process1(const bw_comb_coeffs *BW_RESTRICT coeffs, bw_comb_state *BW_RESTRICT state, float x, float x_mod) {
	const BW_SIZE_T len = bw_delay_get_length(&coeffs->delay_coeffs);
	const float dff = bw_maxf(bw_one_pole_get_y_z1(&coeffs->smooth_delay_ff_state) + x_mod, 0.f);
	float dffif, dfff;
	bw_intfracf(dff, &dffif, &dfff);
	BW_SIZE_T dffi = (BW_SIZE_T)dffif;
	if (dffi >= len) {
		dffi = len;
		dfff = 0.f;
	}
	const float fb = bw_delay_read(&coeffs->delay_coeffs, &state->delay_state, dfbi, dfbf);
	const float v = x + fb;
	bw_delay_write(&coeffs->delay_coeffs, &state->delay_state, v);
	const float ff = bw_delay_read(&coeffs->delay_coeffs, &state->delay_state, dffi, dfff);
	return bw_gain_process1(&coeffs->blend_coeffs, v) + bw_gain_process1(&coeffs->ff_coeffs, ff);
}

static inline void bw_comb_process(bw_comb_coeffs *BW_RESTRICT coeffs, bw_comb_state *BW_RESTRICT state, const float *x, const float *x_mod, float *y, int n_samples) {
	bw_comb_update_coeffs_ctrl(coeffs);
	if (x_mod != NULL)
		for (int i = 0; i < n_samples; i++) {
			bw_comb_update_coeffs_audio(coeffs);
			y[i] = bw_comb_process1(coeffs, state, x[i], x_mod[i]);
		}
	else
		for (int i = 0; i < n_samples; i++) {
			bw_comb_update_coeffs_audio(coeffs);
			y[i] = bw_comb_process1(coeffs, state, x[i], 0.f);
		}
}

static inline void bw_comb_set_delay_ff(bw_comb_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->delay_ff = value;
}

static inline void bw_comb_set_delay_fb(bw_comb_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->delay_fb = value;
}

static inline void bw_mm1_set_coeff_blend(bw_mm1_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->blend_coeffs, value);
}

static inline void bw_mm1_set_coeff_ff(bw_mm1_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->ff_coeffs, value);
}

static inline void bw_mm1_set_coeff_fb(bw_mm1_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->fb_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
