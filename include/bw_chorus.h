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
 *    bw_buf bw_comb bw_common bw_delay bw_gain bw_math bw_one_pole bw_osc_sin
 *    bw_phase_gen
 *  }}}
 *  description {{{
 *    Chorus / vibrato / flanger with variable rate and amount.
 *
 *    It outputs a mix of the dry input signal with itself going through a
 *    modulated delay and an optional feedback, as explained in
 *
 *    J. Dattorro, "Effect Design, Part 2: Delay-Line Modulation and Chorus",
 *    J. Audio Eng. Soc., vol. 45, no. 10, pp. 764-788, October 1997.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added initial value argument in
 *              <code>bw_chorus_reset_state()</code>.</li>
 *          <li>Added overladed C++ <code>reset()</code> functions taking arrays
 *              as arguments.</li>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *          <li><code>bw_chorus_process()</code> and
 *              <code>bw_chorus_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implmenetation.</li>
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
 *          <li>Added <code>bw_chorus_process_multi()</code>.</li>
 *          <li>Updated mem_req/set API.</li>
 *          <li>Now properly setting feedforward delay on reset.</li>
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

#ifndef BW_CHORUS_H
#define BW_CHORUS_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_chorus_coeffs
 *  ```>>> */
typedef struct bw_chorus_coeffs bw_chorus_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_chorus_state
 *  ```>>> */
typedef struct bw_chorus_state bw_chorus_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_chorus_init()
 *  ```>>> */
static inline void bw_chorus_init(bw_chorus_coeffs *BW_RESTRICT coeffs, float max_delay);
/*! <<<```
 *    Initializes input parameter values in `coeffs` using `max_delay` (s) as
 *    the maximum delay time.
 *
 *    #### bw_chorus_set_sample_rate()
 *  ```>>> */
static inline void bw_chorus_set_sample_rate(bw_chorus_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_chorus_mem_req()
 *  ```>>> */
static inline size_t bw_chorus_mem_req(const bw_chorus_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the size, in bytes, of contiguous memory to be supplied to
 *    `bw_chorus_mem_set()` using `coeffs`.
 *
 *    #### bw_chorus_mem_set()
 *  ```>>> */
static inline void bw_chorus_mem_set(const bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT state, void *BW_RESTRICT mem);
/*! <<<```
 *    Associates the contiguous memory block `mem` to the given `state`.
 *
 *    #### bw_chorus_reset_coeffs()
 *  ```>>> */
static inline void bw_chorus_reset_coeffs(bw_chorus_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_chorus_reset_state()
 *  ```>>> */
static inline void bw_chorus_reset_state(const bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT state, float x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x_0`.
 *
 *    #### bw_chorus_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_chorus_update_coeffs_ctrl(bw_chorus_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_chorus_update_coeffs_audio()
 *  ```>>> */
static inline void bw_chorus_update_coeffs_audio(bw_chorus_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_chorus_process1()
 *  ```>>> */
static inline float bw_chorus_process1(const bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_chorus_process()
 *  ```>>> */
static inline void bw_chorus_process(bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_chorus_process_multi()
 *  ```>>> */
static inline void bw_chorus_process_multi(bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_chorus_set_rate()
 *  ```>>> */
static inline void bw_chorus_set_rate(bw_chorus_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the modulation rate `value` (Hz) in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_chorus_set_delay()
 *  ```>>> */
static inline void bw_chorus_set_delay(bw_chorus_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the center delay `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_chorus_set_amount()
 *  ```>>> */
static inline void bw_chorus_set_amount(bw_chorus_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the modulation amount (semi-amplitude) `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_chorus_set_coeff_x()
 *  ```>>> */
static inline void bw_chorus_set_coeff_x(bw_chorus_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the input coefficient `value` in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_chorus_set_coeff_mod()
 *  ```>>> */
static inline void bw_chorus_set_coeff_mod(bw_chorus_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the modulated branch coefficient `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_chorus_set_coeff_fb()
 *  ```>>> */
static inline void bw_chorus_set_coeff_fb(bw_chorus_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the feedback branch coefficient `value` in `coeffs`.
 *
 *    Default value: `0.f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_phase_gen.h>
#include <bw_osc_sin.h>
#include <bw_comb.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bw_chorus_coeffs {
	// Sub-components
	bw_phase_gen_coeffs	phase_gen_coeffs;
	bw_phase_gen_state	phase_gen_state;
	bw_comb_coeffs		comb_coeffs;

	// Parameters
	float			delay;
	float			amount;
};

struct bw_chorus_state {
	// Sub-components
	bw_comb_state		comb_state;
};

static inline void bw_chorus_init(bw_chorus_coeffs *BW_RESTRICT coeffs, float max_delay) {
	bw_phase_gen_init(&coeffs->phase_gen_coeffs);
	bw_comb_init(&coeffs->comb_coeffs, max_delay);
	coeffs->delay = 0.f;
	coeffs->amount = 0.f;
}

static inline void bw_chorus_set_sample_rate(bw_chorus_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_phase_gen_set_sample_rate(&coeffs->phase_gen_coeffs, sample_rate);
	bw_comb_set_sample_rate(&coeffs->comb_coeffs, sample_rate);
}

static inline size_t bw_chorus_mem_req(const bw_chorus_coeffs *BW_RESTRICT coeffs) {
	return bw_comb_mem_req(&coeffs->comb_coeffs);
}

static inline void bw_chorus_mem_set(const bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT state, void *BW_RESTRICT mem) {
	bw_comb_mem_set(&coeffs->comb_coeffs, &state->comb_state, mem);
}

static inline void bw_chorus_reset_coeffs(bw_chorus_coeffs *BW_RESTRICT coeffs) {
	bw_phase_gen_reset_coeffs(&coeffs->phase_gen_coeffs);
	bw_phase_gen_reset_state(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state, 0.f);
	bw_comb_set_delay_ff(&coeffs->comb_coeffs, coeffs->delay);
	bw_comb_reset_coeffs(&coeffs->comb_coeffs);
}

static inline void bw_chorus_reset_state(const bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT state, float x_0) {
	bw_comb_reset_state(&coeffs->comb_coeffs, &state->comb_state, x_0);
}

static inline void bw_chorus_update_coeffs_ctrl(bw_chorus_coeffs *BW_RESTRICT coeffs) {
	bw_phase_gen_update_coeffs_ctrl(&coeffs->phase_gen_coeffs);
	bw_comb_update_coeffs_ctrl(&coeffs->comb_coeffs);
}

static inline void bw_chorus_update_coeffs_audio(bw_chorus_coeffs *BW_RESTRICT coeffs) {
	bw_phase_gen_update_coeffs_audio(&coeffs->phase_gen_coeffs);
	float p, pi;
	bw_phase_gen_process1(&coeffs->phase_gen_coeffs, &coeffs->phase_gen_state, &p, &pi);
	const float mod = coeffs->delay + coeffs->amount * bw_osc_sin_process1(p);
	bw_comb_set_delay_ff(&coeffs->comb_coeffs, mod);
	bw_comb_update_coeffs_ctrl(&coeffs->comb_coeffs);
	bw_comb_update_coeffs_audio(&coeffs->comb_coeffs);
}

static inline float bw_chorus_process1(const bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT state, float x) {
	return bw_comb_process1(&coeffs->comb_coeffs, &state->comb_state, x);
}

static inline void bw_chorus_process(bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples) {
	bw_chorus_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_chorus_update_coeffs_audio(coeffs);
		y[i] = bw_chorus_process1(coeffs, state, x[i]);
	}
}

static inline void bw_chorus_process_multi(bw_chorus_coeffs *BW_RESTRICT coeffs, bw_chorus_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples) {
	bw_chorus_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_chorus_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_chorus_process1(coeffs, state[j], x[j][i]);
	}
}

static inline void bw_chorus_set_rate(bw_chorus_coeffs *BW_RESTRICT coeffs, float value) {
	bw_phase_gen_set_frequency(&coeffs->phase_gen_coeffs, value);
}

static inline void bw_chorus_set_delay(bw_chorus_coeffs *BW_RESTRICT coeffs, float value) {
	bw_comb_set_delay_fb(&coeffs->comb_coeffs, value);
	coeffs->delay = value;
}

static inline void bw_chorus_set_amount(bw_chorus_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->amount = value;
}

static inline void bw_chorus_set_coeff_x(bw_chorus_coeffs *BW_RESTRICT coeffs, float value) {
	bw_comb_set_coeff_blend(&coeffs->comb_coeffs, value);
}

static inline void bw_chorus_set_coeff_mod(bw_chorus_coeffs *BW_RESTRICT coeffs, float value) {
	bw_comb_set_coeff_ff(&coeffs->comb_coeffs, value);
}

static inline void bw_chorus_set_coeff_fb(bw_chorus_coeffs *BW_RESTRICT coeffs, float value) {
	bw_comb_set_coeff_fb(&coeffs->comb_coeffs, value);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Chorus
 *  ```>>> */
template<size_t N_CHANNELS>
class Chorus {
public:
	Chorus(float maxDelay = 0.01f);
	~Chorus();

	void setSampleRate(float sampleRate);
	void reset(float x_0 = 0.f);
	void reset(const float *BW_RESTRICT x_0);
	void reset(const std::array<float, N_CHANNELS> x_0);
	void process(
		const float * const *x,
		float * const *y,
		size_t nSamples);
	void process(
		const std::array<const float *, N_CHANNELS> x,
		const std::array<float *, N_CHANNELS> y,
		size_t nSamples);

	void setRate(float value);
	void setDelay(float value);
	void setAmount(float value);
	void setCoeffX(float value);
	void setCoeffMod(float value);
	void setCoeffFB(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_chorus_coeffs	 coeffs;
	bw_chorus_state		 states[N_CHANNELS];
	bw_chorus_state		*BW_RESTRICT statesP[N_CHANNELS];
	void			*BW_RESTRICT mem;
};

template<size_t N_CHANNELS>
inline Chorus<N_CHANNELS>::Chorus(float maxDelay) {
	bw_chorus_init(&coeffs, maxDelay);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
	mem = nullptr;
}

template<size_t N_CHANNELS>
inline Chorus<N_CHANNELS>::~Chorus() {
	if (mem != nullptr)
		operator delete(mem);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_chorus_set_sample_rate(&coeffs, sampleRate);
	size_t req = bw_chorus_mem_req(&coeffs);
	if (mem != nullptr)
		operator delete(mem);
	mem = operator new(req * N_CHANNELS);
	void *m = mem;
	for (size_t i = 0; i < N_CHANNELS; i++, m = static_cast<char *>(m) + req)
		bw_chorus_mem_set(&coeffs, states + i, m);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::reset(float x_0) {
	bw_chorus_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_chorus_reset_state(&coeffs, states + i, x_0);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::reset(const float *BW_RESTRICT x_0) {
	bw_chorus_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_chorus_reset_state(&coeffs, states + i, x_0[i]);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::reset(const std::array<float, N_CHANNELS> x_0) {
	reset(x_0.data());
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::process(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_chorus_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::process(
		const std::array<const float *, N_CHANNELS> x,
		const std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setRate(float value) {
	bw_chorus_set_rate(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setDelay(float value) {
	bw_chorus_set_delay(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setAmount(float value) {
	bw_chorus_set_amount(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setCoeffX(float value) {
	bw_chorus_set_coeff_x(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setCoeffMod(float value) {
	bw_chorus_set_coeff_mod(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Chorus<N_CHANNELS>::setCoeffFB(float value) {
	bw_chorus_set_coeff_fb(&coeffs, value);
}

}
#endif

#endif
