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
 *  requires {{{ bw_common bw_gain bw_lp1 bw_math bw_one_pole }}}
 *  description {{{
 *    First-order multimode filter.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_mm1_process()</code> and
 *              <code>bw_mm1_process_multi()</code> now use <code>size_t</code>
 *              to count samples and channels.</li>
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
 *          <li>Added <code>bw_mm1_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_mm1_reset_state()</code>.</li>
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

#ifndef BW_MM1_H
#define BW_MM1_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_mm1_coeffs
 *  ```>>> */
typedef struct bw_mm1_coeffs bw_mm1_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_mm1_state
 *  ```>>> */
typedef struct bw_mm1_state bw_mm1_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_mm1_init()
 *  ```>>> */
static inline void bw_mm1_init(
	bw_mm1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_mm1_set_sample_rate()
 *  ```>>> */
static inline void bw_mm1_set_sample_rate(
	bw_mm1_coeffs * BW_RESTRICT coeffs,
	float                       sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_mm1_reset_coeffs()
 *  ```>>> */
static inline void bw_mm1_reset_coeffs(
	bw_mm1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_mm1_reset_state()
 *  ```>>> */
static inline void bw_mm1_reset_state(
	const bw_mm1_coeffs * BW_RESTRICT coeffs,
	bw_mm1_state * BW_RESTRICT        state,
	float                             x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x_0`.
 *
 *    #### bw_mm1_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_mm1_update_coeffs_ctrl(
	bw_mm1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_mm1_update_coeffs_audio()
 *  ```>>> */
static inline void bw_mm1_update_coeffs_audio(
	bw_mm1_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_mm1_process1()
 *  ```>>> */
static inline float bw_mm1_process1(
	const bw_mm1_coeffs * BW_RESTRICT coeffs,
	bw_mm1_state * BW_RESTRICT        state,
	float                             x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_mm1_process()
 *  ```>>> */
static inline void bw_mm1_process(
	bw_mm1_coeffs * BW_RESTRICT coeffs,
	bw_mm1_state * BW_RESTRICT  state,
	const float *               x,
	float *                     y,
	size_t                      n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_mm1_process_multi()
 *  ```>>> */
static inline void bw_mm1_process_multi(
	bw_mm1_coeffs * BW_RESTRICT                    coeffs,
	bw_mm1_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                          x,
	float * const *                                y,
	size_t                                         n_channels,
	size_t                                         n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_mm1_set_cutoff()
 *  ```>>> */
static inline void bw_mm1_set_cutoff(
	bw_mm1_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Valid range: [`1e-6f`, `1e6f`].
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_mm1_set_prewarp_at_cutoff()
 *  ```>>> */
static inline void bw_mm1_set_prewarp_at_cutoff(
	bw_mm1_coeffs * BW_RESTRICT coeffs,
	char                        value);
/*! <<<```
 *    Sets whether bilinear transform prewarping frequency should match the
 *    cutoff frequency (non-`0`) or not (`0`).
 *
 *    Default value: non-`0` (on).
 *
 *    #### bw_mm1_set_prewarp_freq()
 *  ```>>> */
static inline void bw_mm1_set_prewarp_freq(
	bw_mm1_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the prewarping frequency `value` (Hz) in `coeffs`.
 *
 *    Only used when the prewarp\_at\_cutoff parameter is off.
 *
 *    Valid range: [`1e-6f`, `1e6f`].
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_mm1_set_coeff_x()
 *  ```>>> */
static inline void bw_mm1_set_coeff_x(
	bw_mm1_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the input mode coefficient `value` in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_mm1_set_coeff_lp()
 *  ```>>> */
static inline void bw_mm1_set_coeff_lp(
	bw_mm1_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the lowpass mode coefficient `value` in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `0.f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_lp1.h>
#include <bw_gain.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bw_mm1_coeffs {
	// Sub-components
	bw_lp1_coeffs	lp1_coeffs;
	bw_gain_coeffs	gain_x_coeffs;
	bw_gain_coeffs	gain_lp_coeffs;
};

struct bw_mm1_state {
	bw_lp1_state	lp1_state;
};

static inline void bw_mm1_init(
		bw_mm1_coeffs * BW_RESTRICT coeffs) {
	bw_lp1_init(&coeffs->lp1_coeffs);
	bw_gain_init(&coeffs->gain_x_coeffs);
	bw_gain_init(&coeffs->gain_lp_coeffs);
	bw_gain_set_smooth_tau(&coeffs->gain_x_coeffs, 0.005f);
	bw_gain_set_smooth_tau(&coeffs->gain_lp_coeffs, 0.005f);
	bw_gain_set_gain_lin(&coeffs->gain_x_coeffs, 1.f);
	bw_gain_set_gain_lin(&coeffs->gain_lp_coeffs, 0.f);
}

static inline void bw_mm1_set_sample_rate(
		bw_mm1_coeffs * BW_RESTRICT coeffs,
		float                       sample_rate) {
	bw_lp1_set_sample_rate(&coeffs->lp1_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_x_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_lp_coeffs, sample_rate);
}

static inline void bw_mm1_reset_coeffs(
		bw_mm1_coeffs * BW_RESTRICT coeffs) {
	bw_lp1_reset_coeffs(&coeffs->lp1_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_x_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_lp_coeffs);
}

static inline void bw_mm1_reset_state(
		const bw_mm1_coeffs * BW_RESTRICT coeffs,
		bw_mm1_state * BW_RESTRICT        state,
		float                             x_0) {
	bw_lp1_reset_state(&coeffs->lp1_coeffs, &state->lp1_state, x_0);
}

static inline void bw_mm1_update_coeffs_ctrl(
		bw_mm1_coeffs * BW_RESTRICT coeffs) {
	bw_lp1_update_coeffs_ctrl(&coeffs->lp1_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_x_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_lp_coeffs);
}

static inline void bw_mm1_update_coeffs_audio(
		bw_mm1_coeffs * BW_RESTRICT coeffs) {
	bw_lp1_update_coeffs_audio(&coeffs->lp1_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_x_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_lp_coeffs);
}

static inline float bw_mm1_process1(
		const bw_mm1_coeffs * BW_RESTRICT coeffs,
		bw_mm1_state * BW_RESTRICT        state,
		float                             x) {
	const float lp = bw_lp1_process1(&coeffs->lp1_coeffs, &state->lp1_state, x);
	const float vx = bw_gain_process1(&coeffs->gain_x_coeffs, x);
	const float vlp = bw_gain_process1(&coeffs->gain_lp_coeffs, lp);
	return vx + vlp;
}

static inline void bw_mm1_process(
		bw_mm1_coeffs * BW_RESTRICT coeffs,
		bw_mm1_state * BW_RESTRICT  state,
		const float *               x,
		float *                     y,
		size_t                      n_samples) {
	bw_mm1_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_mm1_update_coeffs_audio(coeffs);
		y[i] = bw_mm1_process1(coeffs, state, x[i]);
	}
}

static inline void bw_mm1_process_multi(
		bw_mm1_coeffs * BW_RESTRICT                    coeffs,
		bw_mm1_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                          x,
		float * const *                                y,
		size_t                                         n_channels,
		size_t                                         n_samples) {
	bw_mm1_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_mm1_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_mm1_process1(coeffs, state[j], x[j][i]);
	}
}

static inline void bw_mm1_set_cutoff(
		bw_mm1_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	bw_lp1_set_cutoff(&coeffs->lp1_coeffs, value);
}

static inline void bw_mm1_set_prewarp_at_cutoff(
		bw_mm1_coeffs * BW_RESTRICT coeffs,
		char                        value) {
	bw_lp1_set_prewarp_at_cutoff(&coeffs->lp1_coeffs, value);
}

static inline void bw_mm1_set_prewarp_freq(
		bw_mm1_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	bw_lp1_set_prewarp_freq(&coeffs->lp1_coeffs, value);
}

static inline void bw_mm1_set_coeff_x(
		bw_mm1_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	bw_gain_set_gain_lin(&coeffs->gain_x_coeffs, value);
}

static inline void bw_mm1_set_coeff_lp(
		bw_mm1_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	bw_gain_set_gain_lin(&coeffs->gain_lp_coeffs, value);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::MM1
 *  ```>>> */
template<size_t N_CHANNELS>
class MM1 {
public:
	MM1();

	void setSampleRate(
		float sampleRate);

	void reset(
		float x_0 = 0.f);

	void process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples);

	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);

	void setCutoff(
		float value);

	void setPrewarpAtCutoff(
		bool value);

	void setPrewarpFreq(
		float value);

	void setCoeffX(
		float value);

	void setCoeffLp(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_mm1_coeffs	 coeffs;
	bw_mm1_state	 states[N_CHANNELS];
	bw_mm1_state	*BW_RESTRICT statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline MM1<N_CHANNELS>::MM1() {
	bw_mm1_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void MM1<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_mm1_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void MM1<N_CHANNELS>::reset(float x_0) {
	bw_mm1_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_mm1_reset_state(&coeffs, states + i, x_0);
}

template<size_t N_CHANNELS>
inline void MM1<N_CHANNELS>::process(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_mm1_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void MM1<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void MM1<N_CHANNELS>::setCutoff(float value) {
	bw_mm1_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM1<N_CHANNELS>::setPrewarpAtCutoff(bool value) {
	bw_mm1_set_prewarp_at_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM1<N_CHANNELS>::setPrewarpFreq(float value) {
	bw_mm1_set_prewarp_freq(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM1<N_CHANNELS>::setCoeffX(float value) {
	bw_mm1_set_coeff_x(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM1<N_CHANNELS>::setCoeffLp(float value) {
	bw_mm1_set_coeff_lp(&coeffs, value);
}

}
#endif

#endif
