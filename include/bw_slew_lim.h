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
 *          <li>Refactored API to avoid dynamic memory allocation.</li>
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

/*! api {{{
 *    #### bw_slew_lim
 *  ```>>> */
typedef struct _bw_slew_lim bw_slew_lim;
/*! <<<```
 *    Instance object.
 *  >>> */
 
/*! ...
 *    #### bw_slew_lim_init()
 *  ```>>> */
void bw_slew_lim_new(bw_slew_lim *instance);
/*! <<<```
 *    Initializes the `instance` object.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_sample_rate()
 *  ```>>> */
void bw_slew_lim_set_sample_rate(bw_slew_lim *instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_reset()
 *  ```>>> */
void bw_slew_lim_reset(bw_slew_lim *instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_process()
 *  ```>>> */
void bw_slew_lim_process(bw_slew_lim *instance, const float* x, float* y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_init_val()
 *  ```>>> */
void bw_slew_lim_set_init_val(bw_slew_lim *instance, float value);
/*! <<<```
 *    Sets the initial/quiescent `value` for the given `instance`.
 *
 *    In practice, when processing the first buffer after a reset, the past
 *    input and output are both assumed to have virtually been constant and of
 *    the specified `value`.
 *
 *    Default value: `0.f`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_max_rate()
 *  ```>>> */
void bw_slew_lim_set_max_rate(bw_slew_lim *instance, float value);
/*! <<<```
 *    Sets both the maximum increasing and decreasing variation rate to the
 *    given `value` (1/s) for the given `instance`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    This is equivalent to calling both `bw_slew_lim_set_max_inc_rate()` and
 *    `bw_slew_lim_set_max_dec_rate()` with same `instance` and `value`.
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_max_inc_rate()
 *  ```>>> */
void bw_slew_lim_set_max_inc_rate(bw_slew_lim *instance, float value);
/*! <<<```
 *    Sets the maximum increasing variation rate to the given `value` (1/s) for
 *    the given `instance`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_max_inc_rate()
 *  ```>>> */
void bw_slew_lim_set_max_dec_rate(bw_slew_lim *instance, float value);
/*! <<<```
 *    Sets the maximum decreasing variation rate to the given `value` (1/s) for
 *    the given `instance`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    Default value: `INFINITY`.
 *  }}} */

/* WARNING: the internal definition of this struct is not part of the public
 * API. Its content may change at any time in future versions. Please, do not
 * access its members directly. */
struct _bw_slew_lim {
	// Coefficients
	float	T;

	// Parameters
	float	init_val;
	float	max_inc_rate;
	float	max_dec_rate;

	// State
	char	first_run;
	float	y_z1;
};

#ifdef __cplusplus
}
#endif

#endif
