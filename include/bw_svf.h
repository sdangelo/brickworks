/*
 * Brickworks
 *
 * Copyright (C) 2022-2024 Orastron Srl unipersonale
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
 *  version {{{ 1.0.1 }}}
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    State variable filter (2nd order, 12 dB/oct) model with separated lowpass,
 *    bandpass, and highpass outputs.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.1</strong>:
 *        <ul>
 *          <li>Now using <code>BW_NULL</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Changed model to get positive polarity at the bandpass
 *              output.</li>
 *          <li>Limited actual prewarping frequency to prevent instability.</li>
 *          <li>Added <code>bw_svf_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_svf_reset_state()</code> returns the initial output
 *              values.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_svf_process()</code> and
 *              <code>bw_svf_process_multi()</code> now use <code>size_t</code>
 *              to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
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
 *          <li>Added <code>bw_svf_process_multi()</code>.</li>
 *          <li>Fixed bug in <code>bw_svf_process()</code> when only
 *              <code>y_hp</code> is <code>NULL</code>.</li>
 *          <li>Fixed prewarping-related stability bug.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_svf_reset_state()</code>.</li>
 *          <li>Fixed unused parameter warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Strenghtened algorithm for modulation.</li>
 *          <li>Added prewarping control parameters (prewarp_at_cutoff and
 *              prewarp_freq).</li>
 *          <li>Added <code>BW_RESTRICT</code> to
 *              <code>bw_svf_process1()</code>.</li>
 *          <li>Fixed typo in <code>bw_svf_set_Q()</code> documentation.</li>
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

#ifndef BW_SVF_H
#define BW_SVF_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_svf_coeffs
 *  ```>>> */
typedef struct bw_svf_coeffs bw_svf_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_svf_state
 *  ```>>> */
typedef struct bw_svf_state bw_svf_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_svf_init()
 *  ```>>> */
