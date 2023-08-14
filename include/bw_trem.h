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
 *    bw_common bw_math bw_one_pole bw_osc_sin bw_phase_gen bw_ring_mod
 *  }}}
 *  description {{{
 *    Tremolo with variable speed and amount.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_trem_process()</code> and
 *              <code>bw_trem_process_multi()</code> now use <code>size_t</code>
 *              to count samples and channels.</li>
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
 *          <li>Added <code>bw_trem_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_TREM_H
#define BW_TREM_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_trem_coeffs
 *  ```>>> */
typedef struct bw_trem_coeffs bw_trem_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_trem_state
 *  ```>>> */
typedef struct bw_trem_state bw_trem_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_trem_init()
 *  ```>>> */
static inline void bw_trem_init(bw_trem_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_trem_set_sample_rate()
 *  ```>>> */
static inline void bw_trem_set_sample_rate(bw_trem_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_trem_reset_coeffs()
 *  ```>>> */
static inline void bw_trem_reset_coeffs(bw_trem_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_trem_reset_state()
 *  ```>>> */
static inline void bw_trem_reset_state(const bw_trem_coeffs *BW_RESTRICT coeffs, bw_trem_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_trem_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_trem_update_coeffs_ctrl(bw_trem_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_trem_update_coeffs_audio()
 *  ```>>> */
static inline void bw_trem_update_coeffs_audio(bw_trem_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_trem_process1()
 *  ```>>> */
static inline float bw_trem_process1(const bw_trem_coeffs *BW_RESTRICT coeffs, bw_trem_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_trem_process()
 *  ```>>> */
static inline void bw_trem_process(bw_trem_coeffs *BW_RESTRICT coeffs, bw_trem_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_trem_process_multi()
 *  ```>>> */
static inline void bw_trem_process_multi(bw_trem_coeffs *BW_RESTRICT coeffs, bw_trem_state * const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_trem_set_rate()
 *  ```>>> */
static inline void bw_trem_set_rate(bw_trem_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the modulation rate `value` (Hz) in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_trem_set_amount()
 *  ```>>> */
static inline void bw_trem_set_amount(bw_trem_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the amount parameter to the given `value` (`0.f` = no tremolo, `1.f`
 *    = full tremolo, `-1.f` = full tremolo with inverted polarity) in `coeffs`.
 *
 *    Default value: `1.f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_phase_gen.h>
#include <bw_osc_sin.h>
#include <bw_ring_mod.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bw_trem_coeffs {
	// Sub-components
	bw_phase_gen_coeffs	phase_gen_coeffs;
	bw_ring_mod_coeffs	ring_mod_coeffs;
};

struct bw_trem_state {
	bw_phase_gen_state	phase_gen_state;
};

static inline void bw_trem_init(bw_trem_coeffs *BW_RESTRICT coeffs) {
	bw_phase_gen_init(&coeffs->phase_gen_coeffs);
	bw_ring_mod_init(&coeffs->ring_mod_coeffs);
}

static inline void bw_trem_set_sample_rate(bw_trem_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_phase_gen_set_sample_rate(&coeffs->phase_gen_coeffs, sample_rate);
	bw_ring_mod_set_sample_rate(&coeffs->ring_mod_coeffs, sample_rate);
}

static inline void bw_trem_reset_coeffs(bw_trem_coeffs *BW_RESTRICT coeffs) {
	bw_phase_gen_reset_coeffs(&coeffs->phase_gen_coeffs);
	bw_ring_mod_reset_coeffs(&coeffs->ring_mod_coeffs);
}

static inline void bw_trem_reset_state(const bw_trem_coeffs *BW_RESTRICT coeffs, bw_trem_state *BW_RESTRICT state) {
	bw_phase_gen_reset_state(&coeffs->phase_gen_coeffs, &state->phase_gen_state, 0.f);
}

static inline void bw_trem_update_coeffs_ctrl(bw_trem_coeffs *BW_RESTRICT coeffs) {
	bw_phase_gen_update_coeffs_ctrl(&coeffs->phase_gen_coeffs);
	bw_ring_mod_update_coeffs_ctrl(&coeffs->ring_mod_coeffs);
}

static inline void bw_trem_update_coeffs_audio(bw_trem_coeffs *BW_RESTRICT coeffs) {
	bw_phase_gen_update_coeffs_audio(&coeffs->phase_gen_coeffs);
	bw_ring_mod_update_coeffs_audio(&coeffs->ring_mod_coeffs);
}

static inline float bw_trem_process1(const bw_trem_coeffs *BW_RESTRICT coeffs, bw_trem_state *BW_RESTRICT state, float x) {
	float p, pi;
	bw_phase_gen_process1(&coeffs->phase_gen_coeffs, &state->phase_gen_state, &p, &pi);
	const float c = bw_osc_sin_process1(p);
	return bw_ring_mod_process1(&coeffs->ring_mod_coeffs, x, 1.f + c);
}

static inline void bw_trem_process(bw_trem_coeffs *BW_RESTRICT coeffs, bw_trem_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples) {
	bw_trem_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_trem_update_coeffs_audio(coeffs);
		y[i] = bw_trem_process1(coeffs, state, x[i]);
	}
}

static inline void bw_trem_process_multi(bw_trem_coeffs *BW_RESTRICT coeffs, bw_trem_state * const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples) {
	bw_trem_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_trem_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_trem_process1(coeffs, state[j], x[j][i]);
	}
}

static inline void bw_trem_set_rate(bw_trem_coeffs *BW_RESTRICT coeffs, float value) {
	bw_phase_gen_set_frequency(&coeffs->phase_gen_coeffs, value);
}

static inline void bw_trem_set_amount(bw_trem_coeffs *BW_RESTRICT coeffs, float value) {
	bw_ring_mod_set_amount(&coeffs->ring_mod_coeffs, value);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Trem
 *  ```>>> */
template<size_t N_CHANNELS>
class Trem {
public:
	Trem();

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

	void setRate(float value);
	void setAmount(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_trem_coeffs	 coeffs;
	bw_trem_state		 states[N_CHANNELS];
	bw_trem_state		*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Trem<N_CHANNELS>::Trem() {
	bw_trem_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Trem<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_trem_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Trem<N_CHANNELS>::reset() {
	bw_trem_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_trem_reset_state(&coeffs, states + i);
}

template<size_t N_CHANNELS>
inline void Trem<N_CHANNELS>::process(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_trem_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Trem<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Trem<N_CHANNELS>::setRate(float value) {
	bw_trem_set_rate(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Trem<N_CHANNELS>::setAmount(float value) {
	bw_trem_set_amount(&coeffs, value);
}

}
#endif

#endif
