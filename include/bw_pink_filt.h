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
 *  requires {{{ bw_common }}}
 *  description {{{
 *    Pinking filter.
 *
 *    This is a
 *    <a href="https://en.wikipedia.org/wiki/Linear_time-invariant_system"
 *    target="_blank">linear time-invariant filter</a> approximately attenuating
 *    3 dB/oct from about 0.000046 × Nyquist frequency to about 0.9 × Nyquist
 *    frequency. It can be used to turn white noise into pink noise (hence the
 *    name).
 *
 *    The filter design is based on
 *
 *    P. P. La Pastina and S. D'Angelo, "Optimal Integer Order Approximation of
 *    Fractional Order Filters", Proc. 24th Intl. Conf. Digital Audio Effects
 *    (DAFx20in21), pp. 89-96, Vienna, Austria, September 2021.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_pink_filt_process()</code> and
 *              <code>bw_pink_filt_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
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
 *          <li>Added <code>bw_pink_filt_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Fixed unused parameter warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Fixed <code>bw_pink_filt_set_sample_rate_scaling()</code>
 *              definition.</li>
 *          <li>Added missing documentation for
 *              <code>bw_pink_filt_coeffs</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>Refactored API.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.1.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_PINK_FILT_H
#define BW_PINK_FILT_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_pink_filt_coeffs
 *  ```>>> */
typedef struct bw_pink_filt_coeffs bw_pink_filt_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_pink_filt_state
 *  ```>>> */
typedef struct bw_pink_filt_state bw_pink_filt_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_pink_filt_init()
 *  ```>>> */
static inline void bw_pink_filt_init(bw_pink_filt_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_pink_filt_set_sample_rate()
 *  ```>>> */
static inline void bw_pink_filt_set_sample_rate(bw_pink_filt_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_pink_filt_reset_state()
 *  ```>>> */
static inline void bw_pink_filt_reset_state(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_pink_filt_process1()
 *  ```>>> */
static inline float bw_pink_filt_process1(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, float x);
static inline float bw_pink_filt_process1_scaling(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, float x);
/*! <<<```
 *    These function process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_pink_filt_process1()` assumes that sample rate scaling is disabled;
 *     * `bw_pink_filt_process1_scaling()` assumes that sample rate scaling is
 *       enabled.
 *
 *    #### bw_pink_filt_process()
 *  ```>>> */
static inline void bw_pink_filt_process(bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using `coeffs` and
 *    both using and updating `state`.
 * 
 *    #### bw_pink_filt_process_multi()
 *  ```>>> */
static inline void bw_pink_filt_process_multi(bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using the common `coeffs` and both using and updating each of the
 *    `n_channels` `state`s.
 *
 *    #### bw_pink_filt_set_sample_rate_scaling()
 *  ```>>> */
static inline void bw_pink_filt_set_sample_rate_scaling(bw_pink_filt_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the output should be scaled (`value` non-`0`) or not (`0`)
 *    according to the sample rate in `coeffs`.
 *
 *    In order to keep the magnitude response consistent at different sample
 *    rates, the output of this filter should be accordingly scaled. The 44100
 *    Hz sample rate is used as a reference (that is, the scaling factor at that
 *    sample rate is `1.f`).
 *
 *    Default value: `0` (off).
 *
 *    #### bw_pink_filt_get_scaling_k()
 *  ```>>> */
static inline float bw_pink_filt_get_scaling_k(const bw_pink_filt_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the sample rate scaling factor that is applied or would be applied
 *    if sample rate scaling were enabled, as stored in `coeffs`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#ifdef __cplusplus
extern "C" {
#endif

struct bw_pink_filt_coeffs {
	// Coefficients
	float	scaling_k;
	
	// Parameters
	float	sample_rate_scaling;
};

struct bw_pink_filt_state {
	float	s1_z1;
	float	s2_z1;
	float	s3_z1;
	float	s4_z1;
};

static inline void bw_pink_filt_init(bw_pink_filt_coeffs *BW_RESTRICT coeffs) {
	coeffs->sample_rate_scaling = 0;
}

static inline void bw_pink_filt_set_sample_rate(bw_pink_filt_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->scaling_k = 210.f / bw_sqrtf(sample_rate);
}

static inline void bw_pink_filt_reset_state(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state) {
	(void)coeffs;
	state->s1_z1 = 0.f;
	state->s2_z1 = 0.f;
	state->s3_z1 = 0.f;
	state->s4_z1 = 0.f;
}

static inline float bw_pink_filt_process1(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, float x) {
	(void)coeffs;
	const float s1 = 0.320696754235142f * x + state->s1_z1;
	state->s1_z1 = 0.999760145116749f * s1 - 0.3204568993518913f * x;
	const float s2 = 0.2870206617007935f * s1 + state->s2_z1;
	state->s2_z1 = 0.9974135207366259f * s2 - 0.2844341824374191f * s1;
	const float s3 = 0.2962862885898576f * s2 + state->s3_z1;
	state->s3_z1 = 0.9687905029568185f * s3 - 0.265076791546676f * s2;
	const float s4 = 0.3882183163519794f * s3 + state->s4_z1;
	state->s4_z1 = 0.6573784623288251f * s4 - 0.04559677868080467 * s3;
	return s4;
}

static inline float bw_pink_filt_process1_scaling(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, float x) {
	(void)coeffs;
	return coeffs->scaling_k * bw_pink_filt_process1(coeffs, state, x);
}

static inline void bw_pink_filt_process(bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, const float *x, float* y, size_t n_samples) {
	if (coeffs->sample_rate_scaling)
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_pink_filt_process1_scaling(coeffs, state, x[i]);
	else
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_pink_filt_process1(coeffs, state, x[i]);
}

static inline void bw_pink_filt_process_multi(bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples) {
	for (size_t i = 0; i < n_channels; i++)
		bw_pink_filt_process(coeffs, state[i], x[i], y[i], n_samples);
}

static inline void bw_pink_filt_set_sample_rate_scaling(bw_pink_filt_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->sample_rate_scaling = value;
}

static inline float bw_pink_filt_get_scaling_k(const bw_pink_filt_coeffs *BW_RESTRICT coeffs) {
	return coeffs->scaling_k;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::PinkFilt
 *  ```>>> */
template<size_t N_CHANNELS>
class PinkFilt {
public:
	PinkFilt();

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

	void setSampleRateScaling(bool value);
	
	float getScalingK();
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_pink_filt_coeffs	 coeffs;
	bw_pink_filt_state	 states[N_CHANNELS];
	bw_pink_filt_state	*BW_RESTRICT statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline PinkFilt<N_CHANNELS>::PinkFilt() {
	bw_pink_filt_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_pink_filt_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::reset() {
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_pink_filt_reset_state(&coeffs, states + i);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::process(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_pink_filt_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void PinkFilt<N_CHANNELS>::setSampleRateScaling(bool value) {
	bw_pink_filt_set_sample_rate_scaling(&coeffs, value);
}

template<size_t N_CHANNELS>
inline float PinkFilt<N_CHANNELS>::getScalingK() {
	return bw_pink_filt_get_scaling_k(&coeffs);
}

}
#endif

#endif
