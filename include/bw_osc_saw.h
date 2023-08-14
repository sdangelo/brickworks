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
 *  requires {{{ bw_common bw_math }}}
 *  description {{{
 *    Sawtooth oscillator waveshaper with PolyBLEP antialiasing.
 *
 *    It turns a normalized phase signal, such as that geneated by
 *    [bw\_phase\_gen](bw_phase_gen), into a sawtooth wave.
 *
 *    The antialiasing algorithm is based on
 *
 *    V. Valimaki and A. Huovilainen, "Antialiasing Oscillators in Subtractive
 *    Synthesis", IEEE Signal Processing Magazine, vol. 24, no. 2, pp. 116-125,
 *    March 2007.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_osc_saw_process()</code> and
 *              <code>bw_osc_saw_process_multi()</code> now use
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
 *          <li>Added <code>bw_osc_saw_process_multi()</code>.</li>
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

#ifndef BW_OSC_SAW_H
#define BW_OSC_SAW_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_osc_saw_coeffs
 *  ```>>> */
typedef struct bw_osc_saw_coeffs bw_osc_saw_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_osc_saw_init()
 *  ```>>> */
static inline void bw_osc_saw_init(bw_osc_saw_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_osc_saw_process1\*()
 *  ```>>> */
static inline float bw_osc_saw_process1(const bw_osc_saw_coeffs *BW_RESTRICT coeffs, float x);
static inline float bw_osc_saw_process1_antialias(const bw_osc_saw_coeffs *BW_RESTRICT coeffs, float x, float x_phase_inc);
/*! <<<```
 *    These function process one input sample `x`, indicating the normalized
 *    phase, using `coeffs`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_osc_saw_process1()` assumes that antialiasing is disabled;
 *     * `bw_osc_saw_process1_antialias()` assumes that antialiasing is enabled
 *       and requires the corresponding phase increment value to be passed via
 *       `x_phase_inc`.
 *
 *    #### bw_osc_saw_process()
 *  ```>>> */
static inline void bw_osc_saw_process(bw_osc_saw_coeffs *BW_RESTRICT coeffs, const float *x, const float *x_phase_inc, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x`, containing the
 *    normalized phase signal, and fills the first `n_samples` of the output
 *    buffer `y`, while using `coeffs`.
 *
 *    If antialiasing is enabled, `x_phase_inc` must contain phase increment
 *    values, otherwise it is ignored and can be `NULL`.
 *
 *    #### bw_osc_saw_process_multi()
 *  ```>>> */
static inline void bw_osc_saw_process_multi(bw_osc_saw_coeffs *BW_RESTRICT coeffs, const float * const *x, const float * const *x_phase_inc, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x`,
 *    containing the normalized phase signals, and fills the first `n_samples`
 *    of the `n_samples` output buffers `y`, while using `coeffs`.
 *
 *    If antialiasing is enabled, each of the `n_channels` buffers pointed by
 *    `x_phase_inc` must contain phase increment values, otherwise `x_phase_inc`
 *    is ignored and can be `NULL`.
 *
 *    #### bw_osc_saw_set_antialiasing()
 *  ```>>> */
static inline void bw_osc_saw_set_antialiasing(bw_osc_saw_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the antialiasing is on (`value` non-`0`) or off (`0`) in
 *    `coeffs`.
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

#ifdef __cplusplus
extern "C" {
#endif

struct bw_osc_saw_coeffs {
	// Parameters
	char	antialiasing;
};

static inline void bw_osc_saw_init(bw_osc_saw_coeffs *BW_RESTRICT coeffs) {
	coeffs->antialiasing = 0;
}

static inline float bw_osc_saw_process1(const bw_osc_saw_coeffs *BW_RESTRICT coeffs, float x) {
	(void)coeffs;
	return x + x - 1.f;
}

// PolyBLEP residual based on Parzen window (4th-order B-spline), one-sided (x in [0, 2])
static inline float bw_osc_saw_blep_diff(float x) {
	return x < 1.f
		? x * ((0.25f * x - 0.6666666666666666f) * x * x + 1.333333333333333f) - 1.f
		: x * (x * ((0.6666666666666666f - 0.08333333333333333f * x) * x - 2.f) + 2.666666666666667f) - 1.333333333333333f;
}

static inline float bw_osc_saw_process1_antialias(const bw_osc_saw_coeffs *BW_RESTRICT coeffs, float x, float x_phase_inc) {
	(void)coeffs;
	const float s_1_m_phase = 1.f - x;
	float v = x - s_1_m_phase;
	if (x_phase_inc != 0.f) {
		const float phase_inc_2 = x_phase_inc + x_phase_inc;
		const float phase_inc_rcp = bw_rcpf(x_phase_inc);
		if (s_1_m_phase < phase_inc_2)
			v += bw_osc_saw_blep_diff(s_1_m_phase * phase_inc_rcp);
		if (x < phase_inc_2)
			v -= bw_osc_saw_blep_diff(x * phase_inc_rcp);
	}
	return v;
}

static inline void bw_osc_saw_process(bw_osc_saw_coeffs *BW_RESTRICT coeffs, const float *x, const float *x_phase_inc, float *y, size_t n_samples) {
	if (coeffs->antialiasing)
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_osc_saw_process1_antialias(coeffs, x[i], x_phase_inc[i]);
	else
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_osc_saw_process1(coeffs, x[i]);
}

static inline void bw_osc_saw_process_multi(bw_osc_saw_coeffs *BW_RESTRICT coeffs, const float * const *x, const float * const *x_phase_inc, float * const *y, size_t n_channels, size_t n_samples) {
	if (x_phase_inc != NULL)
		for (size_t i = 0; i < n_channels; i++)
			bw_osc_saw_process(coeffs, x[i], x_phase_inc[i], y[i], n_samples);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_osc_saw_process(coeffs, x[i], NULL, y[i], n_samples);
}

static inline void bw_osc_saw_set_antialiasing(bw_osc_saw_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->antialiasing = value;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::OscSaw
 *  ```>>> */
template<size_t N_CHANNELS>
class OscSaw {
public:
	OscSaw();
	
	void process(
		const float * const *x,
		const float * const *x_phase_inc,
		float * const *y,
		size_t nSamples);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> x_phase_inc,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples);
	
	void setAntialiasing(bool value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_osc_saw_coeffs	 coeffs;
};

template<size_t N_CHANNELS>
inline OscSaw<N_CHANNELS>::OscSaw() {
	bw_osc_saw_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void OscSaw<N_CHANNELS>::process(
		const float * const *x,
		const float * const *x_phase_inc,
		float * const *y,
		size_t nSamples) {
	bw_osc_saw_process_multi(&coeffs, x, x_phase_inc, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void OscSaw<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> x_phase_inc,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), x_phase_inc.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void OscSaw<N_CHANNELS>::setAntialiasing(bool value) {
	bw_osc_saw_set_antialiasing(&coeffs, value);
}

}
#endif

#endif
