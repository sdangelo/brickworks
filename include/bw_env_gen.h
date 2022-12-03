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
 *    * In the off phase (gate off and possible previous release phase ended),
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

#ifndef _BW_ENV_GEN_H
#define _BW_ENV_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_env_gen_coeffs
 *  ```>>> */
typedef struct _bw_env_gen_coeffs bw_env_gen_coeffs;
/*! <<<```
 *    Coefficients and input parameter values.
 *
 *    #### bw_env_gen_state
 *  ```>>> */
typedef struct _bw_env_gen_state bw_env_gen_state;
/*! <<<```
 *    Internal state and output parameter values.
 *
 *    #### bw_env_gen_phase
 *  ```>>> */
typedef enum {
	bw_env_gen_phase_off,
	bw_env_gen_phase_attack,
	bw_env_gen_phase_decay,
	bw_env_gen_phase_sustain,
	bw_env_gen_phase_release
} bw_env_gen_phase;
/*! <<<```
 *    Envelope generator phase:
 *     * `bw_env_gen_phase_off`: off phase;
 *     * `bw_env_gen_phase_attack`: attack phase;
 *     * `bw_env_gen_phase_decay`: decay phase;
 *     * `bw_env_gen_phase_sustain`: sustain phase;
 *     * `bw_env_gen_phase_release`: release phase.
 *
 *    #### bw_env_gen_init()
 *  ```>>> */
static inline void bw_env_gen_init(bw_env_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_env_gen_set_sample_rate()
 *  ```>>> */
