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
 *  requires {{{ bw_config bw_common bw_inline_one_pole bw_math }}}
 *  description {{{
 *    Pulse oscillator waveshaper with variable pulse width (actually, duty
 *    cycle) and PolyBLEP antialiasing.
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

#ifndef _BW_OSC_PULSE_H
#define _BW_OSC_PULSE_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_osc_pulse
 *  ```>>> */
typedef struct _bw_osc_pulse *bw_osc_pulse;
/*! <<<```
 *    Instance handle.
 *  >>> */

/*! ...
 *    #### bw_osc_pulse_new()
 *  ```>>> */
bw_osc_pulse bw_osc_pulse_new();
/*! <<<```
 *    Creates a new instance.
 *
 *    Returns the newly-created instance handle or `NULL` if there was not
 *    enough memory.
 *  >>> */

/*! ...
 *    #### bw_osc_pulse_free()
 *  ```>>> */
void bw_osc_pulse_free(bw_osc_pulse instance);
/*! <<<```
 *    Destroys an `instance`.
 *  >>> */

/*! ...
 *    #### bw_osc_pulse_set_sample_rate()
 *  ```>>> */
void bw_osc_pulse_set_sample_rate(bw_osc_pulse instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_osc_pulse_reset()
 *  ```>>> */
void bw_osc_pulse_reset(bw_osc_pulse instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_osc_pulse_process()
 *  ```>>> */
void bw_osc_pulse_process(bw_osc_pulse instance, const float *x, const float *x_phase_inc, float* y, int n_samples);
/*! <<<```
 *    Lets the given `instance` process `n_samples` samples from the input
 *    buffer `x` containing the normalized phase signal and fills the
 *    corresponding `n_samples` samples in the output buffer `y`.
 *
 *    If antialiasing is on, `x_phase_inc` must contain phase increment values,
 *    otherwise it is ignored and can be `NULL`.
 *  >>> */

/*! ...
 *    #### bw_osc_pulse_set_antialiasing()
 *  ```>>> */
void bw_osc_pulse_set_antialiasing(bw_osc_pulse instance, char value);
/*! <<<```
 *    Sets whether the antialiasing is on (`value` non-`0`) or off (`0`) for the
 *    given `instance`.
 *
 *    Default value: `0`.
 *  >>> */

/*! ...
 *    #### bw_osc_pulse_set_pulse_width()
 *  ```>>> */
void bw_osc_pulse_set_pulse_width(bw_osc_pulse instance, float value);
/*! <<<```
 *    Sets the pulse width (actually, the duty cycle) to `value` (range
 *    [`0.f`, `1.f`]) for the given `instance`.
 *
 *    Default value: `0.5f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

#endif