static inline void bw_svf_init(
	bw_svf_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_svf_set_sample_rate()
 *  ```>>> */
static inline void bw_svf_set_sample_rate(
	bw_svf_coeffs * BW_RESTRICT coeffs,
	float                       sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_svf_reset_coeffs()
 *  ```>>> */
static inline void bw_svf_reset_coeffs(
	bw_svf_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_svf_reset_state()
 *  ```>>> */
static inline void bw_svf_reset_state(
	const bw_svf_coeffs * BW_RESTRICT coeffs,
	bw_svf_state * BW_RESTRICT        state,
	float                             x_0,
	float * BW_RESTRICT               y_lp_0,
	float * BW_RESTRICT               y_bp_0,
	float * BW_RESTRICT               y_hp_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    The corresponding initial lowpass, bandpass, and highpass output values
 *    are put into `y_lp_0`, `y_bp_0`, and `y_hp_0` respectively.
 *
 *    #### bw_svf_reset_state_multi()
 *  ```>>> */
static inline void bw_svf_reset_state_multi(
	const bw_svf_coeffs * BW_RESTRICT              coeffs,
	bw_svf_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                  x_0,
	float *                                        y_lp_0,
	float *                                        y_bp_0,
	float *                                        y_hp_0,
	size_t                                         n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial lowpass, bandpass, and highpass output values
 *    are put into the `y_lp_0`, `y_bp_0`, and `y_hp_0` arrays, respectively, if
 *    they are not `BW_NULL`.
 *
 *    #### bw_svf_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_svf_update_coeffs_ctrl(
	bw_svf_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_svf_update_coeffs_audio()
 *  ```>>> */
static inline void bw_svf_update_coeffs_audio(
	bw_svf_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_svf_process1()
 *  ```>>> */
static inline void bw_svf_process1(
	const bw_svf_coeffs * BW_RESTRICT coeffs,
	bw_svf_state * BW_RESTRICT        state,
	float                             x,
	float * BW_RESTRICT               y_lp,
	float * BW_RESTRICT               y_bp,
	float * BW_RESTRICT               y_hp);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. The lowpass, bandpass, and highpass output samples are put into
 *    `y_lp`, `y_bp`, and `y_hp` respectively.
 *
 *    #### bw_svf_process()
 *  ```>>> */
static inline void bw_svf_process(
	bw_svf_coeffs * BW_RESTRICT coeffs,
	bw_svf_state * BW_RESTRICT  state,
	const float *               x,
	float *                     y_lp,
	float *                     y_bp,
	float *                     y_hp,
	size_t                      n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffers `y_lp` (lowpass), `y_bp`
 *    (bandpass), and `y_hp` (highpass), if they are not `BW_NULL`, while using
 *    and updating both `coeffs` and `state` (control and audio rate).
 * 
 *    #### bw_svf_process_multi()
 *  ```>>> */
static inline void bw_svf_process_multi(
	bw_svf_coeffs * BW_RESTRICT                    coeffs,
	bw_svf_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                          x,
	float * const *                                y_lp,
	float * const *                                y_bp,
	float * const *                                y_hp,
	size_t                                         n_channels,
	size_t                                         n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y_lp`
 *    (lowpass), `y_bp` (bandpass), and `y_hp` (highpass), while using and
 *    updating both the common `coeffs` and each of the `n_channels` `state`s
 *    (control and audio rate).
 * 
 *    `y_lp`, `y_bp`, and `y_hp`, or any of their elements may be `BW_NULL`.
 *
 *    #### bw_svf_set_cutoff()
 *  ```>>> */
static inline void bw_svf_set_cutoff(
	bw_svf_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the cutoff frequency to the given `value` (Hz) in `coeffs`.
 *
 *    Valid range: [`1e-6f`, `1e12f`].
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_svf_set_Q()
 *  ```>>> */
static inline void bw_svf_set_Q(
	bw_svf_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the quality factor to the given `value` in `coeffs`.
 *
 *    Valid range: [`1e-6f`, `1e6f`].
 *
 *    Default value: `0.5f`.
 * 
 *    #### bw_svf_set_prewarp_at_cutoff()
 *  ```>>> */
static inline void bw_svf_set_prewarp_at_cutoff(
	bw_svf_coeffs * BW_RESTRICT coeffs,
	char                        value);
/*! <<<```
 *    Sets whether bilinear transform prewarping frequency should match the
 *    cutoff frequency (non-`0`) or not (`0`).
 *
 *    Default value: non-`0` (on).
 *
 *    #### bw_svf_set_prewarp_freq()
 *  ```>>> */
static inline void bw_svf_set_prewarp_freq(
	bw_svf_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the prewarping frequency `value` (Hz) in `coeffs`.
 *
 *    Only used when the prewarp\_at\_cutoff parameter is off and however
 *    internally limited to avoid instability.
 *
 *    Valid range: [`1e-6f`, `1e12f`].
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_svf_coeffs_is_valid()
 *  ```>>> */
static inline char bw_svf_coeffs_is_valid(
	const bw_svf_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_svf_coeffs`.
 *
 *    #### bw_svf_state_is_valid()
 *  ```>>> */
static inline char bw_svf_state_is_valid(
	const bw_svf_coeffs * BW_RESTRICT coeffs,
	const bw_svf_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `BW_NULL` extra cross-checks might be performed
 *    (`state` is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_svf_state`.
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
enum bw_svf_coeffs_state {
	bw_svf_coeffs_state_invalid,
	bw_svf_coeffs_state_init,
	bw_svf_coeffs_state_set_sample_rate,
	bw_svf_coeffs_state_reset_coeffs
};
#endif

struct bw_svf_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_svf_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_one_pole_coeffs		smooth_coeffs;
	bw_one_pole_state		smooth_cutoff_state;
	bw_one_pole_state		smooth_Q_state;
	bw_one_pole_state		smooth_prewarp_freq_state;
	
	// Coefficients
	float				t_k;
	float				prewarp_freq_max;

	float				t;
	float				kf;
	float				kbl;
	float				k;
	float				hp_hb;
	float				hp_x;

	// Parameters
	float				cutoff;
	float				Q;
	float				prewarp_k;
	float				prewarp_freq;
};

struct bw_svf_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// States
	float		hp_z1;
	float		lp_z1;
	float		bp_z1;
	float		cutoff_z1;
};

static inline void bw_svf_init(
		bw_svf_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->cutoff = 1e3f;
	coeffs->Q = 0.5f;
	coeffs->prewarp_freq = 1e3f;
	coeffs->prewarp_k = 1.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_svf_coeffs");
	coeffs->state = bw_svf_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_svf_coeffs_state_init);
}

static inline void bw_svf_set_sample_rate(
		bw_svf_coeffs * BW_RESTRICT coeffs,
		float                       sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->t_k = 3.141592653589793f / sample_rate;
	coeffs->prewarp_freq_max = 0.499f * sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_svf_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_svf_coeffs_state_set_sample_rate);
}

static inline void bw_svf_do_update_coeffs(
		bw_svf_coeffs * BW_RESTRICT coeffs,
		char                        force) {
	const float prewarp_freq = coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq);
	float cutoff_cur = bw_one_pole_get_y_z1(&coeffs->smooth_cutoff_state);
	float prewarp_freq_cur = bw_one_pole_get_y_z1(&coeffs->smooth_prewarp_freq_state);
	float Q_cur = bw_one_pole_get_y_z1(&coeffs->smooth_Q_state);
	const char cutoff_changed = force || coeffs->cutoff != cutoff_cur;
	const char prewarp_freq_changed = force || prewarp_freq != prewarp_freq_cur;
	const char Q_changed = force || coeffs->Q != Q_cur;
	if (cutoff_changed || prewarp_freq_changed || Q_changed) {
		if (cutoff_changed || prewarp_freq_changed) {
			if (cutoff_changed)
				cutoff_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
			if (prewarp_freq_changed) {
				prewarp_freq_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, prewarp_freq);
				const float f = bw_minf(prewarp_freq_cur, coeffs->prewarp_freq_max);
				coeffs->t = bw_tanf(coeffs->t_k * f);
				coeffs->kf = coeffs->t * bw_rcpf(f);
			}
			coeffs->kbl = coeffs->kf * cutoff_cur;
		}
		if (Q_changed) {
			Q_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_Q_state, coeffs->Q);
			coeffs->k = bw_rcpf(Q_cur);
		}
		coeffs->hp_hb = coeffs->k + coeffs->kbl;
		coeffs->hp_x = bw_rcpf(1.f + coeffs->kbl * coeffs->hp_hb);
	}
}

