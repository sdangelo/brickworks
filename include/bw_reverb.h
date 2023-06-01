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
 *    bw_buf bw_common bw_config bw_delay bw_drywet bw_gain bw_lp1 bw_math
 *    bw_one_pole
 *  }}}
 *  description {{{
 *    Stereo reverb.
 *
 *    Essentially an implementation of the structure described in
 *
 *    J. Dattorro, "Effect Design, Part 1: Reverberator and Other Filters",
 *    J. Audio Eng. Soc., vol. 45, no. 9, pp. 660-684, September 1997.
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

#ifndef _BW_REVERB_H
#define _BW_REVERB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_reverb_coeffs
 *  ```>>> */
typedef struct _bw_reverb_coeffs bw_reverb_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_reverb_state
 *  ```>>> */
typedef struct _bw_reverb_state bw_reverb_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_reverb_init()
 *  ```>>> */
static inline void bw_reverb_init(bw_reverb_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_reverb_set_sample_rate()
 *  ```>>> */
static inline void bw_reverb_set_sample_rate(bw_reverb_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_reverb_mem_req()
 *  ```>>> */
static inline BW_SIZE_T bw_reverb_mem_req(bw_reverb_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the size, in bytes, of contiguous memory to be supplied to
 *    `bw_reverb_mem_set()` using `coeffs`.
 *
 *    #### bw_reverb_mem_set()
 *  ```>>> */
static inline void bw_reverb_mem_set(bw_reverb_state *BW_RESTRICT state, void *mem);
/*! <<<```
 *    Associates the contiguous memory block `mem` to the given `state`.
 *
 *    #### bw_reverb_reset_coeffs()
 *  ```>>> */
static inline void bw_reverb_reset_coeffs(bw_reverb_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_reverb_reset_state()
 *  ```>>> */
static inline void bw_reverb_reset_state(const bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_reverb_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_reverb_update_coeffs_ctrl(bw_reverb_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_reverb_update_coeffs_audio()
 *  ```>>> */
static inline void bw_reverb_update_coeffs_audio(bw_reverb_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_reverb_process1()
 *  ```>>> */
static inline void bw_reverb_process1(const bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state, float xl, float xr, float *yl, float *yr);
/*! <<<```
 *    Processes two input samples `xl` and `xr` using `coeffs`, while using and
 *    updating `state`. Writes the corresponding output samples `yl` and `yr`.
 *
 *    #### bw_reverb_process()
 *  ```>>> */
static inline void bw_reverb_process(bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state, const float *xl, const float *xr, float *yl, float *yr, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffers `xl` and `xr` and
 *    fills the first `n_samples` of the output buffers `yl` and `yr`, while
 *    using and updating both `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_reverb_set_predelay()
 *  ```>>> */
static inline void bw_reverb_set_predelay(bw_reverb_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the predelay time `value` (s) in `coeffs`.
 *
 *    Valid input range: [`0.f`, `0.1f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_reverb_set_bandwidth()
 *  ```>>> */
static inline void bw_reverb_set_bandwidth(bw_reverb_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the input high-frequency attenuation `value` (Hz) in `coeffs`.
 *
 *    Default value: `20e3f`.
 *
 *    #### bw_reverb_set_damping()
 *  ```>>> */
static inline void bw_reverb_set_damping(bw_reverb_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the high-frequency damping `value` (Hz) in `coeffs`.
 *
 *    Default value: `20e3f`.
 *
 *    #### bw_reverb_set_decay()
 *  ```>>> */
static inline void bw_reverb_set_decay(bw_reverb_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the decay rate `value` in `coeffs`.
 *
 *    Valid input range: [`0.f`, `1.f`).
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_reverb_set_wet()
 *  ```>>> */
static inline void bw_reverb_set_wet(bw_reverb_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the output wet mixing `value` (linear gain) in `coeffs`.
 *
 *    Default value: `0.5f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_delay.h>
#include <bw_lp1.h>
#include <bw_drywet.h>
#include <bw_one_pole.h>
#include <bw_math.h>

struct _bw_reverb_coeffs {
	// Sub-components
	bw_delay_coeffs		predelay_coeffs;
	bw_lp1_coeffs		bandwidth_coeffs;
	bw_delay_coeffs		delay_id1_coeffs;
	bw_delay_coeffs		delay_id2_coeffs;
	bw_delay_coeffs		delay_id3_coeffs;
	bw_delay_coeffs		delay_id4_coeffs;
	bw_delay_coeffs		delay_dd1_coeffs;
	bw_delay_coeffs		delay_dd2_coeffs;
	bw_delay_coeffs		delay_dd3_coeffs;
	bw_delay_coeffs		delay_dd4_coeffs;
	bw_delay_coeffs		delay_d1_coeffs;
	bw_delay_coeffs		delay_d2_coeffs;
	bw_delay_coeffs		delay_d3_coeffs;
	bw_delay_coeffs		delay_d4_coeffs;
	bw_drywet_coeffs	drywet_coeffs;
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_predelay_state;

	// Coefficients
	float			fs;
	float			T;
	BW_SIZE_T		id1;
	BW_SIZE_T		id2;
	BW_SIZE_T		id3;
	BW_SIZE_T		id4;
	BW_SIZE_T		dd2;
	BW_SIZE_T		dd4;
	BW_SIZE_T		d1;
	BW_SIZE_T		d2;
	BW_SIZE_T		d3;
	BW_SIZE_T		d4;

	// Parameters
	float			predelay;
};

struct _bw_reverb_state {
	bw_delay_state		predelay_state;
	bw_lp1_state		bandwidth_state;
};

static inline void bw_reverb_init(bw_reverb_coeffs *BW_RESTRICT coeffs) {
	bw_delay_init(&coeffs->predelay_coeffs, 0.1f);
	bw_lp1_init(&coeffs->bandwidth_coeffs);
	bw_delay_init(&coeffs->delay_id1_coeffs, 142.f / 29761.f);
	bw_delay_init(&coeffs->delay_id2_coeffs, 107.f / 29761.f);
	bw_delay_init(&coeffs->delay_id3_coeffs, 379.f / 29761.f);
	bw_delay_init(&coeffs->delay_id4_coeffs, 277.f / 29761.f);
	bw_delay_init(&coeffs->delay_dd1_coeffs, (672.f + 16.f) / 29761.f);
	bw_delay_init(&coeffs->delay_dd2_coeffs, 1800.f / 29761.f);
	bw_delay_init(&coeffs->delay_dd3_coeffs, (908.f + 16.f) / 29761.f);
	bw_delay_init(&coeffs->delay_dd4_coeffs, 2656.f / 29761.f);
	bw_delay_init(&coeffs->delay_d1_coeffs, 4453.f / 29761.f);
	bw_delay_init(&coeffs->delay_d2_coeffs, 3720.f / 29761.f);
	bw_delay_init(&coeffs->delay_d3_coeffs, 4217.f / 29761.f);
	bw_delay_init(&coeffs->delay_d4_coeffs, 3163.f / 29761.f);
	bw_drywet_init(&coeffs->drywet_coeffs);
	bw_one_pole_init(&coeffs->smooth_coeffs);

	bw_lp1_set_cutoff(&coeffs->bandwidth_coeffs, 20e3f);
	bw_drywet_set_wet(&coeffs->drywet_coeffs, 0.5f);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-6f);

	coeffs->predelay = 0.f;
}

static inline void bw_reverb_set_sample_rate(bw_reverb_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_delay_set_sample_rate(&coeffs->predelay_coeffs, sample_rate);
	bw_lp1_set_sample_rate(&coeffs->bandwidth_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_id1_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_id2_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_id3_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_id4_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_dd1_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_dd2_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_dd3_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_dd4_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_d1_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_d2_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_d3_coeffs, sample_rate);
	bw_delay_set_sample_rate(&coeffs->delay_d4_coeffs, sample_rate);
	bw_drywet_set_sample_rate(&coeffs->drywet_coeffs, sample_rate);
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->fs = sample_rate;
	coeffs->T = 1.f / sample_rate;
	coeffs->id1 = (BW_SIZE_T)bw_roundf(coeffs->fs * (142.f / 29761.f));
	coeffs->id2 = (BW_SIZE_T)bw_roundf(coeffs->fs * (107.f / 29761.f));
	coeffs->id3 = (BW_SIZE_T)bw_roundf(coeffs->fs * (379.f / 29761.f));
	coeffs->id4 = (BW_SIZE_T)bw_roundf(coeffs->fs * (277.f / 29761.f));
	coeffs->dd2 = (BW_SIZE_T)bw_roundf(coeffs->fs * (1800.f / 29761.f));
	coeffs->dd4 = (BW_SIZE_T)bw_roundf(coeffs->fs * (2656.f / 29761.f));
	coeffs->d1 = (BW_SIZE_T)bw_roundf(coeffs->fs * (4453.f / 29761.f));
	coeffs->d2 = (BW_SIZE_T)bw_roundf(coeffs->fs * (3720.f / 29761.f));
	coeffs->d3 = (BW_SIZE_T)bw_roundf(coeffs->fs * (4217.f / 29761.f));
	coeffs->d4 = (BW_SIZE_T)bw_roundf(coeffs->fs * (3163.f / 29761.f));
	//set delay...
}

static inline BW_SIZE_T bw_reverb_mem_req(bw_reverb_coeffs *BW_RESTRICT coeffs) {
	return bw_delay_mem_req(&coeffs->predelay_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_id1_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_id2_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_id3_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_id4_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_dd1_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_dd2_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_dd3_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_dd4_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_d1_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_d2_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_d3_coeffs)
		+ bw_delay_mem_req(&coeffs->delay_d4_coeffs);
}

static inline void bw_reverb_mem_set(bw_reverb_state *BW_RESTRICT state, void *mem) {
	bw_delay_mem_set(&state->predelay_state, mem);
	mem += bw_delay_mem_req(&coeffs->predelay_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_id1_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_id1_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_id2_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_id2_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_id3_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_id3_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_id4_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_id4_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_dd1_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_dd1_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_dd2_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_dd2_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_dd3_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_dd3_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_dd4_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_dd4_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_d1_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_d1_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_d2_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_d2_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_d3_state, mem);
	mem += bw_delay_mem_req(&coeffs->delay_d3_coeffs) * sizeof(float);
	bw_delay_mem_set(&state->delay_d4_state, mem);
}

static inline void bw_reverb_reset_coeffs(bw_reverb_coeffs *BW_RESTRICT coeffs) {
	bw_delay_reset_coeffs(&coeffs->predelay_coeffs);
	bw_lp1_reset_coeffs(&coeffs->bandwidth_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_id1_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_id2_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_id3_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_id4_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_dd1_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_dd2_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_dd3_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_dd4_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_d1_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_d2_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_d3_coeffs);
	bw_delay_reset_coeffs(&coeffs->delay_d4_coeffs);
	bw_drywet_reset_coeffs(&coeffs->drywet_coeffs);
	bw_reverb_set_predelay(coeffs, coeffs->predelay); // to get it rounded
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_predelay_state, coeffs->predelay);
}

static inline void bw_reverb_reset_state(const bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state) {
	bw_delay_reset_state(&coeffs->predelay_coeffs, &state->predelay_state);
	bw_lp1_reset_state(&coeffs->bandwidth_coeffs, &state->bandwidth_state, 0.f);
	bw_delay_reset_state(&coeffs->delay_id1_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_id2_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_id3_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_id4_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_dd1_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_dd2_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_dd3_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_dd4_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_d1_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_d2_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_d3_coeffs, &state->predelay_state);
	bw_delay_reset_state(&coeffs->delay_d4_coeffs, &state->predelay_state);
}

static inline void bw_reverb_update_coeffs_ctrl(bw_reverb_coeffs *BW_RESTRICT coeffs) {
	bw_drywet_update_coeffs_ctrl(&coeffs->drywet_coeffs);
	bw_lp1_update_coeffs_ctrl(&coeffs->bandwidth_coeffs);
}

static inline void bw_reverb_update_coeffs_audio(bw_reverb_coeffs *BW_RESTRICT coeffs) {
	bw_delay_update_coeffs_audio(&coeffs->predelay_coeffs);
	bw_lp1_update_coeffs_audio(&coeffs->bandwidth_coeffs);
	bw_drywet_update_coeffs_audio(&coeffs->drywet_coeffs);
	const float pd = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_predelay_state, coeffs->predelay);
	bw_delay_set_delay(&coeffs->predelay_coeffs, pd);
	bw_delay_update_coeffs_ctrl(&coeffs->predelay_coeffs);
	bw_delay_update_coeffs_audio(&coeffs->predelay_coeffs);
	//...
}

static inline void bw_reverb_process1(const bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state, float xl, float xr, float *yl, float *yr) {
	const float i = 0.5f * (xl + xr);
	const float pd = bw_delay_process1(&coeffs->predelay_coeffs, &state->predelay_state, i);
	const float bw = bw_lp1_process1(&coeffs->bandwidth_coeffs, &state->bandwidth_state, pd);
	//...
	*yl = bw_drywet_process1(&coeffs->drywet_coeffs, xl, bw);
	*yr = bw_drywet_process1(&coeffs->drywet_coeffs, xr, bw);
}

static inline void bw_reverb_process(bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state, const float *xl, const float *xr, float *yl, float *yr, int n_samples) {
	bw_reverb_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_reverb_update_coeffs_audio(coeffs);
		bw_reverb_process1(coeffs, state, xl[i], xr[i], yl + i, yr + i);
	}
}

static inline void bw_reverb_set_predelay(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->predelay = coeffs->T * bw_roundf(coeffs->fs * value);
}

static inline void bw_reverb_set_bandwidth(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
	bw_lp1_set_cutoff(&coeffs->bandwidth_coeffs, value);
}

static inline void bw_reverb_set_damping(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
}

static inline void bw_reverb_set_decay(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
}

static inline void bw_reverb_set_wet(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
	bw_drywet_set_wet(&coeffs->drywet_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
