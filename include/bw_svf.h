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
 *  requires {{{ bw_config bw_common bw_one_pole bw_math }}}
 *  description {{{
 *    State variable filter (2nd order, 12 dB/oct) model with separated lowpass,
 *    bandpass, and highpass outputs.
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

#ifndef _BW_SVF_H
#define _BW_SVF_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_svf
 *  ```>>> */
typedef struct _bw_svf bw_svf;
/*! <<<```
 *    Instance object.
 *  >>> */

/*! ...
 *    #### bw_svf_init()
 *  ```>>> */
void bw_svf_init(bw_svf *instance);
/*! <<<```
 *    Initializes the `instance` object.
 *  >>> */

/*! ...
 *    #### bw_svf_set_sample_rate()
 *  ```>>> */
void bw_svf_set_sample_rate(bw_svf *instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_svf_reset()
 *  ```>>> */
void bw_svf_reset(bw_svf *instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_svf_process()
 *  ```>>> */
void bw_svf_process(bw_svf *instance, const float *x, float* y_lp, float *y_bp, float *y_hp, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffers `y_lp` (lowpass), `y_bp` (bandpass), and `y_hp` (highpass), if
 *    they are not `NULL`.
 *  >>> */

/*! ...
 *    #### bw_svf_set_cutoff()
 *  ```>>> */
void bw_svf_set_cutoff(bw_svf *instance, float value);
/*! <<<```
 *    Sets the cutoff frequency to the given `value` (Hz) for the given
 *    `instance`.
 *
 *    Default value: `1e3f`.
 *  >>> */

/*! ...
 *    #### bw_svf_set_Q()
 *  ```>>> */
void bw_svf_set_Q(bw_svf *instance, float value);
/*! <<<```
 *    Sets the quality factor to the given `value` (Hz) for the given
 *    `instance`.
 *
 *    Default value: `0.5f`.
 *  }}} */

#include <bw_one_pole.h>

/* WARNING: the internal definition of this struct is not part of the public
 * API. Its content may change at any time in future versions. Please, do not
 * access its members directly. */
struct _bw_svf {
	// Sub-components
	bw_one_pole_coeffs	smooth_cutoff_coeffs;
	bw_one_pole_state	smooth_cutoff_state;
	bw_one_pole_coeffs	smooth_Q_coeffs;
	bw_one_pole_state	smooth_Q_state;
	
	// Coefficients
	float	t_k;

	float	t;
	float	k;
	float	hp_hp_z1;
	float	hp_bp_z1;
	float	hp_x;

	// Parameters
	float	cutoff;
	float	Q;

	float	cutoff_cur;
	float	Q_cur;

	// State
	char	first_run;
	float	hp_z1;
	float	lp_z1;
	float	bp_z1;
};

#ifdef __cplusplus
}
#endif

#endif
