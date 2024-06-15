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
 *  version {{{ 1.1.1 }}}
 *  requires {{{ bw_common bw_math }}}
 *  description {{{
 *    Sample rate reducer.
 *
 *    This is purely an audio effect, it doesn't actually produce an output
 *    signal at a lower sample rate. The algorithm is deliberately crude to
 *    obtain the characteristic aliasing due to lo-fi downsampling.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.1.1</strong>:
 *        <ul>
 *          <li>Added debugging check in
 *              <code>bw_sr_reduce_process_multi()</code> to ensure that buffers
 *              used for both input and output appear at the same channel
 *              indices.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.1.0</strong>:
 *        <ul>
 *          <li>Now using <code>BW_NULL</code> and
 *              <code>BW_CXX_NO_ARRAY</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_sr_reduce_set_sample_rate()</code>,
 *              <code>bw_sr_reduce_reset_coeffs()</code>,
 *              <code>bw_sr_reduce_update_coeffs_ctrl()</code>, and
 *              <code>bw_sr_reduce_update_coeffs_audio()</code>.</li>
 *          <li>Added initial input value to
 *              <code>bw_sr_reduce_reset_state()</code>.</li>
 *          <li>Added <code>bw_sr_reduce_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_sr_reduce_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_sr_reduce_process()</code> and
 *              <code>bw_sr_reduce_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
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
 *          <li>Added <code>bw_sr_reduce_process_multi()</code>.</li>
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
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_SR_REDUCE_H
#define BW_SR_REDUCE_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_sr_reduce_coeffs
 *  ```>>> */
typedef struct bw_sr_reduce_coeffs bw_sr_reduce_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_sr_reduce_state
 *  ```>>> */
typedef struct bw_sr_reduce_state bw_sr_reduce_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_sr_reduce_init()
 *  ```>>> */
static inline void bw_sr_reduce_init(
	bw_sr_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_sr_reduce_set_sample_rate()
 *  ```>>> */
static inline void bw_sr_reduce_set_sample_rate(
	bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
	float                             sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_sr_reduce_reset_coeffs()
 *  ```>>> */
static inline void bw_sr_reduce_reset_coeffs(
	bw_sr_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_sr_reduce_reset_state()
 *  ```>>> */
static inline float bw_sr_reduce_reset_state(
	const bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
	bw_sr_reduce_state * BW_RESTRICT        state,
	float                                   x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_sr_reduce_reset_state_multi()
 *  ```>>> */
static inline void bw_sr_reduce_reset_state_multi(
	const bw_sr_reduce_coeffs * BW_RESTRICT              coeffs,
	bw_sr_reduce_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                        x_0,
	float *                                              y_0,
	size_t                                               n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `BW_NULL`.
 *
 *    #### bw_sr_reduce_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_sr_reduce_update_coeffs_ctrl(
	bw_sr_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_sr_reduce_update_coeffs_audio()
 *  ```>>> */
static inline void bw_sr_reduce_update_coeffs_audio(
	bw_sr_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_sr_reduce_process1()
 *  ```>>> */
static inline float bw_sr_reduce_process1(
	const bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
	bw_sr_reduce_state * BW_RESTRICT        state,
	float                                   x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_sr_reduce_process()
 *  ```>>> */
static inline void bw_sr_reduce_process(
	bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
	bw_sr_reduce_state * BW_RESTRICT  state,
	const float *                     x,
	float *                           y,
	size_t                            n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using `coeffs` and
 *    both using and updating `state` (control and audio rate).
 *
 *    #### bw_sr_reduce_process_multi()
 *  ```>>> */
static inline void bw_sr_reduce_process_multi(
	bw_sr_reduce_coeffs * BW_RESTRICT                    coeffs,
	bw_sr_reduce_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                                x,
	float * const *                                      y,
	size_t                                               n_channels,
	size_t                                               n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using the common `coeffs` and both using and updating each of the
 *    `n_channels` `state`s.
 *
 *    #### bw_sr_reduce_set_ratio()
 *  ```>>> */
static inline void bw_sr_reduce_set_ratio(
	bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
	float                             value);
/*! <<<```
 *    Sets the output to input sample rate ratio in `coeffs`.
 *
 *    Valid input range: [`0.f`, `1.f`].
 *
 *    Default value: `1.f`.
 *
*    #### bw_sr_reduce_coeffs_is_valid()
 *  ```>>> */
static inline char bw_sr_reduce_coeffs_is_valid(
	const bw_sr_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_sr_reduce_coeffs`.
 *
 *    #### bw_sr_reduce_state_is_valid()
 *  ```>>> */
static inline char bw_sr_reduce_state_is_valid(
	const bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
	const bw_sr_reduce_state * BW_RESTRICT  state);
/*! <<<```
 *    Tries to determine whether `state` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    If `coeffs` is not `BW_NULL` extra cross-checks might be performed
 *    (`state` is supposed to be associated to `coeffs`).
 *
 *    `state` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_sr_reduce_state`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_sr_reduce_coeffs_state {
	bw_sr_reduce_coeffs_state_invalid,
	bw_sr_reduce_coeffs_state_init,
	bw_sr_reduce_coeffs_state_set_sample_rate,
	bw_sr_reduce_coeffs_state_reset_coeffs
};
#endif

struct bw_sr_reduce_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_sr_reduce_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Parameters
	float				ratio;
};

struct bw_sr_reduce_state {
#ifdef BW_DEBUG_DEEP
	uint32_t	hash;
	uint32_t	coeffs_reset_id;
#endif

	// States
	float		phase;
	float		y_z1;
};

static inline void bw_sr_reduce_init(
		bw_sr_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	coeffs->ratio = 1.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_sr_reduce_coeffs");
	coeffs->state = bw_sr_reduce_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_sr_reduce_coeffs_state_init);
}

static inline void bw_sr_reduce_set_sample_rate(
		bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
		float                             sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	(void)coeffs;
	(void)sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_sr_reduce_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_sr_reduce_coeffs_state_set_sample_rate);
}

static inline void bw_sr_reduce_reset_coeffs(
		bw_sr_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_set_sample_rate);

	(void)coeffs;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_sr_reduce_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_sr_reduce_coeffs_state_reset_coeffs);
}

static inline float bw_sr_reduce_reset_state(
		const bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
		bw_sr_reduce_state * BW_RESTRICT        state,
		float                                   x_0) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT(bw_is_finite(x_0));

	(void)coeffs;
	state->y_z1 = x_0;
	state->phase = 1.f;
	const float y = x_0;

#ifdef BW_DEBUG_DEEP
	state->hash = bw_hash_sdbm("bw_sr_reduce_state");
	state->coeffs_reset_id = coeffs->reset_id;
#endif
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_sr_reduce_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_sr_reduce_reset_state_multi(
		const bw_sr_reduce_coeffs * BW_RESTRICT              coeffs,
		bw_sr_reduce_state * BW_RESTRICT const * BW_RESTRICT state,
		const float *                                        x_0,
		float *                                              y_0,
		size_t                                               n_channels) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(state[i] != state[j]);
#endif
	BW_ASSERT(x_0 != BW_NULL);

	if (y_0 != BW_NULL)
		for (size_t i = 0; i < n_channels; i++)
			y_0[i] = bw_sr_reduce_reset_state(coeffs, state[i], x_0[i]);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_sr_reduce_reset_state(coeffs, state[i], x_0[i]);

	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(y_0 != BW_NULL ? bw_has_only_finite(y_0, n_channels) : 1);
}

static inline void bw_sr_reduce_update_coeffs_ctrl(
		bw_sr_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_sr_reduce_update_coeffs_audio(
		bw_sr_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline float bw_sr_reduce_process1(
		const bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
		bw_sr_reduce_state * BW_RESTRICT        state,
		float                                   x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(x));

	state->phase += coeffs->ratio;
	if (state->phase >= 1.f) {
		state->y_z1 = x;
		state->phase -= bw_floorf(state->phase);
	}
	const float y = state->y_z1;

	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_sr_reduce_state_is_valid(coeffs, state));
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_sr_reduce_process(
		bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
		bw_sr_reduce_state * BW_RESTRICT  state,
		const float *                     x,
		float *                           y,
		size_t                            n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT(state != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_state_is_valid(coeffs, state));
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != BW_NULL);

	for (size_t i = 0; i < n_samples; i++)
		y[i] = bw_sr_reduce_process1(coeffs, state, x[i]);

	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_sr_reduce_state_is_valid(coeffs, state));
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_sr_reduce_process_multi(
		bw_sr_reduce_coeffs * BW_RESTRICT                    coeffs,
		bw_sr_reduce_state * BW_RESTRICT const * BW_RESTRICT state,
		const float * const *                                x,
		float * const *                                      y,
		size_t                                               n_channels,
		size_t                                               n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
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
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = 0; j < n_channels; j++)
			BW_ASSERT(i == j || x[i] != y[j]);
#endif

	for (size_t i = 0; i < n_channels; i++)
		bw_sr_reduce_process(coeffs, state[i], x[i], y[i], n_samples);

	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_reset_coeffs);
}

static inline void bw_sr_reduce_set_ratio(
		bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
		float                             value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 1.f);

	coeffs->ratio = value;

	BW_ASSERT_DEEP(bw_sr_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_sr_reduce_coeffs_state_init);
}

static inline char bw_sr_reduce_coeffs_is_valid(
		const bw_sr_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_sr_reduce_coeffs"))
		return 0;
	if (coeffs->state < bw_sr_reduce_coeffs_state_init || coeffs->state > bw_sr_reduce_coeffs_state_reset_coeffs)
		return 0;
#endif

	return bw_is_finite(coeffs->ratio) && coeffs->ratio >= 0.f &&  coeffs->ratio <= 1.f;
}

static inline char bw_sr_reduce_state_is_valid(
		const bw_sr_reduce_coeffs * BW_RESTRICT coeffs,
		const bw_sr_reduce_state * BW_RESTRICT  state) {
	BW_ASSERT(state != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (state->hash != bw_hash_sdbm("bw_sr_reduce_state"))
		return 0;

	if (coeffs != BW_NULL && coeffs->reset_id != state->coeffs_reset_id)
		return 0;
#endif

	(void)coeffs;

	return bw_is_finite(state->phase) && state->phase >= 0.f && bw_is_finite(state->y_z1);
}

#ifdef __cplusplus
}

#ifndef BW_CXX_NO_ARRAY
# include <array>
#endif

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::SRReduce
 *  ```>>> */
template<size_t N_CHANNELS>
class SRReduce {
public:
	SRReduce();
	
	void setSampleRate(
		float sampleRate);

	void reset(
		float               x0 = 0.f,
		float * BW_RESTRICT y0 = nullptr);

#ifndef BW_CXX_NO_ARRAY
	void reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0);
#endif

	void reset(
		const float * x0,
		float *       y0 = nullptr);

#ifndef BW_CXX_NO_ARRAY
	void reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0 = nullptr);
#endif

	void process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples);

#ifndef BW_CXX_NO_ARRAY
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);
#endif

	void setRatio(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_sr_reduce_coeffs			coeffs;
	bw_sr_reduce_state			states[N_CHANNELS];
	bw_sr_reduce_state * BW_RESTRICT	statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline SRReduce<N_CHANNELS>::SRReduce() {
	bw_sr_reduce_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void SRReduce<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_sr_reduce_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void SRReduce<N_CHANNELS>::reset(
		float               x0,
		float * BW_RESTRICT y0) {
	bw_sr_reduce_reset_coeffs(&coeffs);
	if (y0 != nullptr)
		for (size_t i = 0; i < N_CHANNELS; i++)
			y0[i] = bw_sr_reduce_reset_state(&coeffs, states + i, x0);
	else
		for (size_t i = 0; i < N_CHANNELS; i++)
			bw_sr_reduce_reset_state(&coeffs, states + i, x0);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void SRReduce<N_CHANNELS>::reset(
		float                                       x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0, y0 != nullptr ? y0->data() : nullptr);
}
#endif

template<size_t N_CHANNELS>
inline void SRReduce<N_CHANNELS>::reset(
		const float * x0,
		float *       y0) {
	bw_sr_reduce_reset_coeffs(&coeffs);
	bw_sr_reduce_reset_state_multi(&coeffs, statesP, x0, y0, N_CHANNELS);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void SRReduce<N_CHANNELS>::reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0) {
	reset(x0.data(), y0 != nullptr ? y0->data() : nullptr);
}
#endif

template<size_t N_CHANNELS>
inline void SRReduce<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_sr_reduce_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void SRReduce<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}
#endif

template<size_t N_CHANNELS>
inline void SRReduce<N_CHANNELS>::setRatio(float value) {
	bw_sr_reduce_set_ratio(&coeffs, value);
}

}
#endif

#endif
