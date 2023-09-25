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
 *          <li>Renamed <code>bw_env_gen_update_state_ctrl()</code> as
 *              <code>bw_env_gen_process_ctrl()</code>.</li>
 *          <li>Added <code>bw_env_gen_reset_state_multi()</code> and updated
 *              C++ API in this regard.</li>
 *          <li>Now <code>bw_env_gen_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_env_gen_process()</code> and
 *              <code>bw_env_gen_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Faster and more robust implementation.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Added pragmas to silence bogus GCC uninitialized variable
 *              warnings.</li>
 *          <li>Clearly specified parameter validity ranges.</li>
 *          <li>Added debugging code.</li>
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
static inline void bw_env_gen_init(
	bw_env_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_env_gen_set_sample_rate()
 *  ```>>> */
static inline void bw_env_gen_set_sample_rate(
	bw_env_gen_coeffs * BW_RESTRICT coeffs,
	float                           sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_env_gen_reset_coeffs()
 *  ```>>> */
static inline void bw_env_gen_reset_coeffs(
	bw_env_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_env_gen_reset_state()
 *  ```>>> */
static inline float bw_env_gen_reset_state(
	const bw_env_gen_coeffs * BW_RESTRICT coeffs,
	bw_env_gen_state * BW_RESTRICT        state,
	char                                  gate_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial gate value (`0` for off, non-`0` for on) `gate_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_env_gen_reset_state_multi()
 *  ```>>> */
static inline void bw_env_gen_reset_state_multi(
	const bw_env_gen_coeffs * BW_RESTRICT              coeffs,
	bw_env_gen_state * BW_RESTRICT const * BW_RESTRICT state,
	const char * BW_RESTRICT                           gate_0,
	float * BW_RESTRICT                                y_0,
	size_t                                             n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial gate values (`0` for off,
 *    non-`0` for on) in the `gate_0` array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `NULL`.
 *
 *    #### bw_env_gen_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_env_gen_update_coeffs_ctrl(
	bw_env_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_env_gen_update_coeffs_audio()
 *  ```>>> */
static inline void bw_env_gen_update_coeffs_audio(
	bw_env_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_env_gen_process_ctrl()
 *  ```>>> */
static inline void bw_env_gen_process_ctrl(
	const bw_env_gen_coeffs * BW_RESTRICT coeffs,
	bw_env_gen_state * BW_RESTRICT        state,
	char                                  gate);
/*! <<<```
 *    Triggers control-rate update of the internal `state` using `coeffs` and
 *    the given `gate` value (`0` for off, non-`0` for on).
 *
 *    #### bw_env_gen_process1()
 *  ```>>> */
static inline float bw_env_gen_process1(
	const bw_env_gen_coeffs * BW_RESTRICT coeffs,
	bw_env_gen_state * BW_RESTRICT        state);
/*! <<<```
 *    Generates and returns one sample using `coeffs`, while using and updating
 *    `state` (audio rate only).
 *
 *    #### bw_env_gen_process()
 *  ```>>> */
static inline void bw_env_gen_process(
	bw_env_gen_coeffs * BW_RESTRICT coeffs,
	bw_env_gen_state * BW_RESTRICT  state,
	char                            gate,
	float * BW_RESTRICT             y,
	size_t                          n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the output buffer `y` using
 *    the given `gate` value (`0` for off, non-`0` for on), while using and
 *    updating both `coeffs` and `state` (control and audio rate).
 *
 *    `y` may be `NULL`.
 *
 *    #### bw_env_gen_process_multi()
 *  ```>>> */
static inline void bw_env_gen_process_multi(
	bw_env_gen_coeffs * BW_RESTRICT                    coeffs,
	bw_env_gen_state * BW_RESTRICT const * BW_RESTRICT state,
	const char * BW_RESTRICT                           gate,
	float * BW_RESTRICT const * BW_RESTRICT            y,
	size_t                                             n_channels,
	size_t                                             n_samples);
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
static inline void bw_env_gen_set_attack(
	bw_env_gen_coeffs * BW_RESTRICT coeffs,
	float                           value);
/*! <<<```
 *    Sets the attack time to `value` (s) in `coeffs`.
 *
 *    Valid range: [`0.f`, `60.f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_env_gen_set_decay()
 *  ```>>> */
static inline void bw_env_gen_set_decay(
	bw_env_gen_coeffs * BW_RESTRICT coeffs,
	float                           value);
/*! <<<```
 *    Sets the decay time to `value` (s) in `coeffs`.
 *
 *    Valid range: [`0.f`, `60.f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_env_gen_set_sustain()
 *  ```>>> */
static inline void bw_env_gen_set_sustain(
	bw_env_gen_coeffs * BW_RESTRICT coeffs,
	float                           value);
/*! <<<```
 *    Sets the sustain level to `value` in `coeffs`.
 *
 *    Valid range: [`0.f`, `1.f`].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_env_gen_set_release()
 *  ```>>> */
static inline void bw_env_gen_set_release(
	bw_env_gen_coeffs * BW_RESTRICT coeffs,
	float                           value);
/*! <<<```
 *    Sets the release time to `value` (s) in `coeffs`.
 *
 *    Valid range: [`0.f`, `60.f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_env_gen_get_phase()
 *  ```>>> */
static inline bw_env_gen_phase bw_env_gen_get_phase(
	const bw_env_gen_state * BW_RESTRICT state);
/*! <<<```
 *    Returns the current envelope generator phase as stored in `state`.
 *
 *    #### bw_env_gen_get_y_z1()
 *  ```>>> */
static inline float bw_env_gen_get_y_z1(
	const bw_env_gen_state * BW_RESTRICT state);
/*! <<<```
 *    Returns the last output sample as stored in `state`.
 *
 *    #### bw_env_gen_coeffs_is_valid()
 *  ```>>> */
static inline char bw_env_gen_coeffs_is_valid(
	const bw_env_gen_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_env_gen_coeffs`.
 *
 *    #### bw_env_gen_state_is_valid()
 *  ```>>> */
static inline char bw_env_gen_state_is_valid(
	const bw_env_gen_coeffs * BW_RESTRICT coeffs,
	const bw_env_gen_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_env_gen_state`.
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

#ifdef BW_DEBUG_DEEP
enum bw_env_gen_coeffs_state {
	bw_env_gen_coeffs_state_invalid,
	bw_env_gen_coeffs_state_init,
	bw_env_gen_coeffs_state_set_sample_rate,
	bw_env_gen_coeffs_state_reset_coeffs
};
#endif

struct bw_env_gen_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_env_gen_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_one_pole_coeffs		smooth_coeffs;
	
	// Coefficients
	float				k_T;

	uint32_t			attack_inc;
	uint32_t			decay_dec;
	uint32_t			sustain_v;
	uint32_t			release_dec;

	// Parameters
	float				attack;
	float				decay;
	float				sustain;
	float				release;
	int				param_changed;
};

struct bw_env_gen_state {
#ifdef BW_DEBUG_DEEP
	uint32_t		hash;
	uint32_t		coeffs_reset_id;
#endif

	bw_env_gen_phase	phase;
	uint32_t		v;
	bw_one_pole_state	smooth_state;
};

#define BW_ENV_GEN_PARAM_ATTACK		1
#define BW_ENV_GEN_PARAM_DECAY		(1<<1)
#define BW_ENV_GEN_PARAM_SUSTAIN	(1<<2)
#define BW_ENV_GEN_PARAM_RELEASE	(1<<3)

#define BW_ENV_V_MAX			4294967040

static inline void bw_env_gen_init(
		bw_env_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->attack = 0.f;
	coeffs->decay = 0.f;
	coeffs->sustain = 1.f;
	coeffs->release = 0.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_env_gen_coeffs");
	coeffs->state = bw_env_gen_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_env_gen_coeffs_state_init);
}

static inline void bw_env_gen_set_sample_rate(
		bw_env_gen_coeffs * BW_RESTRICT coeffs,
		float                           sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->k_T = (float)BW_ENV_V_MAX / sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_env_gen_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_env_gen_coeffs_state_set_sample_rate);
}

static inline void bw_env_gen_do_update_coeffs_ctrl(
		bw_env_gen_coeffs * BW_RESTRICT coeffs) {
	if (coeffs->param_changed) {
		// 1 ns considered instantaneous
		if (coeffs->param_changed & BW_ENV_GEN_PARAM_ATTACK)
			coeffs->attack_inc = coeffs->attack > 1e-9f ? coeffs->k_T * bw_rcpf(coeffs->attack) : UINT32_MAX;
		if (coeffs->param_changed & (BW_ENV_GEN_PARAM_DECAY | BW_ENV_GEN_PARAM_SUSTAIN))
			coeffs->decay_dec = coeffs->decay > 1e-9f ? (1.f - coeffs->sustain) * (coeffs->k_T * bw_rcpf(coeffs->decay)) : UINT32_MAX;
		if (coeffs->param_changed & BW_ENV_GEN_PARAM_SUSTAIN)
			coeffs->sustain_v = (uint32_t)((float)BW_ENV_V_MAX * coeffs->sustain);
		if (coeffs->param_changed & (BW_ENV_GEN_PARAM_SUSTAIN | BW_ENV_GEN_PARAM_RELEASE))
			coeffs->release_dec = coeffs->release > 1e-9f ? coeffs->sustain * (coeffs->k_T * bw_rcpf(coeffs->release)) : UINT32_MAX;
		coeffs->param_changed = 0;
	}
}

static inline void bw_env_gen_reset_coeffs(
		bw_env_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_set_sample_rate);

	coeffs->param_changed = ~0;
	bw_env_gen_do_update_coeffs_ctrl(coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_env_gen_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_env_gen_coeffs_state_reset_coeffs);
}

static inline float bw_env_gen_reset_state(
		const bw_env_gen_coeffs * BW_RESTRICT coeffs,
		bw_env_gen_state * BW_RESTRICT        state,
		char                                  gate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);

	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &state->smooth_state, coeffs->sustain);
	state->phase = bw_env_gen_phase_off;
	state->v = gate ? coeffs->sustain_v : 0;
	const float y = (1.f / (float)BW_ENV_V_MAX) * state->v;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_env_gen_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_env_gen_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_env_gen_reset_state_multi(
		const bw_env_gen_coeffs * BW_RESTRICT              coeffs,
		bw_env_gen_state * BW_RESTRICT const * BW_RESTRICT state,
		const char * BW_RESTRICT                           gate_0,
		float * BW_RESTRICT                                y_0,
		size_t                                             n_channels) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(gate_0 != NULL);

	if (y_0 != NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_env_gen_reset_state(coeffs, state[i], gate_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_env_gen_reset_state(coeffs, state[i], gate_0[i]);

	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_env_gen_update_coeffs_ctrl(
		bw_env_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);

	bw_env_gen_do_update_coeffs_ctrl(coeffs);

	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
}

static inline void bw_env_gen_update_coeffs_audio(
		bw_env_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_env_gen_process_ctrl(
		const bw_env_gen_coeffs * BW_RESTRICT coeffs,
		bw_env_gen_state * BW_RESTRICT        state,
		char                                  gate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_env_gen_state_is_valid(coeffs, state));

	(void)coeffs;
	if (gate) {
		if (state->phase == bw_env_gen_phase_off || state->phase == bw_env_gen_phase_release)
			state->phase = bw_env_gen_phase_attack;
	} else {
		if (state->phase != bw_env_gen_phase_off)
			state->phase = bw_env_gen_phase_release;
	}

	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_env_gen_state_is_valid(coeffs, state));
}

static inline float bw_env_gen_process1(
		const bw_env_gen_coeffs * BW_RESTRICT coeffs,
		bw_env_gen_state * BW_RESTRICT        state) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_env_gen_state_is_valid(coeffs, state));

	uint32_t v = 0;
	switch (state->phase) {
	case bw_env_gen_phase_attack:
		v = state->v + coeffs->attack_inc;
		if (v == BW_ENV_V_MAX || v <= state->v) {
			v = BW_ENV_V_MAX;
			state->phase = bw_env_gen_phase_decay;
		}
		break;
	case bw_env_gen_phase_decay:
		v = state->v - coeffs->decay_dec;
		if (v <= coeffs->sustain_v || v >= state->v) {
			v = coeffs->sustain_v;
			state->phase = bw_env_gen_phase_sustain;
			bw_one_pole_reset_state(&coeffs->smooth_coeffs, &state->smooth_state, coeffs->sustain);
		}
		break;
	case bw_env_gen_phase_sustain:
		v = (uint32_t)((float)BW_ENV_V_MAX * bw_one_pole_process1(&coeffs->smooth_coeffs, &state->smooth_state, coeffs->sustain));
		break;
	case bw_env_gen_phase_release:
		v = state->v - coeffs->release_dec;
		if (v == 0 || v >= state->v) {
			v = 0;
			state->phase = bw_env_gen_phase_off;
		}
		break;
	case bw_env_gen_phase_off:
		v = 0;
		break;
	}
	state->v = v;
	const float y = (1.f / (float)BW_ENV_V_MAX) * v;
	
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_env_gen_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_env_gen_process(
		bw_env_gen_coeffs * BW_RESTRICT coeffs,
		bw_env_gen_state * BW_RESTRICT  state,
		char                            gate,
		float * BW_RESTRICT             y,
		size_t                          n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_env_gen_state_is_valid(coeffs, state));

	bw_env_gen_update_coeffs_ctrl(coeffs);
	bw_env_gen_process_ctrl(coeffs, state, gate);
	if (y != NULL)
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_env_gen_process1(coeffs, state);
	else
		for (size_t i = 0; i < n_samples; i++)
			bw_env_gen_process1(coeffs, state);

	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_env_gen_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(y != NULL ? bw_has_only_finite(y, n_samples) : 1);
}

static inline void bw_env_gen_process_multi(
		bw_env_gen_coeffs * BW_RESTRICT                    coeffs,
		bw_env_gen_state * BW_RESTRICT const * BW_RESTRICT state,
		const char * BW_RESTRICT                           gate,
		float * BW_RESTRICT const * BW_RESTRICT            y,
		size_t                                             n_channels,
		size_t                                             n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
	BW_ASSERT(state != NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(gate != NULL);
#ifndef BW_NO_DEBUG
	if (y != NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = i + 1; j < n_channels; j++)
				BW_ASSERT(y[i] != y[j]);
#endif

	bw_env_gen_update_coeffs_ctrl(coeffs);
	for (size_t j = 0; j < n_channels; j++)
		bw_env_gen_process_ctrl(coeffs, state[j], gate[j]);
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

	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_reset_coeffs);
}

static inline void bw_env_gen_set_attack(
		bw_env_gen_coeffs * BW_RESTRICT coeffs,
		float                           value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 60.f);

	if (coeffs->attack != value) {
		coeffs->attack = value;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
		coeffs->param_changed |= BW_ENV_GEN_PARAM_ATTACK;
#pragma GCC diagnostic pop
	}

	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_init);
}

static inline void bw_env_gen_set_decay(
		bw_env_gen_coeffs * BW_RESTRICT coeffs,
		float                           value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 60.f);

	if (coeffs->decay != value) {
		coeffs->decay = value;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
		coeffs->param_changed |= BW_ENV_GEN_PARAM_DECAY;
#pragma GCC diagnostic pop
	}

	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_init);
}

static inline void bw_env_gen_set_sustain(
		bw_env_gen_coeffs * BW_RESTRICT coeffs,
		float                           value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 1.f);

	if (coeffs->sustain != value) {
		coeffs->sustain = value;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
		coeffs->param_changed |= BW_ENV_GEN_PARAM_SUSTAIN;
#pragma GCC diagnostic pop
	}

	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_init);
}

static inline void bw_env_gen_set_release(
		bw_env_gen_coeffs * BW_RESTRICT coeffs,
		float                           value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 60.f);

	if (coeffs->release != value) {
		coeffs->release = value;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
		coeffs->param_changed |= BW_ENV_GEN_PARAM_RELEASE;
#pragma GCC diagnostic pop
	}

	BW_ASSERT_DEEP(bw_env_gen_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_env_gen_coeffs_state_init);
}

static inline bw_env_gen_phase bw_env_gen_get_phase(
		const bw_env_gen_state * BW_RESTRICT state) {
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_env_gen_state_is_valid(NULL, state));

	return state->phase;
}

static inline float bw_env_gen_get_y_z1(
		const bw_env_gen_state * BW_RESTRICT state) {
	BW_ASSERT(state != NULL);
	BW_ASSERT_DEEP(bw_env_gen_state_is_valid(NULL, state));

	const float y = (1.f / (float)BW_ENV_V_MAX) * state->v;

	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline char bw_env_gen_coeffs_is_valid(
		const bw_env_gen_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_env_gen_coeffs"))
		return 0;
	if (coeffs->state < bw_env_gen_coeffs_state_init || coeffs->state > bw_env_gen_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->attack) || coeffs->attack < 0.f || coeffs->attack > 60.f)
		return 0;
	if (!bw_is_finite(coeffs->decay) || coeffs->decay < 0.f || coeffs->decay > 60.f)
		return 0;
	if (!bw_is_finite(coeffs->sustain) || coeffs->sustain < 0.f || coeffs->sustain > 1.f)
		return 0;
	if (!bw_is_finite(coeffs->release) || coeffs->release < 0.f || coeffs->release > 60.f)
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_env_gen_coeffs_state_set_sample_rate && coeffs->k_T <= 0.f)
		return 0.f;
#endif

	return bw_one_pole_coeffs_is_valid(&coeffs->smooth_coeffs);
}

static inline char bw_env_gen_state_is_valid(
		const bw_env_gen_coeffs * BW_RESTRICT coeffs,
		const bw_env_gen_state * BW_RESTRICT  state) {
	BW_ASSERT(state != NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_env_gen_state"))
		return 0;

	if (coeffs != NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	return state->phase >= bw_env_gen_phase_off && state->phase <= bw_env_gen_phase_release
		&& bw_one_pole_state_is_valid(coeffs ? &coeffs->smooth_coeffs : NULL, &state->smooth_state);
}

#undef BW_ENV_GEN_PARAM_ATTACK
#undef BW_ENV_GEN_PARAM_DECAY
#undef BW_ENV_GEN_PARAM_SUSTAIN
#undef BW_ENV_GEN_PARAM_RELEASE

#undef BW_ENV_V_MAX

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

	void setSampleRate(
		float sampleRate);

	void reset(
		char                gate0 = 0,
		float * BW_RESTRICT y0 = nullptr);

	void reset(
		char                                        gate0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0);

	void reset(
		const char * BW_RESTRICT gate0,
		float * BW_RESTRICT      y0 = nullptr);

	void reset(
		std::array<char, N_CHANNELS>                gate0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0 = nullptr);

	void process(
		const char * BW_RESTRICT                gate,
		float * BW_RESTRICT const * BW_RESTRICT y,
		size_t                                  nSamples);

	void process(
		std::array<char, N_CHANNELS>                gate,
		std::array<float * BW_RESTRICT, N_CHANNELS> y,
		size_t                                      nSamples);

	void setAttack(
		float value);

	void setDecay(
		float value);

	void setSustain(
		float value);

	void setRelease(
		float value);
	
	bw_env_gen_phase getPhase(
		size_t channel);

	float getYZ1(
		size_t channel);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_env_gen_coeffs		coeffs;
	bw_env_gen_state		states[N_CHANNELS];
	bw_env_gen_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline EnvGen<N_CHANNELS>::EnvGen() {
	bw_env_gen_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_env_gen_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::reset(
		char                gate0,
		float * BW_RESTRICT y0) {
	bw_env_gen_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_env_gen_reset_state(&coeffs, states + i, gate0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_env_gen_reset_state(&coeffs, states + i, gate0);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::reset(
		char                                        gate0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(gate0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::reset(
		const char * BW_RESTRICT gate0,
		float * BW_RESTRICT      y0) {
	bw_env_gen_reset_coeffs(&coeffs);
	bw_env_gen_reset_state_multi(&coeffs, statesP, gate0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::reset(
		std::array<char, N_CHANNELS>                gate0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(gate0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::process(
		const char * BW_RESTRICT                gate,
		float * BW_RESTRICT const * BW_RESTRICT y,
		size_t                                  nSamples) {
	bw_env_gen_process_multi(&coeffs, statesP, gate, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::process(
		std::array<char, N_CHANNELS>                gate,
		std::array<float * BW_RESTRICT, N_CHANNELS> y,
		size_t                                      nSamples) {
	process(gate.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setAttack(
		float value) {
	bw_env_gen_set_attack(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setDecay(
		float value) {
	bw_env_gen_set_decay(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setSustain(
		float value) {
	bw_env_gen_set_sustain(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void EnvGen<N_CHANNELS>::setRelease(
		float value) {
	bw_env_gen_set_release(&coeffs, value);
}

template<size_t N_CHANNELS>
inline bw_env_gen_phase EnvGen<N_CHANNELS>::getPhase(
		size_t channel) {
	return bw_env_gen_get_phase(states + channel);
}

template<size_t N_CHANNELS>
inline float EnvGen<N_CHANNELS>::getYZ1(
		size_t channel) {
	return bw_env_gen_get_y_z1(states + channel);
}

}
#endif

#endif
