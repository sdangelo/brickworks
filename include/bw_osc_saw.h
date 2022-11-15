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
 *    Sawtooth oscillator waveshaper with PolyBLEP antialiasing.
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

#ifndef _BW_OSC_SAW_H
#define _BW_OSC_SAW_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_osc_saw
 *  ```>>> */
typedef struct _bw_osc_saw *bw_osc_saw;
/*! <<<```
 *    Instance handle.
 *  >>> */

/*! ...
 *    #### bw_osc_saw_new()
 *  ```>>> */
bw_osc_saw bw_osc_saw_new();
/*! <<<```
 *    Creates a new instance.
 *
 *    Returns the newly-created instance handle or `NULL` if there was not
 *    enough memory.
 *  >>> */

/*! ...
 *    #### bw_osc_saw_free()
 *  ```>>> */
void bw_osc_saw_free(bw_osc_saw instance);
/*! <<<```
 *    Destroys an `instance`.
 *  >>> */

/*! ...
 *    #### bw_osc_saw_set_sample_rate() and bw_osc_saw_reset()
 *
 *    These do not exist (not needed).
 *  >>> */

/*! ...
 *    #### bw_osc_saw_process()
 *  ```>>> */
void bw_osc_saw_process(bw_osc_saw instance, const float *x, const float *x_phase_inc, float* y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` containing the normalized phase signal and fills the
 *    corresponding `n_samples` samples in the output buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_osc_saw_set_antialiasing()
 *  ```>>> */
void bw_osc_saw_set_antialiasing(bw_osc_saw instance, char value);
/*! <<<```
 *    Sets whether the antialiasing is on (`value` non-`0`) or off (`0`) for the
 *    given `instance`.
 *
 *    Default value: `0`.
 *  }}} */

#ifdef __cplusplus
}
#endif

#endif
