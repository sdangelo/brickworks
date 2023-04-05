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
 *
 * Part of the code in this file is derived from omega.h by Stefano D'Angelo,
 * which is released under the following conditions:
 *
 * Copyright (C) 2019 Stefano D'Angelo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Such part is itself based on the theory in
 *
 * S. D'Angelo, L. Gabrielli, and L. Turchet, "Fast Approximation of the
 * Lambert W Function for Virtual Analog Modeling", 22nd Intl. Conf. Digital
 * Audio Effects (DAFx-19), Birmingham, UK, September 2019.
 */

/*!
 *  module_type {{{ utility }}}
 *  version {{{ 0.4.0 }}}
 *  requires {{{ bw_common bw_config }}}
 *  description {{{
 *    A collection of mathematical routines that strive to be better suited to
 *    DSP than, e.g., those supplied by your C standard library.
 * 
 *    Such a goal is hopefully accomplished by:
 *    * being as branchless as reasonable/convenient;
 *    * not handling uninteresting corner cases, such as invalid,
 *      infinity, and NaN input values;
 *    * returning approximated results (indicated in this documentation).
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
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added `bw_ceilf()`, `bw_intfracf()`, `bw_sinhf_3()`,
 *              `bw_coshf_3()`, `bw_asinhf_3()`, and `bw_acoshf_3()`.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Added `bw_log10f_3()`, `bw_pow10f_3()`, `bw_dB2linf_3()`, and
 *              `bw_lin2dBf_3()`.</li>
 *          <li>Fixed computation bug in `bw_sqrtf_2()`.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>Added `bw_sin2pif_3()`, `bw_cos2pif_3()`, `bw_tan2pif_3()`,
 *              `bw_omega_3lognr()`, and `bw_tanhf_3()`.</li>
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

#ifndef _BW_MATH_H
#define _BW_MATH_H

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
 *    #### bw_min0xf()
 *  ```>>> */
static inline float bw_min0xf(float x);
/*! <<<```
 *    Returns the minimum of `0.f` and `x`.
 *
 *    `x` must be finite.
 *
 *    #### bw_max0xf()
 *  ```>>> */
static inline float bw_max0xf(float x);
/*! <<<```
 *    Returns the maximum of `0.f` and `x`.
 *
 *    `x` must be finite.
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
 *    `x`, `m`, and `M` must be finite.
 *
 *    #### bw_truncf()
 *  ```>>> */
static inline float bw_truncf(float x);
/*! <<<```
 *    Returns `x` with its fractional part set to zero (i.e., rounded towards
 *    zero).
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
 *    #### bw_floorf()
 *  ```>>> */
static inline float bw_floorf(float x);
/*! <<<```
 *    Returns the biggest integer less or equal than `x` (i.e., `x` is rounded
 *    down).
 *
 *    #### bw_ceilf()
 *  ```>>> */
static inline float bw_ceilf(float x);
/*! <<<```
 *    Returns the smallest integer greater or equal than `x` (i.e., `x` is
 *    rounded up).
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
 *    #### bw_omega_3log()
 *  ```>>> */
static inline float bw_omega_3log(float x);
/*! <<<```
 *    Returns an approximation of `omega(x)`, where `omega` is the <a
 *    href="https://en.wikipedia.org/wiki/Wright_omega_function"
 *    target="_blank">Wright omega function</a>.
 *
 *    Absolute error < 0.27, relative error < 14.6% for non-negative input and
 *    decreasing with increasing input, goes rapidly to 100% for progressively
 *    more negative input.
 *
 *    #### bw_omega_3lognr()
 *  ```>>> */
