/*
 * Brickworks
 *
 * Copyright (C) 2023, 2024 Orastron Srl unipersonale
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
 *  module_type {{{ dsp }}}
 *  version {{{ 1.1.1 }}}
 *  requires {{{ bw_common bw_gain bw_math bw_one_pole }}}
 *  description {{{
 *    Stereo balance.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.1.1</strong>:
 *        <ul>
 *          <li>Added debugging check in <code>bw_balance_process_multi()</code>
 *              to ensure that buffers used for both input and output appear at
 *              the same channel indices.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.1.0</strong>:
 *        <ul>
 *          <li>Now using <code>BW_NULL</code> and
 *              <code>BW_CXX_NO_ARRAY</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_balance_process()</code> and
 *              <code>bw_balance_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Clearly specified parameter validity ranges.</li>
 *          <li>Added debugging code.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_balance_process_multi()</code>.</li>
 *          <li><code>bw_balance_process()</code> does not accept
 *              <code>NULL</code> buffers anymore.</li>
 *          <li>Fixed documentation of <code>bw_balance_process1()</code>.</li>
 *          <li>Added C++ wrapper.</li>
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

#ifndef BW_BALANCE_H
#define BW_BALANCE_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_balance_coeffs
 *  ```>>> */
typedef struct bw_balance_coeffs bw_balance_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_balance_init()
 *  ```>>> */
static inline void bw_balance_init(
	bw_balance_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_balance_set_sample_rate()
 *  ```>>> */
