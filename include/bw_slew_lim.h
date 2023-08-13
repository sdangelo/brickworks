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
 *  requires {{{ bw_common bw_math }}}
 *  description {{{
 *    Slew-rate limiter with separate maximum increasing and decreasing rates.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_slew_lim_process()</code> and
 *              <code>bw_slew_lim_process_multi()</code> now use
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
 *          <li>Added <code>bw_slew_lim_process_multi()</code>.</li>
 *          <li>Fixed documentation of <code>bw_slew_lim_process()</code>.</li>
 *          <li>Fixed unused parameter warnings.</li>
 *          <li>Added C++ wrapper.</li>
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

#ifndef BW_SLEW_LIM_H
#define BW_SLEW_LIM_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_slew_lim_coeffs
 *  ```>>> */
typedef struct bw_slew_lim_coeffs bw_slew_lim_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_slew_lim_state
 *  ```>>> */
typedef struct bw_slew_lim_state bw_slew_lim_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_slew_lim_init()
 *  ```>>> */
static inline void bw_slew_lim_init(bw_slew_lim_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_slew_lim_set_sample_rate()
 *  ```>>> */
static inline void bw_slew_lim_set_sample_rate(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_slew_lim_reset_coeffs()
 *  ```>>> */
static inline void bw_slew_lim_reset_coeffs(bw_slew_lim_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_slew_lim_reset_state()
 *  ```>>> */
static inline void bw_slew_lim_reset_state(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float y_z1);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/equilibrium value `y_z1`.
 *
 *    #### bw_slew_lim_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_slew_lim_update_coeffs_ctrl(bw_slew_lim_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_one_pole_update_coeffs_audio()
 *  ```>>> */
static inline void bw_slew_lim_update_coeffs_audio(bw_slew_lim_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_slew_lim_process1\*()
 *  ```>>> */
static inline float bw_slew_lim_process1(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x);
static inline float bw_slew_lim_process1_up(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x);
static inline float bw_slew_lim_process1_down(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x);
/*! <<<```
 *    These function process one input sample `x` using `coeffs`, while using
 *    and updating `state`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_slew_lim_process1()` assumes that both the maximum upgoing and
 *       downgoing variation rates are finite;
 *     * `bw_slew_lim_process1_up()` assumes that both the maximum upgoing
 *       variation rate is finite and the maximum downgoing variation rate is
 *       infinite;
 *     * `bw_slew_lim_process1_down()` assumes that both the maximum upgoing
 *       variation rate is infinite and the maximum downgoing variation rate is
 *       finite.
 *
 *    #### bw_slew_lim_process()
 *  ```>>> */
static inline void bw_slew_lim_process(bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    `y` may be `NULL`.
 * 
 *    #### bw_slew_lim_process_multi()
 *  ```>>> */
static inline void bw_slew_lim_process_multi(bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state * const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    `y` or any element of `y` may be `NULL`.
 *
 *    #### bw_slew_lim_set_max_rate()
 *  ```>>> */
static inline void bw_slew_lim_set_max_rate(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets both the maximum increasing and decreasing variation rate to the
 *    given `value` (1/s) in `coeffs`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    This is equivalent to calling both `bw_slew_lim_set_max_inc_rate()` and
 *    `bw_slew_lim_set_max_dec_rate()` with same `coeffs` and `value`.
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_max_rate_up()
 *  ```>>> */
static inline void bw_slew_lim_set_max_rate_up(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the maximum increasing variation rate to the given `value` (1/s) in
 *    `coeffs`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    Default value: `INFINITY`.
 *  >>> */

/*! ...
 *    #### bw_slew_lim_set_max_inc_rate()
 *  ```>>> */
static inline void bw_slew_lim_set_max_rate_down(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the maximum decreasing variation rate to the given `value` (1/s) in
 *    `coeffs`.
 *
 *    `value` represents the maximum variation per second and must be
 *    non-negative.
 *
 *    Default value: `INFINITY`.
 *  }}} */

static inline float bw_slew_lim_get_y_z1(const bw_slew_lim_state *BW_RESTRICT state);

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

struct bw_slew_lim_coeffs {
	// Coefficients
	float	T;

	float	max_inc;
	float	max_dec;

	// Parameters
	float	max_rate_up;
	float	max_rate_down;
};

struct bw_slew_lim_state {
	float	y_z1;
};

static inline void bw_slew_lim_init(bw_slew_lim_coeffs *BW_RESTRICT coeffs) {
	coeffs->max_inc = INFINITY;
	coeffs->max_dec = INFINITY;
}

static inline void bw_slew_lim_set_sample_rate(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->T = 1.f / sample_rate;
}

static inline void bw_slew_lim_reset_coeffs(bw_slew_lim_coeffs *BW_RESTRICT coeffs) {
	bw_slew_lim_update_coeffs_ctrl(coeffs);
}

static inline void bw_slew_lim_reset_state(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float y_z1) {
	(void)coeffs;
	state->y_z1 = y_z1;
}

static inline void bw_slew_lim_update_coeffs_ctrl(bw_slew_lim_coeffs *BW_RESTRICT coeffs) {
	// tracking parameter changes is more trouble than it's worth
	coeffs->max_inc = coeffs->T * coeffs->max_rate_up;
	coeffs->max_dec = coeffs->T * coeffs->max_rate_down;
}

static inline void bw_slew_lim_update_coeffs_audio(bw_slew_lim_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline float bw_slew_lim_process1(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x) {
	float y = bw_clipf(x, state->y_z1 - coeffs->max_dec, state->y_z1 + coeffs->max_inc);
	state->y_z1 = y;
	return y;
}

static inline float bw_slew_lim_process1_up(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x) {
	float y = bw_minf(x, state->y_z1 + coeffs->max_inc);
	state->y_z1 = y;
	return y;
}

static inline float bw_slew_lim_process1_down(const bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, float x) {
	float y = bw_maxf(x, state->y_z1 - coeffs->max_dec);
	state->y_z1 = y;
	return y;
}

static inline void bw_slew_lim_process(bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples) {
	bw_slew_lim_update_coeffs_ctrl(coeffs);
	if (y != NULL) {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t i = 0; i < n_samples; i++)
					y[i] = bw_slew_lim_process1(coeffs, state, x[i]);
			else
				for (size_t i = 0; i < n_samples; i++)
					y[i] = bw_slew_lim_process1_up(coeffs, state, x[i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t i = 0; i < n_samples; i++)
					y[i] = bw_slew_lim_process1_down(coeffs, state, x[i]);
			else {
				for (size_t i = 0; i < n_samples; i++)
					y[i] = x[i];
				state->y_z1 = x[n_samples - 1];
			}
		}
	} else {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t i = 0; i < n_samples; i++)
					bw_slew_lim_process1(coeffs, state, x[i]);
			else
				for (size_t i = 0; i < n_samples; i++)
					bw_slew_lim_process1_up(coeffs, state, x[i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t i = 0; i < n_samples; i++)
					bw_slew_lim_process1_down(coeffs, state, x[i]);
			else
				state->y_z1 = x[n_samples - 1];
		}
	}
}

static inline void bw_slew_lim_process_multi(bw_slew_lim_coeffs *BW_RESTRICT coeffs, bw_slew_lim_state * const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples) {
	bw_slew_lim_update_coeffs_ctrl(coeffs);
	if (y != NULL) {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t j = 0; j < n_channels; j++)
					if (y[j] != NULL)
						for (size_t i = 0; i < n_samples; i++)
							y[j][i] = bw_slew_lim_process1(coeffs, state[j], x[j][i]);
					else
						for (size_t i = 0; i < n_samples; i++)
							bw_slew_lim_process1(coeffs, state[j], x[j][i]);
			else
				for (size_t j = 0; j < n_channels; j++)
					for (size_t i = 0; i < n_samples; i++)
						if (y[j] != NULL)
							y[j][i] = bw_slew_lim_process1_up(coeffs, state[j], x[j][i]);
						else
							bw_slew_lim_process1_up(coeffs, state[j], x[j][i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t j = 0; j < n_channels; j++)
					if (y[j] != NULL)
						for (size_t i = 0; i < n_samples; i++)
							y[j][i] = bw_slew_lim_process1_down(coeffs, state[j], x[j][i]);
					else
						for (size_t i = 0; i < n_samples; i++)
							bw_slew_lim_process1_down(coeffs, state[j], x[j][i]);
			else
				for (size_t j = 0; j < n_channels; j++) {
					if (y[j] != NULL)
						for (size_t i = 0; i < n_samples; i++)
							y[j][i] = x[j][i];
					state[j]->y_z1 = x[j][n_samples - 1];
				}
		}
	} else {
		if (coeffs->max_rate_up != INFINITY) {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t j = 0; j < n_channels; j++)
					for (size_t i = 0; i < n_samples; i++)
						bw_slew_lim_process1(coeffs, state[j], x[j][i]);
			else
				for (size_t j = 0; j < n_channels; j++)
					for (size_t i = 0; i < n_samples; i++)
						bw_slew_lim_process1_up(coeffs, state[j], x[j][i]);
		} else {
			if (coeffs->max_rate_down != INFINITY)
				for (size_t j = 0; j < n_channels; j++)
					for (size_t i = 0; i < n_samples; i++)
						bw_slew_lim_process1_down(coeffs, state[j], x[j][i]);
			else
				for (size_t j = 0; j < n_channels; j++)
					state[j]->y_z1 = x[j][n_samples - 1];
		}
	}
}

static inline void bw_slew_lim_set_max_rate(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value) {
	bw_slew_lim_set_max_rate_up(coeffs, value);
	bw_slew_lim_set_max_rate_down(coeffs, value);
}

static inline void bw_slew_lim_set_max_rate_up(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->max_rate_up = value;
}

static inline void bw_slew_lim_set_max_rate_down(bw_slew_lim_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->max_rate_down = value;
}

static inline float bw_slew_lim_get_y_z1(const bw_slew_lim_state *BW_RESTRICT state) {
	return state->y_z1;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::SlewLim
 *  ```>>> */
template<size_t N_CHANNELS>
class SlewLim {
public:
	SlewLim();

	void setSampleRate(float sampleRate);
	void reset(float y_z1 = 0.f);
	void process(
		const float * const *x,
		float * const *y,
		size_t nSamples);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples);

	void setMaxRate(float value);
	void setMaxRateUp(float value);
	void setMaxRateDown(float value);
	
	float getYZ1(size_t channel);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_slew_lim_coeffs	 coeffs;
	bw_slew_lim_state	 states[N_CHANNELS];
	bw_slew_lim_state	*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline SlewLim<N_CHANNELS>::SlewLim() {
	bw_slew_lim_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_slew_lim_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::reset(float y_z1) {
	bw_slew_lim_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_slew_lim_reset_state(&coeffs, states + i, y_z1);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::process(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_slew_lim_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setMaxRate(float value) {
	bw_slew_lim_set_max_rate(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setMaxRateUp(float value) {
	bw_slew_lim_set_max_rate_up(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setMaxRateDown(float value) {
	bw_slew_lim_set_max_rate_down(&coeffs, value);
}

template<size_t N_CHANNELS>
inline float SlewLim<N_CHANNELS>::getYZ1(size_t channel) {
	return bw_slew_lim_get_y_z1(states + channel);
}

}
#endif

#endif
