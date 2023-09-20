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
 *    Sawtooth oscillator waveshaper with PolyBLEP antialiasing.
 *
 *    It turns a normalized phase signal, such as that geneated by
 *    [bw\_phase\_gen](bw_phase_gen), into a sawtooth wave.
 *
 *    The antialiasing algorithm is based on
 *
 *    V. Valimaki and A. Huovilainen, "Antialiasing Oscillators in Subtractive
 *    Synthesis", IEEE Signal Processing Magazine, vol. 24, no. 2, pp. 116-125,
 *    March 2007.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_osc_saw_set_sample_rate()</code>,
 *              <code>bw_osc_saw_reset_coeffs()</code>,
 *              <code>bw_osc_saw_update_coeffs_ctrl()</code>,
 *              and <code>bw_osc_saw_update_coeffs_audio()</code>.</li>
 *          <li><code>bw_osc_saw_process()</code> and
 *              <code>bw_osc_saw_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
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
 *          <li>Added <code>bw_osc_saw_process_multi()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Fixed unused parameter warnings.</li>
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

#ifndef BW_OSC_SAW_H
#define BW_OSC_SAW_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_osc_saw_coeffs
 *  ```>>> */
typedef struct bw_osc_saw_coeffs bw_osc_saw_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_osc_saw_init()
 *  ```>>> */
