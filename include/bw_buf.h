/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
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
 *    Common operations on buffers.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *          <li>Changed order of arguments to improve consistency.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overladed C++ functions taking C-style arrays as
 *              arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Now enforcing that buffers are never <code>NULL</code> in
 *              debugging code.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Added debugging code.</li>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_buf_*_multi()</code>.</li>
 *          <li>Added <code>bw_buf_neg()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_buf_fill()</code> and
 *              <code>bw_buf_add()</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_BUF_H
#define BW_BUF_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*** Public API ***/

/*! api {{{
 *    #### bw_buf_fill()
 *  ```>>> */
static inline void bw_buf_fill(float k, float *BW_RESTRICT dest, size_t n_elems);
/*! <<<```
 *    Sets the first `n_elems` in `dest` to `k`.
 *
 *    #### bw_buf_neg()
 *  ```>>> */
static inline void bw_buf_neg(const float *src, float *dest, size_t n_elems);
/*! <<<```
 *    Inverts the sign of the first `n_elems` in `src` and stores the results in
 *    the first `n_elems` of `dest`.
 *
 *    #### bw_buf_add()
 *  ```>>> */
static inline void bw_buf_add(const float *src, float k, float *dest, size_t n_elems);
/*! <<<```
 *    Adds `k` to the first `n_elems` in `src` and stores the results in the
 *    first `n_elems` of `dest`.
 *
 *    #### bw_buf_scale()
 *  ```>>> */
static inline void bw_buf_scale(const float *src, float k, float *dest, size_t n_elems);
/*! <<<```
 *    Multiplies the first `n_elems` in `src` by `k` and stores the results in
 *    the first `n_elems` of `dest`.
 *
 *    #### bw_buf_mix()
 *  ```>>> */
static inline void bw_buf_mix(const float *src1, const float *src2, float *dest, size_t n_elems);
/*! <<<```
 *    Adds the first `n_elems` of `src1` and `src2` and stores the results in
 *    the first `n_elems` of `dest`.
 *
 *    #### bw_buf_mul()
 *  ```>>> */
static inline void bw_buf_mul(const float *src1, const float *src2, float *dest, size_t n_elems);
/*! <<<```
 *    Multiplies the first `n_elems` of `src1` and `src2` and stores the results
 *    in the first `n_elems` of `dest`.
 *
 *    #### bw_buf_fill_multi()
 *  ```>>> */
static inline void bw_buf_fill_multi(float k, float *BW_RESTRICT const *BW_RESTRICT dest, size_t n_channels, size_t n_elems);
/*! <<<```
 *    Sets the first `n_elems` in each of the `n_channels` buffers `dest` to
 *    `k`.
 *
 *    #### bw_buf_neg_multi()
 *  ```>>> */
static inline void bw_buf_neg_multi(const float * const *src, float * const *dest, size_t n_channels, size_t n_elems);
/*! <<<```
 *    Inverts the sign of the first `n_elems` in each of the `n_channels`
 *    buffers `src` and stores the results in the first `n_elems` in each of the
 *    `n_channels` buffers `dest`.
 *
 *    #### bw_buf_add_multi()
 *  ```>>> */
static inline void bw_buf_add_multi(const float * const *src, float k, float * const *dest, size_t n_channels, size_t n_elems);
/*! <<<```
 *    Adds `k` to the first `n_elems` in each of the `n_channels` buffers `src`
 *    and stores the results in the first `n_elems` in each of the `n_channels`
 *    buffers `dest`.
 *
 *    #### bw_buf_scale_multi()
 *  ```>>> */
static inline void bw_buf_scale_multi(const float * const *src, float k, float * const *dest, size_t n_channels, size_t n_elems);
/*! <<<```
 *    Multiplies the first `n_elems` in each of the `n_channels` buffers `src`
 *    by `k` and stores the results in the first `n_elems` in each of the
 *    `n_channels` buffers `dest`.
 *
 *    #### bw_buf_mix_multi()
 *  ```>>> */
static inline void bw_buf_mix_multi(const float * const *src1, const float * const *src2, float * const *dest, size_t n_channels, size_t n_elems);
/*! <<<```
 *    Adds the first `n_elems` in each of the `n_channels` buffers `src1` and
 *    `src2` and stores the results in the first `n_elems` in each of the
 *    `n_channels` buffers `dest`.
 *
 *    #### bw_buf_mul_multi()
 *  ```>>> */
