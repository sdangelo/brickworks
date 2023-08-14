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
 *    bw_common bw_gain bw_math bw_mm2 bw_one_pole bw_peak bw_satur bw_svf
 *  }}}
 *  description {{{
 *    Overdrive effect.
 *
 *    Loosely inspired to the green "screaming" overdrive pedal.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_drive_process()</code> and
 *              <code>bw_drive_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overladed C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
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

#ifndef BW_DRIVE_H
#define BW_DRIVE_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_drive_coeffs
 *  ```>>> */
typedef struct bw_drive_coeffs bw_drive_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_drive_state
 *  ```>>> */
typedef struct bw_drive_state bw_drive_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_drive_init()
 *  ```>>> */
static inline void bw_drive_init(bw_drive_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_drive_set_sample_rate()
 *  ```>>> */
static inline void bw_drive_set_sample_rate(bw_drive_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_drive_reset_coeffs()
 *  ```>>> */
static inline void bw_drive_reset_coeffs(bw_drive_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_drive_reset_state()
 *  ```>>> */
static inline void bw_drive_reset_state(const bw_drive_coeffs *BW_RESTRICT coeffs, bw_drive_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_drive_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_drive_update_coeffs_ctrl(bw_drive_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_drive_update_coeffs_audio()
 *  ```>>> */
static inline void bw_drive_update_coeffs_audio(bw_drive_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_drive_process1()
 *  ```>>> */
static inline float bw_drive_process1(const bw_drive_coeffs *BW_RESTRICT coeffs, bw_drive_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_drive_process()
 *  ```>>> */
static inline void bw_drive_process(bw_drive_coeffs *BW_RESTRICT coeffs, bw_drive_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_drive_process_multi()
 *  ```>>> */
static inline void bw_drive_process_multi(bw_drive_coeffs *BW_RESTRICT coeffs, bw_drive_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_drive_set_drive()
 *  ```>>> */
static inline void bw_drive_set_drive(bw_drive_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the overdrive (input gain, approximately) to the given `value` in
 *    [`0.f`, `1.f`] in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_drive_set_tone()
 *  ```>>> */
static inline void bw_drive_set_tone(bw_drive_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the tone (filter) to the given `value` in [`0.f`, `1.f`] in `coeffs`.
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_drive_set_volume()
 *  ```>>> */
static inline void bw_drive_set_volume(bw_drive_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the volume (output gain) to the given `value` in [`0.f`, `1.f`] in
 *    `coeffs`.
 *
 *    Default value: `1.f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_svf.h>
#include <bw_peak.h>
#include <bw_satur.h>
#include <bw_gain.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bw_drive_coeffs {
	// Sub-components
	bw_svf_coeffs	hp2_coeffs;
	bw_peak_coeffs	peak_coeffs;
	bw_satur_coeffs	satur_coeffs;
	bw_svf_coeffs	lp2_coeffs;
	bw_gain_coeffs	gain_coeffs;
};

struct bw_drive_state {
	// Sub-components
	bw_svf_state	hp2_state;
	bw_peak_state	peak_state;
	bw_satur_state	satur_state;
	bw_svf_state	lp2_state;
};

static inline void bw_drive_init(bw_drive_coeffs *BW_RESTRICT coeffs) {
	bw_svf_init(&coeffs->hp2_coeffs);
	bw_peak_init(&coeffs->peak_coeffs);
	bw_satur_init(&coeffs->satur_coeffs);
	bw_svf_init(&coeffs->lp2_coeffs);
	bw_gain_init(&coeffs->gain_coeffs);
	bw_svf_set_cutoff(&coeffs->hp2_coeffs, 16.f);
	bw_peak_set_cutoff(&coeffs->peak_coeffs, 2e3f);
	bw_peak_set_bandwidth(&coeffs->peak_coeffs, 10.f);
	bw_satur_set_gain(&coeffs->satur_coeffs, 1.5f);
	bw_svf_set_cutoff(&coeffs->lp2_coeffs, 600.f + (7.5e3f - 600.f) * 0.125f);
}

static inline void bw_drive_set_sample_rate(bw_drive_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_svf_set_sample_rate(&coeffs->hp2_coeffs, sample_rate);
	bw_peak_set_sample_rate(&coeffs->peak_coeffs, sample_rate);
	bw_satur_set_sample_rate(&coeffs->satur_coeffs, sample_rate);
	bw_svf_set_sample_rate(&coeffs->lp2_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_coeffs, sample_rate);
	bw_svf_reset_coeffs(&coeffs->hp2_coeffs);
	bw_satur_reset_coeffs(&coeffs->satur_coeffs);
}

static inline void bw_drive_reset_coeffs(bw_drive_coeffs *BW_RESTRICT coeffs) {
	bw_peak_reset_coeffs(&coeffs->peak_coeffs);
	bw_svf_reset_coeffs(&coeffs->lp2_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_coeffs);
}

static inline void bw_drive_reset_state(const bw_drive_coeffs *BW_RESTRICT coeffs, bw_drive_state *BW_RESTRICT state) {
	bw_svf_reset_state(&coeffs->hp2_coeffs, &state->hp2_state, 0.f);
	bw_peak_reset_state(&coeffs->peak_coeffs, &state->peak_state, 0.f);
	bw_satur_reset_state(&coeffs->satur_coeffs, &state->satur_state);
	bw_svf_reset_state(&coeffs->lp2_coeffs, &state->lp2_state, 0.f);
}

static inline void bw_drive_update_coeffs_ctrl(bw_drive_coeffs *BW_RESTRICT coeffs) {
	bw_peak_update_coeffs_ctrl(&coeffs->peak_coeffs);
	bw_svf_update_coeffs_ctrl(&coeffs->lp2_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_coeffs);
}

static inline void bw_drive_update_coeffs_audio(bw_drive_coeffs *BW_RESTRICT coeffs) {
	bw_peak_update_coeffs_audio(&coeffs->peak_coeffs);
	bw_svf_update_coeffs_audio(&coeffs->lp2_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_coeffs);
}

static inline float bw_drive_process1(const bw_drive_coeffs *BW_RESTRICT coeffs, bw_drive_state *BW_RESTRICT state, float x) {
	float v_lp, v_hp, v_bp;
	bw_svf_process1(&coeffs->hp2_coeffs, &state->hp2_state, x, &v_lp, &v_bp, &v_hp);
	float y = bw_peak_process1(&coeffs->peak_coeffs, &state->peak_state, v_hp);
	y = v_hp + 0.5f * bw_satur_process1_comp(&coeffs->satur_coeffs, &state->satur_state, y);
	bw_svf_process1(&coeffs->lp2_coeffs, &state->lp2_state, y, &v_lp, &v_bp, &v_hp);
	return bw_gain_process1(&coeffs->gain_coeffs, v_lp);
}

static inline void bw_drive_process(bw_drive_coeffs *BW_RESTRICT coeffs, bw_drive_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples) {
	bw_drive_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_drive_update_coeffs_audio(coeffs);
		y[i] = bw_drive_process1(coeffs, state, x[i]);
	}
}

static inline void bw_drive_process_multi(bw_drive_coeffs *BW_RESTRICT coeffs, bw_drive_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples) {
	bw_drive_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_drive_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_drive_process1(coeffs, state[j], x[j][i]);
	}
}

static inline void bw_drive_set_drive(bw_drive_coeffs *BW_RESTRICT coeffs, float value) {
	bw_peak_set_peak_gain_dB(&coeffs->peak_coeffs, 40.f * value);
}

static inline void bw_drive_set_tone(bw_drive_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_cutoff(&coeffs->lp2_coeffs, 600.f + (7.5e3f - 600.f) * value * value * value);
}

static inline void bw_drive_set_volume(bw_drive_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_coeffs, value * value * value);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Drive
 *  ```>>> */
template<size_t N_CHANNELS>
class Drive {
public:
	Drive();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		const float * const *x,
		float * const *y,
		size_t nSamples);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples);

	void setDrive(float value);
	void setTone(float value);
	void setVolume(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_drive_coeffs	 coeffs;
	bw_drive_state	 states[N_CHANNELS];
	bw_drive_state	*BW_RESTRICT statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Drive<N_CHANNELS>::Drive() {
	bw_drive_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_drive_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::reset() {
	bw_drive_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_drive_reset_state(&coeffs, states + i);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::process(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_drive_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::setDrive(float value) {
	bw_drive_set_drive(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::setTone(float value) {
	bw_drive_set_tone(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::setVolume(float value) {
	bw_drive_set_volume(&coeffs, value);
}

}
#endif

#endif
