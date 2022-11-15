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
 *  requires {{{ bw_config bw_common }}}
 *  description {{{
 *    Pseudo-random number generators.
 *
 *    These are good enough for music DSP applications but should not be
 *    considered crypthographically secure.
 *
 *    All functions in this module are [reentrant](api#reentrant-function),
 *    [RT-safe](api#rt-safe-function), [thread-safe](api#thread-safe-function),
 *    and have [no side effects](api#no-side-effects).
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

#ifndef _BW_RAND_H
#define _BW_RAND_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_rand_u32()
 *  ```>>> */
uint32_t bw_rand_u32(uint64_t *state);
/*! <<<```
 *    Returns a pseudo-random unsigned 32-bit integer in the range
 *    [`0`, `UINT32_MAX`].
 *
 *    `state` is a pointer to a 64-bit unsigned integer storing the state
 *    between calls, and which gets updated by calls to this function.
 *  >>> */

/*! ...
 *    #### bw_rand_f()
 *  ```>>> */
float bw_rand_f(uint64_t *state);
/*! <<<```
 *    Returns a pseudo-random unsigned 32-bit floating point number in the range
 *    [`-1.f`, `1.f`].
 *
 *    `state` is a pointer to a 64-bit unsigned integer storing the state
 *    between calls, and which gets updated by calls to this function.
 *  }}} */

#ifdef __cplusplus
}
#endif

#endif
