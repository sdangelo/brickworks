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
 *  module_type {{{ foundation }}}
 *  version {{{ 1.0.0 }}}
 *  description {{{
 *    A common header to make sure that a bunch of basic definitions are
 *    available and consistent for all Brickworks modules.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Removed <code>BW_SIZE_T</code> and went for <code>size_t</code>
 *              instead.</li>
 *          <li>Made control over inclusion of standard library headers more
 *              explicit via <code>BW_NO_STDLIB</code>,
 *              <code>BW_NO_STDDEF_H</code>, <code>BW_NO_STDINT_H</code>, and
 *              <code>BW_NO_MATH_H</code>.</li>
 *          <li>Introduced <code>BW_NO_DEBUG</code> to replace
 *              <code>NDEBUG</code> and prevented useless inclusion of
 *              <code>assert.h</code>.</li>
 *          <li>Added <code>BW_RESTRICT</code> specifiers to input
 *              arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Added <code>extern "C"</code> to functions.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Added <code>BW_ASSERT</code>, <code>bw_is_inf()</code>,
 *              <code>bw_is_nan()</code>, <code>bw_is_finite()</code>,
 *              <code>bw_has_inf()</code>, <code>bw_has_nan()</code>,
 *              <code>bw_has_only_finite()</code>, and
 *              <code>bw_hash_sdbm()</code>.</li>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Fixed typo in the documentation of <code>BW_SIZE_T</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added <code>BW_SIZE_T</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>Removed <code>BW_MALLOC</code>, <code>BW_REALLOC</code>,
 *              and <code>BW_FREE</code>.</li>
 *          <li>Added <code>BW_RESTRICT</code>.</li>
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

#ifndef BW_COMMON_H
#define BW_COMMON_H

/*** Public API ***/

/*! api {{{
 *
 *    #### Basic definitions
 *
 *    Brickworks requires definitions of:
 *
 *    * `NULL` and `size_t`, normally supplied by `stddef.h`;
 *    * `(u)int{8,16,32,64}_t`, `INT{8,16,32,64}_{MIN,MAX}`, and
 *      `UINT{8,16,32,64}_MAX`, normally supplied by `stdint.h`;
 *    * `INFINITY`, normally supplied by `math.h`.
 *
 *    You can control whether those definitions are taken from said headers
 *    or you can supply them yourselves. In particular:
 *
 *    * if `BW_NO_STDLIB` or `BW_NO_STDDEF_H` is defined, then `stddef.h` is not
 *      `#include`d;
 *    * if `BW_NO_STDLIB` or `BW_NO_STDINT_H` is defined, then `stdint.h` is not
 *      `#include`d;
 *    * if `BW_NO_STDLIB` or `BW_NO_MATH_H` is defined, then `math.h` is not
 *      `#include`d.
 *
 *  >>> */
#if !defined(BW_NO_STDLIB) && !defined(BW_NO_STDDEF_H)
# include <stddef.h>
#endif

#ifndef NULL
# error NULL not defined
#endif

#if !defined(BW_NO_STDLIB) && !defined(BW_NO_STDINT_H)
# include <stdint.h>
#endif

#if !defined(INT8_MIN) || !defined(INT8_MAX) \
	|| !defined(INT16_MIN) || !defined(INT16_MAX) \
	|| !defined(INT32_MIN) || !defined(INT32_MAX) \
	|| !defined(INT64_MIN) || !defined(INT64_MAX) \
	|| !defined(UINT8_MAX) || !defined(UINT16_MAX) \
	|| !defined(UINT32_MAX) || !defined(UINT64_MAX)
# error At least one definition in INT{8,16,32,64}_{MIN,MAX} or UINT{8,16,32,64}_MAX is missing
#endif

#if !defined(BW_NO_STDLIB) && !defined(BW_NO_MATH_H)
# include <math.h>
#endif

#ifndef INFINITY
# error INFINITY not defined
#endif
/*! ...
 *
 *    #### BW_RESTRICT
 *
 *    `BW_RESTRICT` is a preprocessor definition that wraps the `restrict`
 *    keyword.
 *
 *    If it is not defined already, then it gets defined as `restrict` if C99 is
 *    supported, otherwise it has no effect (typically when compiling C++ code).
 *
 *  >>> */
#ifndef BW_RESTRICT
# if defined(__STDC__) && (__STDC_VERSION__ >= 199901L)
#  define BW_RESTRICT restrict
# else
#  define BW_RESTRICT
# endif
#endif
/*! ...
 *
 *    #### BW_ASSERT and BW_ASSERT_DEEP
 *
 *    Both macros are `assert()`-like. `BW_ASSERT` is meant to perform
 *    rapid/basic validity checks (e.g., input pointer is not `NULL`, input
 *    `float` number is finite), while `BW_ASSERT_DEEP` should be used for
 *    deeper checks (e.g., validate data structures and internal states).
 *
 *    If `BW_NO_DEBUG` is defined then "calls" to both `BW_ASSERT` and
 *    `BW_ASSERT_DEEP` will be stripped away by the preprocessor.
 *
 *    Whether `BW_NO_DEBUG` is defined or not, "calls" to `BW_ASSERT_DEEP` will
 *    still be stripped anyway if `BW_DEBUG_DEEP` is not defined.
 *
 *    Otherwise, `BW_ASSERT` and `BW_ASSERT_DEEP` can either be provided by you,
 *    otherwise `BW_ASSERT` is defined as `assert` (`assert.h` is `#include`d
 *    and `BW_NO_STDLIB` must not be defined -- please mid that `assert.h` is
 *    still influenced by `NDEBUG`), and `BW_ASSERT_DEEP` is defined as
 *    `BW_ASSERT`.
 *
 *  >>> */
#ifdef BW_NO_DEBUG
# undef BW_ASSERT
# undef BW_ASSERT_DEEP
# define BW_ASSERT(ignore)	((void)0)
# define BW_ASSERT_DEEP(ignore)	((void)0)
#else
# ifndef BW_ASSERT
#  ifdef BW_NO_STDLIB
#   error BW_NO_STDLIB defined but BW_ASSERT not defined
#  endif
#  include <assert.h>
# endif
# ifndef BW_ASSERT
#  define BW_ASSERT assert
# endif
# ifdef BW_DEBUG_DEEP
#  ifndef BW_ASSERT_DEEP
#   define BW_ASSERT_DEEP BW_ASSERT
#  endif
# else
#  undef BW_ASSERT_DEEP
#  define BW_ASSERT_DEEP(ignore) ((void)0)
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! ...
 *
 *    #### bw_is_inf()
 *  ```>>> */
static inline char bw_is_inf(float x);
/*! <<<```
 *    Returns non-`0` if `x` is positive or negative infinity, `0` otherwise.
 *
 *    #### bw_is_nan()
 *  ```>>> */
static inline char bw_is_nan(float x);
/*! <<<```
 *    Returns non-`0` if `x` is NaN, `0` otherwise.
 *
 *    #### bw_is_finite()
 *  ```>>> */
static inline char bw_is_finite(float x);
/*! <<<```
 *    Returns non-`0` if `x` is finite (neither NaN nor positive or negative
 *    infinity), `0` otherwise.
 *
 *    #### bw_has_inf()
 *  ```>>> */
static inline char bw_has_inf(const float *BW_RESTRICT x, size_t n_elems);
/*! <<<```
 *    Scans the fist `n_elems` in buffer `x` and returns non-`0` if it contains
 *    at least one positive or negative inifinity value, `0` otherwise.
 *
 *    #### bw_has_nan()
 *  ```>>> */
static inline char bw_has_nan(const float *BW_RESTRICT x, size_t n_elems);
/*! <<<```
 *    Scans the fist `n_elems` in buffer `x` and returns non-`0` if it contains
 *    at least one NaN value, `0` otherwise.
 *
 *    #### bw_has_only_finite()
 *  ```>>> */
static inline char bw_has_only_finite(const float *BW_RESTRICT x, size_t n_elems);
/*! <<<```
 *    Scans the fist `n_elems` in buffer `x` and returns non-`0` if it only
 *    finds finite values (neither NaN nor positive or negative infinity), `0`
 *    otherwise.
 *
 *    #### bw_hash_sdbm()
 *  ```>>> */
static inline uint32_t bw_hash_sdbm(const char *BW_RESTRICT string);
/*! <<<```
 *    Returns the sdbm hash of the given `string`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#ifdef __cplusplus
extern "C" {
#endif

static inline char bw_is_inf(float x) {
	union { uint32_t u; float f; } v;
	v.f = x;
	return (v.u & 0x7fffffff) == 0x7f800000;
}

static inline char bw_is_nan(float x) {
	union { uint32_t u; float f; } v;
	v.f = x;
	return ((v.u & 0x7f800000) == 0x7f800000) && (v.u & 0x7fffff);
}

static inline char bw_is_finite(float x) {
	union { uint32_t u; float f; } v;
	v.f = x;
	return (v.u & 0x7f800000) != 0x7f800000;
}

static inline char bw_has_inf(const float *BW_RESTRICT x, size_t n_elems) {
	char ret = 0;
	for (size_t i = 0; i < n_elems && !ret; i++)
		ret = bw_is_inf(x[i]);
	return ret;
}

static inline char bw_has_nan(const float *BW_RESTRICT x, size_t n_elems) {
	char ret = 0;
	for (size_t i = 0; i < n_elems && !ret; i++)
		ret = bw_is_nan(x[i]);
	return ret;
}

static inline char bw_has_only_finite(const float *BW_RESTRICT x, size_t n_elems) {
	char ret = 1;
	for (size_t i = 0; i < n_elems && ret; i++)
		ret = bw_is_finite(x[i]);
	return ret;
}

static inline uint32_t bw_hash_sdbm(const char *BW_RESTRICT string) {
	uint32_t hash = 0;
	for (; *string != '\0'; string++)
		hash = *string + (hash << 6) + (hash << 16) - hash;
	return hash;
}

#ifdef __cplusplus
}
#endif

#endif
