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
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

/*!
 *  module_type {{{ dsp }}}
 *  version {{{ 0.2.0 }}}
 *  requires {{{ bw_config bw_common bw_math }}}
 *  description {{{
 *    Slew-rate limiter with separate maximum increasing and decreasing rates.
 *  }}}
 *  changelog {{{
 *    <ul>
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

#ifndef _BW_SLEW_LIM_H
#define _BW_SLEW_LIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_slew_lim_coeffs
 *  ```>>> */
typedef struct _bw_slew_lim_coeffs bw_slew_lim_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_slew_lim_state
 *  ```>>> */
typedef struct _bw_slew_lim_state bw_slew_lim_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_slew_lim_init()
 *  ```>>> */
static inline void bw_slew_lim_init(bw_slew_lim_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_slew_lim_set_sample_rate()
 *  ```>>> */
static inline void bw_slew_lim_set_sample_rate(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_slew_lim_reset_coeffs()
 *  ```>>> */
static inline void bw_slew_lim_reset_coeffs(bw_slew_lim_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_slew_lim_reset_state()
 *  ```>>> */
static inline void bw_slew_lim_reset_state(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float y_z1);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/equilibrium value `y_z1`.
 *
 *    #### bw_slew_lim_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_slew_lim_update_coeffs_ctrl(bw_slew_lim_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_one_pole_update_coeffs_audio()
 *  ```>>> */
static inline void bw_slew_lim_update_coeffs_audio(bw_slew_lim_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_slew_lim_process1\*()
 *  ```>>> */
static inline float bw_slew_lim_process1(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x);
static inline float bw_slew_lim_process1_up(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x);
static inline float bw_slew_lim_process1_down(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x);
/*! <<<```
 *    These function process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_slew_lim_process1()` assumes that both the maximum upgoing and
 *       downgoing variation rates are finite;
 *     * `bw_slew_lim_process1_up()` assumes that both the maximum upgoing
 *       variation rate is finite and the maximum downgoing variation rate is
 *       infinite;
 *     * `bw_slew_lim_process1_down()` assumes that both the maximum upgoing
 *       variation rate is infinite and the maximum downgoing variation rate is
 *       finite.
 *
 *    #### bw_slew_lim_process()
 *  ```>>> */
static inline void bw_slew_lim_process(bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *
 *    #### bw_slew_lim_set_max_rate()
 *  ```>>> */
static inline void bw_slew_lim_set_max_rate(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets both the maximum increasing and decreasing variation rate to the
 *    given `value` (1/s) in `coeffs`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    This is equivalent to calling both `bw_slew_lim_set_max_inc_rate()` and
 *    `bw_slew_lim_set_max_dec_rate()` with same `coeffs` and `value`.
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_max_rate_up()
 *  ```>>> */
static inline void bw_slew_lim_set_max_rate_up(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the maximum increasing variation rate to the given `value` (1/s) in
 *    `coeffs`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_max_inc_rate()
 *  ```>>> */
static inline void bw_slew_lim_set_max_rate_down(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the maximum decreasing variation rate to the given `value` (1/s) in
 *    `coeffs`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    Default value: `INFINITY`.
 *  }}} */

static inline float bw_slew_lim_get_y_z1(const bw_slew_lim_state *BW_RESTRICT state);

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

struct _bw_slew_lim_coeffs {
	// Coefficients
	float	T;

	float	max_inc;
	float	max_dec;

	// Parameters
	float	max_rate_up;
	float	max_rate_down;
};

struct _bw_slew_lim_state {
	float	y_z1;
};

static inline void bw_slew_lim_init(bw_slew_lim_coeffs *BW_RESTRICT coeffs) {
	coeffs->max_inc = INFINITY;
	coeffs->max_dec = INFINITY;
}

static inline void bw_slew_lim_set_sample_rate(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->T = 1.f / sample_rate;
}

static inline void bw_slew_lim_reset_coeffs(bw_slew_lim_coeffs *BW_RESTRICT coeffs) {
	bw_slew_lim_update_coeffs_ctrl(coeffs);
}

static inline void bw_slew_lim_reset_state(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float y_z1) {
	state->y_z1 = y_z1;
}

static inline void bw_slew_lim_update_coeffs_ctrl(bw_slew_lim_coeffs *BW_RESTRICT coeffs) {
	// tracking parameter changes is more trouble than it's worth
	coeffs->max_inc = coeffs->T * coeffs->max_rate_up;
	coeffs->max_dec = coeffs->T * coeffs->max_rate_down;
}

static inline void bw_slew_lim_update_coeffs_audio(bw_slew_lim_coeffs *BW_RESTRICT coeffs) {
}

static inline float bw_slew_lim_process1(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x) {
	float y = bw_clipf(x, state->y_z1 - coeffs->max_dec, state->y_z1 + coeffs->max_inc);
	state->y_z1 = y;
	return y;
}

static inline float bw_slew_lim_process1_up(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x) {
	float y = bw_minf(x, state->y_z1 + coeffs->max_inc);
	state->y_z1 = y;
	return y;
}

static inline float bw_slew_lim_process1_down(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x) {
	float y = bw_maxf(x, state->y_z1 - coeffs->max_dec);
	state->y_z1 = y;
	return y;
}

static inline void bw_slew_lim_process(bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_slew_lim_update_coeffs_ctrl(coeffs);
	if (y) {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (int i = 0; i < n_samples; i++)
					y[i] = bw_slew_lim_process1(coeffs, state, x[i]);
			else
				for (int i = 0; i < n_samples; i++)
					y[i] = bw_slew_lim_process1_up(coeffs, state, x[i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (int i = 0; i < n_samples; i++)
					y[i] = bw_slew_lim_process1_down(coeffs, state, x[i]);
			else {
				for (int i = 0; i < n_samples; i++)
					y[i] = x[i];
				state->y_z1 = x[n_samples - 1];
			}
		}
	} else {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (int i = 0; i < n_samples; i++)
					bw_slew_lim_process1(coeffs, state, x[i]);
			else
				for (int i = 0; i < n_samples; i++)
					bw_slew_lim_process1_up(coeffs, state, x[i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (int i = 0; i < n_samples; i++)
					bw_slew_lim_process1_down(coeffs, state, x[i]);
			else
				state->y_z1 = x[n_samples - 1];
		}
	}
}

static inline void bw_slew_lim_set_max_rate(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value) {
	bw_slew_lim_set_max_rate_up(coeffs, value);
	bw_slew_lim_set_max_rate_down(coeffs, value);
}

static inline void bw_slew_lim_set_max_rate_up(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->max_rate_up = value;
}

static inline void bw_slew_lim_set_max_rate_down(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->max_rate_down = value;
}

static inline float bw_slew_lim_get_y_z1(const bw_slew_lim_state *BW_RESTRICT state) {
	return state->y_z1;
}

#ifdef __cplusplus
}
#endif

#endif
