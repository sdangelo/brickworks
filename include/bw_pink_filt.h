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
 *  version {{{ 0.4.0 }}}
 *  requires {{{ bw_common bw_config }}}
 *  description {{{
 *    Pinking filter.
 *
 *    This is a
 *    <a href="https://en.wikipedia.org/wiki/Linear_time-invariant_system"
 *    target="_blank">linear time-invariant filter</a> approximately attenuating
 *    3 dB/oct from about 0.000046 × Nyquist frequency to about 0.9 × Nyquist
 *    frequency. It can be used to turn white noise into pink noise (hence the
 *    name).
 *  }}}
 *  changelog {{{
 *    <ul>
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

#ifndef _BW_PINK_FILT_H
#define _BW_PINK_FILT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_pink_filt_coeffs
 *  ```>>> */
typedef struct _bw_pink_filt_coeffs bw_pink_filt_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_pink_filt_state
 *  ```>>> */
typedef struct _bw_pink_filt_state bw_pink_filt_state;
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
static inline void bw_pink_filt_process(bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, const float *x, float* y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
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

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

struct _bw_pink_filt_coeffs {
	// Coefficients
	float	scaling_k;
	
	// Parameters
	float	sample_rate_scaling;
};

struct _bw_pink_filt_state {
	float	s1_z1;
	float	s2_z1;
	float	s3_z1;
	float	s4_z1;
};

static inline void bw_pink_filt_init(bw_pink_filt_coeffs *BW_RESTRICT coeffs) {
	coeffs->sample_rate_scaling = 0;
}

static inline void bw_pink_filt_set_sample_rate(bw_pink_filt_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->scaling_k = 210.f / bw_sqrtf_2(sample_rate);
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

static inline void bw_pink_filt_process(bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, const float *x, float* y, int n_samples) {
	if (coeffs->sample_rate_scaling)
		for (int i = 0; i < n_samples; i++)
			y[i] = bw_pink_filt_process1_scaling(coeffs, state, x[i]);
	else
		for (int i = 0; i < n_samples; i++)
			y[i] = bw_pink_filt_process1(coeffs, state, x[i]);
}

static inline void bw_pink_filt_set_sample_rate_scaling(bw_pink_filt_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->sample_rate_scaling = value;
}

static inline float bw_pink_filt_get_scaling_k(const bw_pink_filt_coeffs *BW_RESTRICT coeffs) {
	return coeffs->scaling_k;
}

#ifdef __cplusplus
}
#endif

#endif