static inline void bw_env_gen_set_sample_rate(bw_env_gen_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_env_gen_reset_coeffs()
 *  ```>>> */
static inline void bw_env_gen_reset_coeffs(bw_env_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_env_gen_reset_state()
 *  ```>>> */
static inline void bw_env_gen_reset_state(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_env_gen_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_env_gen_update_coeffs_ctrl(bw_env_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_env_gen_update_coeffs_audio()
 *  ```>>> */
static inline void bw_env_gen_update_coeffs_audio(bw_env_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_env_gen_update_state_ctrl()
 *  ```>>> */
static inline void bw_env_gen_update_state_ctrl(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state);
/*! <<<```
 *    Triggers control-rate update of the internal `state` using `coeffs`.
 *
 *    #### bw_env_gen_process1()
 *  ```>>> */
static inline float bw_env_gen_process1(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state);
/*! <<<```
 *    Generates and returns one sample using `coeffs` and updating `state`
 *    (audio rate only).
 *
 *    #### bw_env_gen_process()
 *  ```>>> */
static inline void bw_env_gen_process(bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state, float* y, int n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the output buffer `y`, while
 *    using and updating both `coeffs` and `state` (control and audio rate).
 *
 *    `y` may be `NULL`.
 *
 *    #### bw_env_gen_set_gate()
 *  ```>>> */
static inline void bw_env_gen_set_gate(bw_env_gen_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets the input gate to be either off (`0`) or on (non-`0`) in `coeffs`.
 *
 *    Default value: `0` (off).
 *
 *    #### bw_env_gen_set_attack()
 *  ```>>> */
static inline void bw_env_gen_set_attack(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the attack time to `value` (s) in `coeffs`.
 *
 *    `value` must be smaller than `1e9f`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_env_gen_set_decay()
 *  ```>>> */
static inline void bw_env_gen_set_decay(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the decay time to `value` (s) in `coeffs`.
 *
 *    `value` must be smaller than `1e9f`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_env_gen_set_sustain()
 *  ```>>> */
static inline void bw_env_gen_set_sustain(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the sustain level to `value` in `coeffs`.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_env_gen_set_release()
 *  ```>>> */
static inline void bw_env_gen_set_release(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the release time to `value` (s) in `coeffs`.
 *
 *    `value` must be smaller than `1e9f`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_env_gen_get_phase()
 *  ```>>> */
static inline bw_env_gen_phase bw_env_gen_get_phase(bw_env_gen_state *state);
/*! <<<```
 *    Returns the current envelope generator phase as stored in `state`.
 *
 *    #### bw_env_gen_get_y_z1()
 *  ```>>> */
static inline float bw_env_gen_get_y_z1(bw_env_gen_state *state);
/*! <<<```
 *    Returns the last output sample stored in `state`.
 *  }}} */

static inline float bw_env_gen_get_y_z1(bw_env_gen_state *state);

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_env_gen_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	
	// Coefficients
	float			T;

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
};

struct _bw_env_gen_state {
	bw_env_gen_phase	phase;
	float			y_z1;
	bw_one_pole_state	smooth_state;
};

#define _BW_ENV_GEN_PARAM_ATTACK	1
#define _BW_ENV_GEN_PARAM_DECAY		(1<<1)
#define _BW_ENV_GEN_PARAM_SUSTAIN	(1<<2)
#define _BW_ENV_GEN_PARAM_RELEASE	(1<<3)

static inline void bw_env_gen_init(bw_env_gen_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->gate = 0;
	coeffs->attack = 0.f;
	coeffs->decay = 0.f;
	coeffs->sustain = 1.f;
	coeffs->release = 0.f;
}

static inline void bw_env_gen_set_sample_rate(bw_env_gen_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->T = 1.f / sample_rate;
}

static inline void bw_env_gen_reset_coeffs(bw_env_gen_coeffs *BW_RESTRICT coeffs) {
	coeffs->param_changed = ~0;
	bw_env_gen_update_coeffs_ctrl(coeffs);
}

static inline void bw_env_gen_reset_state(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state) {
	state->phase = bw_env_gen_phase_off;
	state->y_z1 = 0.f;
}

static inline void bw_env_gen_update_coeffs_ctrl(bw_env_gen_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->param_changed) {
		// 1 ns considered instantaneous
		if (coeffs->param_changed & _BW_ENV_GEN_PARAM_ATTACK)
			coeffs->attack_inc = coeffs->attack > 1e-9f ? coeffs->T * bw_rcpf_2(coeffs->attack) : INFINITY;
		if (coeffs->param_changed & (_BW_ENV_GEN_PARAM_DECAY | _BW_ENV_GEN_PARAM_SUSTAIN))
			coeffs->decay_inc = coeffs->decay > 1e-9f ? (coeffs->sustain - 1.f) * coeffs->T * bw_rcpf_2(coeffs->decay) : -INFINITY;
		if (coeffs->param_changed & (_BW_ENV_GEN_PARAM_SUSTAIN | _BW_ENV_GEN_PARAM_RELEASE))
			coeffs->release_inc = coeffs->release > 1e-9f ? -coeffs->sustain * coeffs->T * bw_rcpf_2(coeffs->release) : -INFINITY;
	}
}

static inline void bw_env_gen_update_coeffs_audio(bw_env_gen_coeffs *BW_RESTRICT coeffs) {
}

static inline void bw_env_gen_update_state_ctrl(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state) {
	if (coeffs->gate) {
		if (state->phase == bw_env_gen_phase_off || state->phase == bw_env_gen_phase_release)
			state->phase = bw_env_gen_phase_attack;
	} else {
		if (state->phase != bw_env_gen_phase_off)
			state->phase = bw_env_gen_phase_release;
	}
}

static inline float bw_env_gen_process1(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state) {
	float v;
	switch (state->phase) {
	case bw_env_gen_phase_attack:
		v = state->y_z1 + coeffs->attack_inc;
		if (v >= 1.f) {
			v = 1.f;
			state->phase = bw_env_gen_phase_decay;
		}
		break;
	case bw_env_gen_phase_decay:
		v = state->y_z1 + coeffs->decay_inc;
		if (v <= coeffs->sustain) {
			v = coeffs->sustain;
			state->phase = bw_env_gen_phase_sustain;
			bw_one_pole_reset_state(&coeffs->smooth_coeffs, &state->smooth_state, coeffs->sustain);
		}
		break;
	case bw_env_gen_phase_sustain:
		v = bw_one_pole_process1(&coeffs->smooth_coeffs, &state->smooth_state, coeffs->sustain);
		break;
	case bw_env_gen_phase_release:
		v = state->y_z1 + coeffs->release_inc;
		if (v <= 0.f) {
			v = 0.f;
			state->phase = bw_env_gen_phase_off;
		}
		break;
	case bw_env_gen_phase_off:
		v = 0.f;
		break;
	}
	state->y_z1 = v;
	return v;
}

static inline void bw_env_gen_process(bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state, float* y, int n_samples) {
	bw_env_gen_update_coeffs_ctrl(coeffs);
	bw_env_gen_update_state_ctrl(coeffs, state);
	if (y != NULL)
		for (int i = 0; i < n_samples; i++)
			y[i] = bw_env_gen_process1(coeffs, state);
	else
		for (int i = 0; i < n_samples; i++)
			bw_env_gen_process1(coeffs, state);
}

static inline void bw_env_gen_set_gate(bw_env_gen_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->gate = value;
}

static inline void bw_env_gen_set_attack(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->attack != value) {
		coeffs->attack = value;
		coeffs->param_changed |= _BW_ENV_GEN_PARAM_ATTACK;
	}
}

static inline void bw_env_gen_set_decay(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->decay != value) {
		coeffs->decay = value;
		coeffs->param_changed |= _BW_ENV_GEN_PARAM_DECAY;
	}
}

static inline void bw_env_gen_set_sustain(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->sustain != value) {
		coeffs->sustain = value;
		coeffs->param_changed |= _BW_ENV_GEN_PARAM_SUSTAIN;
	}
}

static inline void bw_env_gen_set_release(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->release != value) {
		coeffs->release = value;
		coeffs->param_changed |= _BW_ENV_GEN_PARAM_RELEASE;
	}
}

static inline bw_env_gen_phase bw_env_gen_get_phase(bw_env_gen_state *state) {
	return state->phase;
}

static inline float bw_env_gen_get_y_z1(bw_env_gen_state *state) {
	return state->y_z1;
}

#undef _BW_ENV_GEN_PARAM_ATTACK
#undef _BW_ENV_GEN_PARAM_DECAY
#undef _BW_ENV_GEN_PARAM_SUSTAIN
#undef _BW_ENV_GEN_PARAM_RELEASE

#ifdef __cplusplus
}
#endif

#endif