static inline void bw_buf_mul_multi(const float * const *src1, const float * const *src2, float * const *dest, size_t n_channels, size_t n_elems);
/*! <<<```
 *    Multiplies the first `n_elems` in each of the `n_channels` buffers `src1`
 *    and `src2` and stores the results in the first `n_elems` in each of the
 *    `n_channels` buffers `dest`.
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

static inline void bw_buf_fill(float k, float *BW_RESTRICT dest, size_t n_elems) {
	BW_ASSERT(!bw_is_nan(k));
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_elems; i++)
		dest[i] = k;

	BW_ASSERT_DEEP(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_neg(const float *src, float *dest, size_t n_elems) {
	BW_ASSERT(src != NULL);
	BW_ASSERT_DEEP(!bw_has_nan(src, n_elems));
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_elems; i++)
		dest[i] = -src[i];

	BW_ASSERT_DEEP(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_add(const float *src, float k, float *dest, size_t n_elems) {
	BW_ASSERT(src != NULL);
	BW_ASSERT_DEEP(!bw_has_nan(src, n_elems));
	BW_ASSERT(!bw_is_nan(k));
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_elems; i++)
		dest[i] = k + src[i];

	BW_ASSERT_DEEP(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_scale(const float *src, float k, float *dest, size_t n_elems) {
	BW_ASSERT(src != NULL);
	BW_ASSERT_DEEP(!bw_has_nan(src, n_elems));
	BW_ASSERT(!bw_is_nan(k));
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_elems; i++)
		dest[i] = k * src[i];

	BW_ASSERT_DEEP(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_mix(const float *src1, const float *src2, float *dest, size_t n_elems) {
	BW_ASSERT(src1 != NULL);
	BW_ASSERT_DEEP(!bw_has_nan(src1, n_elems));
	BW_ASSERT(src2 != NULL);
	BW_ASSERT_DEEP(!bw_has_nan(src2, n_elems));
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_elems; i++)
		dest[i] = src1[i] + src2[i];

	BW_ASSERT_DEEP(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_mul(const float *src1, const float *src2, float *dest, size_t n_elems) {
	BW_ASSERT(src1 != NULL);
	BW_ASSERT_DEEP(!bw_has_nan(src1, n_elems));
	BW_ASSERT(src2 != NULL);
	BW_ASSERT_DEEP(!bw_has_nan(src2, n_elems));
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_elems; i++)
		dest[i] = src1[i] * src2[i];

	BW_ASSERT_DEEP(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_fill_multi(float k, float *BW_RESTRICT const *BW_RESTRICT dest, size_t n_channels, size_t n_elems) {
	BW_ASSERT(!bw_is_nan(k));
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_channels; i++)
		bw_buf_fill(k, dest[i], n_elems);
}

static inline void bw_buf_neg_multi(const float * const *src, float * const *dest, size_t n_channels, size_t n_elems) {
	BW_ASSERT(src != NULL);
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_channels; i++)
		bw_buf_neg(src[i], dest[i], n_elems);
}

static inline void bw_buf_add_multi(const float * const *src, float k, float * const *dest, size_t n_channels, size_t n_elems) {
	BW_ASSERT(src != NULL);
	BW_ASSERT(!bw_is_nan(k));
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_channels; i++)
		bw_buf_add(src[i], k, dest[i], n_elems);
}

static inline void bw_buf_scale_multi(const float * const *src, float k, float * const *dest, size_t n_channels, size_t n_elems) {
	BW_ASSERT(src != NULL);
	BW_ASSERT(!bw_is_nan(k));
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_channels; i++)
		bw_buf_scale(src[i], k, dest[i], n_elems);
}

static inline void bw_buf_mix_multi(const float * const *src1, const float * const *src2, float * const *dest, size_t n_channels, size_t n_elems) {
	BW_ASSERT(src1 != NULL);
	BW_ASSERT(src2 != NULL);
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_channels; i++)
		bw_buf_mix(src1[i], src2[i], dest[i], n_elems);
}

static inline void bw_buf_mul_multi(const float * const *src1, const float * const *src2, float * const *dest, size_t n_channels, size_t n_elems) {
	BW_ASSERT(src1 != NULL);
	BW_ASSERT(src2 != NULL);
	BW_ASSERT(dest != NULL);

	for (size_t i = 0; i < n_channels; i++)
		bw_buf_mul(src1[i], src2[i], dest[i], n_elems);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::bufFill
 *  ```>>> */
template<size_t N_CHANNELS>
inline void bufFill(
		float k,
		float *BW_RESTRICT const *BW_RESTRICT dest,
		int nSamples);

template<size_t N_CHANNELS>
inline void bufFill(
		float k,
		std::array<float *BW_RESTRICT, N_CHANNELS> dest,
		int nSamples);
/*! <<<```
 *
 *    ##### Brickworks::bufNeg
 *  ```>>> */
