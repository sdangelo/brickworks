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
 *    Inline slew-rate limiter.
 *
 *    This is similar to [bw_slew_lim](bw_slew_lim) but can be used to process
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

#ifndef _BW_INLINE_SLEW_LIM_H
#define _BW_INLINE_SLEW_LIM_H

#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_inline_slew_lim_get_max_var()
 *  ```>>> */
static inline float bw_inline_slew_lim_get_max_var(float sample_rate, float rate);
/*! <<<```
 *    Computes a maximum variation coefficient to be used with
 *    `bw_inline_slew_lim()` corresponding to the given `sample_rate` (Hz) and
 *    `rate` (1/s) values.
 *
 *    `rate` represents the maximum variation per second and must be finite and
 *    non-negative.
 *  >>> */

/*! ...
 *    #### bw_inline_slew_lim()
 *  ```>>> */
static inline float bw_inline_slew_lim(float x, float y_z1, float max_inc, float max_dec);
/*! <<<```
 *    Processes one input sample `x`, using the previous output value `y_z1` and
 *    the maximum variation coefficients `max_inc` and `max_dec`, and returns
 *    the corresponding output sample.
 *
 *    Both `max_inc` and `max_dec` must be finite and non-negative.
 *  }}} */

/* Implementation */

static inline float bw_inline_slew_lim_get_max_var(float sample_rate, float rate) {
	return rate * bw_rcpf_2(sample_rate);
}

static inline float bw_inline_slew_lim(float x, float y_z1, float max_inc, float max_dec) {
	return bw_clipf(x, y_z1 - max_dec, y_z1 + max_inc);
}

#ifdef __cplusplus
}
#endif

#endif
