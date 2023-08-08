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
 *  version {{{ 1.0.0 }}}
 *  requires {{{
 *    bw_clip bw_common bw_gain bw_hp1 bw_lp1 bw_math bw_mm2 bw_one_pole bw_peak
 *    bw_satur bw_svf
 *  }}}
 *  description {{{
 *    Distortion effect.
 *
 *    Loosely inspired to the "rodent" distortion pedal.
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
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_DIST_H
#define _BW_DIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_dist_coeffs
 *  ```>>> */
typedef struct _bw_dist_coeffs bw_dist_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_dist_state
 *  ```>>> */
typedef struct _bw_dist_state bw_dist_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_dist_init()
 *  ```>>> */
static inline void bw_dist_init(bw_dist_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_dist_set_sample_rate()
 *  ```>>> */
static inline void bw_dist_set_sample_rate(bw_dist_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_dist_reset_coeffs()
 *  ```>>> */
static inline void bw_dist_reset_coeffs(bw_dist_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_dist_reset_state()
 *  ```>>> */
static inline void bw_dist_reset_state(const bw_dist_coeffs *BW_RESTRICT coeffs, bw_dist_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_dist_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_dist_update_coeffs_ctrl(bw_dist_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_dist_update_coeffs_audio()
 *  ```>>> */
static inline void bw_dist_update_coeffs_audio(bw_dist_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_dist_process1()
 *  ```>>> */
static inline float bw_dist_process1(const bw_dist_coeffs *BW_RESTRICT coeffs, bw_dist_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_dist_process()
 *  ```>>> */
static inline void bw_dist_process(bw_dist_coeffs *BW_RESTRICT coeffs, bw_dist_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_dist_process_multi()
 *  ```>>> */
static inline void bw_dist_process_multi(bw_dist_coeffs *BW_RESTRICT coeffs, bw_dist_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples);
/*! <<<```
  *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_dist_set_distortion()
 *  ```>>> */
static inline void bw_dist_set_distortion(bw_dist_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the distortion (input gain, approximately) to the given `value` in
 *    [`0.f`, `1.f`] in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_dist_set_tone()
 *  ```>>> */
static inline void bw_dist_set_tone(bw_dist_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the tone (filter) to the given `value` in [`0.f`, `1.f`] in `coeffs`.
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_dist_set_volume()
 *  ```>>> */
static inline void bw_dist_set_volume(bw_dist_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the volume (output gain) to the given `value` in [`0.f`, `1.f`] in
 *    `coeffs`.
 *
 *    Default value: `1.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_hp1.h>
#include <bw_peak.h>
#include <bw_clip.h>
#include <bw_satur.h>
#include <bw_lp1.h>
#include <bw_gain.h>

struct _bw_dist_coeffs {
	// Sub-components
	bw_hp1_coeffs	hp1_coeffs;
	bw_peak_coeffs	peak_coeffs;
	bw_clip_coeffs	clip_coeffs;
	bw_satur_coeffs	satur_coeffs;
	bw_lp1_coeffs	lp1_coeffs;
	bw_gain_coeffs	gain_coeffs;
};

struct _bw_dist_state {
	// Sub-components
	bw_hp1_state	hp1_state;
	bw_peak_state	peak_state;
	bw_clip_state	clip_state;
	bw_satur_state	satur_state;
	bw_lp1_state	lp1_state;
};

static inline void bw_dist_init(bw_dist_coeffs *BW_RESTRICT coeffs) {
	bw_hp1_init(&coeffs->hp1_coeffs);
	bw_peak_init(&coeffs->peak_coeffs);
	bw_clip_init(&coeffs->clip_coeffs);
	bw_satur_init(&coeffs->satur_coeffs);
	bw_lp1_init(&coeffs->lp1_coeffs);
	bw_gain_init(&coeffs->gain_coeffs);
	bw_hp1_set_cutoff(&coeffs->hp1_coeffs, 7.f);
	bw_peak_set_cutoff(&coeffs->peak_coeffs, 600.f);
	bw_peak_set_bandwidth(&coeffs->peak_coeffs, 10.f);
	bw_clip_set_bias(&coeffs->clip_coeffs, 0.75f / 4.25f);
	bw_clip_set_gain(&coeffs->clip_coeffs, 1.f / 4.25f);
	bw_satur_set_gain(&coeffs->satur_coeffs, 1.f / 0.7f);
	bw_lp1_set_cutoff(&coeffs->lp1_coeffs, 475.f + (20e3f - 475.f) * 0.125f);
}

static inline void bw_dist_set_sample_rate(bw_dist_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_hp1_set_sample_rate(&coeffs->hp1_coeffs, sample_rate);
	bw_peak_set_sample_rate(&coeffs->peak_coeffs, sample_rate);
	bw_clip_set_sample_rate(&coeffs->clip_coeffs, sample_rate);
	bw_satur_set_sample_rate(&coeffs->satur_coeffs, sample_rate);
	bw_lp1_set_sample_rate(&coeffs->lp1_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_coeffs, sample_rate);
	bw_hp1_reset_coeffs(&coeffs->hp1_coeffs);
	bw_clip_reset_coeffs(&coeffs->clip_coeffs);
	bw_satur_reset_coeffs(&coeffs->satur_coeffs);
}

static inline void bw_dist_reset_coeffs(bw_dist_coeffs *BW_RESTRICT coeffs) {
	bw_peak_reset_coeffs(&coeffs->peak_coeffs);
	bw_lp1_reset_coeffs(&coeffs->lp1_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_coeffs);
}

static inline void bw_dist_reset_state(const bw_dist_coeffs *BW_RESTRICT coeffs, bw_dist_state *BW_RESTRICT state) {
	bw_hp1_reset_state(&coeffs->hp1_coeffs, &state->hp1_state, 0.f);
	bw_peak_reset_state(&coeffs->peak_coeffs, &state->peak_state, 0.f);
	bw_clip_reset_state(&coeffs->clip_coeffs, &state->clip_state);
	bw_satur_reset_state(&coeffs->satur_coeffs, &state->satur_state);
	bw_lp1_reset_state(&coeffs->lp1_coeffs, &state->lp1_state, 0.f);
}

static inline void bw_dist_update_coeffs_ctrl(bw_dist_coeffs *BW_RESTRICT coeffs) {
	bw_peak_update_coeffs_ctrl(&coeffs->peak_coeffs);
	bw_lp1_update_coeffs_ctrl(&coeffs->lp1_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_coeffs);
}

static inline void bw_dist_update_coeffs_audio(bw_dist_coeffs *BW_RESTRICT coeffs) {
	bw_peak_update_coeffs_audio(&coeffs->peak_coeffs);
	bw_lp1_update_coeffs_audio(&coeffs->lp1_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_coeffs);
}

static inline float bw_dist_process1(const bw_dist_coeffs *BW_RESTRICT coeffs, bw_dist_state *BW_RESTRICT state, float x) {
	float y = bw_hp1_process1(&coeffs->hp1_coeffs, &state->hp1_state, x);
	y = bw_peak_process1(&coeffs->peak_coeffs, &state->peak_state, y);
	y = bw_clip_process1_comp(&coeffs->clip_coeffs, &state->clip_state, y);
	y = bw_satur_process1_comp(&coeffs->satur_coeffs, &state->satur_state, y);
	y = bw_lp1_process1(&coeffs->lp1_coeffs, &state->lp1_state, y);
	return bw_gain_process1(&coeffs->gain_coeffs, y);
}

static inline void bw_dist_process(bw_dist_coeffs *BW_RESTRICT coeffs, bw_dist_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_dist_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_dist_update_coeffs_audio(coeffs);
		y[i] = bw_dist_process1(coeffs, state, x[i]);
	}
}

static inline void bw_dist_process_multi(bw_dist_coeffs *BW_RESTRICT coeffs, bw_dist_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples) {
	bw_dist_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_dist_update_coeffs_audio(coeffs);
		for (int j = 0; j < n_channels; j++)
			y[j][i] = bw_dist_process1(coeffs, state[j], x[j][i]);
	}
}

static inline void bw_dist_set_distortion(bw_dist_coeffs *BW_RESTRICT coeffs, float value) {
	bw_peak_set_peak_gain_dB(&coeffs->peak_coeffs, 60.f * value);
}

static inline void bw_dist_set_tone(bw_dist_coeffs *BW_RESTRICT coeffs, float value) {
	bw_lp1_set_cutoff(&coeffs->lp1_coeffs, 475.f + (20e3f - 475.f) * value * value * value);
}

static inline void bw_dist_set_volume(bw_dist_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_coeffs, value * value * value);
}

#ifdef __cplusplus
}
#endif

#endif
