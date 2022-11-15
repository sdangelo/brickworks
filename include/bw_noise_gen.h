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
 *  requires {{{ bw_config bw_common bw_math bw_rand }}}
 *  description {{{
 *    Generator of white noise with uniform distribution.
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

#ifndef _BW_NOISE_GEN_H
#define _BW_NOISE_GEN_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_noise_gen
 *  ```>>> */
typedef struct _bw_noise_gen *bw_noise_gen;
/*! <<<```
 *    Instance handle.
 *  >>> */

/*! ...
 *    #### bw_noise_gen_new()
 *  ```>>> */
bw_noise_gen bw_noise_gen_new(uint64_t *state);
/*! <<<```
 *    Creates a new instance which uses the given `state` pointer to obtain
 *    pseudo-random numbers.
 *
 *    Returns the newly-created instance handle or `NULL` if there was not
 *    enough memory.
 *  >>> */

/*! ...
 *    #### bw_noise_gen_free()
 *  ```>>> */
void bw_noise_gen_free(bw_noise_gen instance);
/*! <<<```
 *    Destroys an `instance`.
 *  >>> */

/*! ...
 *    #### bw_noise_gen_set_sample_rate()
 *  ```>>> */
void bw_noise_gen_set_sample_rate(bw_noise_gen instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_noise_gen_reset()
 *    
 *    There is none (not needed).
 *  >>> */

/*! ...
 *    #### bw_noise_gen_process()
 *  ```>>> */
void bw_noise_gen_process(bw_noise_gen instance, float* y, int n_samples);
/*! <<<```
 *    Lets the given `instance` generate `n_samples` samples and puts them in
 *    the output buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_noise_gen_set_sample_rate_scaling()
 *  ```>>> */
void bw_noise_gen_set_sample_rate_scaling(bw_noise_gen instance, char value);
/*! <<<```
 *    Sets whether the output should be scaled (`value` non-`0`) or not (`0`)
 *    according to the sample rate by the given `instance`.
 *
 *    In order to maintain the perceived loudness at different sample rates, a
 *    white noise signal with uniform distribution should be accordingly scaled.
 *    In this module the 44100 Hz sample rate is used as a reference (that is,
 *    the scaling factor at that sample rate is `1.f`).
 *
 *    Default value: `0`.
 *  }}} */

#ifdef __cplusplus
}
#endif

#endif