static inline void bw_svf_reset_coeffs(
		bw_svf_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_set_sample_rate);

	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_Q_state, coeffs->Q);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq));
	bw_svf_do_update_coeffs(coeffs, 1);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_svf_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_svf_coeffs_state_reset_coeffs);
}

static inline void bw_svf_reset_state(
		const bw_svf_coeffs * BW_RESTRICT coeffs,
		bw_svf_state * BW_RESTRICT        state,
		float                             x_0,
		float * BW_RESTRICT               y_lp_0,
		float * BW_RESTRICT               y_bp_0,
		float * BW_RESTRICT               y_hp_0) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT(bw_is_finite(x_0));
	BW_ASSERT(y_lp_0 != BW_NULL);
	BW_ASSERT(y_bp_0 != BW_NULL);
	BW_ASSERT(y_hp_0 != BW_NULL);
	BW_ASSERT(y_lp_0 != y_bp_0);
	BW_ASSERT(y_lp_0 != y_hp_0);
	BW_ASSERT(y_bp_0 != y_hp_0);

	state->hp_z1 = 0.f;
	state->lp_z1 = x_0;
	state->bp_z1 = 0.f;
	state->cutoff_z1 = coeffs->cutoff;
	*y_lp_0 = x_0;
	*y_bp_0 = 0.f;
	*y_hp_0 = 0.f;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_svf_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_svf_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(*y_lp_0));
	BW_ASSERT(bw_is_finite(*y_bp_0));
	BW_ASSERT(bw_is_finite(*y_hp_0));
}

