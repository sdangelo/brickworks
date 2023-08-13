/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
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
 *  version {{{ 1.0.0 }}}
 *  requires {{{ bw_common bw_math bw_one_pole }}}
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
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_env_gen_process()</code> and
 *              <code>bw_env_gen_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overladed C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_env_follow_process_multi()</code>.</li>
 *          <li>Added <code>gate</code> argument to
 *              <code>bw_env_gen_update_state_ctrl()</code> and
 *              <code>bw_env_gen_process()</code>, and removed gate
 *              parameter.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Fixed unused parameter warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Avoid a warning related to a potentially uninitialized
 *          variable.</li>
 *        </ul>
 *      </li>
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

#ifndef BW_ENV_GEN_H
#define BW_ENV_GEN_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_env_gen_coeffs
 *  ```>>> */
typedef struct bw_env_gen_coeffs bw_env_gen_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_env_gen_state
 *  ```>>> */
typedef struct bw_env_gen_state bw_env_gen_state;
/*! <<<```
 *    Internal state and related.
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
static inline void bw_env_gen_update_state_ctrl(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state, char gate);
/*! <<<```
 *    Triggers control-rate update of the internal `state` using `coeffs` and
 *    the given `gate` value (`0` for off, non-`0` for on).
 *
 *    #### bw_env_gen_process1()
 *  ```>>> */
static inline float bw_env_gen_process1(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state);
/*! <<<```
 *    Generates and returns one sample using `coeffs`, while using and updating
 *    `state` (audio rate only).
 *
 *    #### bw_env_gen_process()
 *  ```>>> */
