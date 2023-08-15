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
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    Antialiased tanh-based saturation with parametric bias and gain
 *    (compensation) and output bias removal.
 *
 *    In other words this implements (approximately)
 *
 *    > y(n) = tanh(gain \* x(n) + bias) - tanh(bias)
 *
 *    with antialiasing and optionally dividing the output by gain.
 *
 *    As a side effect, antialiasing causes attenuation at higher frequencies
 *    (about 3 dB at 0.5 × Nyquist frequency and rapidly increasing at higher
 *    frequencies).
 * 
 *    The antialiasing technique used here is described in
 *
 *    J. D. Parker, V. Zavalishin, and E. Le Bivic, "Reducing the Aliasing of
 *    Nonlinear Waveshaping Using Continuous-Time Convolution", Proc. 19th Intl.
 *    Conf. Digital Audio Effects (DAFx-16), pp. 137-144, Brno, Czech Republic,
 *    September 2016.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Changed default value for gain compensation to off.</li>
 *          <li><code>bw_satur_process()</code> and
 *              <code>bw_satur_process_multi()</code> now use
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
 *          <li>Added <code>bw_satur_process_multi()</code>.</li>
 *          <li>Fixed gain coefficient reset bug.</li>
 *          <li>Fixed initial state bug.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Fixed unused parameter warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_SATUR_H
#define BW_SATUR_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_satur_coeffs
 *  ```>>> */
typedef struct bw_satur_coeffs bw_satur_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_satur_state
 *  ```>>> */
typedef struct bw_satur_state bw_satur_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_satur_init()
 *  ```>>> */
static inline void bw_satur_init(bw_satur_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_satur_set_sample_rate()
 *  ```>>> */
