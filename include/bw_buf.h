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
 *  version {{{ 0.6.0 }}}
 *  requires {{{ bw_common }}}
 *  description {{{
 *    Common operations on buffers.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
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

#ifndef _BW_BUF_H
#define _BW_BUF_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_buf_fill()
 *  ```>>> */
static inline void bw_buf_fill(float *dest, float k, BW_SIZE_T n_elems);
/*! <<<```
 *    Sets the first `n_elems` in `dest` to `k`.
 *
 *    #### bw_buf_neg()
 *  ```>>> */
static inline void bw_buf_neg(float *dest, const float *src, BW_SIZE_T n_elems);
/*! <<<```
 *    Inverts the sign of the first `n_elems` in `src` and stores the results in
 *    the first `n_elems` of `dest`.
 *
 *    #### bw_buf_add()
 *  ```>>> */
static inline void bw_buf_add(float *dest, const float *src, float k, BW_SIZE_T n_elems);
/*! <<<```
 *    Adds `k` to the first `n_elems` in `src` and stores the results in the
 *    first `n_elems` of `dest`.
 *
 *    #### bw_buf_scale()
 *  ```>>> */
static inline void bw_buf_scale(float *dest, const float *src, float k, BW_SIZE_T n_elems);
/*! <<<```
 *    Multiplies the first `n_elems` in `src` by `k` and stores the results in
 *    the first `n_elems` of `dest`.
 *
 *    #### bw_buf_mix()
 *  ```>>> */
static inline void bw_buf_mix(float *dest, const float *src1, const float *src2, BW_SIZE_T n_elems);
/*! <<<```
 *    Adds the first `n_elems` of `src1` and `src2` and stores the results in
 *    the first `n_elems` of `dest`.
 *
 *    #### bw_buf_mul()
 *  ```>>> */
static inline void bw_buf_mul(float *dest, const float *src1, const float *src2, BW_SIZE_T n_elems);
/*! <<<```
 *    Multiplies the first `n_elems` of `src1` and `src2` and stores the results
 *    in the first `n_elems` of `dest`.
 *
 *    #### bw_buf_fill_multi()
 *  ```>>> */
static inline void bw_buf_fill_multi(float **dest, float k, BW_SIZE_T n_channels, BW_SIZE_T n_elems);
/*! <<<```
 *    Sets the first `n_elems` in each of the `n_channels` buffers `dest` to
 *    `k`.
 *
 *    #### bw_buf_neg_multi()
 *  ```>>> */
static inline void bw_buf_neg_multi(float **dest, const float **src, BW_SIZE_T n_channels, BW_SIZE_T n_elems);
/*! <<<```
 *    Inverts the sign of the first `n_elems` in each of the `n_channels`
 *    buffers `src` and stores the results in the first `n_elems` in each of the
 *    `n_channels` buffers `dest`.
 *
 *    #### bw_buf_add_multi()
 *  ```>>> */
static inline void bw_buf_add_multi(float **dest, const float **src, float k, BW_SIZE_T n_channels, BW_SIZE_T n_elems);
/*! <<<```
 *    Adds `k` to the first `n_elems` in each of the `n_channels` buffers `src`
 *    and stores the results in the first `n_elems` in each of the `n_channels`
 *    buffers `dest`.
 *
 *    #### bw_buf_scale_multi()
 *  ```>>> */
static inline void bw_buf_scale_multi(float **dest, const float **src, float k, BW_SIZE_T n_channels, BW_SIZE_T n_elems);
/*! <<<```
 *    Multiplies the first `n_elems` in each of the `n_channels` buffers `src`
 *    by `k` and stores the results in the first `n_elems` in each of the
 *    `n_channels` buffers `dest`.
 *
 *    #### bw_buf_mix_multi()
 *  ```>>> */
static inline void bw_buf_mix_multi(float **dest, const float **src1, const float **src2, BW_SIZE_T n_channels, BW_SIZE_T n_elems);
/*! <<<```
 *    Adds the first `n_elems` in each of the `n_channels` buffers `src1` and
 *    `src2` and stores the results in the first `n_elems` in each of the
 *    `n_channels` buffers `dest`.
 *
 *    #### bw_buf_mul_multi()
 *  ```>>> */
static inline void bw_buf_mul_multi(float **dest, const float **src1, const float **src2, BW_SIZE_T n_channels, BW_SIZE_T n_elems);
/*! <<<```
 *    Multiplies the first `n_elems` in each of the `n_channels` buffers `src1`
 *    and `src2` and stores the results in the first `n_elems` in each of the
 *    `n_channels` buffers `dest`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

static inline void bw_buf_fill(float *dest, float k, BW_SIZE_T n_elems) {
	BW_ASSERT(!(dest == NULL && n_elems != 0));
	BW_ASSERT(!bw_is_nan(k));

	for (BW_SIZE_T i = 0; i < n_elems; i++)
		dest[i] = k;

	BW_ASSERT(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_neg(float *dest, const float *src, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src == NULL) && n_elems != 0));
	BW_ASSERT(!bw_has_nan(src, n_elems));

	for (BW_SIZE_T i = 0; i < n_elems; i++)
		dest[i] = -src[i];

	BW_ASSERT(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_add(float *dest, const float *src, float k, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src == NULL) && n_elems != 0));
	BW_ASSERT(!bw_has_nan(src, n_elems));
	BW_ASSERT(!bw_is_nan(k));

	for (BW_SIZE_T i = 0; i < n_elems; i++)
		dest[i] = k + src[i];

	BW_ASSERT(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_scale(float *dest, const float *src, float k, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src == NULL) && n_elems != 0));
	BW_ASSERT(!bw_has_nan(src, n_elems));
	BW_ASSERT(!bw_is_nan(k));

	for (BW_SIZE_T i = 0; i < n_elems; i++)
		dest[i] = k * src[i];

	BW_ASSERT(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_mix(float *dest, const float *src1, const float *src2, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src1 == NULL || src2 == NULL) && n_elems != 0));
	BW_ASSERT(!bw_has_nan(src1, n_elems));
	BW_ASSERT(!bw_has_nan(src2, n_elems));

	for (BW_SIZE_T i = 0; i < n_elems; i++)
		dest[i] = src1[i] + src2[i];

	BW_ASSERT(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_mul(float *dest, const float *src1, const float *src2, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src1 == NULL || src2 == NULL) && n_elems != 0));
	BW_ASSERT(!bw_has_nan(src1, n_elems));
	BW_ASSERT(!bw_has_nan(src2, n_elems));

	for (BW_SIZE_T i = 0; i < n_elems; i++)
		dest[i] = src1[i] * src2[i];

	BW_ASSERT(!bw_has_nan(dest, n_elems));
}

