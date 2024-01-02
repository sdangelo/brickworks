/*
 * Brickworks
 *
 * Copyright (C) 2023, 2024 Orastron Srl unipersonale
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
 *  requires {{{ bw_common bw_gain bw_math bw_one_pole bw_svf }}}
 *  description {{{
 *    Cab simulator effect.
 *
 *    This is a simple digital effect made of a bunch of filters, not an
 *    emulation of any analog circuit nor a simulation of an actual cabinet.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_CAB_H
#define BW_CAB_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_cab_coeffs
 *  ```>>> */
typedef struct bw_cab_coeffs bw_cab_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_cab_state
 *  ```>>> */
typedef struct bw_cab_state bw_cab_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_cab_init()
 *  ```>>> */
static inline void bw_cab_init(
	bw_cab_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_cab_set_sample_rate()
 *  ```>>> */
static inline void bw_cab_set_sample_rate(
	bw_cab_coeffs * BW_RESTRICT coeffs,
	float                       sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_cab_reset_coeffs()
 *  ```>>> */
static inline void bw_cab_reset_coeffs(
	bw_cab_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_cab_reset_state()
 *  ```>>> */
static inline float bw_cab_reset_state(
	const bw_cab_coeffs * BW_RESTRICT coeffs,
	bw_cab_state * BW_RESTRICT        state,
	float                             x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_cab_reset_state_multi()
 *  ```>>> */
static inline void bw_cab_reset_state_multi(
	const bw_cab_coeffs * BW_RESTRICT              coeffs,
	bw_cab_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                  x_0,
	float *                                        y_0,
	size_t                                         n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `BW_NULL`.
 *
 *    #### bw_cab_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_cab_update_coeffs_ctrl(
	bw_cab_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_cab_update_coeffs_audio()
 *  ```>>> */
static inline void bw_cab_update_coeffs_audio(
	bw_cab_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_cab_process1()
 *  ```>>> */
static inline float bw_cab_process1(
	const bw_cab_coeffs * BW_RESTRICT coeffs,
	bw_cab_state * BW_RESTRICT        state,
	float                             x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_cab_process()
 *  ```>>> */
static inline void bw_cab_process(
	bw_cab_coeffs * BW_RESTRICT coeffs,
	bw_cab_state * BW_RESTRICT  state,
	const float *               x,
	float *                     y,
	size_t                      n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_cab_process_multi()
 *  ```>>> */
static inline void bw_cab_process_multi(
	bw_cab_coeffs * BW_RESTRICT                    coeffs,
	bw_cab_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                          x,
	float * const *                                y,
	size_t                                         n_channels,
	size_t                                         n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_cab_set_cutoff_low()
 *  ```>>> */
static inline void bw_cab_set_cutoff_low(
	bw_cab_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the low cutoff frequency to the given (relative) `value` in `coeffs`.
 *
 *    Valid range: [`0.f`, `1.f`].
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_cab_set_cutoff_high()
 *  ```>>> */
static inline void bw_cab_set_cutoff_high(
	bw_cab_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the high cutoff frequency to the given (relative) `value` in
 *    `coeffs`.
 *
 *    Valid range: [`0.f`, `1.f`].
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_cab_set_tone()
 *  ```>>> */
static inline void bw_cab_set_tone(
	bw_cab_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the tone to the given `value` in `coeffs`.
 *
 *    Valid range: [`0.f` (dark), `1.f` (bright)].
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_cab_coeffs_is_valid()
 *  ```>>> */
static inline char bw_cab_coeffs_is_valid(
	const bw_cab_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_cab_coeffs`.
 *
 *    #### bw_cab_state_is_valid()
 *  ```>>> */
static inline char bw_cab_state_is_valid(
	const bw_cab_coeffs * BW_RESTRICT coeffs,
	const bw_cab_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `BW_NULL` extra cross-checks might be performed (`state`
 *    is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_cab_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_svf.h>
#include <bw_gain.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_cab_coeffs_state {
	bw_cab_coeffs_state_invalid,
	bw_cab_coeffs_state_init,
	bw_cab_coeffs_state_set_sample_rate,
	bw_cab_coeffs_state_reset_coeffs
};
#endif

struct bw_cab_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_cab_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_svf_coeffs			lp_coeffs;
	bw_svf_coeffs			hp_coeffs;
	bw_svf_coeffs			bpl_coeffs;
	bw_svf_coeffs			bph_coeffs;
	bw_gain_coeffs			gain_bpl_coeffs;
	bw_gain_coeffs			gain_bph_coeffs;
};

struct bw_cab_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// Sub-components
	bw_svf_state	lp_state;
	bw_svf_state	hp_state;
	bw_svf_state	bpl_state;
	bw_svf_state	bph_state;
};

static inline void bw_cab_init(
		bw_cab_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	bw_svf_init(&coeffs->lp_coeffs);
	bw_svf_init(&coeffs->hp_coeffs);
	bw_svf_init(&coeffs->bpl_coeffs);
	bw_svf_init(&coeffs->bph_coeffs);
	bw_gain_init(&coeffs->gain_bpl_coeffs);
	bw_gain_init(&coeffs->gain_bph_coeffs);

	bw_svf_set_cutoff(&coeffs->lp_coeffs, 4e3f);
	bw_svf_set_cutoff(&coeffs->hp_coeffs, 100.f);
	bw_svf_set_cutoff(&coeffs->bpl_coeffs, 100.f);
	bw_svf_set_cutoff(&coeffs->bph_coeffs, 4e3f);
	bw_gain_set_gain_lin(&coeffs->gain_bpl_coeffs, 2.25f);
	bw_gain_set_gain_lin(&coeffs->gain_bph_coeffs, 3.75f);

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_cab_coeffs");
	coeffs->state = bw_cab_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_cab_coeffs_state_init);
}

static inline void bw_cab_set_sample_rate(
		bw_cab_coeffs * BW_RESTRICT coeffs,
		float                       sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_svf_set_sample_rate(&coeffs->lp_coeffs, sample_rate);
	bw_svf_set_sample_rate(&coeffs->hp_coeffs, sample_rate);
	bw_svf_set_sample_rate(&coeffs->bpl_coeffs, sample_rate);
	bw_svf_set_sample_rate(&coeffs->bph_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_bpl_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_bph_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_cab_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_cab_coeffs_state_set_sample_rate);
}

static inline void bw_cab_reset_coeffs(
		bw_cab_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_set_sample_rate);

	bw_svf_reset_coeffs(&coeffs->lp_coeffs);
	bw_svf_reset_coeffs(&coeffs->hp_coeffs);
	bw_svf_reset_coeffs(&coeffs->bpl_coeffs);
	bw_svf_reset_coeffs(&coeffs->bph_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_bpl_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_bph_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_cab_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_cab_coeffs_state_reset_coeffs);
}

static inline float bw_cab_reset_state(
		const bw_cab_coeffs * BW_RESTRICT coeffs,
		bw_cab_state * BW_RESTRICT        state,
		float                             x_0) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT(bw_is_finite(x_0));

	float v_lp, v_hp, v_bp;
	bw_svf_reset_state(&coeffs->lp_coeffs, &state->lp_state, x_0, &v_lp, &v_bp, &v_hp);
	bw_svf_reset_state(&coeffs->hp_coeffs, &state->hp_state, v_lp, &v_lp, &v_bp, &v_hp);
	float v_bpl, v_bph;
	float y = v_hp;
	bw_svf_reset_state(&coeffs->bpl_coeffs, &state->bpl_state, y, &v_lp, &v_bpl, &v_hp);
	bw_svf_reset_state(&coeffs->bph_coeffs, &state->bph_state, y, &v_lp, &v_bph, &v_hp);
	y = bw_gain_get_gain_cur(&coeffs->gain_bpl_coeffs) * v_bpl
		+ bw_gain_get_gain_cur(&coeffs->gain_bph_coeffs) * v_bph
		+ 0.45f * y;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_cab_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_cab_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_cab_reset_state_multi(
		const bw_cab_coeffs * BW_RESTRICT              coeffs,
		bw_cab_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                  x_0,
		float *                                        y_0,
		size_t                                         n_channels) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != BW_NULL);

	if (y_0 != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_cab_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_cab_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != BW_NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_cab_update_coeffs_ctrl(
		bw_cab_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);

	bw_svf_update_coeffs_ctrl(&coeffs->lp_coeffs);
	bw_svf_update_coeffs_ctrl(&coeffs->hp_coeffs);
	bw_svf_update_coeffs_ctrl(&coeffs->bpl_coeffs);
	bw_svf_update_coeffs_ctrl(&coeffs->bph_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_bpl_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_bph_coeffs);

	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
}

static inline void bw_cab_update_coeffs_audio(
		bw_cab_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);

	bw_svf_update_coeffs_audio(&coeffs->lp_coeffs);
	bw_svf_update_coeffs_audio(&coeffs->hp_coeffs);
	bw_svf_update_coeffs_audio(&coeffs->bpl_coeffs);
	bw_svf_update_coeffs_audio(&coeffs->bph_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_bpl_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_bph_coeffs);

	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
}

static inline float bw_cab_process1(
		const bw_cab_coeffs * BW_RESTRICT coeffs,
		bw_cab_state * BW_RESTRICT        state,
		float                             x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	float v_lp, v_hp, v_bp;
	bw_svf_process1(&coeffs->lp_coeffs, &state->lp_state, x, &v_lp, &v_bp, &v_hp);
	bw_svf_process1(&coeffs->hp_coeffs, &state->hp_state, v_lp, &v_lp, &v_bp, &v_hp);
	float v_bpl, v_bph;
	float y = v_hp;
	bw_svf_process1(&coeffs->bpl_coeffs, &state->bpl_state, y, &v_lp, &v_bpl, &v_hp);
	bw_svf_process1(&coeffs->bph_coeffs, &state->bph_state, y, &v_lp, &v_bph, &v_hp);
	y = bw_gain_process1(&coeffs->gain_bpl_coeffs, v_bpl)
		+ bw_gain_process1(&coeffs->gain_bph_coeffs, v_bph)
		+ 0.45f * y;

	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_cab_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_cab_process(
		bw_cab_coeffs * BW_RESTRICT coeffs,
		bw_cab_state * BW_RESTRICT  state,
		const float *               x,
		float *                     y,
		size_t                      n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_state_is_valid(coeffs, state));
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != BW_NULL);

	bw_cab_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_cab_update_coeffs_audio(coeffs);
		y[i] = bw_cab_process1(coeffs, state, x[i]);
	}

	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_cab_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_cab_process_multi(
		bw_cab_coeffs * BW_RESTRICT                    coeffs,
		bw_cab_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                          x,
		float * const *                                y,
		size_t                                         n_channels,
		size_t                                         n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT(y != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(y[i] != y[j]);
#endif

	bw_cab_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_cab_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_cab_process1(coeffs, state[j], x[j][i]);
	}

	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_reset_coeffs);
}

