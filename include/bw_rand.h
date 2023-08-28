/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
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
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

/*!
 *  module_type {{{ utility }}}
 *  version {{{ 1.0.0 }}}
 *  requires {{{ bw_common }}}
 *  description {{{
 *    Pseudo-random number generators.
 *
 *    These are good enough for music DSP applications but should not be
 *    considered crypthographically secure.
 *
 *    All functions in this module are [reentrant](api#reentrant-function),
 *    [RT-safe](api#rt-safe-function), [thread-safe](api#thread-safe-function),
 *    and have [no side effects](api#no-side-effects).
 *
 *    The algorithms are based on the ["simplified" Permuted Congruential
 *    Generator (PCG)](https://nullprogram.com/blog/2017/09/21/#permuted-congruential-generator-pcg)
 *    by Chris Wellons, which is itself taken from
 *
 *    M. E. O'Neill, "PCG: A Family of Simple Fast Space-Efficient Statistically
 *    Good Algorithms for Random Number Generation", September 2014, available
 *    at https://www.cs.hmc.edu/tr/hmc-cs-2014-0905.pdf.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added one more assertion in <code>bw_randf()</code>.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Added debugging code.</li>
 *          <li>Removed dependency on bw_config.</li>
 *          <li>Fixed harmless warning in <code>bw_randu32()</code>.</li>
 *        </ul>
 *      </li>
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

#ifndef BW_RAND_H
#define BW_RAND_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*** Public API ***/

/*! api {{{
 *    #### bw_randu32()
 *  ```>>> */
static inline uint32_t bw_randu32(
	uint64_t * BW_RESTRICT state);
/*! <<<```
 *    Returns a pseudo-random unsigned 32-bit integer in the range
 *    [`0`, `UINT32_MAX`].
 *
 *    `state` is a pointer to a 64-bit unsigned integer storing the state
 *    between calls and which gets updated by this function.
 *
 *    #### bw_randf()
 *  ```>>> */
static inline float bw_randf(
	uint64_t * BW_RESTRICT state);
/*! <<<```
 *    Returns a pseudo-random unsigned 32-bit floating point number in the range
 *    [`-1.f`, `1.f`].
 *
 *    `state` is a pointer to a 64-bit unsigned integer storing the state
 *    between calls and which gets updated by this function.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

#ifdef __cplusplus
extern "C" {
#endif

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

static inline uint32_t bw_randu32(
		uint64_t * BW_RESTRICT state) {
	BW_ASSERT(state != NULL);
	// Permuted Congruential Generator,
	// taken from https://nullprogram.com/blog/2017/09/21/
	*state = *state * 0x9b60933458e17d7d + 0xd737232eeccdf7ed;
	return (uint32_t)(*state >> (29 - (*state >> 61)));
}

static inline float bw_randf(
		uint64_t * BW_RESTRICT state) {
	BW_ASSERT(state != NULL);
	const float y = (2.f / (float)UINT32_MAX) * (float)bw_randu32(state) - 1.f;
	BW_ASSERT(bw_is_finite(y));
	return y;
}

#ifdef __cplusplus
}
#endif

#endif
