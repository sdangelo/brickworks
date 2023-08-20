/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
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
 *  requires {{{ bw_buf bw_common bw_math }}}
 *  description {{{
 *    Interpolated delay line, not smoothed.
 *
 *    You can either use the usual API for updating coefficients and processing
 *    signals or you can directly write and read from the delay line which,
 *    for example, allows you to implement smoothing and multi-tap output.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added initial value argument in
 *              <code>bw_delay_reset_state()</code>.</li>
 *          <li>Added overladed C++ <code>reset()</code> functions taking arrays
 *              as arguments.</li>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *          <li><code>bw_delay_process()</code> and
 *              <code>bw_delay_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> and <code>BW_RESTRICT</code>
 *              specifiers to input arguments and implementation.</li>
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
 *          <li>Added <code>bw_delay_process_multi()</code>.</li>
 *          <li>Updated mem_req/set API.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_DELAY_H
#define BW_DELAY_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_delay_coeffs
 *  ```>>> */
typedef struct bw_delay_coeffs bw_delay_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_delay_state
 *  ```>>> */
typedef struct bw_delay_state bw_delay_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_delay_init()
 *  ```>>> */
static inline void bw_delay_init(bw_delay_coeffs *BW_RESTRICT coeffs, float max_delay);
/*! <<<```
 *    Initializes input parameter values in `coeffs` using `max_delay` (s) as
 *    the maximum delay time.
 *
 *    #### bw_delay_set_sample_rate()
 *  ```>>> */
static inline void bw_delay_set_sample_rate(bw_delay_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_delay_mem_req()
 *  ```>>> */
static inline size_t bw_delay_mem_req(const bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the size, in bytes, of contiguous memory to be supplied to
 *    `bw_delay_mem_set()` using `coeffs`.
 *
 *    #### bw_delay_mem_set()
 *  ```>>> */
static inline void bw_delay_mem_set(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, void *BW_RESTRICT mem);
/*! <<<```
 *    Associates the contiguous memory block `mem` to the given `state`.
 *
 *    #### bw_delay_reset_coeffs()
 *  ```>>> */
static inline void bw_delay_reset_coeffs(bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_delay_reset_state()
 *  ```>>> */
static inline void bw_delay_reset_state(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x_0`.
 *
 *    #### bw_delay_read()
 *  ```>>> */
static float bw_delay_read(const bw_delay_coeffs *BW_RESTRICT coeffs, const bw_delay_state *BW_RESTRICT state, size_t di, float df);
/*! <<<```
 *    Returns the interpolated value read from the delay line identified by
 *    `coeffs` and `state` by applying a delay of `di` + `df` samples.
 *
 *    `df` must be in [`0.f`, `1.f`) and `di` + `df` must not exceed the delay
 *    line length (maximum delay times the sample rate).
 *
 *    #### bw_delay_write()
 *  ```>>> */
static void bw_delay_write(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Pushes the new sample `x` on the delay line identified by `coeffs` and
 *    `state`.
 *
 *    #### bw_delay_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_delay_update_coeffs_ctrl(bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_delay_update_coeffs_audio()
 *  ```>>> */
static inline void bw_delay_update_coeffs_audio(bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_delay_process1()
 *  ```>>> */
static inline float bw_delay_process1(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_delay_process()
 *  ```>>> */
static inline void bw_delay_process(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_delay_process_multi()
 *  ```>>> */
static inline void bw_delay_process_multi(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_delay_set_delay()
 *  ```>>> */
static inline void bw_delay_set_delay(bw_delay_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the delay time `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_delay_get_length()
 *  ```>>> */
static inline size_t bw_delay_get_length(const bw_delay_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Returns the length of the delay line in samples.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_buf.h>
#include <bw_math.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bw_delay_coeffs {
	// Coefficients
	float	fs;
	size_t	len;

	size_t	di;
	float	df;

	// Parameters
	float	max_delay;
	float	delay;
	char	delay_changed;
};

struct bw_delay_state {
	float	*BW_RESTRICT buf;
	size_t	 idx;
};

static inline void bw_delay_init(bw_delay_coeffs *BW_RESTRICT coeffs, float max_delay) {
	coeffs->max_delay = max_delay;
	coeffs->delay = 0.f;
}

static inline void bw_delay_set_sample_rate(bw_delay_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	coeffs->fs = sample_rate;
	coeffs->len = (size_t)bw_ceilf(coeffs->fs * coeffs->max_delay) + 1;
}

static inline size_t bw_delay_mem_req(const bw_delay_coeffs *BW_RESTRICT coeffs) {
	return coeffs->len * sizeof(float);
}

static inline void bw_delay_mem_set(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, void *BW_RESTRICT mem) {
	(void)coeffs;
	state->buf = (float *)mem;
}

static inline void bw_delay_reset_coeffs(bw_delay_coeffs *BW_RESTRICT coeffs) {
	coeffs->delay_changed = 1;
	bw_delay_update_coeffs_ctrl(coeffs);
}

static inline void bw_delay_reset_state(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x_0) {
	bw_buf_fill(x_0, state->buf, coeffs->len);
	state->idx = 0;
}

static float bw_delay_read(const bw_delay_coeffs *BW_RESTRICT coeffs, const bw_delay_state *BW_RESTRICT state, size_t di, float df) {
	const size_t n = (state->idx + (state->idx >= di ? 0 : coeffs->len)) - di;
	const size_t p = (n ? n : coeffs->len) - 1;
	return state->buf[n] + df * (state->buf[p] - state->buf[n]);
}

static void bw_delay_write(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x) {
	state->idx++;
	state->idx = state->idx == coeffs->len ? 0 : state->idx;
	state->buf[state->idx] = x;
}

static inline void bw_delay_update_coeffs_ctrl(bw_delay_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->delay_changed) {
		float i;
		bw_intfracf(coeffs->fs * coeffs->delay, &i, &coeffs->df);
		coeffs->di = (size_t)i;
		coeffs->delay_changed = 0;
	}
}

static inline void bw_delay_update_coeffs_audio(bw_delay_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline float bw_delay_process1(const bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, float x) {
	bw_delay_write(coeffs, state, x);
	return bw_delay_read(coeffs, state, coeffs->di, coeffs->df);
}

static inline void bw_delay_process(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT state, const float *x, float *y, size_t n_samples) {
	bw_delay_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++)
		y[i] = bw_delay_process1(coeffs, state, x[i]);
}

static inline void bw_delay_process_multi(bw_delay_coeffs *BW_RESTRICT coeffs, bw_delay_state *BW_RESTRICT const *BW_RESTRICT state, const float * const *x, float * const *y, size_t n_channels, size_t n_samples) {
	bw_delay_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++)
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_delay_process1(coeffs, state[j], x[j][i]);
}

static inline void bw_delay_set_delay(bw_delay_coeffs *BW_RESTRICT coeffs, float value) {
	if (value != coeffs->delay) {
		coeffs->delay = value;
		coeffs->delay_changed = 1;
	}
}

static inline size_t bw_delay_get_length(const bw_delay_coeffs *BW_RESTRICT coeffs) {
	return coeffs->len;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::Delay
 *  ```>>> */
template<size_t N_CHANNELS>
class Delay {
public:
	Delay(float maxDelay = 1.f);
	~Delay();

	void setSampleRate(float sampleRate);
	void reset(float x_0 = 0.f);
	void reset(const float *BW_RESTRICT x_0);
	void reset(const std::array<float, N_CHANNELS> x_0);
	void process(
		const float * const *x,
		float * const *y,
		size_t nSamples);
	void process(
		const std::array<const float *, N_CHANNELS> x,
		const std::array<float *, N_CHANNELS> y,
		size_t nSamples);
		
	float read(size_t channel, size_t di, float df);
	void write(size_t channel, float x);

	void setDelay(float value);
	
	size_t getLength();
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_delay_coeffs	 coeffs;
	bw_delay_state	 states[N_CHANNELS];
	bw_delay_state	*BW_RESTRICT statesP[N_CHANNELS];
	void		*BW_RESTRICT mem;
};

template<size_t N_CHANNELS>
inline Delay<N_CHANNELS>::Delay(float maxDelay) {
	bw_delay_init(&coeffs, maxDelay);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
	mem = nullptr;
}

template<size_t N_CHANNELS>
inline Delay<N_CHANNELS>::~Delay() {
	if (mem != nullptr)
		operator delete(mem);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_delay_set_sample_rate(&coeffs, sampleRate);
	size_t req = bw_delay_mem_req(&coeffs);
	if (mem != nullptr)
		operator delete(mem);
	mem = operator new(req * N_CHANNELS);
	void *m = mem;
	for (size_t i = 0; i < N_CHANNELS; i++, m = static_cast<char *>(m) + req)
		bw_delay_mem_set(&coeffs, states + i, m);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::reset(float x_0) {
	bw_delay_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_delay_reset_state(&coeffs, states + i, x_0);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::reset(const float *BW_RESTRICT x_0) {
	bw_delay_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_delay_reset_state(&coeffs, states + i, x_0[i]);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::reset(const std::array<float, N_CHANNELS> x_0) {
	reset(x_0.data());
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::process(
		const float * const *x,
		float * const *y,
		size_t nSamples) {
	bw_delay_process_multi(&coeffs, statesP, x, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::process(
		const std::array<const float *, N_CHANNELS> x,
		const std::array<float *, N_CHANNELS> y,
		size_t nSamples) {
	process(x.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline float Delay<N_CHANNELS>::read(size_t channel, size_t di, float df) {
	return bw_delay_read(&coeffs, states + channel, di, df);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::write(size_t channel, float x) {
	bw_delay_write(&coeffs, states + channel, x);
}

template<size_t N_CHANNELS>
inline void Delay<N_CHANNELS>::setDelay(float value) {
	bw_delay_set_delay(&coeffs, value);
}

template<size_t N_CHANNELS>
inline size_t Delay<N_CHANNELS>::getLength() {
	return bw_delay_get_length(&coeffs);
}

}
#endif

#endif