static inline void bw_svf_reset_state_multi(
		const bw_svf_coeffs * BW_RESTRICT              coeffs,
		bw_svf_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                  x_0,
		float *                                        y_lp_0,
		float *                                        y_bp_0,
		float *                                        y_hp_0,
		size_t                                         n_channels) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != BW_NULL);

	if (y_lp_0 != BW_NULL) {
		if (y_bp_0 != BW_NULL) {
			if (y_hp_0 != BW_NULL) {
				for (size_t i = 0; i < n_channels; i++)
					bw_svf_reset_state(coeffs, state[i], x_0[i], y_lp_0 + i, y_bp_0 + i, y_hp_0 + i);
			} else {
				for (size_t i = 0; i < n_channels; i++) {
					float v_hp;
					bw_svf_reset_state(coeffs, state[i], x_0[i], y_lp_0 + i, y_bp_0 + i, &v_hp);
				}
			}
		} else {
			if (y_hp_0 != BW_NULL) {
				for (size_t i = 0; i < n_channels; i++) {
					float v_bp;
					bw_svf_reset_state(coeffs, state[i], x_0[i], y_lp_0 + i, &v_bp, y_hp_0 + i);
				}
			} else {
				for (size_t i = 0; i < n_channels; i++) {
					float v_bp, v_hp;
					bw_svf_reset_state(coeffs, state[i], x_0[i], y_lp_0 + i, &v_bp, &v_hp);
				}
			}
		}
	} else {
		if (y_bp_0 != BW_NULL) {
			if (y_hp_0 != BW_NULL) {
				for (size_t i = 0; i < n_channels; i++) {
					float v_lp;
					bw_svf_reset_state(coeffs, state[i], x_0[i], &v_lp, y_bp_0 + i, y_hp_0 + i);
				}
			} else {
				for (size_t i = 0; i < n_channels; i++) {
					float v_lp, v_hp;
					bw_svf_reset_state(coeffs, state[i], x_0[i], &v_lp, y_bp_0 + i, &v_hp);
				}
			}
		} else {
			if (y_hp_0 != BW_NULL) {
				for (size_t i = 0; i < n_channels; i++) {
					float v_lp, v_bp;
					bw_svf_reset_state(coeffs, state[i], x_0[i], &v_lp, &v_bp, y_hp_0 + i);
				}
			} else {
				for (size_t i = 0; i < n_channels; i++) {
					float v_lp, v_bp, v_hp;
					bw_svf_reset_state(coeffs, state[i], x_0[i], &v_lp, &v_bp, &v_hp);
				}
			}
		}
	}

	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_lp_0 != BW_NULL ? bw_has_only_finite(y_lp_0, n_channels) : 1);
	BW_ASSERT_DEEP(y_bp_0 != BW_NULL ? bw_has_only_finite(y_bp_0, n_channels) : 1);
	BW_ASSERT_DEEP(y_hp_0 != BW_NULL ? bw_has_only_finite(y_hp_0, n_channels) : 1);
}

static inline void bw_svf_update_coeffs_ctrl(
		bw_svf_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_svf_update_coeffs_audio(
		bw_svf_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);

	bw_svf_do_update_coeffs(coeffs, 0);

	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
}

static inline void bw_svf_process1(
		const bw_svf_coeffs * BW_RESTRICT coeffs,
		bw_svf_state * BW_RESTRICT        state,
		float                             x,
		float * BW_RESTRICT               y_lp,
		float * BW_RESTRICT               y_bp,
		float * BW_RESTRICT               y_hp) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(y_lp != BW_NULL);
	BW_ASSERT(y_bp != BW_NULL);
	BW_ASSERT(y_hp != BW_NULL);
	BW_ASSERT(y_lp != y_bp);
	BW_ASSERT(y_lp != y_hp);
	BW_ASSERT(y_bp != y_hp);

	const float kk = coeffs->kf * state->cutoff_z1;
	const float lp_xz1 = state->lp_z1 + kk * state->bp_z1;
	const float bp_xz1 = state->bp_z1 + kk * state->hp_z1;
	*y_hp = coeffs->hp_x * (x - coeffs->hp_hb * bp_xz1 - lp_xz1);
	*y_bp = bp_xz1 + coeffs->kbl * *y_hp;
	*y_lp = lp_xz1 + coeffs->kbl * *y_bp;
	state->hp_z1 = *y_hp;
	state->lp_z1 = *y_lp;
	state->bp_z1 = *y_bp;
	state->cutoff_z1 = bw_one_pole_get_y_z1(&coeffs->smooth_cutoff_state);

	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_svf_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(*y_lp));
	BW_ASSERT(bw_is_finite(*y_bp));
	BW_ASSERT(bw_is_finite(*y_hp));
}

