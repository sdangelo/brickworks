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
 *  requires {{{ bw_common bw_config bw_math }}}
 *  description {{{
 *    Bit depth reducer.
 *
 *    This is purely an audio effect, it doesn't actually produce an output
 *    signal with a different encoding. The algorithm is deliberately crude to
 *    obtain the characteristic noise due to lo-fi A/D quantization.
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
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_BD_REDUCE_H
#define _BW_BD_REDUCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_bd_reduce_coeffs
 *  ```>>> */
typedef struct _bw_bd_reduce_coeffs bw_bd_reduce_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_bd_reduce_init()
 *  ```>>> */
static inline void bw_bd_reduce_init(bw_bd_reduce_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_bd_reduce_reset_coeffs()
 *  ```>>> */
static inline void bw_bd_reduce_reset_coeffs(bw_bd_reduce_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_bd_reduce_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_bd_reduce_update_coeffs_ctrl(bw_bd_reduce_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_bd_reduce_update_coeffs_audio()
 *  ```>>> */
static inline void bw_bd_reduce_update_coeffs_audio(bw_bd_reduce_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_bd_reduce_process1()
 *  ```>>> */
static inline float bw_bd_reduce_process1(const bw_bd_reduce_coeffs *BW_RESTRICT coeffs, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`. Returns the corresponding
 *    output sample.
 *
 *    #### bw_bd_reduce_process()
 *  ```>>> */
static inline void bw_bd_reduce_process(bw_bd_reduce_coeffs *BW_RESTRICT coeffs, const float *x, float* y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating
 *    `coeffs` (control and audio rate).
 *
 *    #### bw_bd_reduce_set_bit_depth()
 *  ```>>> */
static inline void bw_bd_reduce_set_bit_depth(bw_bd_reduce_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets the output bit depth `value` in `coeffs`. `value` must be strictly positive.
 *
 *    Default value: `16`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

struct _bw_bd_reduce_coeffs {
	// Coefficients
	float	ki;
	float	k;
	float	max;
	
	// Parameters
	char	bit_depth;
	char	bit_depth_prev;
};

static inline void bw_bd_reduce_init(bw_bd_reduce_coeffs *BW_RESTRICT coeffs) {
	coeffs->bit_depth = 16;
}

static inline void bw_bd_reduce_reset_coeffs(bw_bd_reduce_coeffs *BW_RESTRICT coeffs) {
	coeffs->bit_depth_prev = 0;
	bw_bd_reduce_update_coeffs_ctrl(coeffs);
}

static inline void bw_bd_reduce_update_coeffs_ctrl(bw_bd_reduce_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->bit_depth_prev != coeffs->bit_depth) {
		coeffs->k = bw_pow2f_3(coeffs->bit_depth - 1);
		coeffs->ki = bw_rcpf_2(coeffs->k);
		coeffs->max = 1.f - 0.5f * coeffs->ki;
		coeffs->bit_depth_prev = coeffs->bit_depth;
	}
}

static inline void bw_bd_reduce_update_coeffs_audio(bw_bd_reduce_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline float bw_bd_reduce_process1(const bw_bd_reduce_coeffs *BW_RESTRICT coeffs, float x) {
	return coeffs->ki * (bw_floorf(coeffs->k * bw_clipf(x, -coeffs->max, coeffs->max)) + 0.5f);
}

static inline void bw_bd_reduce_process(bw_bd_reduce_coeffs *BW_RESTRICT coeffs, const float *x, float* y, int n_samples) {
	bw_bd_reduce_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_bd_reduce_process1(coeffs, x[i]);
}

static inline void bw_bd_reduce_set_bit_depth(bw_bd_reduce_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->bit_depth = value;
}

#ifdef __cplusplus
}
#endif

#endif