static inline void bw_cab_set_cutoff_low(
		bw_cab_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 1.f);

	const float f = 50.f + value * (50.f + 100.f * value);
	bw_svf_set_cutoff(&coeffs->hp_coeffs, f);
	bw_svf_set_cutoff(&coeffs->bpl_coeffs, f);

	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_init);
}

static inline void bw_cab_set_cutoff_high(
		bw_cab_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 1.f);

	const float f = 2e3f + value * (2e3f + 4e3f * value);
	bw_svf_set_cutoff(&coeffs->lp_coeffs, f);
	bw_svf_set_cutoff(&coeffs->bph_coeffs, f);

	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_init);
}

static inline void bw_cab_set_tone(
		bw_cab_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 1.f);

	bw_gain_set_gain_lin(&coeffs->gain_bpl_coeffs, 3.f - 1.5f * value);
	bw_gain_set_gain_lin(&coeffs->gain_bph_coeffs, 3.f + 1.5f * value);

	BW_ASSERT_DEEP(bw_cab_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_cab_coeffs_state_init);
}

static inline char bw_cab_coeffs_is_valid(
		const bw_cab_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_cab_coeffs"))
		return 0;
	if (coeffs->state < bw_cab_coeffs_state_init || coeffs->state > bw_cab_coeffs_state_reset_coeffs)
		return 0;
#endif

	return bw_svf_coeffs_is_valid(&coeffs->lp_coeffs)
		&& bw_svf_coeffs_is_valid(&coeffs->hp_coeffs)
		&& bw_svf_coeffs_is_valid(&coeffs->bpl_coeffs)
		&& bw_svf_coeffs_is_valid(&coeffs->bph_coeffs)
		&& bw_gain_coeffs_is_valid(&coeffs->gain_bpl_coeffs)
		&& bw_gain_coeffs_is_valid(&coeffs->gain_bph_coeffs);
}

