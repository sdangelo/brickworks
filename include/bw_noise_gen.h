/*
 * Brickworks
 *
 * Copyright (C) 2022 Orastron Srl unipersonale
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
 *
 * File author: Stefano D'Angelo
 */

/*!
 *  module_type {{{ dsp }}}
 *  version {{{ 0.2.0 }}}
 *  requires {{{ bw_config bw_common bw_math bw_rand }}}
 *  description {{{
 *    Generator of white noise with uniform distribution.
 *
 *    This module has no internal state, rather its state is stored into a
 *    `uint64_t` value to which the API user supplies a pointer (as in
 *    [bw\_rand](bw_rand)).
 *  }}}
 *  changelog {{{
 *    <ul>
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

#ifndef _BW_NOISE_GEN_H
#define _BW_NOISE_GEN_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_noise_gen_coeffs
 *  ```>>> */
typedef struct _bw_noise_gen_coeffs bw_noise_gen_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_noise_gen_init()
 *  ```>>> */
static inline void bw_noise_gen_init(bw_noise_gen_coeffs *BW_RESTRICT coeffs, uint64_t *BW_RESTRICT state);
/*! <<<```
 *    Initializes input parameter values and sets the `state` pointer to obtain
 *    pseudo-random numbers in `coeffs`.
 *
 *    #### bw_noise_gen_set_sample_rate()
 *  ```>>> */
static inline void bw_noise_gen_set_sample_rate(bw_noise_gen_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_noise_gen_process1\*()
 *  ```>>> */
static inline float bw_noise_gen_process1(const bw_noise_gen_coeffs *BW_RESTRICT coeffs);
static inline float bw_noise_gen_process1_scaling(const bw_noise_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    These functions generates and return one sample using `coeffs`, where:
 *     * `bw_noise_gen_process1()` assumes that sample rate scaling is disabled;
 *     * `bw_noise_gen_process1_scaling()` assumes that sample rate scaling is
 *       enabled.
 *
 *    #### bw_env_gen_process()
 *  ```>>> */
static inline void bw_noise_gen_process(bw_noise_gen_coeffs *BW_RESTRICT coeffs, float *BW_RESTRICT y, int n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the output buffer `y` using
 *    `coeffs`.
 *
 *    #### bw_noise_gen_set_sample_rate_scaling()
 *  ```>>> */
static inline void bw_noise_gen_set_sample_rate_scaling(bw_noise_gen_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the output should be scaled (`value` non-`0`) or not (`0`)
 *    according to the sample rate in `coeffs`.
 *
 *    In order to maintain the same perceived loudness at different sample
 *    rates, a white noise signal with uniform distribution should be
 *    accordingly scaled. The 44100 Hz sample rate is used as a reference (that
 *    is, the scaling factor at that sample rate is `1.f`).
 *
 *    Default value: `0` (off).
 *
 *    #### bw_noise_gen_get_scaling_k()
 *  ```>>> */
static inline float bw_noise_gen_get_scaling_k(bw_noise_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the sample rate scaling factor that is applied or would be applied
 *    if sample rate scaling were enabled, as stored in `coeffs`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_rand.h>
 
struct _bw_noise_gen_coeffs {
	// Coefficients
	float		 scaling_k;

	// Parameters
	uint64_t	*state;
	char		 sample_rate_scaling;
};

static inline void bw_noise_gen_init(bw_noise_gen_coeffs *BW_RESTRICT coeffs, uint64_t *BW_RESTRICT state) {
	coeffs->state = state;
	coeffs->sample_rate_scaling = 0;
}

static inline void bw_noise_gen_set_sample_rate(bw_noise_gen_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->scaling_k = 0.004761904761904762f * bw_sqrtf_2(sample_rate);
}

static inline float bw_noise_gen_process1(const bw_noise_gen_coeffs *BW_RESTRICT coeffs) {
	return bw_randf(coeffs->state);
}

static inline float bw_noise_gen_process1_scaling(const bw_noise_gen_coeffs *BW_RESTRICT coeffs) {
	return coeffs->scaling_k * bw_randf(coeffs->state);
}

static inline void bw_noise_gen_process(bw_noise_gen_coeffs *BW_RESTRICT coeffs, float *BW_RESTRICT y, int n_samples) {
	if (coeffs->sample_rate_scaling)
		for (int i = 0; i < n_samples; i++)
			y[i] = bw_noise_gen_process1(coeffs);
	else
		for (int i = 0; i < n_samples; i++)
			y[i] = bw_noise_gen_process1_scaling(coeffs);
}

static inline void bw_noise_gen_set_sample_rate_scaling(bw_noise_gen_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->sample_rate_scaling = value;
}

static inline float bw_noise_gen_get_scaling_k(bw_noise_gen_coeffs *BW_RESTRICT coeffs) {
	return coeffs->scaling_k;
}

#ifdef __cplusplus
}
#endif

#endif
