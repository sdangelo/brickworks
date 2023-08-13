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
 *  module_type {{{ dsp }}}
 *  version {{{ 1.0.0 }}}
 *  requires {{{ bw_common bw_gain bw_math bw_one_pole }}}
 *  description {{{
 *    Stereo balance.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_balance_process()</code> and
 *              <code>bw_balance_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overladed C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
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
static inline void bw_balance_init(bw_balance_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_balance_set_sample_rate()
 *  ```>>> */
static inline void bw_balance_set_sample_rate(bw_balance_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_balance_reset_coeffs()
 *  ```>>> */
static inline void bw_balance_reset_coeffs(bw_balance_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_balance_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_balance_update_coeffs_ctrl(bw_balance_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_balance_update_coeffs_audio()
 *  ```>>> */
static inline void bw_balance_update_coeffs_audio(bw_balance_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_balance_process1()
 *  ```>>> */
static inline void bw_balance_process1(const bw_balance_coeffs *BW_RESTRICT coeffs, float x_l, float x_r, float *BW_RESTRICT y_l, float *BW_RESTRICT y_r);
/*! <<<```
 *    Processes one set of input samples `x_l` (left) and `x_r` (right) using
 *    `coeffs`. The left and right output samples are put into `y_l` (left) and
 *    `y_r` (right) respectively.
 *
 *    #### bw_balance_process()
 *  ```>>> */
static inline void bw_balance_process(bw_balance_coeffs *BW_RESTRICT coeffs, const float *x_l, const float *x_r, float *y_l, float *y_r, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffers `x_l` (left) and
 *    `x_r` (right) and fills the first `n_samples` of the output buffers `y_l`
 *    (left) and `y_r` (right), while using and updating `coeffs` (control and
 *    audio rate).
 *
 *    #### bw_balance_process_multi()
 *  ```>>> */
static inline void bw_balance_process_multi(bw_balance_coeffs *BW_RESTRICT coeffs, const float * const *x_l, const float * const *x_r, float * const *y_l, float * const *y_r, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x_l`
 *    (left) and `x_r` (right) and fills the first `n_samples` of the
 *    `n_channels` output buffers `y_l` (left) and `y_r` (right), while using
 *    and updating the common `coeffs` (control and audio rate).
 *
 *    #### bw_balance_set_balance()
 *  ```>>> */
static inline void bw_balance_set_balance(bw_balance_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the balance `value`, where `-1.f` corresponds to hard left balance,
 *    `0.f` to center balance, and `1.f` to hard right balance.
 *
 *    Default value: `0.f`.
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

struct bw_balance_coeffs {
	// Sub-components
	bw_gain_coeffs	l_coeffs;
	bw_gain_coeffs	r_coeffs;

	// Parameters
	float		balance;
	float		balance_prev;
};

static inline void bw_balance_init(bw_balance_coeffs *BW_RESTRICT coeffs) {
	bw_gain_init(&coeffs->l_coeffs);
	bw_gain_init(&coeffs->r_coeffs);
	coeffs->balance = 0.f;
}

static inline void bw_balance_set_sample_rate(bw_balance_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_gain_set_sample_rate(&coeffs->l_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->r_coeffs, sample_rate);
}

static inline void bw_balance_do_update_coeffs(bw_balance_coeffs *BW_RESTRICT coeffs, char force) {
	if (force || coeffs->balance != coeffs->balance_prev) {
		bw_gain_set_gain_lin(&coeffs->l_coeffs, bw_minf(1.f - coeffs->balance, 1.f));
		bw_gain_set_gain_lin(&coeffs->r_coeffs, bw_minf(1.f + coeffs->balance, 1.f));
		coeffs->balance_prev = coeffs->balance;
	}
}

static inline void bw_balance_reset_coeffs(bw_balance_coeffs *BW_RESTRICT coeffs) {
	bw_balance_do_update_coeffs(coeffs, 1);
	bw_gain_reset_coeffs(&coeffs->l_coeffs);
	bw_gain_reset_coeffs(&coeffs->r_coeffs);
}

static inline void bw_balance_update_coeffs_ctrl(bw_balance_coeffs *BW_RESTRICT coeffs) {
	bw_balance_do_update_coeffs(coeffs, 0);
	bw_gain_update_coeffs_ctrl(&coeffs->l_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->r_coeffs);
}

static inline void bw_balance_update_coeffs_audio(bw_balance_coeffs *BW_RESTRICT coeffs) {
	bw_gain_update_coeffs_audio(&coeffs->l_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->r_coeffs);
}

static inline void bw_balance_process1(const bw_balance_coeffs *BW_RESTRICT coeffs, float x_l, float x_r, float *BW_RESTRICT y_l, float *BW_RESTRICT y_r) {
	*y_l = bw_gain_process1(&coeffs->l_coeffs, x_l);
	*y_r = bw_gain_process1(&coeffs->r_coeffs, x_r);
}

static inline void bw_balance_process(bw_balance_coeffs *BW_RESTRICT coeffs, const float *x_l, const float *x_r, float *y_l, float *y_r, size_t n_samples){
	bw_balance_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_balance_update_coeffs_audio(coeffs);
		bw_balance_process1(coeffs, x_l[i], x_r[i], y_l + i, y_r + i);
	}
}

static inline void bw_balance_process_multi(bw_balance_coeffs *BW_RESTRICT coeffs, const float * const *x_l, const float * const *x_r, float * const *y_l, float * const *y_r, size_t n_channels, size_t n_samples) {
	bw_balance_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_balance_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			bw_balance_process1(coeffs, x_l[j][i], x_r[j][i], y_l[j] + i, y_r[j] + i);
	}
}

static inline void bw_balance_set_balance(bw_balance_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->balance = value;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Balance
 *  ```>>> */
template<size_t N_CHANNELS>
class Balance {
public:
	Balance();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		const float * const *x_l,
		const float * const *x_r,
		float * const *y_l,
		float * const *y_r,
		size_t nSamples);
	void process(
		std::array<const float *, N_CHANNELS> x_l,
		std::array<const float *, N_CHANNELS> x_r,
		std::array<float *, N_CHANNELS> y_l,
		std::array<float *, N_CHANNELS> y_r,
		size_t nSamples);

	void setBalance(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_balance_coeffs	 coeffs;
};

template<size_t N_CHANNELS>
inline Balance<N_CHANNELS>::Balance() {
	bw_balance_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_balance_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::reset() {
	bw_balance_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::process(
		const float * const *x_l,
		const float * const *x_r,
		float * const *y_l,
		float * const *y_r,
		size_t nSamples) {
	bw_balance_process_multi(&coeffs, x_l, x_r, y_l, y_r, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x_l,
		std::array<const float *, N_CHANNELS> x_r,
		std::array<float *, N_CHANNELS> y_l,
		std::array<float *, N_CHANNELS> y_r,
		size_t nSamples) {
	process(x_l.data(), x_r.data(), y_l.data(), y_r.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Balance<N_CHANNELS>::setBalance(float value) {
	bw_balance_set_balance(&coeffs, value);
}

}
#endif

#endif
