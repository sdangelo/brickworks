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
 *    Sample rate reducer.
 *
 *    This is purely an audio effect, it doesn't actually produce an output
 *    signal at a lower sample rate. The algorithm is deliberately crude to
 *    obtain the characteristic aliasing due to lo-fi downsampling.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_sr_reduce_process_multi()</code>.</li>
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
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_SR_REDUCE_H
#define _BW_SR_REDUCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_sr_reduce_coeffs
 *  ```>>> */
typedef struct _bw_sr_reduce_coeffs bw_sr_reduce_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_sr_reduce_state
 *  ```>>> */
typedef struct _bw_sr_reduce_state bw_sr_reduce_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_sr_reduce_init()
 *  ```>>> */
static inline void bw_sr_reduce_init(bw_sr_reduce_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_sr_reduce_reset_state()
 *  ```>>> */
static inline void bw_sr_reduce_reset_state(const bw_sr_reduce_coeffs *BW_RESTRICT coeffs, bw_sr_reduce_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_sr_reduce_process1()
 *  ```>>> */
static inline float bw_sr_reduce_process1(const bw_sr_reduce_coeffs *BW_RESTRICT coeffs, bw_sr_reduce_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_sr_reduce_process()
 *  ```>>> */
static inline void bw_sr_reduce_process(bw_sr_reduce_coeffs *BW_RESTRICT coeffs, bw_sr_reduce_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using `coeffs` and
 *    both using and updating `state` (control and audio rate).
 *
 *    #### bw_sr_reduce_process_multi()
 *  ```>>> */
static inline void bw_sr_reduce_process_multi(bw_sr_reduce_coeffs *BW_RESTRICT coeffs, bw_sr_reduce_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using the common `coeffs` and both using and updating each of the
 *    `n_channels` `state`s.
 *
 *    #### bw_sr_reduce_set_ratio()
 *  ```>>> */
static inline void bw_sr_reduce_set_ratio(bw_sr_reduce_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the output to input sample rate ratio in `coeffs`.
 *
 *    Valid input range: [`0.f`, `1.f`].
 *
 *    Default value: `1.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

struct _bw_sr_reduce_coeffs {
	// Parameters
	float	ratio;
};

struct _bw_sr_reduce_state {
	float	phase;
	float	y_z1;
};

static inline void bw_sr_reduce_init(bw_sr_reduce_coeffs *BW_RESTRICT coeffs) {
	coeffs->ratio = 1.f;
}

static inline void bw_sr_reduce_reset_state(const bw_sr_reduce_coeffs *BW_RESTRICT coeffs, bw_sr_reduce_state *BW_RESTRICT state) {
	(void)coeffs;
	state->phase = 1.f;
}

static inline float bw_sr_reduce_process1(const bw_sr_reduce_coeffs *BW_RESTRICT coeffs, bw_sr_reduce_state *BW_RESTRICT state, float x) {
	state->phase += coeffs->ratio;
	if (state->phase >= 1.f) {
		state->y_z1 = x;
		state->phase -= bw_floorf(state->phase);
	}
	return state->y_z1;
}

static inline void bw_sr_reduce_process(bw_sr_reduce_coeffs *BW_RESTRICT coeffs, bw_sr_reduce_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_sr_reduce_process1(coeffs, state, x[i]);
}

static inline void bw_sr_reduce_process_multi(bw_sr_reduce_coeffs *BW_RESTRICT coeffs, bw_sr_reduce_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples) {
	for (int i = 0; i < n_channels; i++)
		bw_sr_reduce_process(coeffs, state[i], x[i], y[i], n_samples);
}

static inline void bw_sr_reduce_set_ratio(bw_sr_reduce_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->ratio = value;
}

#ifdef __cplusplus
}
#endif

#endif
