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
 *  module_type {{{ utility }}}
 *  version {{{ 0.1.0 }}}
 *  requires {{{ bw_config bw_common bw_math }}}
 *  description {{{
 *    Inline one-pole (6 dB/oct) lowpass filter with unitary DC gain and sticky
 *    target-reach threshold.
 *
 *    This is similar to [bw_one_pole](bw_one_pole) but can be used to process
 *    on a sample-by-sample basis without buffers.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.1.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_INLINE_ONE_POLE_H
#define _BW_INLINE_ONE_POLE_H

#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_inline_one_pole_get_mA1()
 *  ```>>> */
static inline float bw_inline_one_pole_get_mA1(float sample_rate, float tau);
/*! <<<```
 *    Computes the `mA1` coefficient requested by other functions in this
 *    module, corresponding to the given `sample_rate` (Hz) and time constant
 *    `tau` (s) values.
 *  >>> */

/*! ...
 *    #### bw_inline_one_pole()
 *  ```>>> */
static inline float bw_inline_one_pole(float x, float y_z1, float mA1);
/*! <<<```
 *    Processes one input sample `x`, using the previous output value `y_z1` and
 *    the `mA1` coefficient, and returns the corresponding output sample.
 *
 *    This function does not feature sticky target-reach threshold.
 *  >>> */

/*! ...
 *    #### bw_inline_one_pole_sticky_abs()
 *  ```>>> */
static inline float bw_inline_one_pole_sticky_abs(float x, float y_z1, float mA1, float thresh_sq);
/*! <<<```
 *    Like `bw_inline_one_pole()` but when the absolute difference between the
 *    output and the input (|*value to be returned* - `x`|) would be smaller
 *    than the square root of `thresh_sq`, it just returns `x`.
 *  >>> */

/*! ...
 *    #### bw_inline_one_pole_sticky_rel()
 *  ```>>> */
static inline float bw_inline_one_pole_sticky_rel(float x, float y_z1, float mA1, float thresh_sq);
/*! <<<```
 *    Like `bw_inline_one_pole()` but when the relative difference of the output
 *    with respect to the input (|*value to be returned* - `x`| / |`x`|) would
 *    be smaller than the square root of `thresh_sq`, it just returns `x`.
 *  }}} */

/* Implementation */

static inline float bw_inline_one_pole_get_mA1(float sample_rate, float tau) {
	return bw_expf_3(-bw_rcpf_2(sample_rate * tau));
}

static inline float bw_inline_one_pole(float x, float y_z1, float mA1) {
	return x + mA1 * (y_z1 - x);
}

static inline float bw_inline_one_pole_sticky_abs(float x, float y_z1, float mA1, float thresh_sq) {
	float y = bw_inline_one_pole(x, y_z1, mA1);
	const float d = x - y;
	return d * d < thresh_sq ? x : y;
}

static inline float bw_inline_one_pole_sticky_rel(float x, float y_z1, float mA1, float thresh_sq) {
	float y = bw_inline_one_pole(x, y_z1, mA1);
	const float d = x - y;
	return d * d < thresh_sq * (x * x) ? x : y;
}

#ifdef __cplusplus
}
#endif

#endif
