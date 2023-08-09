/*
 * Brickworks
 *
 * Copyright (C) 2021-2023 Orastron Srl unipersonale
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
 *    A collection of mathematical routines that strive to be better suited to
 *    DSP than, e.g., those supplied by your C standard library.
 * 
 *    Such a goal is hopefully accomplished by:
 *
 *    * being as branchless as reasonable/convenient;
 *    * not handling uninteresting corner cases, such as out-of-range,
 *      NaN, and sometimes infinity input values (out-of-range and NaN inputs
 *      are always considered invalid and lead to undefined behavior);
 *    * returning approximated results (indicated in this documentation);
 *    * making no distinction between `0.f` and `-0.f`.
 * 
 *    In practice they should guarantee fast and consistent performance, but
 *    always do your own benchmarking.
 *
 *    All functions in this module are [reentrant](api#reentrant-function),
 *    [RT-safe](api#rt-safe-function), [thread-safe](api#thread-safe-function),
 *    and have [no side effects](api#no-side-effects).
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>WRITEME: document changes!!!</li>
 *          <li>Avoided using reserved identifiers.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Added debugging code.</li>
 *          <li>Removed dependency on bw_config.</li>
 *          <li>Removed <code>bw_omega_3log()</code> and
 *              <code>bw_omega_3lognr()</code>.
 *          <li>Fixed <code>bw_pow10f_3()</code> and
 *              <code>bw_acoshf_3()</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_ceilf()</code>, <code>bw_intfracf()</code>,
 *              <code>bw_sinhf_3()</code>, <code>bw_coshf_3()</code>,
 *              <code>bw_asinhf_3()</code>, and <code>bw_acoshf_3()</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_log10f_3()</code>, <code>bw_pow10f_3()</code>,
 *              <code>bw_dB2linf_3()</code>, and
 *              <code>bw_lin2dBf_3()</code>.</li>
 *          <li>Fixed computation bug in <code>bw_sqrtf_2()</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_sin2pif_3()</code>, <code>bw_cos2pif_3()</code>,
 *              <code>bw_tan2pif_3()</code>, <code>bw_omega_3lognr()</code>, and
 *              <code>bw_tanhf_3()</code>.</li>
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

#ifndef BW_MATH_H
#define BW_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_signfilli32()
 *  ```>>> */
static inline int32_t bw_signfilli32(int32_t x);
/*! <<<```
 *    Returns `~0` if `x` is negative, `0` otherwise.
 *
 *    #### bw_mini32()
 *  ```>>> */
static inline int32_t bw_mini32(int32_t a, int32_t b);
/*! <<<```
 *    Returns the minimum of `a` and `b`.
 *
 *    #### bw_maxi32()
 *  ```>>> */
static inline int32_t bw_maxi32(int32_t a, int32_t b);
/*! <<<```
 *    Returns the maximum of `a` and `b`.
 *
 *    #### bw_clipi32()
 *  ```>>> */
static inline int32_t bw_clipi32(int32_t x, int32_t m, int32_t M);
/*! <<<```
 *    Returns `x` unless it is smaller than `m`, in which case it returns `m`,
 *    or bigger than `M`, in which case it returns `M`.
 *
 *    #### bw_minu32()
 *  ```>>> */
static inline uint32_t bw_minu32(uint32_t a, uint32_t b);
/*! <<<```
 *    Returns the minimum of `a` and `b`.
 *
 *    #### bw_maxu32()
 *  ```>>> */
static inline uint32_t bw_maxu32(uint32_t a, uint32_t b);
/*! <<<```
 *    Returns the maximum of `a` and `b`.
 *
 *    #### bw_clipu32()
 *  ```>>> */
static inline uint32_t bw_clipu32(uint32_t x, uint32_t m, uint32_t M);
/*! <<<```
 *    Returns `x` unless it is smaller than `m`, in which case it returns `m`,
 *    or bigger than `M`, in which case it returns `M`.
 *
 *    #### bw_copysignf()
 *  ```>>> */
static inline float bw_copysignf(float x, float y);
/*! <<<```
 *    Returns a value that has the absolute value of `x` and the sign of `y`.
 *
 *    #### bw_signf()
 *  ```>>> */