static inline char bw_cab_state_is_valid(
		const bw_cab_coeffs * BW_RESTRICT coeffs,
		const bw_cab_state * BW_RESTRICT  state) {
	BW_ASSERT(state != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_cab_state"))
		return 0;

	if (coeffs != BW_NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	return bw_svf_state_is_valid(coeffs ? &coeffs->lp_coeffs : BW_NULL, &state->lp_state)
		&& bw_svf_state_is_valid(coeffs ? &coeffs->hp_coeffs : BW_NULL, &state->hp_state)
		&& bw_svf_state_is_valid(coeffs ? &coeffs->bpl_coeffs : BW_NULL, &state->bpl_state)
		&& bw_svf_state_is_valid(coeffs ? &coeffs->bph_coeffs : BW_NULL, &state->bph_state);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Cab
 *  ```>>> */
template<size_t N_CHANNELS>
class Cab {
public:
	Cab();

	void setSampleRate(
		float sampleRate);

	void reset(
		float               x0 = 0.f,
		float * BW_RESTRICT y0 = nullptr);

	void reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0);

	void reset(
		const float * x0,
		float *       y0 = nullptr);

	void reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0 = nullptr);

	void process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples);

	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);

	void setCutoffLow(
		float value);

	void setCutoffHigh(
		float value);

	void setTone(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_cab_coeffs			coeffs;
	bw_cab_state			states[N_CHANNELS];
	bw_cab_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Cab<N_CHANNELS>::Cab() {
	bw_cab_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_cab_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_cab_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_cab_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_cab_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_cab_reset_coeffs(&coeffs);
	bw_cab_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_cab_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::setCutoffLow(
		float value) {
	bw_cab_set_cutoff_low(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::setCutoffHigh(
		float value) {
	bw_cab_set_cutoff_high(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Cab<N_CHANNELS>::setTone(
		float value) {
	bw_cab_set_tone(&coeffs, value);
}

}
#endif

#endif