static inline void bw_svf_process(
		bw_svf_coeffs * BW_RESTRICT coeffs,
		bw_svf_state * BW_RESTRICT  state,
		const float *               x,
		float *                     y_lp,
		float *                     y_bp,
		float *                     y_hp,
		size_t                      n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_state_is_valid(coeffs, state));
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y_lp == BW_NULL || y_bp == BW_NULL || y_lp != y_bp);
	BW_ASSERT(y_lp == BW_NULL || y_hp == BW_NULL || y_lp != y_hp);
	BW_ASSERT(y_bp == BW_NULL || y_hp == BW_NULL || y_bp != y_hp);

	if (y_lp != BW_NULL) {
		if (y_bp != BW_NULL) {
			if (y_hp != BW_NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					bw_svf_process1(coeffs, state, x[i], y_lp + i, y_bp + i, y_hp + i);
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_hp;
					bw_svf_process1(coeffs, state, x[i], y_lp + i, y_bp + i, &v_hp);
				}
			}
		} else {
			if (y_hp != BW_NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_bp;
					bw_svf_process1(coeffs, state, x[i], y_lp + i, &v_bp, y_hp + i);
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_bp, v_hp;
					bw_svf_process1(coeffs, state, x[i], y_lp + i, &v_bp, &v_hp);
				}
			}
		}
	} else {
		if (y_bp != BW_NULL) {
			if (y_hp != BW_NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, y_bp + i, y_hp + i);
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp, v_hp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, y_bp + i, &v_hp);
				}
			}
		} else {
			if (y_hp != BW_NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp, v_bp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, &v_bp, y_hp + i);
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp, v_bp, v_hp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, &v_bp, &v_hp);
				}
			}
		}
	}

	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_svf_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(y_lp != BW_NULL ? bw_has_only_finite(y_lp, n_samples) : 1);
	BW_ASSERT_DEEP(y_bp != BW_NULL ? bw_has_only_finite(y_bp, n_samples) : 1);
	BW_ASSERT_DEEP(y_hp != BW_NULL ? bw_has_only_finite(y_hp, n_samples) : 1);
}

static inline void bw_svf_process_multi(
		bw_svf_coeffs * BW_RESTRICT                    coeffs,
		bw_svf_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                          x,
		float * const *                                y_lp,
		float * const *                                y_bp,
		float * const *                                y_hp,
		size_t                                         n_channels,
		size_t                                         n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT(y_lp == BW_NULL || y_bp == BW_NULL || y_lp != y_bp);
	BW_ASSERT(y_lp == BW_NULL || y_hp == BW_NULL || y_lp != y_hp);
	BW_ASSERT(y_bp == BW_NULL || y_hp == BW_NULL || y_bp != y_hp);
#ifndef BW_NO_DEBUG
	if (y_lp != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = i + 1; j < n_channels; j++)
				BW_ASSERT(y_lp[i] == BW_NULL || y_lp[j] == BW_NULL || y_lp[i] != y_lp[j]);
	if (y_bp != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = i + 1; j < n_channels; j++)
				BW_ASSERT(y_bp[i] == BW_NULL || y_bp[j] == BW_NULL || y_bp[i] != y_bp[j]);
	if (y_hp != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = i + 1; j < n_channels; j++)
				BW_ASSERT(y_hp[i] == BW_NULL || y_hp[j] == BW_NULL || y_hp[i] != y_hp[j]);
	if (y_lp != BW_NULL && y_bp != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = 0; j < n_channels; j++)
				BW_ASSERT(y_lp[i] == BW_NULL || y_bp[j] == BW_NULL || y_lp[i] != y_bp[j]);
	if (y_lp != BW_NULL && y_hp != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = 0; j < n_channels; j++)
				BW_ASSERT(y_lp[i] == BW_NULL || y_hp[j] == BW_NULL || y_lp[i] != y_hp[j]);
	if (y_bp != BW_NULL && y_hp != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = 0; j < n_channels; j++)
				BW_ASSERT(y_bp[i] == BW_NULL || y_hp[j] == BW_NULL || y_bp[i] != y_hp[j]);
#endif

	if (y_lp != BW_NULL) {
		if (y_bp != BW_NULL) {
			if (y_hp != BW_NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_lp[j])
							y_lp[j][i] = v_lp;
						if (y_bp[j])
							y_bp[j][i] = v_bp;
						if (y_hp[j])
							y_hp[j][i] = v_hp;
					}
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_lp[j])
							y_lp[j][i] = v_lp;
						if (y_bp[j])
							y_bp[j][i] = v_bp;
					}
				}
			}
		} else {
			if (y_hp != BW_NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_lp[j])
							y_lp[j][i] = v_lp;
						if (y_hp[j])
							y_hp[j][i] = v_hp;
					}
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_lp[j])
							y_lp[j][i] = v_lp;
					}
				}
			}
		}
	} else {
		if (y_bp != BW_NULL) {
			if (y_hp != BW_NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_bp[j])
							y_bp[j][i] = v_bp;
						if (y_hp[j])
							y_hp[j][i] = v_hp;
					}
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_bp[j])
							y_bp[j][i] = v_bp;
					}
				}
			}
		} else {
			if (y_hp != BW_NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_hp[j])
							y_hp[j][i] = v_hp;
					}
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
					}
				}
			}
		}
	}

	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_reset_coeffs);
}

