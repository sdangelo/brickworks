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
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    Ring modulator with variable modulation amount.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Module renamed as bw_ring_mod.</li>
 *          <li><code>bw_ring_mod_process()</code> and
 *              <code>bw_ring_mod_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
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
 *          <li>Added <code>bw_ringmod_process_multi()</code>.</li>
 *          <li>Fixed inverted-polarity modulation.</li>
 *          <li>"modulator signal" -> "modulation signal" in documentation.</li>
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

#ifndef BW_RINGMOD_H
#define BW_RINGMOD_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_ring_mod_coeffs
 *  ```>>> */
typedef struct bw_ring_mod_coeffs bw_ring_mod_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_ring_mod_init()
 *  ```>>> */
static inline void bw_ring_mod_init(bw_ring_mod_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_ring_mod_set_sample_rate()
 *  ```>>> */
static inline void bw_ring_mod_set_sample_rate(bw_ring_mod_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_ring_mod_reset_coeffs()
 *  ```>>> */
static inline void bw_ring_mod_reset_coeffs(bw_ring_mod_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_ring_mod_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_ring_mod_update_coeffs_ctrl(bw_ring_mod_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ring_mod_update_coeffs_audio()
 *  ```>>> */
static inline void bw_ring_mod_update_coeffs_audio(bw_ring_mod_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_ring_mod_process1()
 *  ```>>> */
static inline float bw_ring_mod_process1(const bw_ring_mod_coeffs *BW_RESTRICT coeffs, float x_mod, float x_car);
/*! <<<```
 *    Processes one modulation input sample `x_mod` and one carrier input sample
 *    `x_car` using `coeffs` and returns the corresponding output sample.
 *
 *    #### bw_ring_mod_process()
 *  ```>>> */
static inline void bw_ring_mod_process(bw_ring_mod_coeffs *BW_RESTRICT coeffs, const float *x_mod, const float *x_car, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the modulation input buffer `x_mod` and
 *    of the carrier input buffer `x_car` and fills the first `n_samples` of the
 *    output buffer `y`, while using and updating `coeffs` (control and audio
 *    rate).
 *
 *    #### bw_ring_mod_process_multi()
 *  ```>>> */
static inline void bw_ring_mod_process_multi(bw_ring_mod_coeffs *BW_RESTRICT coeffs, const float * const *x_mod, const float * const *x_car, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` modulation input
 *    buffers `x_mod` and of the `n_channels` carrier input buffers `x_car`, and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating the common `coeffs` (control and audio rate).
 *
 *    #### bw_ring_mod_set_amount()
 *  ```>>> */
static inline void bw_ring_mod_set_amount(bw_ring_mod_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the modulation amount parameter to the given `value` (`0.f` = no
 *    modulation, `1.f` = full modulation, `-1.f` = full modulation with
 *    inverted polarity) in `coeffs`.
 *
 *    Default value: `1.f`.
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

struct bw_ring_mod_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_state;

	// Parameters
	float			mod_amount;
};

static inline void bw_ring_mod_init(bw_ring_mod_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->mod_amount = 1.f;
}

static inline void bw_ring_mod_set_sample_rate(bw_ring_mod_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
}

static inline void bw_ring_mod_reset_coeffs(bw_ring_mod_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->mod_amount);
}

static inline void bw_ring_mod_update_coeffs_ctrl(bw_ring_mod_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline void bw_ring_mod_update_coeffs_audio(bw_ring_mod_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->mod_amount);
}

static inline float bw_ring_mod_process1(const bw_ring_mod_coeffs *BW_RESTRICT coeffs, float x_mod, float x_car) {
	const float k = bw_one_pole_get_y_z1(&coeffs->smooth_state);
	return k * x_car * x_mod + bw_absf(1.f - k) * x_mod;
}

static inline void bw_ring_mod_process(bw_ring_mod_coeffs *BW_RESTRICT coeffs, const float *x_mod, const float *x_car, float *y, size_t n_samples) {
	for (size_t i = 0; i < n_samples; i++) {
		bw_ring_mod_update_coeffs_audio(coeffs);
		y[i] = bw_ring_mod_process1(coeffs, x_mod[i], x_car[i]);
	}
}

static inline void bw_ring_mod_process_multi(bw_ring_mod_coeffs *BW_RESTRICT coeffs, const float * const *x_mod, const float * const *x_car, float * const *y, size_t n_channels, size_t n_samples) {
	for (size_t i = 0; i < n_samples; i++) {
		bw_ring_mod_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_ring_mod_process1(coeffs, x_mod[j][i], x_car[j][i]);
	}
}

static inline void bw_ring_mod_set_amount(bw_ring_mod_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->mod_amount = value;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::RingMod
 *  ```>>> */
template<size_t N_CHANNELS>
class RingMod {
public:
	RingMod();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		const float * const *x_mod,
		const float * const *x_car,
		float * const *y,
		size_t nSamples);
	void process(
		std::array<const float *, N_CHANNELS> x_mod,
		std::array<const float *, N_CHANNELS> x_car,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples);

	void setAmount(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_ring_mod_coeffs	 coeffs;
};

template<size_t N_CHANNELS>
inline RingMod<N_CHANNELS>::RingMod() {
	bw_ring_mod_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_ring_mod_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::reset() {
	bw_ring_mod_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::process(
		const float * const *x_mod,
		const float * const *x_car,
		float * const *y,
		size_t nSamples) {
	bw_ring_mod_process_multi(&coeffs, x_mod, x_car, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x_mod,
		std::array<const float *, N_CHANNELS> x_car,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x_mod.data(), x_car.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void RingMod<N_CHANNELS>::setAmount(float value) {
	bw_ring_mod_set_amount(&coeffs, value);
}

}
#endif

#endif
