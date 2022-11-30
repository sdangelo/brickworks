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
 *  requires {{{ bw_config bw_common }}}
 *  description {{{
 *    Pinking filter.
 *
 *    ...
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

#ifndef _BW_PINK_FILT_H
#define _BW_PINK_FILT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

typedef struct _bw_pink_filt_coeffs bw_pink_filt_coeffs;

/*! api {{{
 *    #### bw_pink_filt_state
 *  ```>>> */
typedef struct _bw_pink_filt_state bw_pink_filt_state;
/*! <<<```
 *    State
 *  >>> */

static inline void bw_pink_filt_set_sample_rate(bw_pink_filt_coeffs *BW_RESTRICT coeffs, float sample_rate);

/*! ...
 *    #### bw_pink_filt_reset()
 *  ```>>> */
static inline void bw_pink_filt_reset_state(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

static inline float bw_pink_filt_process1(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, float x);

/*! ...
 *    #### bw_pink_filt_process()
 *  ```>>> */
static inline void bw_pink_filt_process(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, const float *x, float* y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

struct _bw_pink_filt_coeffs {
	// Coefficients
	float	k;
};

struct _bw_pink_filt_state {
	float	s1_z1;
	float	s2_z1;
	float	s3_z1;
	float	s4_z1;
};

static inline void bw_pink_filt_set_sample_rate(bw_pink_filt_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->k = bw_sqrtf_2(44100.f / sample_rate);
}

static inline void bw_pink_filt_reset_state(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state) {
	state->s1_z1 = 0.f;
	state->s2_z1 = 0.f;
	state->s3_z1 = 0.f;
	state->s4_z1 = 0.f;
}

static inline float bw_pink_filt_process1(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, float x) {
	const float s1 = 0.320696754235142f * x + state->s1_z1;
	state->s1_z1 = 0.999760145116749f * s1 - 0.3204568993518913f * x;
	const float s2 = 0.2870206617007935f * s1 + state->s2_z1;
	state->s2_z1 = 0.9974135207366259f * s2 - 0.2844341824374191f * s1;
	const float s3 = 0.2962862885898576f * s2 + state->s3_z1;
	state->s3_z1 = 0.9687905029568185f * s3 - 0.265076791546676f * s2;
	const float s4 = 0.3882183163519794f * s3 + state->s4_z1;
	state->s4_z1 = 0.6573784623288251f * s4 - 0.04559677868080467 * s3;
	return coeffs->k * s4;
}

static inline void bw_pink_filt_process(const bw_pink_filt_coeffs *BW_RESTRICT coeffs, bw_pink_filt_state *BW_RESTRICT state, const float *x, float* y, int n_samples) {
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_pink_filt_process1(coeffs, state, x[i]);
}

#ifdef __cplusplus
}
#endif

#endif
