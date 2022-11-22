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
 *    Linear ADSR envelope generator.
 *
 *    * In the off state (gate off and possible previous release phase ended),
 *      the output is `0.f`;
 *    * in the attack phase the output increases from `0.f` to `1.f`;
 *    * in the decay phase the output decreases from `1.f` to the given sustain
 *      level;
 *    * in the sustain phase the output stays constant;
 *    * in the release phase the output decreases from the current level to
 *      `0.f`.
 *
 *    Attack, decay, and release time parameters are not actually controlling
 *    times but rather slopes. If all phases of the ADSR fully execute, then the
 *    actual times will match the parameter settings.
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

#ifndef _BW_ENV_GEN_H
#define _BW_ENV_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_env_gen
 *  ```>>> */
typedef struct _bw_env_gen bw_env_gen;
/*! <<<```
 *    Instance object.
 *  >>> */

/*! ...
 *    #### bw_env_gen_init()
 *  ```>>> */
void bw_env_gen_init(bw_env_gen *instance);
/*! <<<```
 *    Initializes the `instance` object.
 *  >>> */

/*! ...
 *    #### bw_env_gen_set_sample_rate()
 *  ```>>> */
void bw_env_gen_set_sample_rate(bw_env_gen *instance, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value for the given `instance`.
 *  >>> */

/*! ...
 *    #### bw_env_gen_reset()
 *  ```>>> */
void bw_env_gen_reset(bw_env_gen *instance);
/*! <<<```
 *    Resets the given `instance` to its initial state.
 *  >>> */

/*! ...
 *    #### bw_env_gen_process()
 *  ```>>> */
void bw_env_gen_process(bw_env_gen *instance, float* y, int n_samples);
/*! <<<```
 *    Lets the given `instance` generate `n_samples` samples and puts them in
 *    the output buffer `y`.
 *  >>> */

/*! ...
 *    #### bw_env_gen_set_gate()
 *  ```>>> */
void bw_env_gen_set_gate(bw_env_gen *instance, char value);
/*! <<<```
 *    Sets the input gate to be either off (`0`) or on (non-`0`) for the given
 *    `instance`.
 *
 *    Default value: `0` (off).
 *  >>> */

/*! ...
 *    #### bw_env_gen_set_attack()
 *  ```>>> */
void bw_env_gen_set_attack(bw_env_gen *instance, float value);
/*! <<<```
 *    Sets the attack time to `value` (s) for the given `instance`.
 *
 *    `value` must be smaller than `1e9f`.
 *
 *    Default value: `0.f`.
 *  >>> */

/*! ...
 *    #### bw_env_gen_set_decay()
 *  ```>>> */
void bw_env_gen_set_decay(bw_env_gen *instance, float value);
/*! <<<```
 *    Sets the decay time to `value` (s) for the given `instance`.
 *
 *    `value` must be smaller than `1e9f`.
 *
 *    Default value: `0.f`.
 *  >>> */

/*! ...
 *    #### bw_env_gen_set_sustain()
 *  ```>>> */
void bw_env_gen_set_sustain(bw_env_gen *instance, float value);
/*! <<<```
 *    Sets the sustain level to `value` for the given `instance`.
 *
 *    Default value: `1.f`.
 *  >>> */

/*! ...
 *    #### bw_env_gen_set_release()
 *  ```>>> */
void bw_env_gen_set_release(bw_env_gen *instance, float value);
/*! <<<```
 *    Sets the release time to `value` (s) for the given `instance`.
 *
 *    `value` must be smaller than `1e9f`.
 *
 *    Default value: `0.f`.
 *  >>> */

/*! ...
 *    #### bw_env_gen_get_is_off()
 *  ```>>> */
char bw_env_gen_get_is_off(bw_env_gen *instance);
/*! <<<```
 *    Returns `0` if the given `instance` is in the off state, non-`0` otherwise.
 *  }}} */

/* WARNING: this enum is not part of the public API. Please, do not use it. */
typedef enum {
	_bw_env_gen_state_off,
	_bw_env_gen_state_attack,
	_bw_env_gen_state_decay,
	_bw_env_gen_state_sustain,
	_bw_env_gen_state_release
} _bw_env_gen_state;

/* WARNING: the internal definition of this struct is not part of the public
 * API. Its content may change at any time in future versions. Please, do not
 * access its members directly. */
struct _bw_env_gen {
	// Coefficients
	float			T;
	float			smooth_mA1;

	float			attack_inc;
	float			decay_inc;
	float			release_inc;

	// Parameters
	char			gate;
	float			attack;
	float			decay;
	float			sustain;
	float			release;
	int			param_changed;

	// State
	char			first_run;
	_bw_env_gen_state	state;
	float			y_z1;
};

#ifdef __cplusplus
}
#endif

#endif