template<size_t N_CHANNELS>
inline void bufNeg(
		const float * const *src,
		float * const *dest,
		int nSamples);

template<size_t N_CHANNELS>
inline void bufNeg(
		std::array<const float *, N_CHANNELS> src,
		std::array<float *, N_CHANNELS> dest,
		int nSamples);
/*! <<<```
 *
 *    ##### Brickworks::bufAdd
 *  ```>>> */		
template<size_t N_CHANNELS>
inline void bufAdd(
		const float * const *src,
		float * const *dest,
		float k,
		int nSamples);

template<size_t N_CHANNELS>
inline void bufAdd(
		std::array<const float *, N_CHANNELS> src,
		std::array<float *, N_CHANNELS> dest,
		float k,
		int nSamples);
/*! <<<```
 *
 *    ##### Brickworks::bufScale
 *  ```>>> */
template<size_t N_CHANNELS>
inline void bufScale(
		const float * const *src,
		float k,
		float * const *dest,
		int nSamples);

template<size_t N_CHANNELS>
inline void bufScale(
		std::array<const float *, N_CHANNELS> src,
		std::array<float *, N_CHANNELS> dest,
		float k,
		int nSamples);
/*! <<<```
 *
 *    ##### Brickworks::bufMix
 *  ```>>> */
template<size_t N_CHANNELS>
inline void bufMix(
		const float * const *src1,
		const float * const *src2,
		float * const *dest,
		int nSamples);

template<size_t N_CHANNELS>
inline void bufMix(
		std::array<const float *, N_CHANNELS> src1,
		std::array<const float *, N_CHANNELS> src2,
		std::array<float *, N_CHANNELS> dest,
		int nSamples);
/*! <<<```
 *
 *    ##### Brickworks::bufMul
 *  ```>>> */
template<size_t N_CHANNELS>
inline void bufMul(
		const float * const *src1,
		const float * const *src2,
		float * const *dest,
		int nSamples);

template<size_t N_CHANNELS>
inline void bufMul(
		std::array<const float *, N_CHANNELS> src1,
		std::array<const float *, N_CHANNELS> src2,
		std::array<float *, N_CHANNELS> dest,
		int nSamples);
/*! <<<```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

template<size_t N_CHANNELS>
inline void bufFill(
		float k,
		float *BW_RESTRICT const *BW_RESTRICT dest,
		int nSamples) {
	bw_buf_fill_multi(k, dest, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void bufFill(
		float k,
		std::array<float *BW_RESTRICT, N_CHANNELS> dest,
		int nSamples) {
	bufFill<N_CHANNELS>(k, dest.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void bufNeg(
		const float * const *src,
		float * const *dest,
		int nSamples) {
	bw_buf_neg_multi(src, dest, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void bufNeg(
		std::array<const float *, N_CHANNELS> src,
		std::array<float *, N_CHANNELS> dest,
		int nSamples) {
	bufNeg<N_CHANNELS>(src.data(), dest.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void bufAdd(
		const float * const *src,
		float * const *dest,
		float k,
		int nSamples) {
	bw_buf_add_multi(src, k, dest, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void bufAdd(
		std::array<const float *, N_CHANNELS> src,
		float k,
		std::array<float *, N_CHANNELS> dest,
		int nSamples) {
	bufAdd<N_CHANNELS>(src.data(), k, dest.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void bufScale(
		const float * const *src,
		float k,
		float * const *dest,
		int nSamples) {
	bw_buf_scale_multi(src, k, dest, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void bufScale(
		std::array<const float *, N_CHANNELS> src,
		float k,
		std::array<float *, N_CHANNELS> dest,
		int nSamples) {
	bufScale<N_CHANNELS>(src.data(), k, dest.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void bufMix(
		const float * const *src1,
		const float * const *src2,
		float * const *dest,
		int nSamples) {
	bw_buf_mix_multi(src1, src2, dest, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void bufMix(
		std::array<const float *, N_CHANNELS> src1,
		std::array<const float *, N_CHANNELS> src2,
		std::array<float *, N_CHANNELS> dest,
		int nSamples) {
	bufMix<N_CHANNELS>(src1.data(), src2.data(), dest.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void bufMul(
		const float * const *src1,
		const float * const *src2,
		float * const *dest,
		int nSamples) {
	bw_buf_mul_multi(src1, src2, dest, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void bufMul(
		std::array<const float *, N_CHANNELS> src1,
		std::array<const float *, N_CHANNELS> src2,
		std::array<float *, N_CHANNELS> dest,
		int nSamples) {
	bufMul<N_CHANNELS>(src1.data(), src2.data(), dest.data(), nSamples);
}

}
#endif

#endif