static inline void bw_balance_set_sample_rate(
	bw_balance_coeffs * BW_RESTRICT coeffs,
	float                           sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_balance_reset_coeffs()
 *  ```>>> */
static inline void bw_balance_reset_coeffs(
	bw_balance_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_balance_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_balance_update_coeffs_ctrl(
	bw_balance_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_balance_update_coeffs_audio()
 *  ```>>> */
static inline void bw_balance_update_coeffs_audio(
	bw_balance_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_balance_process1()
 *  ```>>> */
static inline void bw_balance_process1(
	const bw_balance_coeffs * BW_RESTRICT coeffs,
	float                                 x_l,
	float                                 x_r,
	float * BW_RESTRICT                   y_l,
	float * BW_RESTRICT                   y_r);
/*! <<<```
 *    Processes one set of input samples `x_l` (left) and `x_r` (right) using
 *    `coeffs`. The left and right output samples are put into `y_l` (left) and
 *    `y_r` (right) respectively.
 *
 *    #### bw_balance_process()
 *  ```>>> */
static inline void bw_balance_process(
	bw_balance_coeffs * BW_RESTRICT coeffs,
	const float *                   x_l,
	const float *                   x_r,
	float *                         y_l,
	float *                         y_r,
	size_t                          n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffers `x_l` (left) and
 *    `x_r` (right) and fills the first `n_samples` of the output buffers `y_l`
 *    (left) and `y_r` (right), while using and updating `coeffs` (control and
 *    audio rate).
 *
 *    #### bw_balance_process_multi()
 *  ```>>> */
static inline void bw_balance_process_multi(
	bw_balance_coeffs * BW_RESTRICT coeffs,
	const float * const *           x_l,
	const float * const *           x_r,
	float * const *                 y_l,
	float * const *                 y_r,
	size_t                          n_channels,
	size_t                          n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x_l`
 *    (left) and `x_r` (right) and fills the first `n_samples` of the
 *    `n_channels` output buffers `y_l` (left) and `y_r` (right), while using
 *    and updating the common `coeffs` (control and audio rate).
 *
 *    #### bw_balance_set_balance()
 *  ```>>> */
static inline void bw_balance_set_balance(
	bw_balance_coeffs * BW_RESTRICT coeffs,
	float                           value);
/*! <<<```
 *    Sets the balance `value`, where `-1.f` corresponds to hard left balance,
 *    `0.f` to center balance, and `1.f` to hard right balance.
 *
 *    Valid range: [`-1.f` (hard left balance), `1.f` (hard right balance)].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_balance_coeffs_is_valid()
 *  ```>>> */
static inline char bw_balance_coeffs_is_valid(
	const bw_balance_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_balance_coeffs`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_gain.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_balance_coeffs_state {
	bw_balance_coeffs_state_invalid,
	bw_balance_coeffs_state_init,
	bw_balance_coeffs_state_set_sample_rate,
	bw_balance_coeffs_state_reset_coeffs
};
#endif

struct bw_balance_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_balance_coeffs_state	state;
#endif

	// Sub-components
	bw_gain_coeffs			l_coeffs;
	bw_gain_coeffs			r_coeffs;

	// Parameters
	float				balance;
	float				balance_prev;
};

static inline void bw_balance_init(
		bw_balance_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	bw_gain_init(&coeffs->l_coeffs);
	bw_gain_init(&coeffs->r_coeffs);
	coeffs->balance = 0.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_balance_coeffs");
	coeffs->state = bw_balance_coeffs_state_init;
#endif
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_balance_coeffs_state_init);
}

static inline void bw_balance_set_sample_rate(
		bw_balance_coeffs * BW_RESTRICT coeffs,
		float                           sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_gain_set_sample_rate(&coeffs->l_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->r_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_balance_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_balance_coeffs_state_set_sample_rate);
}

static inline void bw_balance_do_update_coeffs(
		bw_balance_coeffs * BW_RESTRICT coeffs,
		char                            force) {
	if (force || coeffs->balance != coeffs->balance_prev) {
		bw_gain_set_gain_lin(&coeffs->l_coeffs, bw_minf(1.f - coeffs->balance, 1.f));
		bw_gain_set_gain_lin(&coeffs->r_coeffs, bw_minf(1.f + coeffs->balance, 1.f));
		coeffs->balance_prev = coeffs->balance;
	}
}

static inline void bw_balance_reset_coeffs(
		bw_balance_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_set_sample_rate);

	bw_balance_do_update_coeffs(coeffs, 1);
	bw_gain_reset_coeffs(&coeffs->l_coeffs);
	bw_gain_reset_coeffs(&coeffs->r_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_balance_coeffs_state_reset_coeffs;
#endif
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_balance_coeffs_state_reset_coeffs);
}

static inline void bw_balance_update_coeffs_ctrl(
		bw_balance_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);

	bw_balance_do_update_coeffs(coeffs, 0);
	bw_gain_update_coeffs_ctrl(&coeffs->l_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->r_coeffs);

	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);
}

static inline void bw_balance_update_coeffs_audio(
		bw_balance_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);

	bw_gain_update_coeffs_audio(&coeffs->l_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->r_coeffs);

	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);
}

static inline void bw_balance_process1(
		const bw_balance_coeffs * BW_RESTRICT coeffs,
		float                                 x_l,
		float                                 x_r,
		float * BW_RESTRICT                   y_l,
		float * BW_RESTRICT                   y_r) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(x_l));
	BW_ASSERT(bw_is_finite(x_r));
	BW_ASSERT(y_l != y_r);

	*y_l = bw_gain_process1(&coeffs->l_coeffs, x_l);
	*y_r = bw_gain_process1(&coeffs->r_coeffs, x_r);

	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(*y_l));
	BW_ASSERT(bw_is_finite(*y_r));
}

static inline void bw_balance_process(
		bw_balance_coeffs * BW_RESTRICT coeffs,
		const float *                   x_l,
		const float *                   x_r,
		float *                         y_l,
		float *                         y_r,
		size_t                          n_samples){
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);
	BW_ASSERT(x_l != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x_l, n_samples));
	BW_ASSERT(x_r != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x_r, n_samples));
	BW_ASSERT(y_l != BW_NULL);
	BW_ASSERT(y_r != BW_NULL);
	BW_ASSERT(y_l != y_r);

	bw_balance_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_balance_update_coeffs_audio(coeffs);
		bw_balance_process1(coeffs, x_l[i], x_r[i], y_l + i, y_r + i);
	}

	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_has_only_finite(y_l, n_samples));
	BW_ASSERT_DEEP(bw_has_only_finite(y_r, n_samples));
}

static inline void bw_balance_process_multi(
		bw_balance_coeffs * BW_RESTRICT coeffs,
		const float * const *           x_l,
		const float * const *           x_r,
		float * const *                 y_l,
		float * const *                 y_r,
		size_t                          n_channels,
		size_t                          n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);
	BW_ASSERT(x_l != BW_NULL);
	BW_ASSERT(x_r != BW_NULL);
	BW_ASSERT(y_l != BW_NULL);
	BW_ASSERT(y_r != BW_NULL);
	BW_ASSERT(y_l != y_r);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++) {
			BW_ASSERT(y_l[i] != y_l[j]);
			BW_ASSERT(y_r[i] != y_r[j]);
		}
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = 0; j < n_channels; j++)
			BW_ASSERT(y_l[i] != y_r[j]);
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = 0; j < n_channels; j++) {
			BW_ASSERT(i == j || x_l[i] != y_l[j]);
			BW_ASSERT(i == j || x_l[i] != y_r[j]);
			BW_ASSERT(i == j || x_r[i] != y_l[j]);
			BW_ASSERT(i == j || x_r[i] != y_r[j]);
		}
#endif

	bw_balance_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_balance_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			bw_balance_process1(coeffs, x_l[j][i], x_r[j][i], y_l[j] + i, y_r[j] + i);
	}

	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_reset_coeffs);
}

static inline void bw_balance_set_balance(
		bw_balance_coeffs * BW_RESTRICT coeffs,
		float                           value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= -1.f && value <= 1.f);

	coeffs->balance = value;

	BW_ASSERT_DEEP(bw_balance_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_balance_coeffs_state_init);
}

static inline char bw_balance_coeffs_is_valid(
		const bw_balance_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_balance_coeffs"))
		return 0;
	if (coeffs->state < bw_balance_coeffs_state_init || coeffs->state > bw_balance_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->balance) || coeffs->balance < -1.f || coeffs->balance > 1.f)
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_balance_coeffs_state_reset_coeffs) {
		if (!bw_is_finite(coeffs->balance_prev) || coeffs->balance_prev < -1.f || coeffs->balance_prev > 1.f)
			return 0;
	}
#endif

	return bw_gain_coeffs_is_valid(&coeffs->l_coeffs) && bw_gain_coeffs_is_valid(&coeffs->r_coeffs);
}

#ifdef __cplusplus
}

#ifndef BW_CXX_NO_ARRAY
# include <array>
#endif

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Balance
 *  ```>>> */
template<size_t N_CHANNELS>
class Balance {
public:
	Balance();

	void setSampleRate(
		float sampleRate);

	void reset();

	void process(
		const float * const * xL,
		const float * const * xR,
		float * const *       yL,
		float * const *       yR,
		size_t                nSamples);

#ifndef BW_CXX_NO_ARRAY
	void process(
		std::array<const float *, N_CHANNELS> xL,
		std::array<const float *, N_CHANNELS> xR,
		std::array<float *, N_CHANNELS>       yL,
		std::array<float *, N_CHANNELS>       yR,
		size_t                                nSamples);
#endif

	void setBalance(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_balance_coeffs	coeffs;
};

template<size_t N_CHANNELS>
inline Balance<N_CHANNELS>::Balance() {
	bw_balance_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_balance_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::reset() {
	bw_balance_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::process(
		const float * const * xL,
		const float * const * xR,
		float * const *       yL,
		float * const *       yR,
		size_t                nSamples) {
	bw_balance_process_multi(&coeffs, xL, xR, yL, yR, N_CHANNELS, nSamples);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> xL,
		std::array<const float *, N_CHANNELS> xR,
		std::array<float *, N_CHANNELS>       yL,
		std::array<float *, N_CHANNELS>       yR,
		size_t                                nSamples) {
	process(xL.data(), xR.data(), yL.data(), yR.data(), nSamples);
}
#endif

template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::setBalance(
		float value) {
	bw_balance_set_balance(&coeffs, value);
}

}
#endif

#endif