static inline void bw_svf_set_cutoff(
		bw_svf_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e12f);

	coeffs->cutoff = value;

	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_init);
}

static inline void bw_svf_set_Q(
		bw_svf_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e6f);

	coeffs->Q = value;

	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_init);
}

static inline void bw_svf_set_prewarp_at_cutoff(
		bw_svf_coeffs * BW_RESTRICT coeffs,
		char                        value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_init);

	coeffs->prewarp_k = value ? 1.f : 0.f;

	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_init);
}

static inline void bw_svf_set_prewarp_freq(
		bw_svf_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 1e-6f && value <= 1e12f);

	coeffs->prewarp_freq = value;

	BW_ASSERT_DEEP(bw_svf_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_svf_coeffs_state_init);
}

static inline char bw_svf_coeffs_is_valid(
		const bw_svf_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_svf_coeffs"))
		return 0;
	if (coeffs->state < bw_svf_coeffs_state_init || coeffs->state > bw_svf_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (!bw_is_finite(coeffs->cutoff) || coeffs->cutoff < 1e-6f || coeffs->cutoff > 1e12f)
		return 0;
	if (!bw_is_finite(coeffs->Q) || coeffs->Q < 1e-6f || coeffs->Q > 1e6f)
		return 0;
	if (!bw_is_finite(coeffs->prewarp_k) || (coeffs->prewarp_k != 0.f && coeffs->prewarp_k != 1.f))
		return 0;
	if (!bw_is_finite(coeffs->prewarp_freq) || coeffs->prewarp_freq < 1e-6f || coeffs->prewarp_freq > 1e12f)
		return 0;

	if (!bw_one_pole_coeffs_is_valid(&coeffs->smooth_coeffs))
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_svf_coeffs_state_set_sample_rate) {
		if (!bw_is_finite(coeffs->t_k) || coeffs->t_k <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->prewarp_freq_max) || coeffs->prewarp_freq_max <= 0.f)
			return 0;
	}

	if (coeffs->state >= bw_svf_coeffs_state_reset_coeffs) {
		if (!bw_is_finite(coeffs->t) || coeffs->t <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->kf) || coeffs->kf <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->kbl) || coeffs->kbl <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->k) || coeffs->k <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->hp_hb) || coeffs->hp_hb <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->hp_x) || coeffs->hp_x <= 0.f || coeffs->hp_x >= 1.f)
			return 0;

		if (!bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state))
			return 0;
		if (!bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_Q_state))
			return 0;
		if (!bw_one_pole_state_is_valid(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state))
			return 0;
	}
#endif

	return 1;
}

