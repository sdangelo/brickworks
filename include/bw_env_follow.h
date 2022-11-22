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
 *  requires {{{ bw_config bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    Envelope follower made of a full-wave rectifier followed by
 *    [bw_one_pole](bw_one_pole) one-pole filter (6 dB/oct).
 *
 *    Unlike other dsp modules, you can directly access the parameters of the
 *    internal one-pole filter via the `bw_env_follow_get_one_pole()` function.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>Refactored API to avoid dynamic memory allocation.</li>
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

#ifndef _BW_ENV_FOLLOW_H
#define _BW_ENV_FOLLOW_H

#include <bw_one_pole.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_env_follow
 *  ```>>> */
typedef struct _bw_env_follow bw_env_follow;
/*! <<<```
 *    Instance object.
 *  >>> */

/*! ...
 *    #### bw_env_follow_init()
 *  ```>>> */
void bw_env_follow_init(bw_env_follow *instance);
/*! <<<```
 *    Initializes the `instance` object.
 *  >>> */

/*! ...
 *    #### bw_env_follow_set_sample_rate()
 *  ```>>> */
void bw_env_follow_set_sample_rate(bw_env_follow *instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_env_follow_reset()
 *  ```>>> */
void bw_env_follow_reset(bw_env_follow *instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_env_follow_process()
 *  ```>>> */
void bw_env_follow_process(bw_env_follow *instance, const float *x, float *y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_env_follow_get_one_pole()
 *  ```>>> */
bw_one_pole *bw_env_follow_get_one_pole(bw_env_follow *instance);
/*! <<<```
 *    Returns a pointer to the internal one-pole filter of the given
 *    `instance`.
 *
 *    The returned pointer must not be used for any other purpose than setting
 *    parameters.
 *
 *    This is **NOT** a function that gets an output parameter as described in
 *    the [documentation for DSP modules](api#dsp).
 *
 *    This function is [reentrant](api#reentrant-function),
 *    [RT-safe](api#rt-safe-function),
 *    [**NOT** thread-safe](api#thread-safe-function), and has
 *    [no side effects](api#no-side-effects).
 *  }}} */

/* WARNING: the internal definition of this struct is not part of the public
 * API. Its content may change at any time in future versions. Please, do not
 * access its members directly. */
struct _bw_env_follow {
	// Sub-components
	bw_one_pole_coeffs	one_pole_coeffs;
	bw_one_pole_state	one_pole_state;
};

#ifdef __cplusplus
}
#endif

#endif
