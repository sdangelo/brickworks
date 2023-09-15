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
 *  requires {{{
 *    bw_common bw_env_follow bw_gain bw_math bw_one_pole
 *  }}}
 *  description {{{
 *    Feedforward compressor/limiter with independent sidechain input.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_comp_reset_state_multi()</code> and updated C++
 *              API in this regard.</li>
 *          <li>Now <code>bw_comp_reset_state()</code> returns the initial
 *              output value.</li>
 *          <li>Added overloaded C++ <code>reset()</code> functions taking
 *              arrays as arguments.</li>
 *          <li><code>bw_comp_process()</code> and
 *              <code>bw_comp_process_multi()</code> now use <code>size_t</code>
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
 *          <li>Added <code>bw_comp_process_multi()</code>.</li>
 *          <li>Fixed typo in <code>bw_comp_set_ratio()</code>
 *              documentation.</li>
 *          <li>Added C++ wrapper.</li>
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

#ifndef BW_COMP_H
#define BW_COMP_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_comp_coeffs
 *  ```>>> */
typedef struct bw_comp_coeffs bw_comp_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_comp_state
 *  ```>>> */
typedef struct bw_comp_state bw_comp_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_comp_init()
 *  ```>>> */
static inline void bw_comp_init(
	bw_comp_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_comp_set_sample_rate()
 *  ```>>> */
static inline void bw_comp_set_sample_rate(
	bw_comp_coeffs * BW_RESTRICT coeffs,
	float                        sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_comp_reset_coeffs()
 *  ```>>> */
static inline void bw_comp_reset_coeffs(
	bw_comp_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_comp_reset_state()
 *  ```>>> */
static inline float bw_comp_reset_state(
	const bw_comp_coeffs * BW_RESTRICT coeffs,
	bw_comp_state * BW_RESTRICT        state,
	float                              x_0,
	float                              x_sc_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the initial input value `x_0` and sidechain input value `x_sc_0`.
 *
 *    Returns the corresponding initial output value.
 *
 *    #### bw_comp_reset_state_multi()
 *  ```>>> */
static inline void bw_comp_reset_state_multi(
	const bw_comp_coeffs * BW_RESTRICT              coeffs,
	bw_comp_state * BW_RESTRICT const * BW_RESTRICT state,
	const float *                                   x_0,
	const float *                                   x_sc_0,
	float *                                         y_0,
	size_t                                          n_channels);
/*! <<<```
 *    Resets each of the `n_channels` `state`s to its initial values using the
 *    given `coeffs` and the corresponding initial input value in the `x_0`
 *    array and sidechain input value in the `x_sc_0` array.
 *
 *    The corresponding initial output values are written into the `y_0` array,
 *    if not `NULL`.
 *
 *    #### bw_comp_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_comp_update_coeffs_ctrl(
	bw_comp_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_comp_update_coeffs_audio()
 *  ```>>> */
static inline void bw_comp_update_coeffs_audio(
	bw_comp_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_comp_process1()
 *  ```>>> */
static inline float bw_comp_process1(
	const bw_comp_coeffs * BW_RESTRICT coeffs,
	bw_comp_state * BW_RESTRICT        state,
	float                              x,
	float                              x_sc);
/*! <<<```
 *    Processes one input sample `x` and the corresponding sidechain input
 *    sample `x_sc` using `coeffs`, while using and updating `state`. Returns
 *    the corresponding output sample.
 *
 *    #### bw_comp_process()
 *  ```>>> */
static inline void bw_comp_process(
	bw_comp_coeffs * BW_RESTRICT coeffs,
	bw_comp_state * BW_RESTRICT  state,
	const float *                x,
	const float *                x_sc,
	float *                      y,
	size_t                       n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and the first
 *    `n_samples` of the sidechain input buffer `x_sc`, and fills the first
 *    `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_comp_process_multi()
 *  ```>>> */
static inline void bw_comp_process_multi(
	bw_comp_coeffs * BW_RESTRICT                    coeffs,
	bw_comp_state * BW_RESTRICT const * BW_RESTRICT state,
	const float * const *                           x,
	const float * const *                           x_sc,
	float * const *                                 y,
	size_t                                          n_channels,
	size_t                                          n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    the first `n_samples` of the `n_channels` sidechain input buffers `x_sc`,
 *    and fills the first `n_samples` of the `n_channels` output buffers `y`,
 *    while using and updating both the common `coeffs` and each of the
 *    `n_channels` `state`s (control and audio rate).
 *
 *    #### bw_comp_set_thresh_lin()
 *  ```>>> */
static inline void bw_comp_set_thresh_lin(
	bw_comp_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the threshold `value` (linear) in `coeffs`.
 *
 *    Valid range: [`1e-20f`, `1e20f`].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_comp_set_thresh_dBFS()
 *  ```>>> */
static inline void bw_comp_set_thresh_dBFS(
	bw_comp_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the threshold `value` (dBFS) in `coeffs`.
 *
 *    Valid range: [`-400.f`, `400.f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comp_set_ratio()
 *  ```>>> */
static inline void bw_comp_set_ratio(
	bw_comp_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the compression ratio `value` in `coeffs`.
 *
 *    `value` is actually the slope of the gain curve above the threshold,
 *    hence `1.f` means no compression and `0.f` is a hard limit.
 *
 *    Valid range: [`0.f`, `1.f`].
 *
 *    Default value: `1.f`.
 *
 *    #### bw_comp_set_attack_tau()
 *  ```>>> */
static inline void bw_comp_set_attack_tau(
	bw_comp_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the attack time constant `value` (s) in `coeffs`.
 *
 *    `value` must be non-negative.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comp_set_release_tau()
 *  ```>>> */
static inline void bw_comp_set_release_tau(
	bw_comp_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the release time constant `value` (s) in `coeffs`.
 *
 *    `value` must be non-negative.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_comp_set_gain_lin()
 *  ```>>> */
static inline void bw_comp_set_gain_lin(
	bw_comp_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the output makeup gain `value` (linear ratio) in `coeffs`.
 *
 *    `value` must be finite.
 *
 *    Default value: `1.f`.
 *
 *    #### bw_comp_set_gain_dB()
 *  ```>>> */
static inline void bw_comp_set_gain_dB(
	bw_comp_coeffs * BW_RESTRICT coeffs,
	float                        value);
/*! <<<```
 *    Sets the output makeup gain `value` (dB) in `coeffs`.
 *
 *    `value` must be less than or equal to `770.630f`.
 *
 *    Default value: `0.f`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_env_follow.h>
#include <bw_gain.h>
#include <bw_one_pole.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BW_DEBUG_DEEP
enum bw_comp_coeffs_state {
	bw_comp_coeffs_state_invalid,
	bw_comp_coeffs_state_init,
	bw_comp_coeffs_state_set_sample_rate,
	bw_comp_coeffs_state_reset_coeffs
};
#endif

struct bw_comp_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_comp_coeffs_state	state;
	uint32_t			reset_id;
#endif

	// Sub-components
	bw_env_follow_coeffs		env_follow_coeffs;
	bw_gain_coeffs			gain_coeffs;
	bw_one_pole_coeffs		smooth_coeffs;
	bw_one_pole_state		smooth_thresh_state;
	bw_one_pole_state		smooth_ratio_state;
	
	// Coefficients
	float				kc;

	// Parameters
	float				thresh;
	float				ratio;
};

struct bw_comp_state {
#ifdef BW_DEBUG_DEEP
	uint32_t		hash;
	uint32_t		coeffs_reset_id;
#endif

	// Sub-components
	bw_env_follow_state	env_follow_state;
};

static inline void bw_comp_init(
		bw_comp_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	bw_env_follow_init(&coeffs->env_follow_coeffs);
	bw_gain_init(&coeffs->gain_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.05f);
	coeffs->thresh = 1.f;
	coeffs->ratio = 1.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_comp_coeffs");
	coeffs->state = bw_comp_coeffs_state_init;
	coeffs->reset_id = coeffs->hash + 1;
#endif
	BW_ASSERT_DEEP(bw_comp_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_comp_coeffs_state_init);
}

static inline void bw_comp_set_sample_rate(
		bw_comp_coeffs * BW_RESTRICT coeffs,
		float                        sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comp_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comp_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_env_follow_set_sample_rate(&coeffs->env_follow_coeffs, sample_rate);
	bw_gain_set_sample_rate(&coeffs->gain_coeffs, sample_rate);
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_comp_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_comp_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_comp_coeffs_state_set_sample_rate);
}

static inline void bw_comp_reset_coeffs(
		bw_comp_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_comp_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_comp_coeffs_state_set_sample_rate);

	bw_env_follow_reset_coeffs(&coeffs->env_follow_coeffs);
	bw_gain_reset_coeffs(&coeffs->gain_coeffs);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_thresh_state, coeffs->thresh);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_ratio_state, coeffs->ratio);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_comp_coeffs_state_reset_coeffs;
	coeffs->reset_id++;
#endif
	BW_ASSERT_DEEP(bw_comp_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_comp_coeffs_state_reset_coeffs);
}

static inline float bw_comp_reset_state(
		const bw_comp_coeffs * BW_RESTRICT coeffs,
		bw_comp_state * BW_RESTRICT state,
		float x_0,
		float x_sc_0) {
	(void)x_0;
	bw_env_follow_reset_state(&coeffs->env_follow_coeffs, &state->env_follow_state, x_sc_0);
}

static inline void bw_comp_update_coeffs_ctrl(bw_comp_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_ctrl(&coeffs->env_follow_coeffs);
	bw_gain_update_coeffs_ctrl(&coeffs->gain_coeffs);
}

static inline void bw_comp_update_coeffs_audio(bw_comp_coeffs *BW_RESTRICT coeffs) {
	bw_env_follow_update_coeffs_audio(&coeffs->env_follow_coeffs);
	bw_gain_update_coeffs_audio(&coeffs->gain_coeffs);
	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_thresh_state, coeffs->thresh);
	coeffs->kc = 1.f - bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_ratio_state, coeffs->ratio);
}

static inline float bw_comp_process1(const bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state, float x, float x_sc) {
	const float env = bw_env_follow_process1(&coeffs->env_follow_coeffs, &state->env_follow_state, x_sc);
	const float thresh = bw_one_pole_get_y_z1(&coeffs->smooth_thresh_state);
	float y;
	if (env > thresh) {
		float v = thresh * bw_rcpf(env);
		if (v >= 1.175494350822287e-38f) {
			v = coeffs->kc * bw_log2f(v);
			y = v > -126.f ? bw_pow2f(v) * x: thresh;
		} else
			y = thresh;
	} else
		y = x;
	y = bw_gain_process1(&coeffs->gain_coeffs, y);
	return y;
}

static inline void bw_comp_process(bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT state, const float *x, const float *x_sc, float *y, size_t n_samples) {
	bw_comp_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_comp_update_coeffs_audio(coeffs);
		y[i] = bw_comp_process1(coeffs, state, x[i], x_sc[i]);
	}
}

static inline void bw_comp_process_multi(bw_comp_coeffs *BW_RESTRICT coeffs, bw_comp_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, const float * const *x_sc, float * const *y, size_t n_channels, size_t n_samples) {
	bw_comp_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++) {
		bw_comp_update_coeffs_audio(coeffs);
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_comp_process1(coeffs, state[j], x[j][i], x_sc[j][i]);
	}
}

static inline void bw_comp_set_thresh_lin(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->thresh = value;
}

static inline void bw_comp_set_thresh_dBFS(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->thresh = bw_dB2linf(value);
}

static inline void bw_comp_set_ratio(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->ratio = value;
}

static inline void bw_comp_set_attack_tau(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	bw_env_follow_set_attack_tau(&coeffs->env_follow_coeffs, value);
}

static inline void bw_comp_set_release_tau(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	bw_env_follow_set_release_tau(&coeffs->env_follow_coeffs, value);
}

static inline void bw_comp_set_gain_lin(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_lin(&coeffs->gain_coeffs, value);
}

static inline void bw_comp_set_gain_dB(bw_comp_coeffs *BW_RESTRICT coeffs, float value) {
	bw_gain_set_gain_dB(&coeffs->gain_coeffs, value);
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Comp
 *  ```>>> */
template<size_t N_CHANNELS>
class Comp {
public:
	Comp();

	void setSampleRate(float sampleRate);
	void reset(
		float x_0 = 0.f,
		float x_sc_0 = 0.f);
	void reset(
		const float *x_0,
		const float *x_sc_0);
	void reset(
		const std::array<float, N_CHANNELS> x_0,
		const std::array<float, N_CHANNELS> x_sc_0);
	void process(
		const float * const *x,
		const float * const *x_sc,
		float * const *y,
		size_t nSamples);
	void process(
		const std::array<const float *, N_CHANNELS> x,
		const std::array<const float *, N_CHANNELS> x_sc,
		const std::array<float *, N_CHANNELS> y,
		size_t nSamples);

	void setTreshLin(float value);
	void setTreshDBFS(float value);
	void setRatio(float value);
	void setAttackTau(float value);
	void setReleaseTau(float value);
	void setGainLin(float value);
	void setGainDB(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_comp_coeffs	 coeffs;
	bw_comp_state	 states[N_CHANNELS];
	bw_comp_state	*BW_RESTRICT statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Comp<N_CHANNELS>::Comp() {
	bw_comp_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_comp_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::reset(
		float x_0,
		float x_sc_0) {
	bw_comp_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_comp_reset_state(&coeffs, states + i, x_0, x_sc_0);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::reset(
		const float *x_0,
		const float *x_sc_0) {
	bw_comp_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_comp_reset_state(&coeffs, states + i, x_0[i], x_sc_0[i]);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::reset(
		const std::array<float, N_CHANNELS> x_0,
		const std::array<float, N_CHANNELS> x_sc_0) {
	reset(x_0.data(), x_sc_0.data());
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::process(
		const float * const *x,
		const float * const *x_sc,
		float * const *y,
		size_t nSamples) {
	bw_comp_process_multi(&coeffs, statesP, x, x_sc, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::process(
		const std::array<const float *, N_CHANNELS> x,
		const std::array<const float *, N_CHANNELS> x_sc,
		const std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), x_sc.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::setTreshLin(float value) {
	bw_comp_set_thresh_lin(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::setTreshDBFS(float value) {
	bw_comp_set_thresh_dBFS(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::setRatio(float value) {
	bw_comp_set_ratio(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::setAttackTau(float value) {
	bw_comp_set_attack_tau(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::setReleaseTau(float value) {
	bw_comp_set_release_tau(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::setGainLin(float value) {
	bw_comp_set_gain_lin(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Comp<N_CHANNELS>::setGainDB(float value) {
	bw_comp_set_gain_dB(&coeffs, value);
}

}
#endif

#endif