static inline char bw_svf_state_is_valid(
		const bw_svf_coeffs * BW_RESTRICT coeffs,
		const bw_svf_state * BW_RESTRICT  state) {
	BW_ASSERT(state != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_svf_state"))
		return 0;

	if (coeffs != BW_NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	(void)coeffs;

	if (!bw_is_finite(state->hp_z1))
		return 0;
	if (!bw_is_finite(state->lp_z1))
		return 0;
	if (!bw_is_finite(state->bp_z1))
		return 0;
	if (!bw_is_finite(state->cutoff_z1) || state->cutoff_z1 < 1e-6f || state->cutoff_z1 > 1e12f)
		return 0;

	return 1;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::SVF
 *  ```>>> */
template<size_t N_CHANNELS>
class SVF {
public:
	SVF();

	void setSampleRate(
		float sampleRate);

	void reset(
		float               x0 = 0.f,
		float * BW_RESTRICT yLp0 = nullptr,
		float * BW_RESTRICT yBp0 = nullptr,
		float * BW_RESTRICT yHp0 = nullptr);

	void reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yLp0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yBp0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yHp0);

	void reset(
		const float * x0,
		float *       yLp0 = nullptr,
		float *       yBp0 = nullptr,
		float *       yHp0 = nullptr);

	void reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yLp0 = nullptr,
		std::array<float, N_CHANNELS> * BW_RESTRICT yBp0 = nullptr,
		std::array<float, N_CHANNELS> * BW_RESTRICT yHp0 = nullptr);

	void process(
		const float * const * x,
		float * const *       yLp,
		float * const *       yBp,
		float * const *       yHp,
		size_t                nSamples);

	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       yLp,
		std::array<float *, N_CHANNELS>       yBp,
		std::array<float *, N_CHANNELS>       yHp,
		size_t                                nSamples);

	void setCutoff(
		float value);

	void setQ(
		float value);

	void setPrewarpAtCutoff(
		bool value);

	void setPrewarpFreq(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_svf_coeffs			coeffs;
	bw_svf_state			states[N_CHANNELS];
	bw_svf_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline SVF<N_CHANNELS>::SVF() {
	bw_svf_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_svf_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT yLp0,
		float * BW_RESTRICT yBp0,
		float * BW_RESTRICT yHp0) {
	bw_svf_reset_coeffs(&coeffs);
	if (yLp0 != nullptr) {
		if (yBp0 != nullptr) {
			if (yHp0 != nullptr) {
				for (size_t i = 0; i < N_CHANNELS; i++)
					bw_svf_reset_state(&coeffs, states + i, x0, yLp0 + i, yBp0 + i, yHp0 + i);
			} else {
				for (size_t i = 0; i < N_CHANNELS; i++) {
					float vHp;
					bw_svf_reset_state(&coeffs, states + i, x0, yLp0 + i, yBp0 + i, &vHp);
				}
			}
		} else {
			if (yHp0 != nullptr) {
				for (size_t i = 0; i < N_CHANNELS; i++) {
					float vBp;
					bw_svf_reset_state(&coeffs, states + i, x0, yLp0 + i, &vBp, yHp0 + i);
				}
			} else {
				for (size_t i = 0; i < N_CHANNELS; i++) {
					float vBp, vHp;
					bw_svf_reset_state(&coeffs, states + i, x0, yLp0 + i, &vBp, &vHp);
				}
			}
		}
	} else {
		if (yBp0 != nullptr) {
			if (yHp0 != nullptr) {
				for (size_t i = 0; i < N_CHANNELS; i++) {
					float vLp;
					bw_svf_reset_state(&coeffs, states + i, x0, &vLp, yBp0 + i, yHp0 + i);
				}
			} else {
				for (size_t i = 0; i < N_CHANNELS; i++) {
					float vLp, vHp;
					bw_svf_reset_state(&coeffs, states + i, x0, &vLp, yBp0 + i, &vHp);
				}
			}
		} else {
			if (yHp0 != nullptr) {
				for (size_t i = 0; i < N_CHANNELS; i++) {
					float vLp, vBp;
					bw_svf_reset_state(&coeffs, states + i, x0, &vLp, &vBp, yHp0 + i);
				}
			} else {
				for (size_t i = 0; i < N_CHANNELS; i++) {
					float vLp, vBp, vHp;
					bw_svf_reset_state(&coeffs, states + i, x0, &vLp, &vBp, &vHp);
				}
			}
		}
	}
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yLp0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yBp0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yHp0) {
	reset(x0, yLp0 != nullptr ? yLp0->data() : nullptr, yBp0 != nullptr ? yBp0->data() : nullptr, yHp0 != nullptr ? yHp0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::reset(
		const float * x0,
		float *       yLp0,
		float *       yBp0,
		float *       yHp0) {
	bw_svf_reset_coeffs(&coeffs);
	bw_svf_reset_state_multi(&coeffs, statesP, x0, yLp0, yBp0, yHp0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yLp0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yBp0,
		std::array<float, N_CHANNELS> * BW_RESTRICT yHp0) {
	reset(x0.data(), yLp0 != nullptr ? yLp0->data() : nullptr, yBp0 != nullptr ? yBp0->data() : nullptr, yHp0 != nullptr ? yHp0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::process(
		const float * const * x,
		float * const *       yLp,
		float * const *       yBp,
		float * const *       yHp,
		size_t                nSamples) {
	bw_svf_process_multi(&coeffs, statesP, x, yLp, yBp, yHp, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       yLp,
		std::array<float *, N_CHANNELS>       yBp,
		std::array<float *, N_CHANNELS>       yHp,
		size_t                                nSamples) {
	process(x.data(), yLp.data(), yBp.data(), yHp.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setCutoff(
		float value) {
	bw_svf_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setQ(
		float value) {
	bw_svf_set_Q(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setPrewarpAtCutoff(
		bool value) {
	bw_svf_set_prewarp_at_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setPrewarpFreq(
		float value) {
	bw_svf_set_prewarp_freq(&coeffs, value);
}

}
#endif

#endif
