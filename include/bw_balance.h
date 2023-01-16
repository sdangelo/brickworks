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
 *  version {{{ 0.3.0 }}}
 *  requires {{{ bw_config bw_common bw_gain bw_math bw_one_pole }}}
 *  description {{{
 *    Stereo balance.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_BALANCE_H
#define _BW_BALANCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_balance_coeffs
 *  ```>>> */
typedef struct _bw_balance_coeffs bw_balance_coeffs;
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
static inline float bw_balance_process1(const bw_balance_coeffs *BW_RESTRICT coeffs, float x_l, float x_r, float *BW_RESTRICT y_l, float *BW_RESTRICT y_r);
/*! <<<```
 *    Processes one set of input samples `x_l` (left) and `x_r` (right) using
 *    `coeffs`, while using and updating `state`. The left and right output
 *    samples are put into `y_l` (left) and `y_r` (right) respectively.
 *
 *    #### bw_balance_process()
 *  ```>>> */
static inline void bw_balance_process(bw_balance_coeffs *BW_RESTRICT coeffs, const float *x_l, const float *x_r, float *y_l, float *y_r, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffers `x_l` (left) and
 *    `x_r` (right) and fills the first `n_samples` of the output buffers `y_l`
 *    (left) and `y_r` (right), if they are not `NULL`, while using and updating
 *    `coeffs` (control and audio rate).
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

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_gain.h>

struct _bw_balance_coeffs {
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

static inline void _bw_balance_do_update_coeffs(bw_gain_coeffs *BW_RESTRICT coeffs, char force) {
	if (force || coeffs->balance != coeffs->balance_prev) {
		bw_gain_set_gain_lin(&coeffs->l_coeffs, bw_minf(1.f - coeffs->balance, 1.f));
		bw_gain_set_gain_lin(&coeffs->r_coeffs, bw_minf(1.f + coeffs->balance, 1.f));
		coeffs->balance_prev = coeffs->balance;
	}
}

static inline void bw_balance_reset_coeffs(bw_balance_coeffs *BW_RESTRICT coeffs) {
	_bw_balance_do_update_coeffs(coeffs, 1);
	bw_gain_reset_coeffs(&coeffs->l_coeffs);
	bw_gain_reset_coeffs(&coeffs->r_coeffs);
}

static inline void bw_balance_update_coeffs_ctrl(bw_balance_coeffs *BW_RESTRICT coeffs) {
	_bw_balance_do_update_coeffs(coeffs, 0);
	bw_gain_update_coeffs_ctrl(&coeffs->l_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->r_coeffs);
}

static inline void bw_balance_update_coeffs_audio(bw_balance_coeffs *BW_RESTRICT coeffs) {
	bw_gain_update_coeffs_audio(&coeffs->l_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->r_coeffs);
}

static inline float bw_balance_process1(const bw_balance_coeffs *BW_RESTRICT coeffs, float x_l, float x_r, float *BW_RESTRICT y_l, float *BW_RESTRICT y_r) {
	*y_l = bw_gain_process1(&coeffs->l_coeffs, x_l);
	*y_r = bw_gain_process1(&coeffs->r_coeffs, x_r);
}

static inline void bw_balance_process(bw_balance_coeffs *BW_RESTRICT coeffs, const float *x_l, const float *x_r, float *y_l, float *y_r, int n_samples){
	bw_balance_update_coeffs_ctrl(coeffs);
	if (y_l != NULL) {
		if (y_r != NULL) {
			for (int i = 0; i < n_samples; i++) {
				bw_balance_update_coeffs_audio(coeffs);
				bw_balance_process1(coeffs, state, x_l[i], x_r[i], y_l + i, y_r + i);
			}
		} else {
			for (int i = 0; i < n_samples; i++) {
				bw_balance_update_coeffs_audio(coeffs);
				float r;
				bw_balance_process1(coeffs, state, x_l[i], x_r[i], y_l + i, &r);
			}
		}
	} else {
		if (y_r != NULL) {
			for (int i = 0; i < n_samples; i++) {
				bw_balance_update_coeffs_audio(coeffs);
				float l;
				bw_balance_process1(coeffs, state, x_l[i], x_r[i], &l, y_r + i);
			}
		} else {
			for (int i = 0; i < n_samples; i++) {
				bw_balance_update_coeffs_audio(coeffs);
				float l, r;
				bw_balance_process1(coeffs, state, x_l[i], x_r[i], &l, &r);
			}
		}
	}
}

static inline void bw_balance_set_balance(bw_balance_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->balance = value;
}

#ifdef __cplusplus
}
#endif

#endif
