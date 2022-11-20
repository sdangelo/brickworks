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
 *  requires {{{ bw_config bw_common bw_inline_one_pole bw_math bw_wah }}}
 *  description {{{
 *    Wah effect.
 *
 *    This is a simple digital effect, not an emulation of any analog circuit.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_WAH_H
#define _BW_WAH_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_wah
 *  ```>>> */
typedef struct _bw_wah bw_wah;
/*! <<<```
 *    Instance object.
 *  >>> */

/*! ...
 *    #### bw_wah_init()
 *  ```>>> */
void bw_wah_init(bw_wah *instance);
/*! <<<```
 *    Initializes the `instance` object.
 *  >>> */

/*! ...
 *    #### bw_wah_set_sample_rate()
 *  ```>>> */
void bw_wah_set_sample_rate(bw_wah *instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_wah_reset()
 *  ```>>> */
void bw_wah_reset(bw_wah *instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_wah_process()
 *  ```>>> */
void bw_wah_process(bw_wah *instance, const float *x, float *y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` and fills the corresponding `n_samples` samples in the output
 *    buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_wah_set_wah()
 *  ```>>> */
void bw_wah_set_wah(bw_wah *instance, float value);
/*! <<<```
 *    Sets the wah pedal position to the given `value` in [`0.f` (low cutoff),
 *    `1.f` (high cutoff)].
 *
 *    Default value: `0.5f`.
 *  }}} */

#include <bw_svf.h>

/* WARNING: the internal definition of this struct is not part of the public
 * API. Its content may change at any time in future versions. Please, do not
 * access its members directly. */
struct _bw_wah {
	// Sub-components
	bw_svf	svf;
};

#ifdef __cplusplus
}
#endif

#endif
