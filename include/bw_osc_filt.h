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
 *  requires {{{ bw_config bw_common }}}
 *  description {{{
 *    Post-filter to decolorate oscillator waveshapers when antialiasing is on.
 *
 *    This filter can be added in series of oscillator waveshapers that use
 *    PolyBLEP antialiasing (i.e., [bw_osc_saw](bw_osc_saw),
 *    [bw_osc_pulse](bw_osc_pulse), [bw_osc_tri](bw_osc_tri)) to compensate for
 *    high-frequency attenuation.
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

#ifndef _BW_OSC_FILT_H
#define _BW_OSC_FILT_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_osc_filt
 *  ```>>> */
typedef struct _bw_osc_filt bw_osc_filt;
/*! <<<```
 *    Instance object.
 *  >>> */

/*! ...
 *    #### bw_osc_filt_init()
 *  ```>>> */
void bw_osc_filt_init(bw_osc_filt *instance);
/*! <<<```
 *    Initializes the `instance` object.
 *  >>> */

/*! ...
 *    #### bw_osc_filt_set_sample_rate()
 *
 *    There is none (not needed).
 *  >>> */

/*! ...
 *    #### bw_osc_filt_reset()
 *  ```>>> */
void bw_osc_filt_reset(bw_osc_filt *instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_osc_filt_process()
 *  ```>>> */
void bw_osc_filt_process(bw_osc_filt *instance, const float *x, float* y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_osc_filt_set_enabled()
 *  ```>>> */
void bw_osc_filt_set_enabled(bw_osc_filt *instance, char value);
/*! <<<```
 *    Sets whether the filter is enabled (`value` non-`0`) or bypassed (`0`)
 *    for the given `instance`.
 *
 *    Default value: non-`0`.
 *  }}} */

/* WARNING: the internal definition of this struct is not part of the public
 * API. Its content may change at any time in future versions. Please, do not
 * access its members directly. */
struct _bw_osc_filt {
	// Parameters
	char	enabled;

	// State
	float	x_z1;
	float	y_z1;
};


#ifdef __cplusplus
}
#endif

#endif
