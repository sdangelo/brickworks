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
 *    Phase generator with portamento and exponential frequency modulation.
 *
 *    It outputs a normalized phase signal (range [`0.f`, `1.f`]).
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

#ifndef _BW_PHASE_GEN_H
#define _BW_PHASE_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_phase_gen
 *  ```>>> */
typedef struct _bw_phase_gen *bw_phase_gen;
/*! <<<```
 *    Instance handle.
 *  >>> */

/*! ...
 *    #### bw_phase_gen_new()
 *  ```>>> */
bw_phase_gen bw_phase_gen_new();
/*! <<<```
 *    Creates a new instance.
 *
 *    Returns the newly-created instance handle or `NULL` if there was not
 *    enough memory.
 *  >>> */

/*! ...
 *    #### bw_phase_gen_free()
 *  ```>>> */
void bw_phase_gen_free(bw_phase_gen instance);
/*! <<<```
 *    Destroys an `instance`.
 *  >>> */

/*! ...
 *    #### bw_phase_gen_set_sample_rate()
 *  ```>>> */
void bw_phase_gen_set_sample_rate(bw_phase_gen instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_phase_gen_reset()
 *  ```>>> */
void bw_phase_gen_reset(bw_phase_gen instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_phase_gen_process()
 *  ```>>> */
void bw_phase_gen_process(bw_phase_gen instance, const float *x_mod, float* y, float *y_phase_inc, int n_samples);
/*! <<<```
 *    Lets the given `instance` generate `n_samples` samples and puts them in
 *    the output buffer `y`.
 *
 *    If `x_mod` is not `NULL`, it is used as a source of exponential frequency
 *    modulation (scale `1.f`/octave).
 *
 *    If `y_inc` is not `NULL`, it is filled with phase increment values.
 *  >>> */

/*! ...
 *    #### bw_phase_gen_set_frequency()
 *  ```>>> */
void bw_phase_gen_set_frequency(bw_phase_gen instance, float value);
/*! <<<```
 *    Sets the base frequency to `value` (Hz) for the given `instance`.
 *    
 *    Default value: `1.f`.
 *  >>> */

/*! ...
 *    #### bw_phase_gen_set_portamento_tau()
 *  ```>>> */
void bw_phase_gen_set_portamento_tau(bw_phase_gen instance, float value);
/*! <<<```
 *    Sets the portamento time constant `value` (s) for the given `instance`.
 *
 *    Default value: `0.f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

#endif