static inline float bw_signf(float x);
/*! <<<```
 *    Returns `1.f` if `x > 0.f`, `-1.f` if `x < 0.f` and `0.f` if `x == 0.f`.
 *
 *    #### bw_absf()
 *  ```>>> */
static inline float bw_absf(float x);
/*! <<<```
 *    Returns the absolute value of `x`.
 *
 *    #### bw_min0f()
 *  ```>>> */
static inline float bw_min0f(float x);
/*! <<<```
 *    Returns the minimum of `0.f` and `x`.
 *
 *    #### bw_max0f()
 *  ```>>> */
static inline float bw_max0f(float x);
/*! <<<```
 *    Returns the maximum of `0.f` and `x`.
 *
 *    #### bw_minf()
 *  ```>>> */
static inline float bw_minf(float a, float b);
/*! <<<```
 *    Returns the minimum of `a` and `b`.
 *
 *    `a` and `b` must be finite.
 *
 *    #### bw_maxf()
 *  ```>>> */
static inline float bw_maxf(float a, float b);
/*! <<<```
 *    Returns the maximum of `a` and `b`.
 *
 *    `a` and `b` must be finite.
 *
 *    #### bw_clipf()
 *  ```>>> */
static inline float bw_clipf(float x, float m, float M);
/*! <<<```
 *    Returns `x` unless it is smaller than `m`, in which case it returns `m`,
 *    or bigger than `M`, in which case it returns `M`.
 *
 *    `M` must be greater than or equal to `m`.
 *
 *    #### bw_truncf()
 *  ```>>> */
static inline float bw_truncf(float x);
/*! <<<```
 *    Returns `x` with its fractional part set to zero (i.e., rounded towards
 *    zero).
 *
 *    `x` must be finite.
 *
 *    #### bw_roundf()
 *  ```>>> */
static inline float bw_roundf(float x);
/*! <<<```
 *    Returns `x` rounded to the nearest integer.
 *
 *    Halfway cases are rounded away from zero. E.g., `bw_roundf(0.5f)` gives
 *    `1.f` and `bw_roundf(-0.5f)` gives `-1.f`.
 *
 *    `x` must be finite.
 *
 *    #### bw_floorf()
 *  ```>>> */
static inline float bw_floorf(float x);
/*! <<<```
 *    Returns the biggest integer less or equal than `x` (i.e., `x` is rounded
 *    down).
 *
 *    `x` must be finite.
 *
 *    #### bw_ceilf()
 *  ```>>> */
static inline float bw_ceilf(float x);
/*! <<<```
 *    Returns the smallest integer greater or equal than `x` (i.e., `x` is
 *    rounded up).
 *
 *    `x` must be finite.
 *
 *    #### bw_intfracf()
 *  ```>>> */
static inline void bw_intfracf(float x, float *i, float *f);
/*! <<<```
 *    Puts the integer part (floor) of `x` in `i` and the fractional part in
 *    `f`.
 *
 *    #### bw_rcpf_2()
 *  ```>>> */
static inline float bw_rcpf_2(float x);
/*! <<<```
 *    Returns the reciprocal of `x` (i.e., `1.f / x`).
 *
 *    Not guaranteed to work for `x` having exponent too big or too small. Safe
 *    range: |`x`| in [2^-90, 2^90].
 *
 *    Relative error < 0.0013%.
 *
 *    #### bw_sin2pif_3()
 *  ```>>> */
static inline float bw_sin2pif_3(float x);
/*! <<<```
 *    Returns an approximation of the sine of 2 * pi * `x`, where `x` is given
 *    in radians.
 * 
 *    Absolute error < 0.011, relative error < 1.7%.
 *
 *    #### bw_sinf_3()
 *  ```>>> */
static inline float bw_sinf_3(float x);
/*! <<<```
 *    Returns an approximation of the sine of `x`, where `x` is given in
 *    radians.
 * 
 *    Absolute error < 0.011, relative error < 1.7%.
 *
 *    #### bw_cos2pif_3()
 *  ```>>> */
