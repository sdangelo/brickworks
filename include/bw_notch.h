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
 *  requires {{{ bw_common bw_math bw_one_pole bw_svf }}}
 *  description {{{
 *    Second-order notch filter with unitary gain at DC and asymptotically as
 *    frequency increases, and null gain at cutoff frequency.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_notch_process()</code> and
 *              <code>bw_notch_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
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
 *          <li>Added <code>bw_notch_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_notch_reset_state()</code>.</li>
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

#ifndef BW_NOTCH_H
#define BW_NOTCH_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_notch_coeffs
 *  ```>>> */
typedef struct bw_notch_coeffs bw_notch_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_notch_state
 *  ```>>> */
typedef struct bw_notch_state bw_notch_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_notch_init()
 *  ```>>> */
static inline void bw_notch_init(bw_notch_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_notch_set_sample_rate()
 *  ```>>> */
static inline void bw_notch_set_sample_rate(bw_notch_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_notch_reset_coeffs()
 *  ```>>> */
static inline void bw_notch_reset_coeffs(bw_notch_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_notch_reset_state()
 *  ```>>> */
static inline void bw_notch_reset_state(const bw_notch_coeffs *BW_RESTRICT coeffs, bw_notch_state *BW_RESTRICT state, float x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x_0`.
 *
 *    #### bw_notch_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_notch_update_coeffs_ctrl(bw_notch_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_notch_update_coeffs_audio()
 *  ```>>> */
static inline void bw_notch_update_coeffs_audio(bw_notch_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_notch_process1()
 *  ```>>> */
static inline float bw_notch_process1(const bw_notch_coeffs *BW_RESTRICT coeffs, bw_notch_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_notch_process()
 *  ```>>> */
static inline void bw_notch_process(bw_notch_coeffs *BW_RESTRICT coeffs, bw_notch_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_notch_process_multi()
 *  ```>>> */
static inline void bw_notch_process_multi(bw_notch_coeffs *BW_RESTRICT coeffs, bw_notch_state * const *BW_RESTRICT state, const float * const *x, float **y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_notch_set_cutoff()
 *  ```>>> */
static inline void bw_notch_set_cutoff(bw_notch_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_notch_set_Q()
 *  ```>>> */
static inline void bw_notch_set_Q(bw_notch_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the quality factor to the given `value` in `coeffs`.
 *
 *    `value` must be equal or bigger than `0.5f`.
 *
 *    Default value: `0.5f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_svf.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bw_notch_coeffs {
	// Sub-components
	bw_svf_coeffs	svf_coeffs;
};

struct bw_notch_state {
	bw_svf_state	svf_state;
};

static inline void bw_notch_init(bw_notch_coeffs *BW_RESTRICT coeffs) {
	bw_svf_init(&coeffs->svf_coeffs);
}

static inline void bw_notch_set_sample_rate(bw_notch_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_svf_set_sample_rate(&coeffs->svf_coeffs, sample_rate);
}

static inline void bw_notch_reset_coeffs(bw_notch_coeffs *BW_RESTRICT coeffs) {
	bw_svf_reset_coeffs(&coeffs->svf_coeffs);
}

static inline void bw_notch_reset_state(const bw_notch_coeffs *BW_RESTRICT coeffs, bw_notch_state *BW_RESTRICT state, float x_0) {
	bw_svf_reset_state(&coeffs->svf_coeffs, &state->svf_state, x_0);
}

static inline void bw_notch_update_coeffs_ctrl(bw_notch_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_ctrl(&coeffs->svf_coeffs);
}

static inline void bw_notch_update_coeffs_audio(bw_notch_coeffs *BW_RESTRICT coeffs) {
	bw_svf_update_coeffs_audio(&coeffs->svf_coeffs);
}

static inline float bw_notch_process1(const bw_notch_coeffs *BW_RESTRICT coeffs, bw_notch_state *BW_RESTRICT state, float x) {
	float lp, bp, hp;
	bw_svf_process1(&coeffs->svf_coeffs, &state->svf_state, x, &lp, &bp, &hp);
	return lp + hp;
}

static inline void bw_notch_process(bw_notch_coeffs *BW_RESTRICT coeffs, bw_notch_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples) {
	bw_notch_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_notch_update_coeffs_audio(coeffs);
		y[i] = bw_notch_process1(coeffs, state, x[i]);
	}
}

static inline void bw_notch_process_multi(bw_notch_coeffs *BW_RESTRICT coeffs, bw_notch_state * const *BW_RESTRICT state, const float * const *x, float **y, size_t n_channels, size_t n_samples) {
	bw_notch_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_notch_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_notch_process1(coeffs, state[j], x[j][i]);
	}
}

static inline void bw_notch_set_cutoff(bw_notch_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_cutoff(&coeffs->svf_coeffs, value);
}

static inline void bw_notch_set_Q(bw_notch_coeffs *BW_RESTRICT coeffs, float value) {
	bw_svf_set_Q(&coeffs->svf_coeffs, value);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Notch
 *  ```>>> */
template<size_t N_CHANNELS>
class Notch {
public:
	Notch();

	void setSampleRate(float sampleRate);
	void reset(float x_0 = 0.f);
	void process(
		const float * const *x,
		float **y,
		size_t nSamples);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples);

	void setCutoff(float value);
	void setQ(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_notch_coeffs	 coeffs;
	bw_notch_state	 states[N_CHANNELS];
	bw_notch_state	*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Notch<N_CHANNELS>::Notch() {
	bw_notch_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Notch<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_notch_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Notch<N_CHANNELS>::reset(float x_0) {
	bw_notch_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_notch_reset_state(&coeffs, states + i, x_0);
}

template<size_t N_CHANNELS>
inline void Notch<N_CHANNELS>::process(
		const float * const *x,
		float **y,
		size_t nSamples) {
	bw_notch_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Notch<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Notch<N_CHANNELS>::setCutoff(float value) {
	bw_notch_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Notch<N_CHANNELS>::setQ(float value) {
	bw_notch_set_Q(&coeffs, value);
}

}
#endif

#endif
