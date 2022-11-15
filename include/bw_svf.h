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
 *  requires {{{ bw_config bw_common bw_inline_one_pole bw_math }}}
 *  description {{{
 *    State variable filter (2nd order, 12 dB/oct) model with separated lowpass,
 *    bandpass, and highpass outputs.
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

#ifndef _BW_SVF_H
#define _BW_SVF_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_svf
 *  ```>>> */
typedef struct _bw_svf *bw_svf;
/*! <<<```
 *    Instance handle.
 *  >>> */

/*! ...
 *    #### bw_svf_new()
 *  ```>>> */
bw_svf bw_svf_new();
/*! <<<```
 *    Creates a new instance.
 *
 *    Returns the newly-created instance handle or `NULL` if there was not
 *    enough memory.
 *  >>> */

/*! ...
 *    #### bw_svf_free()
 *  ```>>> */
void bw_svf_free(bw_svf instance);
/*! <<<```
 *    Destroys an `instance`.
 *  >>> */

/*! ...
 *    #### bw_svf_set_sample_rate()
 *  ```>>> */
void bw_svf_set_sample_rate(bw_svf instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_svf_reset()
 *  ```>>> */
void bw_svf_reset(bw_svf instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_svf_process()
 *  ```>>> */
void bw_svf_process(bw_svf instance, const float *x, float* y_lp, float *y_bp, float *y_hp, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffers `y_lp` (lowpass), `y_bp` (bandpass), and `y_hp` (highpass), if
 *    they are not `NULL`.
 *  >>> */

/*! ...
 *    #### bw_svf_set_cutoff()
 *  ```>>> */
void bw_svf_set_cutoff(bw_svf instance, float value);
/*! <<<```
 *    Sets the cutoff frequency to the given `value` (Hz) for the given
 *    `instance`.
 *
 *    Default value: `1e3f`.
 *  >>> */

/*! ...
 *    #### bw_svf_set_Q()
 *  ```>>> */
void bw_svf_set_Q(bw_svf instance, float value);
/*! <<<```
 *    Sets the quality factor to the given `value` (Hz) for the given
 *    `instance`.
 *
 *    Default value: `0.5f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

#endif
