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
 *  version {{{ 0.1.0 }}}
 *  requires {{{ bw_config bw_common bw_math }}}
 *  description {{{
 *    Sinusoidal oscillator waveshaper.
 *
 *    This module only consists of the signal processing function.
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

#ifndef _BW_OSC_SIN_H
#define _BW_OSC_SIN_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_osc_sin_process()
 *  ```>>> */
void bw_osc_sin_process(const float *x, float* y, int n_samples);
/*! <<<```
 *    Turns `n_samples` samples of the normalized phase signal in the `x` buffer
 *    into a sinusoidal signal, filling the corresponding `n_samples` in the
 *    output buffer `y`.
 *  }}} */

#ifdef __cplusplus
}
#endif

#endif
