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
 *  version {{{ 1.2.0 }}}
 *  requires {{{ bw_common bw_math }}}
 *  description {{{
 *    Bit depth reducer with input gate.
 *
 *    This is purely an audio effect, it doesn't actually produce an output
 *    signal with a different encoding. The algorithm is deliberately crude to
 *    obtain the characteristic noise due to lo-fi A/D quantization.
 *
 *    It 
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.2.0</strong>:
 *        <ul>
 *          <li>Added gate parameter.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.1.0</strong>:
 *        <ul>
 *          <li>Added silence_dc parameter.</li>
 *          <li>Now using <code>BW_NULL</code> and
 *              <code>BW_CXX_NO_ARRAY</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_bd_reduce_set_sample_rate()</code>.</li>
 *          <li><code>bw_bd_reduce_process()</code> and
 *              <code>bw_bd_reduce_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
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
 *          <li>Added <code>bw_bd_reduce_process_multi()</code>.</li>
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

#ifndef BW_BD_REDUCE_H
#define BW_BD_REDUCE_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_bd_reduce_coeffs
 *  ```>>> */
typedef struct bw_bd_reduce_coeffs bw_bd_reduce_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_bd_reduce_init()
 *  ```>>> */
static inline void bw_bd_reduce_init(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_bw_reduce_set_sample_rate()
 *  ```>>> */
static inline void bw_bd_reduce_set_sample_rate(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
	float                             sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_bd_reduce_reset_coeffs()
 *  ```>>> */
static inline void bw_bd_reduce_reset_coeffs(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_bd_reduce_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_bd_reduce_update_coeffs_ctrl(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_bd_reduce_update_coeffs_audio()
 *  ```>>> */
static inline void bw_bd_reduce_update_coeffs_audio(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_bd_reduce_process1()
 *  ```>>> */
static inline float bw_bd_reduce_process1(
	const bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
	float                                   x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`. Returns the corresponding
 *    output sample.
 *
 *    #### bw_bd_reduce_process()
 *  ```>>> */
static inline void bw_bd_reduce_process(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
	const float *                     x,
	float *                           y,
	size_t                            n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating
 *    `coeffs` (control and audio rate).
 *
 *    #### bw_bd_reduce_process_multi()
 *  ```>>> */
static inline void bw_bd_reduce_process_multi(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
	const float * const *             x,
	float * const *                   y,
	size_t                            n_channels,
	size_t                            n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating the common `coeffs` (control and audio rate).
 *
 *    #### bw_bd_reduce_set_bit_depth()
 *  ```>>> */
static inline void bw_bd_reduce_set_bit_depth(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
	char                              value);
/*! <<<```
 *    Sets the output bit depth `value` in `coeffs`.
 *
 *    Valid range: [`1`, `64`].
 *
 *    Default value: `16`.
 *
 *    #### bw_bd_reduce_set_silence_dc()
 *  ```>>> */
static inline void bw_bd_reduce_set_silence_dc(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
	char                              value);
/*! <<<```
 *    Sets whether the output value corresponding to silence has null dc
 *    (`value` `0`) or not (non-`0`) in `coeffs`.
 *
 *    Default value: non-`0` (non-null dc).
 *
 *    #### bw_bd_reduce_set_gate_lin()
 *  ```>>> */
static inline void bw_bd_reduce_set_gate_lin(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
	float                             value);
/*! <<<```
 *    Sets the input gate threshold `value` (linear) in `coeffs`.
 *
 *    Valid range: [`0.f`, `1.f`].
 *
 *    Default value: `0.f`.
 *
 *    #### bw_bd_reduce_set_gate_dBFS()
 *  ```>>> */
static inline void bw_bd_reduce_set_gate_dBFS(
	bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
	float                             value);
/*! <<<```
 *    Sets the input gate threshold `value` (dBFS) in `coeffs`.
 *
 *    Valid range: [`-INFINITY`, `0.f`].
 *
 *    Default value: `-INFINITY`.
 *
 *    #### bw_bd_reduce_coeffs_is_valid()
 *  ```>>> */
static inline char bw_bd_reduce_coeffs_is_valid(
	const bw_bd_reduce_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_bd_reduce_coeffs`.
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
enum bw_bd_reduce_coeffs_state {
	bw_bd_reduce_coeffs_state_invalid,
	bw_bd_reduce_coeffs_state_init,
	bw_bd_reduce_coeffs_state_set_sample_rate,
	bw_bd_reduce_coeffs_state_reset_coeffs
};
#endif

struct bw_bd_reduce_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_bd_reduce_coeffs_state	state;
#endif

	// Coefficients
	float				ki;
	float				k;
	float				ko;
	float				max;
	float				gate;
	
	// Parameters
	char				bit_depth;
	char				bit_depth_prev;
};

static inline void bw_bd_reduce_init(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	coeffs->bit_depth = 16;
	coeffs->ko = 0.5f;
	coeffs->gate = 0.f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_bd_reduce_coeffs");
	coeffs->state = bw_bd_reduce_coeffs_state_init;
#endif
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_bd_reduce_coeffs_state_init);
}

static inline void bw_bd_reduce_set_sample_rate(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
		float                             sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	(void)coeffs;
	(void)sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_bd_reduce_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_bd_reduce_coeffs_state_set_sample_rate);
}

static inline void bw_bd_reduce_do_update_coeffs_ctrl(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs) {
	if (coeffs->bit_depth_prev != coeffs->bit_depth) {
		coeffs->k = bw_pow2f(coeffs->bit_depth - 1);
		coeffs->ki = bw_rcpf(coeffs->k);
		coeffs->max = 1.f - 0.5f * coeffs->ki;
		coeffs->bit_depth_prev = coeffs->bit_depth;
	}
}

static inline void bw_bd_reduce_reset_coeffs(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_set_sample_rate);

	coeffs->bit_depth_prev = 0;
	bw_bd_reduce_do_update_coeffs_ctrl(coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_bd_reduce_coeffs_state_reset_coeffs;
#endif
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_bd_reduce_coeffs_state_reset_coeffs);
}

static inline void bw_bd_reduce_update_coeffs_ctrl(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs);

	bw_bd_reduce_do_update_coeffs_ctrl(coeffs);

	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs);
}

static inline void bw_bd_reduce_update_coeffs_audio(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline float bw_bd_reduce_process1(
		const bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
		float                                   x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(x));

	x = bw_absf(x) < coeffs->gate ? 0.f : x;
	const float y = coeffs->ki * (bw_floorf(coeffs->k * bw_clipf(x, -coeffs->max, coeffs->max)) + coeffs->ko);

	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

static inline void bw_bd_reduce_process(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
		const float *                     x,
		float *                           y,
		size_t                            n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != BW_NULL);

	bw_bd_reduce_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++)
		y[i] = bw_bd_reduce_process1(coeffs, x[i]);

	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_bd_reduce_process_multi(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
		const float * const *             x,
		float * const *                   y,
		size_t                            n_channels,
		size_t                            n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs);
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT(y != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(y[i] != y[j]);
#endif

	bw_bd_reduce_update_coeffs_ctrl(coeffs);
	for (size_t i = 0; i < n_samples; i++)
		for (size_t j = 0; j < n_channels; j++)
			y[j][i] = bw_bd_reduce_process1(coeffs, x[j][i]);

	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs);
}

static inline void bw_bd_reduce_set_bit_depth(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
		char                              value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_init);
	BW_ASSERT(value >= 1 && value <= 64);

	coeffs->bit_depth = value;

	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_init);
}

static inline void bw_bd_reduce_set_silence_dc(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
		char                              value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_init);

	coeffs->ko = value ? 0.5f : 0.f;

	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_init);
}

static inline void bw_bd_reduce_set_gate_lin(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
		float                             value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 1.f);

	coeffs->gate = value;

	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_init);
}

static inline void bw_bd_reduce_set_gate_dBFS(
		bw_bd_reduce_coeffs * BW_RESTRICT coeffs,
		float                             value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value <= 0.f);

	coeffs->gate = bw_dB2linf(value);

	BW_ASSERT_DEEP(bw_bd_reduce_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_bd_reduce_coeffs_state_init);
}

static inline char bw_bd_reduce_coeffs_is_valid(
		const bw_bd_reduce_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_bd_reduce_coeffs"))
		return 0;
	if (coeffs->state < bw_bd_reduce_coeffs_state_init || coeffs->state > bw_bd_reduce_coeffs_state_reset_coeffs)
		return 0;
#endif

	if (coeffs->bit_depth < 1 || coeffs->bit_depth > 64)
		return 0;

#ifdef BW_DEBUG_DEEP
	if (coeffs->state >= bw_bd_reduce_coeffs_state_reset_coeffs) {
		if (coeffs->bit_depth_prev < 1 || coeffs->bit_depth_prev > 64)
			return 0;
		if (!bw_is_finite(coeffs->ki) || coeffs->ki <= 0.f)
			return 0;
		if (!bw_is_finite(coeffs->k) || coeffs->k <= 0.f)
			return 0;
		if (coeffs->ko != 0.f && coeffs->ko != 0.5f)
			return 0;
		if (!bw_is_finite(coeffs->max) || coeffs->max < 0.5f || coeffs->max > 1.f)
			return 0;
		if (!bw_is_finite(coeffs->gate) || coeffs->gate < 0.f || coeffs->gate > 1.f)
			return 0;
	}
#endif

	return 1;
}

#ifdef __cplusplus
}

#ifndef BW_CXX_NO_ARRAY
# include <array>
#endif

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::BDReduce
 *  ```>>> */
template<size_t N_CHANNELS>
class BDReduce {
public:
	BDReduce();

	void setSampleRate(
		float sampleRate);

	void reset();

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

	void setBitDepth(
		char value);

	void setSilenceDc(
		char value);

	void setGateLin(
		float value);

	void setGateDBFS(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_bd_reduce_coeffs	 coeffs;
};

template<size_t N_CHANNELS>
inline BDReduce<N_CHANNELS>::BDReduce() {
	bw_bd_reduce_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void BDReduce<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_bd_reduce_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void BDReduce<N_CHANNELS>::reset() {
	bw_bd_reduce_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void BDReduce<N_CHANNELS>::process(
		const float * const * x,
		float * const *       y,
		size_t                nSamples) {
	bw_bd_reduce_process_multi(&coeffs, x, y, N_CHANNELS, nSamples);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void BDReduce<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), y.data(), nSamples);
}
#endif

template<size_t N_CHANNELS>
inline void BDReduce<N_CHANNELS>::setBitDepth(
		char value) {
	bw_bd_reduce_set_bit_depth(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void BDReduce<N_CHANNELS>::setSilenceDc(
		char value) {
	bw_bd_reduce_set_silence_dc(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void BDReduce<N_CHANNELS>::setGateLin(
		float value) {
	bw_bd_reduce_set_gate_lin(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void BDReduce<N_CHANNELS>::setGateDBFS(
		float value) {
	bw_bd_reduce_set_gate_dBFS(&coeffs, value);
}

}
#endif

#endif
