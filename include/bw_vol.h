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
 *  version {{{ 0.1.0 }}}
 *  requires {{{ bw_config bw_common bw_inline_slew_lim bw_math }}}
 *  description {{{
 *    Volume control for an arbitrary number of channels.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.1.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_VOL_H
#define _BW_VOL_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_vol
 *  ```>>> */
typedef struct _bw_vol *bw_vol;
/*! <<<```
 *    Instance handle.
 *  >>> */

/*! ...
 *    #### bw_vol_new()
 *  ```>>> */
bw_vol bw_vol_new();
/*! <<<```
 *    Creates a new instance.
 *
 *    Returns the newly-created instance handle or `NULL` if there was not
 *    enough memory.
 *  >>> */

/*! ...
 *    #### bw_vol_free()
 *  ```>>> */
void bw_vol_free(bw_vol instance);
/*! <<<```
 *    Destroys an `instance`.
 *  >>> */

/*! ...
 *    #### bw_vol_set_sample_rate()
 *  ```>>> */
void bw_vol_set_sample_rate(bw_vol instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_vol_reset()
 *  ```>>> */
void bw_vol_reset(bw_vol instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_vol_process()
 *  ```>>> */
void bw_vol_process(bw_vol instance, const float **x, float **y, int n_channels, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from each of the
 *    `n_channels` input buffers and fills the corresponding `n_samples` samples
 *    in each of the `n_channels` output buffers.
 *
 *    `x` is an array of `n_channels` input buffers and similarly `y` is an
 *    array of `n_channels` output buffers.
 *  >>> */

/*! ...
 *    #### bw_vol_set_volume()
 *  ```>>> */
void bw_vol_set_volume(bw_vol instance, float value);
/*! <<<```
 *    Sets the volume parameter to the given `value` (range [`0.f`, `1.f`]) for
 *    the given `instance`.
 *
 *    This parameter is not linearly mapped, but the range extremes correspond
 *    to silence (gain = `0.f`) and bypass (gain = `1.f`).
 *
 *    Default value: `1.f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

#endif