static inline float bw_cos2pif_3(float x); 
/*! <<<```
 *    Returns an approximation of the cosine of 2 * pi * `x`, where `x` is given
 *    in radians.
 * 
 *    Absolute error < 0.011, relative error < 1.7%.
 *
 *    #### bw_cosf_3()
 *  ```>>> */
static inline float bw_cosf_3(float x);
/*! <<<```
 *    Returns an approximation of the cosine of `x`, where `x` is given in
 *    radians.
 * 
 *    Absolute error < 0.011, relative error < 1.7%.
 *
 *    #### bw_tan2pif_3()
 *  ```>>> */
static inline float bw_tan2pif_3(float x);
/*! <<<```
 *    Returns an approximation of the tangent of 2 * pi * `x`, where `x` is
 *    given in radians.
 * 
 *    Not guaranteed to work for `x` too close to singularities. Safe
 *    range: `x` in [-1/4 + 5e-4f / pi, 1/4 - 5e-4f / pi] + k / 2, where k is
 *    any integer number.
 *
 *    Absolute error < 0.06, relative error < 0.8%.
 *
 *    #### bw_tanf_3()
 *  ```>>> */
static inline float bw_tanf_3(float x);
/*! <<<```
 *    Returns an approximation of the tangent of `x`, where `x` is given in
 *    radians.
 * 
 *    Not guaranteed to work for `x` too close to singularities. Safe
 *    range: `x` in [-pi/2 + 1e-3f, pi/2 - 1e-3f] + k * pi, where k is any
 *    integer number.
 *
 *    Absolute error < 0.06, relative error < 0.8%.
 *
 *    #### bw_log2f_3()
 *  ```>>> */
static inline float bw_log2f_3(float x);
/*! <<<```
 *    Returns an approximation of the base-2 logarithm of `x`.
 * 
 *    Absolute error < 0.0055, relative error < 1.5%.
 *
 *    #### bw_logf_3()
 *  ```>>> */
static inline float bw_logf_3(float x);
/*! <<<```
 *    Returns an approximation of the natural logarithm of `x`.
 * 
 *    Absolute error < 0.0039, relative error < 1.5%.
 *
 *    #### bw_log10f_3()
 *  ```>>> */
static inline float bw_log10f_3(float x);
/*! <<<```
 *    Returns an approximation of the base-10 logarithm of `x`.
 * 
 *    Absolute error < 0.0017, relative error < 1.5%.
 *
 *    #### bw_pow2f_3()
 *  ```>>> */
static inline float bw_pow2f_3(float x);
/*! <<<```
 *    Returns an approximation of 2 raised to the power of `x`.
 * 
 *    Relative error < 0.062%.
 *
 *    #### bw_expf_3()
 *  ```>>> */
static inline float bw_expf_3(float x);
/*! <<<```
 *    Returns an approximation of e (Euler's number) raised to the power of `x`.
 * 
 *    Relative error < 0.062%.
 *
 *    #### bw_pow10f_3() 
 *  ```>>> */
static inline float bw_pow10f_3(float x);
/*! <<<```
 *    Returns an approximation of 10 raised to the power of `x`.
 * 
 *    Relative error < 0.062%.
 *
 *    #### bw_dB2linf_3()
 *  ```>>> */
static inline float bw_dB2linf_3(float x);
/*! <<<```
 *    Returns an approximation of 10 raised to the power of `x` / 20 (dB to
 *    linear ratio conversion).
 *
 *    Relative error < 0.062%.
 *
 *    #### bw_lin2dBf_3()
 *  ```>>> */
static inline float bw_lin2dBf_3(float x);
/*! <<<```
 *    Returns an approximation of 20 times the base-10 logarithm of `x` (linear
 *    ratio to dB conversion).
 *
 *    Absolute error < 0.032, relative error < 1.5%.
 * 
 *    #### bw_sqrtf_2()
 *  ```>>> */
static inline float bw_sqrtf_2(float x);
/*! <<<```
 *    Returns an approximation of the square root of `x`.
 *
 *    Do not feed `0.f`.
 * 
 *    Relative error < 0.0007%.
 *
 *    #### bw_tanhf_3()
 *  ```>>> */
