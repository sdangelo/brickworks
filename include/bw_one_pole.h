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
 *    One-pole (6 dB/oct) lowpass filter with unitary DC gain, separate attack
 *    and decay time constants, and sticky target-reach threshold.
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

#ifndef _BW_ONE_POLE_H
#define _BW_ONE_POLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_one_pole
 *  ```>>> */
typedef struct _bw_one_pole bw_one_pole;
/*! <<<```
 *    Instance object.
 *  >>> */
 
/*! ...
 *    #### bw_one_pole_sticky_mode
 *  ```>>> */
typedef enum {
	bw_one_pole_sticky_mode_abs,
	bw_one_pole_sticky_mode_rel
} bw_one_pole_sticky_mode;
/*! <<<```
 *    Distance metrics for sticky behavior:
 *     * `bw_one_pole_sticky_mode_abs`: absolute difference (|`out` - `in`|);
 *     * `bw_one_pole_sticky_mode_rel`: relative difference with respect to
 *       input (|`out` - `in`| / |`in`|);
 *  >>> */

/*! ...
 *    #### bw_one_pole_init()
 *  ```>>> */
void bw_one_pole_init(bw_one_pole *instance);
/*! <<<```
 *    Initializes the `instance` object.
 *  >>> */

/*! ...
 *    #### bw_one_pole_set_sample_rate()
 *  ```>>> */
void bw_one_pole_set_sample_rate(bw_one_pole *instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_one_pole_reset()
 *  ```>>> */
void bw_one_pole_reset(bw_one_pole *instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_one_pole_process()
 *  ```>>> */
void bw_one_pole_process(bw_one_pole *instance, const float* x, float* y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_one_pole_set_init_val()
 *  ```>>> */
void bw_one_pole_set_init_val(bw_one_pole *instance, float value);
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
 *    #### bw_one_pole_set_cutoff()
 *  ```>>> */
void bw_one_pole_set_cutoff(bw_one_pole *instance, float value);
/*! <<<```
 *    Sets both the upgoing (attack) and downgoing (decay) cutoff frequency to
 *    the given `value` (Hz) for the given `instance`.
 *
 *    This is equivalent to calling both `bw_one_pole_set_cutoff_up()` and
 *    `bw_one_pole_set_cutoff_down()` with same `instance` and `value` or
 *    calling `bw_one_pole_set_tau()` with same `instance` and
 *    value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_one_pole_set_cutoff_up()
 *  ```>>> */
void bw_one_pole_set_cutoff_up(bw_one_pole *instance, float value);
/*! <<<```
 *    Sets the upgoing (attack) cutoff frequency to the given `value` (Hz) for
 *    the given `instance`.
 *
 *    This is equivalent to calling `bw_one_pole_set_tau_up()` with same
 *    `instance` and value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_one_pole_set_cutoff_down()
 *  ```>>> */
void bw_one_pole_set_cutoff_down(bw_one_pole *instance, float value);
/*! <<<```
 *    Sets the downgoing (attack) cutoff frequency to the given `value` (Hz)
 *    for the given `instance`.
 *
 *    This is equivalent to calling `bw_one_pole_set_tau_down()` with same
 *    `instance` and value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_one_pole_set_tau()
 *  ```>>> */
void bw_one_pole_set_tau(bw_one_pole *instance, float value);
/*! <<<```
 *    Sets both the upgoing (attack) and downgoing (decay) time constant to the
 *    given `value` (s) for the given `instance`.
 *
 *    This is equivalent to calling both `bw_one_pole_set_tau_up()` and
 *    `bw_one_pole_set_tau_down()` with same `instance` and `value` or calling
 *    `bw_one_pole_set_cutoff()` with same `instance` and
 *    value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    Default value: `0.f`.
 *  >>> */

/*! ...
 *    #### bw_one_pole_set_tau_up()
 *  ```>>> */
void bw_one_pole_set_tau_up(bw_one_pole *instance, float value);
/*! <<<```
 *    Sets the upgoing (attack) time constant to the given `value` (s) for the
 *    given `instance`.
 *
 *    This is equivalent to calling `bw_one_pole_set_cutoff_up()` with same
 *    `instance` and value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    Default value: `0.f`.
 *  >>> */

/*! ...
 *    #### bw_one_pole_set_tau_down()
 *  ```>>> */
void bw_one_pole_set_tau_down(bw_one_pole *instance, float value);
/*! <<<```
 *    Sets the downgoing (decay) time constant to the given `value` (s) for the
 *    given `instance`.
 *
 *    This is equivalent to calling `bw_one_pole_set_cutoff_down()` with same
 *    `instance` and value = 1 / (2 * pi * `value`) (net of numerical errors).
 *
 *    Default value: `0.f`.
 *  >>> */

/*! ...
 *    #### bw_one_pole_set_sticky_thresh()
 *  ```>>> */
void bw_one_pole_set_sticky_thresh(bw_one_pole *instance, float value);
/*! <<<```
 *    Sets the target-reach threshold specified by `value` for the given
 *    `instance`.
 *
 *    When the difference between the output and the input would fall under such
 *    threshold according to the current distance metric (see
 *    `bw_one_pole_set_sticky_mode()`), the output is forcefully set to be equal
 *    to the input value.
 *
 *    Default value: `0.f`.
 *  >>> */

/*! ...
 *    #### bw_one_pole_set_sticky_mode()
 *  ```>>> */
void bw_one_pole_set_sticky_mode(bw_one_pole *instance, bw_one_pole_sticky_mode value);
/*! <<<```
 *    Sets the current distance metric for sticky behavior.
 *  }}} */

/* WARNING: the internal definition of this struct is not part of the public
 * API. Its content may change at any time in future versions. Please, do not
 * access its members directly. */
struct _bw_one_pole {
	// Coefficients
	float			Ttm2pi;

	float			mA1u;
	float			mA1d;
	float			st2;

	// Parameters
	float			init_val;
	float			cutoff_up;
	float			cutoff_down;
	float			sticky_thresh;
	bw_one_pole_sticky_mode sticky_mode;
	int			param_changed;

	// State
	char			first_run;
	float			x_z1;
	float			y_z1;
};

#ifdef __cplusplus
}
#endif

#endif
