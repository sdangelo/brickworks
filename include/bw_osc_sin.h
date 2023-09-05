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
 *    Sinusoidal oscillator waveshaper.
 * 
 *    It turns a normalized phase signal, such as that geneated by
 *    [bw\_phase\_gen](bw_phase_gen), into a sinusoidal wave.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_osc_sin_process()</code> and
 *              <code>bw_osc_sin_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>oscSinProcess()</code> function taking
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
 *          <li>Added <code>bw_osc_sin_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
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

#ifndef BW_OSC_SIN_H
#define BW_OSC_SIN_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_osc_sin_process1()
 *  ```>>> */
static inline float bw_osc_sin_process1(float x);
/*! <<<```
 *    Processes one input sample `x`, indicating the normalized phase, and
 *    returns the corresponding output
 *    sample.
 *
 *    #### bw_osc_sin_process()
 *  ```>>> */
static inline void bw_osc_sin_process(const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x`, containing the
 *    normalized phase signal, and fills the first `n_samples` of the output
 *    buffer `y`.
 *
 *    #### bw_osc_sin_process_multi()
 *  ```>>> */
static inline void bw_osc_sin_process_multi(const float * const *x, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x`,
 *    containing the normalized phase signals, and fills the first `n_samples`
 *    of the `n_channels` output buffers `y`.
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

static inline float bw_osc_sin_process1(float x) {
	return bw_sin2pif(x);
}

static inline void bw_osc_sin_process(const float *x, float *y, size_t n_samples) {
	for (size_t i = 0; i < n_samples; i++)
		y[i] = bw_osc_sin_process1(x[i]);
}

static inline void bw_osc_sin_process_multi(const float * const *x, float * const *y, size_t n_channels, size_t n_samples) {
	for (size_t i = 0; i < n_channels; i++)
		bw_osc_sin_process(x[i], y[i], n_samples);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::oscSinProcess
 *  ```>>> */
template<size_t N_CHANNELS>
void oscSinProcess(
		const float * const *x,
		float * const *y,
		size_t nSamples);

template<size_t N_CHANNELS>
void oscSinProcess(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples);
/*! <<<```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

template<size_t N_CHANNELS>
inline void oscSinProcess(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_osc_sin_process_multi(x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void oscSinProcess(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	oscSinProcess<N_CHANNELS>(x.data(), y.data(), nSamples);
}

}
#endif

#endif
