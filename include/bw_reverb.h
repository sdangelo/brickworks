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
 *    bw_buf bw_common bw_delay bw_drywet bw_gain bw_lp1 bw_math bw_one_pole
 *    bw_osc_sin bw_phase_gen
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
static inline size_t bw_reverb_mem_req(const bw_reverb_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the size, in bytes, of contiguous memory to be supplied to
 *    `bw_reverb_mem_set()` using `coeffs`.
 *
 *    #### bw_reverb_mem_set()
 *  ```>>> */
static inline void bw_reverb_mem_set(const bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state, void *mem);
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
 *    Processes one set of input samples `xl` (left) and `xr` (right) using
 *    `coeffs`, while using and updating `state`. The left and right output
 *    samples are put into `yl` (left) and `yr` (right) respectively.
 *
 *    #### bw_reverb_process()
 *  ```>>> */
static inline void bw_reverb_process(bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state, const float *xl, const float *xr, float *yl, float *yr, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffers `xl` (left) and `xr`
 *    (right) and fills the first `n_samples` of the output buffers `yl` (left)
 *    and `yr` (right), while using and updating both `coeffs` and `state`
 *    (control and audio rate).
 * 
 *    #### bw_reverb_process_multi()
 *  ```>>> */
static inline void bw_reverb_process_multi(bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state **BW_RESTRICT state, const float **xl, const float **xr, float **yl, float **yr, int n_channels, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `xl`
 *    (left) and `xr` (right) and fills the first `n_samples` of the
 *    `n_channels` output buffers `yl` (left) and `yr` (right), while using and
 *    updating both the common `coeffs` and each of the `n_channels` `state`s
 *    (control and audio rate).
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
 *    Sets the input high-frequency attenuation cutoff `value` (Hz) in `coeffs`.
 *
 *    Default value: `20e3f`.
 *
 *    #### bw_reverb_set_damping()
 *  ```>>> */
static inline void bw_reverb_set_damping(bw_reverb_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the high-frequency damping cutoff `value` (Hz) in `coeffs`.
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
#include <bw_phase_gen.h>
#include <bw_osc_sin.h>
#include <bw_gain.h>
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
	bw_gain_coeffs		decay_coeffs;
	bw_phase_gen_coeffs	phase_gen_coeffs;
	bw_phase_gen_state	phase_gen_state;
	bw_lp1_coeffs		damping_coeffs;
	bw_drywet_coeffs	drywet_coeffs;
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_predelay_state;

	// Coefficients
	float			fs;
	float			T;
	size_t			id1;
	size_t			id2;
	size_t			id3;
	size_t			id4;
	size_t			dd2;
	size_t			dd4;
	size_t			d1;
	size_t			d2;
	size_t			d3;
	size_t			d4;
	size_t			dl1;
	size_t			dl2;
	size_t			dl3;
	size_t			dl4;
	size_t			dl5;
	size_t			dl6;
	size_t			dl7;
	size_t			dr1;
	size_t			dr2;
	size_t			dr3;
	size_t			dr4;
	size_t			dr5;
	size_t			dr6;
	size_t			dr7;

	float			s;
	float			diff2;

	// Parameters
	float			predelay;
};

struct _bw_reverb_state {
	bw_delay_state		predelay_state;
	bw_lp1_state		bandwidth_state;
	bw_delay_state		delay_id1_state;
	bw_delay_state		delay_id2_state;
	bw_delay_state		delay_id3_state;
	bw_delay_state		delay_id4_state;
	bw_delay_state		delay_dd1_state;
	bw_delay_state		delay_dd2_state;
	bw_delay_state		delay_dd3_state;
	bw_delay_state		delay_dd4_state;
	bw_delay_state		delay_d1_state;
	bw_delay_state		delay_d2_state;
	bw_delay_state		delay_d3_state;
	bw_delay_state		delay_d4_state;
	bw_lp1_state		damping_1_state;
	bw_lp1_state		damping_2_state;
};

static inline void bw_reverb_init(bw_reverb_coeffs *BW_RESTRICT coeffs) {
	bw_delay_init(&coeffs->predelay_coeffs, 0.1f);
	bw_lp1_init(&coeffs->bandwidth_coeffs);
	bw_delay_init(&coeffs->delay_id1_coeffs, 142.f / 29761.f);
	bw_delay_init(&coeffs->delay_id2_coeffs, 107.f / 29761.f);
	bw_delay_init(&coeffs->delay_id3_coeffs, 379.f / 29761.f);
	bw_delay_init(&coeffs->delay_id4_coeffs, 277.f / 29761.f);
	bw_delay_init(&coeffs->delay_dd1_coeffs, (672.f + 8.f) / 29761.f);
	bw_delay_init(&coeffs->delay_dd2_coeffs, 1800.f / 29761.f);
	bw_delay_init(&coeffs->delay_dd3_coeffs, (908.f + 8.f) / 29761.f);
	bw_delay_init(&coeffs->delay_dd4_coeffs, 2656.f / 29761.f);
	bw_delay_init(&coeffs->delay_d1_coeffs, 4453.f / 29761.f);
	bw_delay_init(&coeffs->delay_d2_coeffs, 3720.f / 29761.f);
	bw_delay_init(&coeffs->delay_d3_coeffs, 4217.f / 29761.f);
	bw_delay_init(&coeffs->delay_d4_coeffs, 3163.f / 29761.f);
	bw_gain_init(&coeffs->decay_coeffs);
	bw_phase_gen_init(&coeffs->phase_gen_coeffs);
	bw_lp1_init(&coeffs->damping_coeffs);
	bw_drywet_init(&coeffs->drywet_coeffs);
	bw_one_pole_init(&coeffs->smooth_coeffs);

	bw_lp1_set_cutoff(&coeffs->bandwidth_coeffs, 20e3f);
	bw_lp1_set_cutoff(&coeffs->damping_coeffs, 20e3f);
	bw_gain_set_gain_lin(&coeffs->decay_coeffs, 0.5f);
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
	bw_gain_set_sample_rate(&coeffs->decay_coeffs, sample_rate);
	bw_phase_gen_set_sample_rate(&coeffs->phase_gen_coeffs, sample_rate);
	bw_lp1_set_sample_rate(&coeffs->damping_coeffs, sample_rate);
	bw_drywet_set_sample_rate(&coeffs->drywet_coeffs, sample_rate);
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->fs = sample_rate;
	coeffs->T = 1.f / sample_rate;
	coeffs->id1 = (size_t)bw_roundf(coeffs->fs * (142.f / 29761.f));
	coeffs->id2 = (size_t)bw_roundf(coeffs->fs * (107.f / 29761.f));
	coeffs->id3 = (size_t)bw_roundf(coeffs->fs * (379.f / 29761.f));
	coeffs->id4 = (size_t)bw_roundf(coeffs->fs * (277.f / 29761.f));
	coeffs->dd2 = (size_t)bw_roundf(coeffs->fs * (1800.f / 29761.f));
	coeffs->dd4 = (size_t)bw_roundf(coeffs->fs * (2656.f / 29761.f));
	coeffs->d1 = (size_t)bw_roundf(coeffs->fs * (4453.f / 29761.f));
	coeffs->d2 = (size_t)bw_roundf(coeffs->fs * (3720.f / 29761.f));
	coeffs->d3 = (size_t)bw_roundf(coeffs->fs * (4217.f / 29761.f));
	coeffs->d4 = (size_t)bw_roundf(coeffs->fs * (3163.f / 29761.f));
	coeffs->dl1 = (size_t)bw_roundf(coeffs->fs * (266.f / 29761.f));
	coeffs->dl2 = (size_t)bw_roundf(coeffs->fs * (2974.f / 29761.f));
	coeffs->dl3 = (size_t)bw_roundf(coeffs->fs * (1913.f / 29761.f));
	coeffs->dl4 = (size_t)bw_roundf(coeffs->fs * (1996.f / 29761.f));
	coeffs->dl5 = (size_t)bw_roundf(coeffs->fs * (1990.f / 29761.f));
	coeffs->dl6 = (size_t)bw_roundf(coeffs->fs * (187.f / 29761.f));
	coeffs->dl7 = (size_t)bw_roundf(coeffs->fs * (1066.f / 29761.f));
	coeffs->dr1 = (size_t)bw_roundf(coeffs->fs * (353.f / 29761.f));
	coeffs->dr2 = (size_t)bw_roundf(coeffs->fs * (3627.f / 29761.f));
	coeffs->dr3 = (size_t)bw_roundf(coeffs->fs * (1228.f / 29761.f));
	coeffs->dr4 = (size_t)bw_roundf(coeffs->fs * (2673.f / 29761.f));
	coeffs->dr5 = (size_t)bw_roundf(coeffs->fs * (2111.f / 29761.f));
	coeffs->dr6 = (size_t)bw_roundf(coeffs->fs * (335.f / 29761.f));
	coeffs->dr7 = (size_t)bw_roundf(coeffs->fs * (121.f / 29761.f));
}

static inline size_t bw_reverb_mem_req(const bw_reverb_coeffs *BW_RESTRICT coeffs) {
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

static inline void bw_reverb_mem_set(const bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state, void *mem) {
	char *m = (char *)mem;
	bw_delay_mem_set(&coeffs->predelay_coeffs, &state->predelay_state, m);
	m += bw_delay_mem_req(&coeffs->predelay_coeffs);
	bw_delay_mem_set(&coeffs->delay_id1_coeffs, &state->delay_id1_state, m);
	m += bw_delay_mem_req(&coeffs->delay_id1_coeffs);
	bw_delay_mem_set(&coeffs->delay_id2_coeffs, &state->delay_id2_state, m);
	m += bw_delay_mem_req(&coeffs->delay_id2_coeffs);
	bw_delay_mem_set(&coeffs->delay_id3_coeffs, &state->delay_id3_state, m);
	m += bw_delay_mem_req(&coeffs->delay_id3_coeffs);
	bw_delay_mem_set(&coeffs->delay_id4_coeffs, &state->delay_id4_state, m);
	m += bw_delay_mem_req(&coeffs->delay_id4_coeffs);
	bw_delay_mem_set(&coeffs->delay_dd1_coeffs, &state->delay_dd1_state, m);
	m += bw_delay_mem_req(&coeffs->delay_dd1_coeffs);
	bw_delay_mem_set(&coeffs->delay_dd2_coeffs, &state->delay_dd2_state, m);
	m += bw_delay_mem_req(&coeffs->delay_dd2_coeffs);
	bw_delay_mem_set(&coeffs->delay_dd3_coeffs, &state->delay_dd3_state, m);
	m += bw_delay_mem_req(&coeffs->delay_dd3_coeffs);
	bw_delay_mem_set(&coeffs->delay_dd4_coeffs, &state->delay_dd4_state, m);
	m += bw_delay_mem_req(&coeffs->delay_dd4_coeffs);
	bw_delay_mem_set(&coeffs->delay_d1_coeffs, &state->delay_d1_state, m);
	m += bw_delay_mem_req(&coeffs->delay_d1_coeffs);
	bw_delay_mem_set(&coeffs->delay_d2_coeffs, &state->delay_d2_state, m);
	m += bw_delay_mem_req(&coeffs->delay_d2_coeffs);
	bw_delay_mem_set(&coeffs->delay_d3_coeffs, &state->delay_d3_state, m);
	m += bw_delay_mem_req(&coeffs->delay_d3_coeffs);
	bw_delay_mem_set(&coeffs->delay_d4_coeffs, &state->delay_d4_state, m);
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
	bw_gain_reset_coeffs(&coeffs->decay_coeffs);
	bw_phase_gen_reset_coeffs(&coeffs->phase_gen_coeffs);
	bw_phase_gen_reset_state(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state, 0.f);
	bw_lp1_reset_coeffs(&coeffs->damping_coeffs);
	bw_drywet_reset_coeffs(&coeffs->drywet_coeffs);
	bw_reverb_set_predelay(coeffs, coeffs->predelay); // to get it rounded
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_predelay_state, coeffs->predelay);
}

static inline void bw_reverb_reset_state(const bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state) {
	bw_delay_reset_state(&coeffs->predelay_coeffs, &state->predelay_state);
	bw_lp1_reset_state(&coeffs->bandwidth_coeffs, &state->bandwidth_state, 0.f);
	bw_delay_reset_state(&coeffs->delay_id1_coeffs, &state->delay_id1_state);
	bw_delay_reset_state(&coeffs->delay_id2_coeffs, &state->delay_id2_state);
	bw_delay_reset_state(&coeffs->delay_id3_coeffs, &state->delay_id3_state);
	bw_delay_reset_state(&coeffs->delay_id4_coeffs, &state->delay_id4_state);
	bw_delay_reset_state(&coeffs->delay_dd1_coeffs, &state->delay_dd1_state);
	bw_delay_reset_state(&coeffs->delay_dd2_coeffs, &state->delay_dd2_state);
	bw_delay_reset_state(&coeffs->delay_dd3_coeffs, &state->delay_dd3_state);
	bw_delay_reset_state(&coeffs->delay_dd4_coeffs, &state->delay_dd4_state);
	bw_delay_reset_state(&coeffs->delay_d1_coeffs, &state->delay_d1_state);
	bw_delay_reset_state(&coeffs->delay_d2_coeffs, &state->delay_d2_state);
	bw_delay_reset_state(&coeffs->delay_d3_coeffs, &state->delay_d3_state);
	bw_delay_reset_state(&coeffs->delay_d4_coeffs, &state->delay_d4_state);
	bw_lp1_reset_state(&coeffs->damping_coeffs, &state->damping_1_state, 0.f);
	bw_lp1_reset_state(&coeffs->damping_coeffs, &state->damping_2_state, 0.f);
}

static inline void bw_reverb_update_coeffs_ctrl(bw_reverb_coeffs *BW_RESTRICT coeffs) {
	bw_lp1_update_coeffs_ctrl(&coeffs->bandwidth_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->decay_coeffs);
	bw_phase_gen_update_coeffs_ctrl(&coeffs->phase_gen_coeffs);
	bw_drywet_update_coeffs_ctrl(&coeffs->drywet_coeffs);
	bw_lp1_update_coeffs_ctrl(&coeffs->damping_coeffs);
}

static inline void bw_reverb_update_coeffs_audio(bw_reverb_coeffs *BW_RESTRICT coeffs) {
	bw_delay_update_coeffs_audio(&coeffs->predelay_coeffs);
	bw_lp1_update_coeffs_audio(&coeffs->bandwidth_coeffs);
	const float pd = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_predelay_state, coeffs->predelay);
	bw_delay_set_delay(&coeffs->predelay_coeffs, pd);
	bw_delay_update_coeffs_ctrl(&coeffs->predelay_coeffs);
	bw_delay_update_coeffs_audio(&coeffs->predelay_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->decay_coeffs);
	bw_phase_gen_update_coeffs_audio(&coeffs->phase_gen_coeffs);
	float p, pi;
	bw_phase_gen_process1(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state, &p, &pi);
	coeffs->s = (8.f / 29761.f) * bw_osc_sin_process1(p);
	bw_lp1_update_coeffs_audio(&coeffs->damping_coeffs);
	coeffs->diff2 = bw_clipf(bw_gain_get_gain(&coeffs->decay_coeffs) + 0.15f, 0.25f, 0.5f);
	bw_drywet_update_coeffs_audio(&coeffs->drywet_coeffs);
}

static inline void bw_reverb_process1(const bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state, float xl, float xr, float *yl, float *yr) {
	const float i = 0.5f * (xl + xr);
	const float pd = bw_delay_process1(&coeffs->predelay_coeffs, &state->predelay_state, i);
	const float bw = bw_lp1_process1(&coeffs->bandwidth_coeffs, &state->bandwidth_state, pd);

	const float n14 = bw_delay_read(&coeffs->delay_id1_coeffs, &state->delay_id1_state, coeffs->id1, 0.f);
	const float n13 = bw - 0.75f * n14;
	const float id1 = n14 + 0.75f * n13;
	bw_delay_write(&coeffs->delay_id1_coeffs, &state->delay_id1_state, n13);
	const float n20 = bw_delay_read(&coeffs->delay_id2_coeffs, &state->delay_id2_state, coeffs->id2, 0.f);
	const float n19 = id1 - 0.75f * n20;
	const float id2 = n20 + 0.75f * n19;
	bw_delay_write(&coeffs->delay_id2_coeffs, &state->delay_id2_state, n19);
	const float n16 = bw_delay_read(&coeffs->delay_id3_coeffs, &state->delay_id3_state, coeffs->id3, 0.f);
	const float n15 = id2 - 0.625f * n16;
	const float id3 = n16 + 0.625f * n15;
	bw_delay_write(&coeffs->delay_id3_coeffs, &state->delay_id3_state, n15);
	const float n22 = bw_delay_read(&coeffs->delay_id4_coeffs, &state->delay_id4_state, coeffs->id4, 0.f);
	const float n21 = id3 - 0.625f * n22;
	const float id4 = n22 + 0.625f * n21;
	bw_delay_write(&coeffs->delay_id4_coeffs, &state->delay_id4_state, n21);

	const float n39 = bw_delay_read(&coeffs->delay_d2_coeffs, &state->delay_d2_state, coeffs->d2, 0.f);
	const float n63 = bw_delay_read(&coeffs->delay_d4_coeffs, &state->delay_d4_state, coeffs->d4, 0.f);
	const float s1 = id4 + bw_gain_process1(&coeffs->decay_coeffs, n63);
	const float s2 = id4 + bw_gain_process1(&coeffs->decay_coeffs, n39);

	float dd1if, dd1f;
	bw_intfracf(coeffs->fs * ((672.f / 29761.f) + coeffs->s), &dd1if, &dd1f);
	const size_t dd1i = (size_t)dd1if;
	float dd3if, dd3f;
	bw_intfracf(coeffs->fs * ((908.f / 29761.f) + coeffs->s), &dd3if, &dd3f);
	const size_t dd3i = (size_t)dd3if;

	const float n24 = bw_delay_read(&coeffs->delay_dd1_coeffs, &state->delay_dd1_state, dd1i, dd1f);
	const float n23 = s1 + 0.7f * n24;
	const float dd1 = n24 - 0.7f * n23;
	bw_delay_write(&coeffs->delay_dd1_coeffs, &state->delay_dd1_state, n23);
	const float n48 = bw_delay_read(&coeffs->delay_dd3_coeffs, &state->delay_dd3_state, dd3i, dd3f);
	const float n46 = s2 + 0.7f * n48;
	const float dd3 = n48 - 0.7f * n46;
	bw_delay_write(&coeffs->delay_dd3_coeffs, &state->delay_dd3_state, n46);
	const float n30 = bw_delay_read(&coeffs->delay_d1_coeffs, &state->delay_d1_state, coeffs->d1, 0.f);
	bw_delay_write(&coeffs->delay_d1_coeffs, &state->delay_d1_state, dd1);
	const float n54 = bw_delay_read(&coeffs->delay_d3_coeffs, &state->delay_d3_state, coeffs->d3, 0.f);
	bw_delay_write(&coeffs->delay_d3_coeffs, &state->delay_d3_state, dd3);
	const float damp1 = bw_lp1_process1(&coeffs->damping_coeffs, &state->damping_1_state, n30);
	const float damp2 = bw_lp1_process1(&coeffs->damping_coeffs, &state->damping_2_state, n54);
	const float decay1 = bw_gain_process1(&coeffs->decay_coeffs, damp1);
	const float decay2 = bw_gain_process1(&coeffs->decay_coeffs, damp2);
	const float n33 = bw_delay_read(&coeffs->delay_dd2_coeffs, &state->delay_dd2_state, coeffs->dd2, 0.f);
	const float n31 = decay1 - coeffs->diff2 * n33;
	const float dd2 = n33 + coeffs->diff2 * n31;
	bw_delay_write(&coeffs->delay_dd2_coeffs, &state->delay_dd2_state, n31);
	const float n59 = bw_delay_read(&coeffs->delay_dd4_coeffs, &state->delay_dd4_state, coeffs->dd4, 0.f);
	const float n55 = decay2 - coeffs->diff2 * n59;
	const float dd4 = n59 + coeffs->diff2 * n55;
	bw_delay_write(&coeffs->delay_dd2_coeffs, &state->delay_dd2_state, n55);
	bw_delay_write(&coeffs->delay_d2_coeffs, &state->delay_d2_state, dd2);
	bw_delay_write(&coeffs->delay_d4_coeffs, &state->delay_d4_state, dd4);

	*yl = 0.6f * (
			bw_delay_read(&coeffs->delay_d3_coeffs, &state->delay_d3_state, coeffs->dl1, 0.f)
			+ bw_delay_read(&coeffs->delay_d3_coeffs, &state->delay_d3_state, coeffs->dl2, 0.f)
			- bw_delay_read(&coeffs->delay_dd4_coeffs, &state->delay_dd4_state, coeffs->dl3, 0.f)
			+ bw_delay_read(&coeffs->delay_d4_coeffs, &state->delay_d4_state, coeffs->dl4, 0.f)
			- bw_delay_read(&coeffs->delay_d1_coeffs, &state->delay_d1_state, coeffs->dl5, 0.f)
			- bw_delay_read(&coeffs->delay_dd2_coeffs, &state->delay_dd2_state, coeffs->dl6, 0.f)
			- bw_delay_read(&coeffs->delay_d2_coeffs, &state->delay_d2_state, coeffs->dl7, 0.f)
		);
	*yr = 0.6f * (
			bw_delay_read(&coeffs->delay_d1_coeffs, &state->delay_d1_state, coeffs->dr1, 0.f)
			+ bw_delay_read(&coeffs->delay_d1_coeffs, &state->delay_d1_state, coeffs->dr2, 0.f)
			- bw_delay_read(&coeffs->delay_dd2_coeffs, &state->delay_dd2_state, coeffs->dr3, 0.f)
			+ bw_delay_read(&coeffs->delay_d2_coeffs, &state->delay_d2_state, coeffs->dr4, 0.f)
			- bw_delay_read(&coeffs->delay_d3_coeffs, &state->delay_d3_state, coeffs->dr5, 0.f)
			- bw_delay_read(&coeffs->delay_dd4_coeffs, &state->delay_dd4_state, coeffs->dr6, 0.f)
			- bw_delay_read(&coeffs->delay_d4_coeffs, &state->delay_d4_state, coeffs->dr7, 0.f)
		);
	*yl = bw_drywet_process1(&coeffs->drywet_coeffs, xl, *yl);
	*yr = bw_drywet_process1(&coeffs->drywet_coeffs, xr, *yr);
}

static inline void bw_reverb_process(bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state *BW_RESTRICT state, const float *xl, const float *xr, float *yl, float *yr, int n_samples) {
	bw_reverb_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_reverb_update_coeffs_audio(coeffs);
		bw_reverb_process1(coeffs, state, xl[i], xr[i], yl + i, yr + i);
	}
}

static inline void bw_reverb_process_multi(bw_reverb_coeffs *BW_RESTRICT coeffs, bw_reverb_state **BW_RESTRICT state, const float **xl, const float **xr, float **yl, float **yr, int n_channels, int n_samples) {
	bw_reverb_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_reverb_update_coeffs_audio(coeffs);
		for (int j = 0; j < n_channels; j++)
			bw_reverb_process1(coeffs, state[j], xl[j][i], xr[j][i], yl[j] + i, yr[j] + i);
	}
}

static inline void bw_reverb_set_predelay(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->predelay = coeffs->T * bw_roundf(coeffs->fs * value);
}

static inline void bw_reverb_set_bandwidth(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
	bw_lp1_set_cutoff(&coeffs->bandwidth_coeffs, value);
}

static inline void bw_reverb_set_damping(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
	bw_lp1_set_cutoff(&coeffs->damping_coeffs, value);
}

static inline void bw_reverb_set_decay(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->decay_coeffs, value);
}

static inline void bw_reverb_set_wet(bw_reverb_coeffs *BW_RESTRICT coeffs, float value) {
	bw_drywet_set_wet(&coeffs->drywet_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
