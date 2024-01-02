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
 *  requires {{{ bw_common bw_math bw_one_pole bw_svf }}}
 *  description {{{
 *    Wah effect.
 *
 *    This is a simple digital effect, not an emulation of any analog circuit.
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
 *          <li>Added initial input value to
 *              <code>bw_wah_reset_state()</code>.</li>
 *          <li>Added <code>bw_wah_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_wah_reset_state()</code> returns the initial output
 *              value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_wah_process()</code> and
 *              <code>bw_wah_process_multi()</code> now use <code>size_t</code>
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
 *          <li>Added <code>bw_wah_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Now specifying <code>0.f</code> as initial input value for
 *              <code>bw_svf_reset_state()</code>.</li>
 *          <li>Fixed documentation for <code>bw_wah_state</code>,
 *              <code>bw_wah_reset_state()</code>, and
 *              <code>bw_wah_process()</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Use bandpass SVF output rather than lowpass.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.2.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_WAH_H
#define BW_WAH_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_wah_coeffs
 *  ```>>> */
typedef struct bw_wah_coeffs bw_wah_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_wah_state
 *  ```>>> */
typedef struct bw_wah_state bw_wah_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_wah_init()
 *  ```>>> */
static inline void bw_wah_init(
	bw_wah_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_wah_set_sample_rate()
 *  ```>>> */
static inline void bw_wah_set_sample_rate(
	bw_wah_coeffs * BW_RESTRICT coeffs,
	float                       sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_wah_reset_coeffs()
 *  ```>>> */
static inline void bw_wah_reset_coeffs(
	bw_wah_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_wah_reset_state()
 *  ```>>> */
static inline float bw_wah_reset_state(
	const bw_wah_coeffs * BW_RESTRICT coeffs,
	bw_wah_state * BW_RESTRICT        state,
	float                             x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_wah_reset_state_multi()
 *  ```>>> */
static inline void bw_wah_reset_state_multi(
	const bw_wah_coeffs * BW_RESTRICT              coeffs,
	bw_wah_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_wah_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_wah_update_coeffs_ctrl(
	bw_wah_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_wah_update_coeffs_audio()
 *  ```>>> */
static inline void bw_wah_update_coeffs_audio(
	bw_wah_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_wah_process1()
 *  ```>>> */
static inline float bw_wah_process1(
	const bw_wah_coeffs * BW_RESTRICT coeffs,
	bw_wah_state * BW_RESTRICT        state,
	float                             x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_wah_process()
 *  ```>>> */
static inline void bw_wah_process(
	bw_wah_coeffs * BW_RESTRICT coeffs,
	bw_wah_state * BW_RESTRICT  state,
	const float *               x,
	float *                     y,
	size_t                      n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_wah_process_multi()
 *  ```>>> */
static inline void bw_wah_process_multi(
	bw_wah_coeffs * BW_RESTRICT                    coeffs,
	bw_wah_state * BW_RESTRICT const * BW_RESTRICT state,
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
 *    #### bw_wah_set_wah()
 *  ```>>> */
static inline void bw_wah_set_wah(
	bw_wah_coeffs * BW_RESTRICT coeffs,
	float                       value);
/*! <<<```
 *    Sets the wah pedal position to the given `value`.
 *
 *    Valid range: [`0.f` (low cutoff), `1.f` (high cutoff)].
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_wah_coeffs_is_valid()
 *  ```>>> */
static inline char bw_wah_coeffs_is_valid(
	const bw_wah_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_wah_coeffs`.
 *
 *    #### bw_wah_state_is_valid()
 *  ```>>> */
static inline char bw_wah_state_is_valid(
	const bw_wah_coeffs * BW_RESTRICT coeffs,
	const bw_wah_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `BW_NULL` extra cross-checks might be performed
 *    (`state` is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_wah_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_svf.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_wah_coeffs_state {
	bw_wah_coeffs_state_invalid,
	bw_wah_coeffs_state_init,
	bw_wah_coeffs_state_set_sample_rate,
	bw_wah_coeffs_state_reset_coeffs
};
#endif

struct bw_wah_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_wah_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_svf_coeffs			svf_coeffs;
};

struct bw_wah_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// Sub-components
	bw_svf_state	svf_state;
};

static inline void bw_wah_init(
		bw_wah_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	bw_svf_init(&coeffs->svf_coeffs);
	bw_svf_set_cutoff(&coeffs->svf_coeffs, 600.f);
	bw_svf_set_Q(&coeffs->svf_coeffs, 9.f);

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_wah_coeffs");
	coeffs->state = bw_wah_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_wah_coeffs_state_init);
}

static inline void bw_wah_set_sample_rate(
		bw_wah_coeffs * BW_RESTRICT coeffs,
		float                       sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_svf_set_sample_rate(&coeffs->svf_coeffs, sample_rate);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_wah_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_wah_coeffs_state_set_sample_rate);
}

static inline void bw_wah_reset_coeffs(
		bw_wah_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_set_sample_rate);

	bw_svf_reset_coeffs(&coeffs->svf_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_wah_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_wah_coeffs_state_reset_coeffs);
}

static inline float bw_wah_reset_state(
		const bw_wah_coeffs * BW_RESTRICT coeffs,
		bw_wah_state * BW_RESTRICT        state,
		float                             x_0) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT(bw_is_finite(x_0));

	float lp, bp, hp;
	bw_svf_reset_state(&coeffs->svf_coeffs, &state->svf_state, x_0, &lp, &bp, &hp);

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_wah_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_wah_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(bp));

	return bp;
}

static inline void bw_wah_reset_state_multi(
		const bw_wah_coeffs * BW_RESTRICT              coeffs,
		bw_wah_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                  x_0,
		float *                                        y_0,
		size_t                                         n_channels) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != BW_NULL);

	if (y_0 != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_wah_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_wah_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != BW_NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_wah_update_coeffs_ctrl(
		bw_wah_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);

	bw_svf_update_coeffs_ctrl(&coeffs->svf_coeffs);

	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
}

static inline void bw_wah_update_coeffs_audio(
		bw_wah_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);

	bw_svf_update_coeffs_audio(&coeffs->svf_coeffs);

	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
}

static inline float bw_wah_process1(
		const bw_wah_coeffs * BW_RESTRICT coeffs,
		bw_wah_state * BW_RESTRICT        state,
		float                             x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	float v_lp, v_hp, v_bp;
	bw_svf_process1(&coeffs->svf_coeffs, &state->svf_state, x, &v_lp, &v_bp, &v_hp);

	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_wah_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(v_bp));

	return v_bp;
}