static inline float bw_tanhf_3(float x);
/*! <<<```
 *    Returns an approximation of the hyperbolic tangent of `x`.
 *
 *    Absolute error < 0.035, relative error < 6.5%.
 *
 *    #### bw_sinhf_3()
 *  ```>>> */
static inline float bw_sinhf_3(float x);
/*! <<<```
 *    Returns an approximation of the hyperbolic sine of `x`.
 *
 *    Relative error < 0.07%.
 *
 *    #### bw_coshf_3()
 *  ```>>> */
static inline float bw_coshf_3(float x);
/*! <<<```
 *    Returns an approximation of the hyperbolic cosine of `x`.
 *
 *    Relative error < 0.07%.
 *
 *    #### bw_asinhf_3()
 *  ```>>> */
static inline float bw_asinhf_3(float x);
/*! <<<```
 *    Returns an approximation of the hyperbolic arcsine of `x`.
 *
 *    Absolute error < 0.004, relative error < 1.2%.
 *
 *    #### bw_acoshf_3()
 *  ```>>> */
static inline float bw_acoshf_3(float x);
/*! <<<```
 *    Returns an approximation of the hyperbolic arccosine of `x`.
 *
 *    Absolute error < 0.004, relative error < 0.8%.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

// I hope the target architecture and compiler will use conditional ops here

static inline int32_t bw_signfilli32(int32_t x) {
	return x < 0 ? ~0 : 0;
}

static inline int32_t bw_mini32(int32_t a, int32_t b) {
	return a < b ? a : b;
}

static inline int32_t bw_maxi32(int32_t a, int32_t b) {
	return a > b ? a : b;
}

static inline int32_t bw_clipi32(int32_t x, int32_t m, int32_t M) {
	return x < m ? m : (x > M ? M : x);
}

static inline uint32_t bw_minu32(uint32_t a, uint32_t b) {
	return a < b ? a : b;
}

static inline uint32_t bw_maxu32(uint32_t a, uint32_t b) {
	return a > b ? a : b;
}

static inline uint32_t bw_clipu32(uint32_t x, uint32_t m, uint32_t M) {
	return x < m ? m : (x > M ? M : x);
}

// Here instead I don't trust C semantics to get close to conditional ops for
// floating point numbers

static inline float bw_copysignf(float x, float y) {
	BW_ASSERT(!bw_is_nan(x));
	BW_ASSERT(!bw_is_nan(y));
	union { float f; uint32_t u; } v, s;
	v.f = x;
	s.f = y;
	v.u = (v.u & 0x7fffffffu) | (s.u & 0x80000000u);
	BW_ASSERT(!bw_is_nan(v.f));
	return v.f;
}

static inline float bw_signf(float x) {
	BW_ASSERT(!bw_is_nan(x));
	static const float y[4] = { 0.f, 1.f, 0.f, -1.f };
	union { float f; uint32_t u; } v;
	v.f = x;
	const float r = y[bw_minu32(v.u & 0x7fffffffu, 1) | ((v.u >> 30) & 0x2)];
	BW_ASSERT(!bw_is_nan(r));
	return r;
}

static inline float bw_absf(float x) {
	BW_ASSERT(!bw_is_nan(x));
	union { float f; uint32_t u; } v;
	v.f = x;
	v.u = v.u & 0x7fffffffu;
	BW_ASSERT(!bw_is_nan(v.f));
	return v.f;
}

static inline float bw_min0f(float x) {
	BW_ASSERT(!bw_is_nan(x));
	union { float f; int32_t i; } v;
	v.f = x;
	v.i = bw_mini32(0, v.i);
	BW_ASSERT(!bw_is_nan(v.f));
	return v.f;
}

static inline float bw_max0f(float x) {
	BW_ASSERT(!bw_is_nan(x));
	union { float f; int32_t i; } v;
	v.f = x;
	v.i = bw_maxi32(0, v.i);
	BW_ASSERT(!bw_is_nan(v.f));
	return v.f;
}

static inline float bw_minf(float a, float b) {
	BW_ASSERT(!bw_is_nan(a));
	BW_ASSERT(!bw_is_nan(b));
	const float y = a < b ? a : b;
	BW_ASSERT(!bw_is_nan(y));
	return y;
}

static inline float bw_maxf(float a, float b) {
	BW_ASSERT(!bw_is_nan(a));
	BW_ASSERT(!bw_is_nan(b));
	const float y = a > b ? a : b;
	BW_ASSERT(!bw_is_nan(y));
	return y;
}

static inline float bw_clipf(float x, float m, float M) {
	BW_ASSERT(!bw_is_nan(x));
	BW_ASSERT(!bw_is_nan(m));
	BW_ASSERT(!bw_is_nan(M));
	BW_ASSERT(M >= m);
	const float y = bw_minf(bw_maxf(x, m), M);
	BW_ASSERT(!bw_is_nan(y));
	return y;
}

static inline float bw_truncf(float x) {
	BW_ASSERT(bw_is_finite(x));
	union { float f; uint32_t u; } v;
	v.f = x;
	const int32_t ex = (v.u & 0x7f800000u) >> 23;
	int32_t m = (~0u) << bw_clipi32(150 - ex, 0, 23);
	m &= bw_signfilli32(126 - ex) | 0x80000000;
	v.u &= m;
	BW_ASSERT(bw_is_finite(v.f));
	return v.f;
}

static inline float bw_roundf(float x) {
	BW_ASSERT(bw_is_finite(x));
	union { float f; uint32_t u; } v, s;
	v.f = x;
	const int32_t ex = (v.u & 0x7f800000u) >> 23;
	const int32_t sh = bw_clipi32(150 - ex, 0, 24);
	int32_t mt = (~0u) << sh;
	mt &= bw_signfilli32(126 - ex) | 0x80000000;
	int32_t mr = (1 << sh) >> 1;
	mr &= bw_signfilli32(125 - ex);
	s.f = bw_copysignf(1.f, x);
	int32_t ms = bw_signfilli32(((v.u | 0x00800000u) & mr) << (32 - sh));
	v.u &= mt;
	s.u &= ms;
	const float y = v.f + s.f;
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_floorf(float x) {
	BW_ASSERT(bw_is_finite(x));
	union { float f; int32_t i; } t, y, s;
	t.f = bw_truncf(x); // first bit set when t < 0
	y.f = x - t.f; // first bit set when t > x
	s.f = 1.f;
	s.i &= bw_signfilli32(t.i & y.i);
	const float r = t.f - s.f;
	BW_ASSERT(bw_is_finite(r));
	return r;
}

static inline float bw_ceilf(float x) {
	BW_ASSERT(bw_is_finite(x));
	union { float f; int32_t i; } t, y, s;
	t.f = bw_truncf(x); // first bit set when t < 0
	y.f = t.f - x; // first bit set when t < x
	s.f = 1.f;
	s.i &= bw_signfilli32(~t.i & y.i);
	const float r = t.f + s.f;
	BW_ASSERT(bw_is_finite(r));
	return r;
}

static inline void bw_intfracf(float x, float *i, float *f) {
	BW_ASSERT(bw_is_finite(x));
	*i = bw_floorf(x);
	*f = x - *i;
	BW_ASSERT(bw_is_finite(*i));
	BW_ASSERT(bw_is_finite(*f));
}

static inline float bw_rcpf_2(float x) {
	union { float f; int32_t i; } v;
	v.f = x;
	v.i = 0x7ef0e840 - v.i;
	v.f = v.f + v.f - x * v.f * v.f;
	v.f = v.f + v.f - x * v.f * v.f;
	return v.f;
}

static inline float bw_sin2pif_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	x = x - bw_floorf(x);
	float xp1 = x + x - 1.f;
	float xp2 = bw_absf(xp1);
	float xp = 1.570796326794897f - 1.570796326794897f * bw_absf(xp2 + xp2 - 1.f);
	const float y = -bw_copysignf(1.f, xp1) * (xp + xp * xp * (-0.05738534102710938f - 0.1107398163618408f * xp));
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_sinf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	const float y = bw_sin2pif_3(0.1591549430918953f * x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_cos2pif_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	const float y = bw_sin2pif_3(x + 0.25f);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_cosf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	const float y = bw_cos2pif_3(0.1591549430918953f * x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_tan2pif_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT((x - 0.5f * bw_floorf(x + x) <= 0.249840845056908f)
			|| (x - 0.5f * bw_floorf(x + x) >= 0.250159154943092f));
	const float y = bw_sin2pif_3(x) * bw_rcpf_2(bw_cos2pif_3(x));
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_tanf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT((x - 3.141592653589793f * bw_floorf(0.318309886183791f * x) <= 1.569796326794897f)
			|| (x - 3.141592653589793f * bw_floorf(0.318309886183791f * x) >= 1.571796326794896f));
	x = 0.1591549430918953f * x;
	const float y = bw_sin2pif_3(x) * bw_rcpf_2(bw_cos2pif_3(x));
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_log2f_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 1.175494350822287e-38f);
	union { float f; int32_t i; } v;
	v.f = x;
	int e = v.i >> 23;
	v.i = (v.i & 0x007fffff) | 0x3f800000;
	const float y = (float)e - 129.213475204444817f + v.f * (3.148297929334117f + v.f * (-1.098865286222744f + v.f * 0.1640425613334452f));
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_logf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 1.175494350822287e-38f);
	const float y = 0.693147180559945f * bw_log2f_3(x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_log10f_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 1.175494350822287e-38f);
	const float y = 0.3010299956639811f * bw_log2f_3(x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_pow2f_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x <= 127.999f);
	if (x < -126.f)
		return 0.f;
	union { float f; int32_t i; } v;
	v.f = x;
	int xi = (int)x;
	int l = xi - ((v.i >> 31) & 1);
	float f = x - (float)l;
	v.i = (l + 127) << 23;
	const float y = v.f + v.f * f * (0.6931471805599453f + f * (0.2274112777602189f + f * 0.07944154167983575f));
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_expf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x <= 88.722f);
	const float y = bw_pow2f_3(1.442695040888963f * x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_pow10f_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x <= 38.531f);
	const float y = bw_pow2f_3(3.321928094887363f * x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_dB2linf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x <= 770.630f);
	const float y = bw_pow2f_3(0.1660964047443682f * x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_lin2dBf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 1.175494350822287e-38f);
	const float y = 20.f * bw_log10f_3(x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_sqrtf_2(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 0.f);
	if (x < 8.077935669463161e-28f) {
		const float y = 3.518437208883201e13f * x;
		BW_ASSERT(bw_is_finite(y));
		return y;
	}
	union { float f; uint32_t u; } v;
	v.f = x;
	v.u = (((v.u - 0x3f82a127) >> 1) + 0x3f7d8fc7) & 0x7fffffff;
	float r = bw_rcpf_2(x);
	v.f = v.f + v.f * (0.5f - 0.5f * r * v.f * v.f);
	v.f = v.f + v.f * (0.5f - 0.5f * r * v.f * v.f);
	BW_ASSERT(bw_is_finite(v.f));
	return v.f;
}

static inline float bw_tanhf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	const float xm = bw_clipf(x, -2.115287308554551f, 2.115287308554551f);
	const float axm = bw_absf(xm);
	const float y = xm * axm * (0.01218073260037716f * axm - 0.2750231331124371f) + xm;
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_sinhf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= -88.722f && x <= 88.722f);
	const float y = 0.5f * (bw_expf_3(x) - bw_expf_3(-x));
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_coshf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= -88.722f && x <= 88.722f);
	const float y = 0.5f * (bw_expf_3(x) + bw_expf_3(-x));
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_asinhf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= -1.7e38f && x <= 1.7e38f);
	float a = bw_absf(x);
	const float y = bw_copysignf(bw_logf_3((a >= 4096.f ? a : bw_sqrtf_2(a * a + 1.f)) + a), x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

static inline float bw_acoshf_3(float x) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 1.f);
	const float y = bw_logf_3((x >= 8192.f ? x : bw_sqrtf_2(x * x - 1.f)) + x);
	BW_ASSERT(bw_is_finite(y));
	return y;
}

#ifdef __cplusplus
}
#endif

#endif