static inline float bw_omega_3lognr(float x);
/*! <<<```
 *    Returns an approximation of `omega(x)`, where `omega` is the <a
 *    href="https://en.wikipedia.org/wiki/Wright_omega_function"
 *    target="_blank">Wright omega function</a>.
 *
 *    Absolute error < 0.045, relative error < 3.7%.
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

typedef union {
	float f;
	int32_t i;
	uint32_t u;
} _bw_floatint;

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
	_bw_floatint v = {.f = x};
	_bw_floatint s = {.f = y};
	v.i = (v.i & 0x7fffffff) | (s.i & 0x80000000);
	return v.f;
}

static inline float bw_signf(float x) {
	static const float y[4] = { 0.f, 1.f, 0.f, -1.f };
	_bw_floatint v = {.f = x};
	return y[bw_minu32(v.u & 0x7fffffff, 1) | ((v.u >> 30) & 0x2)];
}

static inline float bw_absf(float x) {
	_bw_floatint v = {.f = x};
	v.i = v.i & 0x7fffffff;
	return v.f;
}

static inline float bw_min0xf(float x) {
	return 0.5f * (x - bw_absf(x));
}

static inline float bw_max0xf(float x) {
	return 0.5f * (x + bw_absf(x));
}

static inline float bw_minf(float a, float b) {
	return a + bw_min0xf(b - a);
}

static inline float bw_maxf(float a, float b) {
	return a + bw_max0xf(b - a);
}

static inline float bw_clipf(float x, float m, float M) {
	return bw_minf(bw_maxf(x, m), M);
}

static inline float bw_truncf(float x) {
	_bw_floatint v = {.f = x};
	int32_t ex = (v.i & 0x7f800000) >> 23;
	int32_t m = (~0) << bw_clipi32(150 - ex, 0, 23);
	m &= bw_signfilli32(126 - ex) | 0x80000000;
	v.i &= m;
	return v.f;
}

static inline float bw_roundf(float x) {
	_bw_floatint v = {.f = x};
	int32_t ex = (v.i & 0x7f800000) >> 23;
	int32_t sh = bw_clipi32(150 - ex, 0, 23);
	int32_t mt = (~0) << sh;
	mt &= bw_signfilli32(126 - ex) | 0x80000000;
	int32_t mr = (1 << sh) >> 1;
	mr &= bw_signfilli32(125 - ex);
	_bw_floatint s = {.f = bw_copysignf(1.f, x)};
	int32_t ms = bw_signfilli32((v.i & mr) << (32 - sh));
	v.i &= mt;
	s.i &= ms;
	return v.f + s.f;
}

static inline float bw_floorf(float x) {
	_bw_floatint t = {.f = bw_truncf(x)}; // first bit set when t < 0
	_bw_floatint y = {.f = x - t.f}; // first bit set when t > x
	_bw_floatint s = {.f = 1.f};
	s.i &= bw_signfilli32(t.i & y.i);
	return t.f - s.f;
}

static inline float bw_ceilf(float x) {
	_bw_floatint t = {.f = bw_truncf(x)}; // first bit set when t < 0
	_bw_floatint y = {.f = x - t.f}; // first bit set when t > x
	_bw_floatint s = {.f = 1.f};
	s.i &= bw_signfilli32(~t.i & y.i);
	return t.f + s.f;
}

static inline void bw_intfracf(float x, float *i, float *f) {
	*i = bw_floorf(x);
	*f = x - *i;
}

static inline float bw_rcpf_2(float x) {
	_bw_floatint v = {.f = x};
	v.i = 0x7ef0e840 - v.i;
	v.f = v.f + v.f - x * v.f * v.f;
	v.f = v.f + v.f - x * v.f * v.f;
	return v.f;
}

static inline float bw_sin2pif_3(float x) {
	x = x - bw_floorf(x);
	float xp1 = x + x - 1.f;
	float xp2 = bw_absf(xp1);
	float xp = 1.570796326794897f - 1.570796326794897f * bw_absf(xp2 + xp2 - 1.f);
	return -bw_copysignf(1.f, xp1) * (xp + xp * xp * (-0.05738534102710938f - 0.1107398163618408f * xp));
}

static inline float bw_sinf_3(float x) {
	return bw_sin2pif_3(0.1591549430918953f * x);
}

static inline float bw_cos2pif_3(float x) {
	return bw_sin2pif_3(x + 0.25f);
}

static inline float bw_cosf_3(float x) {
	return bw_cos2pif_3(0.1591549430918953f * x);
}

static inline float bw_tan2pif_3(float x) {
	return bw_sin2pif_3(x) * bw_rcpf_2(bw_cos2pif_3(x));
}

static inline float bw_tanf_3(float x) {
	x = 0.1591549430918953f * x;
	return bw_sin2pif_3(x) * bw_rcpf_2(bw_cos2pif_3(x));
}

static inline float bw_log2f_3(float x) {
	_bw_floatint v = {.f = x};
	int e = v.i >> 23;
	v.i = (v.i & 0x007fffff) | 0x3f800000;
	return (float)e - 129.213475204444817f + v.f * (3.148297929334117f + v.f * (-1.098865286222744f + v.f * 0.1640425613334452f));
}

static inline float bw_logf_3(float x) {
	return 0.693147180559945f * bw_log2f_3(x);
}

static inline float bw_log10f_3(float x) {
	return 0.3010299956639811f * bw_log2f_3(x);
}

static inline float bw_pow2f_3(float x) {
	if (x < -126.f)
		return 0.f;
	_bw_floatint v = {.f = x};
	int xi = (int)x;
	int l = xi - ((v.i >> 31) & 1);
	float f = x - (float)l;
	v.i = (l + 127) << 23;
	return v.f + v.f * f * (0.6931471805599453f + f * (0.2274112777602189f + f * 0.07944154167983575f));
}

static inline float bw_expf_3(float x) {
	return bw_pow2f_3(1.442695040888963f * x);
}

static inline float bw_pow10f_3(float x) {
	return 3.321928094887363f * bw_pow2f_3(x);
}

static inline float bw_dB2linf_3(float x) {
	return bw_pow2f_3(0.1660964047443682f * x);
}

static inline float bw_lin2dBf_3(float x) {
	return 20.f * bw_log10f_3(x);
}

static inline float bw_omega_3log(float x) {
	static const float x1 = -3.341459552768620f;
	static const float x2 = 8.f;
	static const float a = -1.314293149877800e-3f;
	static const float b = 4.775931364975583e-2f;
	static const float c = 3.631952663804445e-1f;
	static const float d = 6.313183464296682e-1f;
	x = bw_maxf(x, x1);
	return x <= x2 ? d + x * (c + x * (b + x * a)) : x - bw_logf_3(x);
}

static inline float bw_omega_3lognr(float x) {
	// omega(x) ~ x with relative error smaller than epsilon (2^-23) for x > 1.6e8)
	// (need to avoid big arguments for bw_rcpf_2())
	if (x > 1.6e8f)
		return x;
	float y = bw_omega_3log(x);
	return y - (y - bw_expf_3(x - y)) * bw_rcpf_2(y + 1.f);
}

static inline float bw_sqrtf_2(float x) {
	_bw_floatint v = {.f = x};
	v.u = (((v.u - 0x3f82a127) >> 1) + 0x3f7d8fc7) & 0x7fffffff;
	float r = bw_rcpf_2(x);
	v.f = v.f + v.f * (0.5f - 0.5f * r * v.f * v.f);
	v.f = v.f + v.f * (0.5f - 0.5f * r * v.f * v.f);
	return v.f;
}

static inline float bw_tanhf_3(float x) {
	const float xm = bw_clipf(x, -2.115287308554551f, 2.115287308554551f);
	const float axm = bw_absf(xm);
	return xm * axm * (0.01218073260037716f * axm - 0.2750231331124371f) + xm;
}

static inline float bw_sinhf_3(float x) {
	return 0.5f * (bw_expf_3(x) - bw_expf_3(-x));
}

static inline float bw_coshf_3(float x) {
	return 0.5f * (bw_expf_3(x) + bw_expf_3(-x));
}

static inline float bw_asinhf_3(float x) {
	float a = bw_absf(x);
	return bw_copysignf(bw_logf_3(bw_sqrtf_2(a * a + 1.f) + a), x);
}

static inline float bw_acoshf_3(float x) {
	return x == 0.f ? 0.f : bw_logf_3(bw_sqrtf_2(x * x + 1.f) + x);
}

#ifdef __cplusplus
}
#endif

#endif