static inline void bw_osc_saw_init(
	bw_osc_saw_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_osc_saw_set_sample_rate()
 *  ```>>> */
static inline void bw_osc_saw_set_sample_rate(
	bw_osc_saw_coeffs * BW_RESTRICT coeffs,
	float                           sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_osc_saw_reset_coeffs()
 *  ```>>> */
static inline void bw_osc_saw_reset_coeffs(
	bw_osc_saw_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_osc_saw_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_osc_saw_update_coeffs_ctrl(
	bw_osc_saw_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_osc_saw_update_coeffs_audio()
 *  ```>>> */
static inline void bw_osc_saw_update_coeffs_audio(
	bw_osc_saw_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_osc_saw_process1\*()
 *  ```>>> */
static inline float bw_osc_saw_process1(
	const bw_osc_saw_coeffs * BW_RESTRICT coeffs,
	float                                 x);

static inline float bw_osc_saw_process1_antialias(
	const bw_osc_saw_coeffs * BW_RESTRICT coeffs,
	float                                 x,
	float                                 x_inc);
/*! <<<```
 *    These function process one input sample `x`, representing the normalized
 *    phase, using `coeffs`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_osc_saw_process1()` assumes that antialiasing is disabled;
 *     * `bw_osc_saw_process1_antialias()` assumes that antialiasing is enabled
 *       and requires the corresponding phase increment value to be passed via
 *       `x_inc`.
 *
 *    Whether antialiasing is enabled or not is unchecked even for debugging
 *    purposes.
 *
 *    `x` must be in [`0.f`, `1.f`).
 *
 *    `x_inc` must be in [`-0.5f`, `0.5f`].
 *
 *    #### bw_osc_saw_process()
 *  ```>>> */
static inline void bw_osc_saw_process(
	bw_osc_saw_coeffs * BW_RESTRICT coeffs,
	const float *                   x,
	const float *                   x_inc,
	float *                         y,
	size_t                          n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x`, containing the
 *    normalized phase signal, and fills the first `n_samples` of the output
 *    buffer `y`, while using `coeffs`.
 *
 *    If antialiasing is enabled, `x_inc` must contain phase increment values,
 *    otherwise it is ignored and can be `NULL`.
 *
 *    All samples in `x` must be in [`0.f`, `1.f`).
 *
 *    All samples is `x_inc`, if not ignored, must be in [`-0.5f`, `0.5f`].
 *
 *    #### bw_osc_saw_process_multi()
 *  ```>>> */
static inline void bw_osc_saw_process_multi(
	bw_osc_saw_coeffs * BW_RESTRICT coeffs,
	const float * const *           x,
	const float * const *           x_inc,
	float * const *                 y,
	size_t                          n_channels,
	size_t                          n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x`,
 *    containing the normalized phase signals, and fills the first `n_samples`
 *    of the `n_samples` output buffers `y`, while using `coeffs`.
 *
 *    If antialiasing is enabled, each of the `n_channels` buffers pointed by
 *    `x_inc` must contain phase increment values, otherwise `x_inc` is ignored
 *    and can be `NULL`.
 *
 *    All samples in `x` must be in [`0.f`, `1.f`).
 *
 *    All samples is `x_inc`, if not ignored, must be in [`-0.5f`, `0.5f`].
 *
 *    #### bw_osc_saw_set_antialiasing()
 *  ```>>> */
static inline void bw_osc_saw_set_antialiasing(
	bw_osc_saw_coeffs * BW_RESTRICT coeffs,
	char                            value);
/*! <<<```
 *    Sets whether the antialiasing is on (`value` non-`0`) or off (`0`) in
 *    `coeffs`.
 *
 *    Default value: `0` (off).
 *
 *    #### bw_osc_saw_coeffs_is_valid()
 *  ```>>> */
static inline char bw_osc_saw_coeffs_is_valid(
	const bw_osc_saw_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_osc_saw_coeffs`.
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
enum bw_osc_saw_coeffs_state {
	bw_osc_saw_coeffs_state_invalid,
	bw_osc_saw_coeffs_state_init,
	bw_osc_saw_coeffs_state_set_sample_rate,
	bw_osc_saw_coeffs_state_reset_coeffs
};
#endif

struct bw_osc_saw_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_osc_saw_coeffs_state	state;
#endif

	// Parameters
	char				antialiasing;
};

static inline void bw_osc_saw_init(
		bw_osc_saw_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

	coeffs->antialiasing = 0;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_osc_saw_coeffs");
	coeffs->state = bw_osc_saw_coeffs_state_init;
#endif
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_osc_saw_coeffs_state_init);
}

static inline void bw_osc_saw_set_sample_rate(
		bw_osc_saw_coeffs * BW_RESTRICT coeffs,
		float                           sample_rate) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	(void)coeffs;
	(void)sample_rate;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_osc_saw_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_osc_saw_coeffs_state_set_sample_rate);
}

static inline void bw_osc_saw_reset_coeffs(
		bw_osc_saw_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_set_sample_rate);

	(void)coeffs;

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_osc_saw_coeffs_state_reset_coeffs;
#endif
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_osc_saw_coeffs_state_reset_coeffs);
}

static inline void bw_osc_saw_update_coeffs_ctrl(
		bw_osc_saw_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_osc_saw_update_coeffs_audio(
		bw_osc_saw_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline float bw_osc_saw_process1(
		const bw_osc_saw_coeffs * BW_RESTRICT coeffs,
		float                                 x) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 0.f && x < 1.f);

	(void)coeffs;
	const float y = x + x - 1.f;

	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

// PolyBLEP residual based on Parzen window (4th-order B-spline), one-sided (x in [0, 2])
static inline float bw_osc_saw_blep_diff(
		float x) {
	return x < 1.f
		? x * ((0.25f * x - 0.6666666666666666f) * x * x + 1.333333333333333f) - 1.f
		: x * (x * ((0.6666666666666666f - 0.08333333333333333f * x) * x - 2.f) + 2.666666666666667f) - 1.333333333333333f;
}

static inline float bw_osc_saw_process1_antialias(
		const bw_osc_saw_coeffs * BW_RESTRICT coeffs,
		float                                 x,
		float                                 x_inc) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 0.f && x < 1.f);
	BW_ASSERT(bw_is_finite(x_inc));
	BW_ASSERT(x_inc >= -0.5f && x_inc <= 0.5f);

	(void)coeffs;
	const float s_1_m_phase = 1.f - x;
	float v = x - s_1_m_phase;
	const float a_inc = bw_absf(x_inc);
	if (a_inc > 1e-6f) {
		const float a_inc_2 = a_inc + a_inc;
		const float a_inc_rcp = bw_rcpf(a_inc);
		if (s_1_m_phase < a_inc_2)
			v += bw_osc_saw_blep_diff(s_1_m_phase * a_inc_rcp);
		if (x < a_inc_2)
			v -= bw_osc_saw_blep_diff(x * a_inc_rcp);
	}

	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(v));

	return v;
}

static inline void bw_osc_saw_process(
		bw_osc_saw_coeffs * BW_RESTRICT coeffs,
		const float *                   x,
		const float *                   x_inc,
		float *                         y,
		size_t                          n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);
	BW_ASSERT(x != NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(coeffs->antialiasing ? x_inc != NULL : 1);
	BW_ASSERT_DEEP(coeffs->antialiasing ? bw_has_only_finite(x_inc, n_samples) : 1);
	BW_ASSERT(y != NULL);

	if (coeffs->antialiasing)
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_osc_saw_process1_antialias(coeffs, x[i], x_inc[i]);
	else
		for (size_t i = 0; i < n_samples; i++)
			y[i] = bw_osc_saw_process1(coeffs, x[i]);

	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_osc_saw_process_multi(
		bw_osc_saw_coeffs * BW_RESTRICT coeffs,
		const float * const *           x,
		const float * const *           x_inc,
		float * const *                 y,
		size_t                          n_channels,
		size_t                          n_samples) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);
	BW_ASSERT(x != NULL);
	BW_ASSERT(y != NULL);

	if (x_inc != NULL)
		for (size_t i = 0; i < n_channels; i++)
			bw_osc_saw_process(coeffs, x[i], x_inc[i], y[i], n_samples);
	else
		for (size_t i = 0; i < n_channels; i++)
			bw_osc_saw_process(coeffs, x[i], NULL, y[i], n_samples);

	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_reset_coeffs);
}

static inline void bw_osc_saw_set_antialiasing(
		bw_osc_saw_coeffs * BW_RESTRICT coeffs,
		char                            value) {
	BW_ASSERT(coeffs != NULL);
	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_init);

	coeffs->antialiasing = value;

	BW_ASSERT_DEEP(bw_osc_saw_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_saw_coeffs_state_init);
}

static inline char bw_osc_saw_coeffs_is_valid(
		const bw_osc_saw_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_osc_saw_coeffs"))
		return 0;
	if (coeffs->state < bw_osc_saw_coeffs_state_init || coeffs->state > bw_osc_saw_coeffs_state_reset_coeffs)
		return 0;
#endif

	(void)coeffs;

	return 1;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::OscSaw
 *  ```>>> */
template<size_t N_CHANNELS>
class OscSaw {
public:
	OscSaw();

	void setSampleRate(
		float sampleRate);

	void reset();

	void process(
		const float * const * x,
		const float * const * x_inc,
		float * const *       y,
		size_t                nSamples);

	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> x_inc,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);
	
	void setAntialiasing(bool value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_osc_saw_coeffs	coeffs;
};

template<size_t N_CHANNELS>
inline OscSaw<N_CHANNELS>::OscSaw() {
	bw_osc_saw_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void OscSaw<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_osc_saw_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void OscSaw<N_CHANNELS>::reset() {
	bw_osc_saw_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void OscSaw<N_CHANNELS>::process(
		const float * const * x,
		const float * const * x_inc,
		float * const *       y,
		size_t                nSamples) {
	bw_osc_saw_process_multi(&coeffs, x, x_inc, y, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void OscSaw<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> x_inc,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), x_inc.data(), y.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void OscSaw<N_CHANNELS>::setAntialiasing(
		bool value) {
	bw_osc_saw_set_antialiasing(&coeffs, value);
}

}
#endif

#endif