static inline void bw_env_gen_process(bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state, char gate, float *y, size_t n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the output buffer `y` using
 *    the given `gate` value (`0` for off, non-`0` for on), while using and
 *    updating both `coeffs` and `state` (control and audio rate).
 *
 *    `y` may be `NULL`.
 *
 *    #### bw_env_gen_process_multi()
 *  ```>>> */
static inline void bw_env_gen_process_multi(bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state * const *BW_RESTRICT state, const char *gate, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the `n_channels` output
 *    buffers `y` using the given `n_channels` `gate` values (`0` for off,
 *    non-`0` for on), while using and updating both the common `coeffs` and
 *    each of the `n_channels` `state`s (control and audio rate).
 *
 *    `y` or any element of `y` may be `NULL`.
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
static inline bw_env_gen_phase bw_env_gen_get_phase(const bw_env_gen_state *state);
/*! <<<```
 *    Returns the current envelope generator phase as stored in `state`.
 *
 *    #### bw_env_gen_get_y_z1()
 *  ```>>> */
static inline float bw_env_gen_get_y_z1(const bw_env_gen_state *state);
/*! <<<```
 *    Returns the last output sample as stored in `state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bw_env_gen_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	
	// Coefficients
	float			T;

	float			attack_inc;
	float			decay_inc;
	float			release_inc;

	// Parameters
	float			attack;
	float			decay;
	float			sustain;
	float			release;
	int			param_changed;
};

struct bw_env_gen_state {
	bw_env_gen_phase	phase;
	float			y_z1;
	bw_one_pole_state	smooth_state;
};

#define BW_ENV_GEN_PARAM_ATTACK		1
#define BW_ENV_GEN_PARAM_DECAY		(1<<1)
#define BW_ENV_GEN_PARAM_SUSTAIN	(1<<2)
#define BW_ENV_GEN_PARAM_RELEASE	(1<<3)

static inline void bw_env_gen_init(bw_env_gen_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
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
	(void)coeffs;
	state->phase = bw_env_gen_phase_off;
	state->y_z1 = 0.f;
}

static inline void bw_env_gen_update_coeffs_ctrl(bw_env_gen_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->param_changed) {
		// 1 ns considered instantaneous
		if (coeffs->param_changed & BW_ENV_GEN_PARAM_ATTACK)
			coeffs->attack_inc = coeffs->attack > 1e-9f ? coeffs->T * bw_rcpf(coeffs->attack) : INFINITY;
		if (coeffs->param_changed & (BW_ENV_GEN_PARAM_DECAY | BW_ENV_GEN_PARAM_SUSTAIN))
			coeffs->decay_inc = coeffs->decay > 1e-9f ? (coeffs->sustain - 1.f) * coeffs->T * bw_rcpf(coeffs->decay) : -INFINITY;
		if (coeffs->param_changed & (BW_ENV_GEN_PARAM_SUSTAIN | BW_ENV_GEN_PARAM_RELEASE))
			coeffs->release_inc = coeffs->release > 1e-9f ? -coeffs->sustain * coeffs->T * bw_rcpf(coeffs->release) : -INFINITY;
	}
}

static inline void bw_env_gen_update_coeffs_audio(bw_env_gen_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline void bw_env_gen_update_state_ctrl(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state, char gate) {
	(void)coeffs;
	if (gate) {
		if (state->phase == bw_env_gen_phase_off || state->phase == bw_env_gen_phase_release)
			state->phase = bw_env_gen_phase_attack;
	} else {
		if (state->phase != bw_env_gen_phase_off)
			state->phase = bw_env_gen_phase_release;
	}
}

static inline float bw_env_gen_process1(const bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state) {
	float v = 0.f;
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

static inline void bw_env_gen_process(bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state *BW_RESTRICT state, char gate, float *y, size_t n_samples) {
	bw_env_gen_update_coeffs_ctrl(coeffs);
	bw_env_gen_update_state_ctrl(coeffs, state, gate);
	if (y != NULL)
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_env_gen_process1(coeffs, state);
	else
		for (size_t i = 0; i < n_samples; i++)
			bw_env_gen_process1(coeffs, state);
}

static inline void bw_env_gen_process_multi(bw_env_gen_coeffs *BW_RESTRICT coeffs, bw_env_gen_state * const *BW_RESTRICT state, const char *gate, float * const *y, size_t n_channels, size_t n_samples) {
	bw_env_gen_update_coeffs_ctrl(coeffs);
	for (size_t j = 0; j < n_channels; j++)
		bw_env_gen_update_state_ctrl(coeffs, state[j], gate[j]);
	if (y != NULL)
		for (size_t i = 0; i < n_samples; i++)
			for (size_t j = 0; j < n_channels; j++) {
				const float v = bw_env_gen_process1(coeffs, state[j]);
				if (y[j] != NULL)
					y[j][i] = v;
			}
	else
		for (size_t i = 0; i < n_samples; i++)
			for (size_t j = 0; j < n_channels; j++)
				bw_env_gen_process1(coeffs, state[j]);
}

static inline void bw_env_gen_set_attack(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->attack != value) {
		coeffs->attack = value;
		coeffs->param_changed |= BW_ENV_GEN_PARAM_ATTACK;
	}
}

static inline void bw_env_gen_set_decay(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->decay != value) {
		coeffs->decay = value;
		coeffs->param_changed |= BW_ENV_GEN_PARAM_DECAY;
	}
}

static inline void bw_env_gen_set_sustain(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->sustain != value) {
		coeffs->sustain = value;
		coeffs->param_changed |= BW_ENV_GEN_PARAM_SUSTAIN;
	}
}

static inline void bw_env_gen_set_release(bw_env_gen_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->release != value) {
		coeffs->release = value;
		coeffs->param_changed |= BW_ENV_GEN_PARAM_RELEASE;
	}
}

static inline bw_env_gen_phase bw_env_gen_get_phase(const bw_env_gen_state *state) {
	return state->phase;
}

static inline float bw_env_gen_get_y_z1(const bw_env_gen_state *state) {
	return state->y_z1;
}

#undef BW_ENV_GEN_PARAM_ATTACK
#undef BW_ENV_GEN_PARAM_DECAY
#undef BW_ENV_GEN_PARAM_SUSTAIN
#undef BW_ENV_GEN_PARAM_RELEASE

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::EnvGen
 *  ```>>> */
template<size_t N_CHANNELS>
class EnvGen {
public:
	EnvGen();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		const char *gate,
		float * const *y,
		size_t nSamples);
	void process(
		std::array<char, N_CHANNELS> gate,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples);

	void setAttack(float value);
	void setDecay(float value);
	void setSustain(float value);
	void setRelease(float value);
	
	bw_env_gen_phase getPhase(size_t channel);
	float getYZ1(size_t channel);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_env_gen_coeffs	 coeffs;
	bw_env_gen_state	 states[N_CHANNELS];
	bw_env_gen_state	*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline EnvGen<N_CHANNELS>::EnvGen() {
	bw_env_gen_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_env_gen_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::reset() {
	bw_env_gen_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_env_gen_reset_state(&coeffs, states + i);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::process(
		const char *gate,
		float * const *y,
		size_t nSamples) {
	bw_env_gen_process_multi(&coeffs, statesP, gate, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::process(
		std::array<char, N_CHANNELS> gate,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(gate.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setAttack(float value) {
	bw_env_gen_set_attack(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setDecay(float value) {
	bw_env_gen_set_decay(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setSustain(float value) {
	bw_env_gen_set_sustain(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setRelease(float value) {
	bw_env_gen_set_release(&coeffs, value);
}

template<size_t N_CHANNELS>
inline bw_env_gen_phase EnvGen<N_CHANNELS>::getPhase(size_t channel) {
	return bw_env_gen_get_phase(states + channel);
}

template<size_t N_CHANNELS>
inline float EnvGen<N_CHANNELS>::getYZ1(size_t channel) {
	return bw_env_gen_get_y_z1(states + channel);
}

}
#endif

#endif
