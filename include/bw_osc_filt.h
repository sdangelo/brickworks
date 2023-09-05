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
 *    Post-filter to decolorate oscillator waveshapers when antialiasing is on.
 *
 *    This <a href="https://en.wikipedia.org/wiki/Linear_time-invariant_system"
 *    target="_blank">linear time-invariant filter</a> can be added in series of
 *    oscillator waveshapers that use PolyBLEP antialiasing
 *    (i.e., [bw_osc_saw](bw_osc_saw), [bw_osc_pulse](bw_osc_pulse),
 *    [bw_osc_tri](bw_osc_tri)) to compensate for high-frequency attenuation.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_osc_filt_process()</code> and
 *              <code>bw_osc_filt_process_multi()</code> now use
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
 *          <li>Added <code>bw_osc_filt_process_multi()</code>.</li>
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

#ifndef BW_OSC_FILT_H
#define BW_OSC_FILT_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_osc_filt_state
 *  ```>>> */
typedef struct bw_osc_filt_state bw_osc_filt_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_osc_filt_reset()
 *  ```>>> */
static inline void bw_osc_filt_reset_state(bw_osc_filt_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values.
 *
 *    #### bw_osc_filt_process1()
 *  ```>>> */
static inline float bw_osc_filt_process1(bw_osc_filt_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` usign and updating `state`. Returns the
 *    corresponding output sample.
 *
 *    #### bw_osc_filt_process()
 *  ```>>> */
static inline void bw_osc_filt_process(bw_osc_filt_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating
 *    `state`.
 *
 *    #### bw_osc_filt_process_multi()
 *  ```>>> */
static inline void bw_osc_filt_process_multi(bw_osc_filt_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating each of the `n_channels` `state`s.
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

struct bw_osc_filt_state {
	float	x_z1;
	float	y_z1;
};

static inline void bw_osc_filt_reset_state(bw_osc_filt_state *BW_RESTRICT state) {
	state->x_z1 = 0.f;
	state->y_z1 = 0.f;
}

static inline float bw_osc_filt_process1(bw_osc_filt_state *BW_RESTRICT state, float x) {
	const float y = 1.371308261611209f * x + 0.08785458027104826f * state->x_z1 - 4.591628418822578e-1f * state->y_z1;
	state->x_z1 = x;
	state->y_z1 = y;
	return y;
}

static inline void bw_osc_filt_process(bw_osc_filt_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples) {
	for (size_t i = 0; i < n_samples; i++)
		y[i] = bw_osc_filt_process1(state, x[i]);
}

static inline void bw_osc_filt_process_multi(bw_osc_filt_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples) {
	for (size_t i = 0; i < n_channels; i++)
		bw_osc_filt_process(state[i], x[i], y[i], n_samples);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::OscFilt
 *  ```>>> */
template<size_t N_CHANNELS>
class OscFilt {
public:
	OscFilt();
	
	void reset();
	void process(
		const float * const *x,
		float * const *y,
		size_t nSamples);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_osc_filt_state	 states[N_CHANNELS];
	bw_osc_filt_state	*BW_RESTRICT statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline OscFilt<N_CHANNELS>::OscFilt() {
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void OscFilt<N_CHANNELS>::reset() {
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_osc_filt_reset_state(states + i);
}

template<size_t N_CHANNELS>
inline void OscFilt<N_CHANNELS>::process(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_osc_filt_process_multi(statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void OscFilt<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

}
#endif

#endif