static inline void bw_satur_set_sample_rate(bw_satur_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_satur_reset_coeffs()
 *  ```>>> */
static inline void bw_satur_reset_coeffs(bw_satur_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_satur_reset_state()
 *  ```>>> */
static inline void bw_satur_reset_state(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_satur_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_satur_update_coeffs_ctrl(bw_satur_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_satur_update_coeffs_audio()
 *  ```>>> */
static inline void bw_satur_update_coeffs_audio(bw_satur_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_satur_process1()
 *  ```>>> */
static inline float bw_satur_process1(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, float x);
static inline float bw_satur_process1_comp(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, float x);
/*! <<<```
 *    These function process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_satur_process1()` assumes that gain compensation is disabled;
 *     * `bw_satur_process1_comp()` assumes that gain compensation is enabled.
 *
 *    #### bw_satur_process()
 *  ```>>> */
static inline void bw_satur_process(bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_satur_process_multi()
 *  ```>>> */
static inline void bw_satur_process_multi(bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_satur_set_bias()
 *  ```>>> */
static inline void bw_satur_set_bias(bw_satur_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the input bias `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_satur_set_gain()
 *  ```>>> */
static inline void bw_satur_set_gain(bw_satur_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the gain `value` in `coeffs`.
 *
 *    Do not set it to `0.f`, obviously.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_satur_set_gain_compensation()
 *  ```>>> */
static inline void bw_satur_set_gain_compensation(bw_satur_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the output should be divided by gain (`value` non-`0`) or not
 *    (`0`).
 *
 *    Default value: `0` (off).
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bw_satur_coeffs {
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

struct bw_satur_state {
	float	x_z1;
	float	F_z1;
};

static inline float bw_satur_tanhf(float x) {
	const float xm = bw_clipf(x, -2.115287308554551f, 2.115287308554551f);
	const float axm = bw_absf(xm);
	return xm * axm * (0.01218073260037716f * axm - 0.2750231331124371f) + xm;
}

static inline void bw_satur_init(bw_satur_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->bias = 0.f;
	coeffs->gain = 1.f;
	coeffs->gain_compensation = 0;
}

static inline void bw_satur_set_sample_rate(bw_satur_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void bw_satur_do_update_coeffs(bw_satur_coeffs *BW_RESTRICT coeffs, char force) {
	float bias_cur = bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	if (force || coeffs->bias != bias_cur) {
		bias_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_bias_state, coeffs->bias);
		coeffs->bias_dc = bw_satur_tanhf(bias_cur);
	}
	float gain_cur = bw_one_pole_get_y_z1(&coeffs->smooth_gain_state);
	if (force || coeffs->gain != gain_cur) {
		gain_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_gain_state, coeffs->gain);
		coeffs->inv_gain = bw_rcpf(gain_cur);
	}
}

static inline void bw_satur_reset_coeffs(bw_satur_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_bias_state, coeffs->bias);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_gain_state, coeffs->gain);
	bw_satur_do_update_coeffs(coeffs, 1);
}

static inline void bw_satur_reset_state(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state) {
	state->x_z1 = bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	const float ax = bw_absf(state->x_z1);
	state->F_z1 = ax >= 2.115287308554551f ? ax - 0.6847736211329452f : ax * ax * ((0.00304518315009429f * ax - 0.09167437770414569f) * ax + 0.5f);
}

static inline void bw_satur_update_coeffs_ctrl(bw_satur_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline void bw_satur_update_coeffs_audio(bw_satur_coeffs *BW_RESTRICT coeffs) {
	bw_satur_do_update_coeffs(coeffs, 0);
}

static inline float bw_satur_process1(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, float x) {
	x = bw_one_pole_get_y_z1(&coeffs->smooth_gain_state) * x + bw_one_pole_get_y_z1(&coeffs->smooth_bias_state);
	const float ax = bw_absf(x);
	const float F = ax >= 2.115287308554551f ? ax - 0.6847736211329452f : ax * ax * ((0.00304518315009429f * ax - 0.09167437770414569f) * ax + 0.5f);
	const float d = x - state->x_z1;
	const float y = d * d < 1e-6f ? bw_satur_tanhf(0.5f * (x + state->x_z1)) : (F - state->F_z1) * bw_rcpf(d);
	state->x_z1 = x;
	state->F_z1 = F;
	return y - coeffs->bias_dc;
}

static inline float bw_satur_process1_comp(const bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, float x) {
	float y = bw_satur_process1(coeffs, state, x);
	return coeffs->inv_gain * y;
}

static inline void bw_satur_process(bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples) {
	if (coeffs->gain_compensation)
		for (size_t i = 0; i < n_samples; i++) {
			bw_satur_update_coeffs_audio(coeffs);
			y[i] = bw_satur_process1_comp(coeffs, state, x[i]);
		}
	else
		for (size_t i = 0; i < n_samples; i++) {
			bw_satur_update_coeffs_audio(coeffs);
			y[i] = bw_satur_process1(coeffs, state, x[i]);
		}
}

static inline void bw_satur_process_multi(bw_satur_coeffs *BW_RESTRICT coeffs, bw_satur_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples) {
	bw_satur_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_satur_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_satur_process1(coeffs, state[j], x[j][i]);
	}
}

static inline void bw_satur_set_bias(bw_satur_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->bias = value;
}

static inline void bw_satur_set_gain(bw_satur_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->gain = value;
}

static inline void bw_satur_set_gain_compensation(bw_satur_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->gain_compensation = value;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Satur
 *  ```>>> */
template<size_t N_CHANNELS>
class Satur {
public:
	Satur();

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

	void setBias(float value);
	void setGain(float value);
	void setGainCompensation(bool value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_satur_coeffs	 coeffs;
	bw_satur_state	 states[N_CHANNELS];
	bw_satur_state	*BW_RESTRICT statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Satur<N_CHANNELS>::Satur() {
	bw_satur_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Satur<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_satur_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Satur<N_CHANNELS>::reset() {
	bw_satur_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_satur_reset_state(&coeffs, states + i);
}

template<size_t N_CHANNELS>
inline void Satur<N_CHANNELS>::process(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_satur_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Satur<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Satur<N_CHANNELS>::setBias(float value) {
	bw_satur_set_bias(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Satur<N_CHANNELS>::setGain(float value) {
	bw_satur_set_gain(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Satur<N_CHANNELS>::setGainCompensation(bool value) {
	bw_satur_set_gain_compensation(&coeffs, value);
}

}
#endif

#endif