static inline void bw_buf_fill_multi(float **dest, float k, BW_SIZE_T n_channels, BW_SIZE_T n_elems) {
	BW_ASSERT(!(dest == NULL && n_channels != 0));

	for (BW_SIZE_T i = 0; i < n_channels; i++)
		bw_buf_fill(dest[i], k, n_elems);
}

static inline void bw_buf_neg_multi(float **dest, const float **src, BW_SIZE_T n_channels, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src == NULL) && n_channels != 0));

	for (BW_SIZE_T i = 0; i < n_channels; i++)
		bw_buf_neg(dest[i], src[i], n_elems);
}

static inline void bw_buf_add_multi(float **dest, const float **src, float k, BW_SIZE_T n_channels, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src == NULL) && n_channels != 0));

	for (BW_SIZE_T i = 0; i < n_channels; i++)
		bw_buf_add(dest[i], src[i], k, n_elems);
}

static inline void bw_buf_scale_multi(float **dest, const float **src, float k, BW_SIZE_T n_channels, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src == NULL) && n_channels != 0));

	for (BW_SIZE_T i = 0; i < n_channels; i++)
		bw_buf_scale(dest[i], src[i], k, n_elems);
}

static inline void bw_buf_mix_multi(float **dest, const float **src1, const float **src2, BW_SIZE_T n_channels, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src1 == NULL || src2 == NULL) && n_channels != 0));

	for (BW_SIZE_T i = 0; i < n_channels; i++)
		bw_buf_mix(dest[i], src1[i], src2[i], n_elems);
}

static inline void bw_buf_mul_multi(float **dest, const float **src1, const float **src2, BW_SIZE_T n_channels, BW_SIZE_T n_elems) {
	BW_ASSERT(!((dest == NULL || src1 == NULL || src2 == NULL) && n_channels != 0));

	for (BW_SIZE_T i = 0; i < n_channels; i++)
		bw_buf_mul(dest[i], src1[i], src2[i], n_elems);
}

#ifdef __cplusplus
}
#endif

#endif
