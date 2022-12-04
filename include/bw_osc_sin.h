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
 *  requires {{{ bw_config bw_common bw_math }}}
 *  description {{{
 *    Sinusoidal oscillator waveshaper.
 * 
 *    It turns a normalized phase signal, such as that geneated by
 *    [bw\_phase\_gen](bw_phase_gen), into a sinusoidal wave.
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

#ifndef _BW_OSC_SIN_H
#define _BW_OSC_SIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

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
static inline void bw_osc_sin_process(const float *x, float* y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x`, containing the
 *    normalized phase signal, and fills the first `n_samples` of the output
 *    buffer `y`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

static inline float bw_osc_sin_process1(float x) {
	return bw_sinf_3(6.283185307179586f * x);
}

static inline void bw_osc_sin_process(const float *x, float* y, int n_samples) {
	for (int i = 0; i < n_samples; i++)
		y[i] = bw_osc_sin_process1(x[i]);
}

#ifdef __cplusplus
}
#endif

#endif