static inline void bw_wah_process(
		bw_wah_coeffs * BW_RESTRICT coeffs,
		bw_wah_state * BW_RESTRICT  state,
		const float *               x,
		float *                     y,
		size_t                      n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_state_is_valid(coeffs, state));
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != BW_NULL);

	bw_wah_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_wah_update_coeffs_audio(coeffs);
		y[i] = bw_wah_process1(coeffs, state, x[i]);
	}

	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_wah_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_wah_process_multi(
		bw_wah_coeffs * BW_RESTRICT                    coeffs,
		bw_wah_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                          x,
		float * const *                                y,
		size_t                                         n_channels,
		size_t                                         n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
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

	bw_wah_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_wah_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_wah_process1(coeffs, state[j], x[j][i]);
	}

	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_reset_coeffs);
}

static inline void bw_wah_set_wah(
		bw_wah_coeffs * BW_RESTRICT coeffs,
		float                       value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 1.f);

	bw_svf_set_cutoff(&coeffs->svf_coeffs, 400.f + (2e3f - 400.f) * value * value * value);

	BW_ASSERT_DEEP(bw_wah_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_wah_coeffs_state_init);
}

static inline char bw_wah_coeffs_is_valid(
		const bw_wah_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_wah_coeffs"))
		return 0;
	if (coeffs->state < bw_wah_coeffs_state_init || coeffs->state > bw_wah_coeffs_state_reset_coeffs)
		return 0;
#endif

	return bw_svf_coeffs_is_valid(&coeffs->svf_coeffs);
}

static inline char bw_wah_state_is_valid(
		const bw_wah_coeffs * BW_RESTRICT coeffs,
		const bw_wah_state * BW_RESTRICT  state) {
	BW_ASSERT(state != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_wah_state"))
		return 0;

	if (coeffs != BW_NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	return bw_svf_state_is_valid(coeffs ? &coeffs->svf_coeffs : BW_NULL, &state->svf_state);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Wah
 *  ```>>> */
template<size_t N_CHANNELS>
class Wah {
public:
	Wah();

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

	void setWah(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_wah_coeffs			coeffs;
	bw_wah_state			states[N_CHANNELS];
	bw_wah_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Wah<N_CHANNELS>::Wah() {
	bw_wah_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Wah<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_wah_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Wah<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_wah_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_wah_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_wah_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void Wah<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Wah<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_wah_reset_coeffs(&coeffs);
	bw_wah_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

template<size_t N_CHANNELS>
inline void Wah<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}

template<size_t N_CHANNELS>
inline void Wah<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_wah_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Wah<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Wah<N_CHANNELS>::setWah(
		float value) {
	bw_wah_set_wah(&coeffs, value);
}

}
#endif

#endif
