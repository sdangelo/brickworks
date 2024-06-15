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
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    Pulse oscillator waveshaper with variable pulse width (actually, duty
 *    cycle) and PolyBLEP antialiasing.
 *
 *    It turns a normalized phase signal, such as that geneated by
 *    [bw\_phase\_gen](bw_phase_gen), into a pulse wave.
 *
 *    The antialiasing algorithm is based on
 *
 *    V. Valimaki and A. Huovilainen, "Antialiasing Oscillators in Subtractive
 *    Synthesis", IEEE Signal Processing Magazine, vol. 24, no. 2, pp. 116-125,
 *    March 2007.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.1.1</strong>:
 *        <ul>
 *          <li>Added debugging checks in
 *              <code>bw_osc_pulse_process_multi()</code> to ensure that
 *              <code>x_inc</code> is not <code>BW_NULL</code> when antialiasing
 *              is on and that buffers used for both input and output appear at
 *              the same channel indices.</li>
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
 *          <li><code>bw_osc_pulse_process()</code> and
 *              <code>bw_osc_pulse_process_multi()</code> now use
 *              <code>size_t</code> to count samples and channels.</li>
 *          <li>Added more <code>const</code> specifiers to input
 *              arguments.</li>
 *          <li>Moved C++ code to C header.</li>
 *          <li>Added overloaded C++ <code>process()</code> function taking
 *              C-style arrays as arguments.</li>
 *          <li>Now antialiasing should work also with negative input
 *              increments.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *          <li>Clearly specified validity ranges for audio-rate signals.</li>
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
 *          <li>Added <code>bw_osc_pulse_process_multi()</code>.</li>
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

#ifndef BW_OSC_PULSE_H
#define BW_OSC_PULSE_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_osc_pulse_coeffs
 *  ```>>> */
typedef struct bw_osc_pulse_coeffs bw_osc_pulse_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_osc_pulse_init()
 *  ```>>> */
static inline void bw_osc_pulse_init(
	bw_osc_pulse_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_osc_pulse_set_sample_rate()
 *  ```>>> */
static inline void bw_osc_pulse_set_sample_rate(
	bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
	float                             sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_osc_pulse_reset_coeffs()
 *  ```>>> */
static inline void bw_osc_pulse_reset_coeffs(
	bw_osc_pulse_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_osc_pulse_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_osc_pulse_update_coeffs_ctrl(
	bw_osc_pulse_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_osc_pulse_update_coeffs_audio()
 *  ```>>> */
static inline void bw_osc_pulse_update_coeffs_audio(
	bw_osc_pulse_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_osc_pulse_process1\*()
 *  ```>>> */
static inline float bw_osc_pulse_process1(
	const bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
	float                                   x);

static inline float bw_osc_pulse_process1_antialias(
	const bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
	float                                   x,
	float                                   x_inc);
/*! <<<```
 *    These function process one input sample `x`, representing the normalized
 *    phase, using `coeffs`. They return the corresponding output sample.
 *
 *    In particular:
 *     * `bw_osc_pulse_process1()` assumes that antialiasing is disabled;
 *     * `bw_osc_pulse_process1_antialias()` assumes that antialiasing is
 *       enabled and requires the corresponding phase increment value to be
 *       passed via `x_inc`.
 *
 *    Whether antialiasing is enabled or not is unchecked even for debugging
 *    purposes.
 *
 *    `x` must be in [`0.f`, `1.f`).
 *
 *    `x_inc` must be in [`-0.5f`, `0.5f`].
 *
 *    #### bw_osc_pulse_process()
 *  ```>>> */
static inline void bw_osc_pulse_process(
	bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
	const float *                     x,
	const float *                     x_inc,
	float *                           y,
	size_t                            n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x`, containing the
 *    normalized phase signal, and fills the first `n_samples` of the output
 *    buffer `y`, while using and updating `coeffs` (control and audio rate).
 *
 *    If antialiasing is enabled, `x_inc` must contain phase increment
 *    values, otherwise it is ignored and can be `BW_NULL`.
 *
 *    All samples in `x` must be in [`0.f`, `1.f`).
 *
 *    All samples is `x_inc`, if not ignored, must be in [`-0.5f`, `0.5f`].
 *
 *    #### bw_osc_pulse_process_multi()
 *  ```>>> */
static inline void bw_osc_pulse_process_multi(
	bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
	const float * const *             x,
	const float * const *             x_inc,
	float * const *                   y,
	size_t                            n_channels,
	size_t                            n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x`,
 *    containing the normalized phase signals, and fills the first `n_samples`
 *    of the `n_channels` output buffers `y`, while using and updating the
 *    common `coeffs` (control and audio rate).
 *
 *    If antialiasing is enabled, each of the `n_channels` buffers pointed by
 *    `x_inc` must contain phase increment values, otherwise `x_inc` is ignored
 *    and can be `BW_NULL`.
 *
 *    All samples in `x` must be in [`0.f`, `1.f`).
 *
 *    All samples is `x_inc`, if not ignored, must be in [`-0.5f`, `0.5f`].
 *
 *    #### bw_osc_pulse_set_antialiasing()
 *  ```>>> */
static inline void bw_osc_pulse_set_antialiasing(
	bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
	char                              value);
/*! <<<```
 *    Sets whether the antialiasing is on (`value` non-`0`) or off (`0`) in
 *    `coeffs`.
 *
 *    Default value: `0` (off).
 *
 *    #### bw_osc_pulse_set_pulse_width()
 *  ```>>> */
static inline void bw_osc_pulse_set_pulse_width(
	bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
	float                             value);
/*! <<<```
 *    Sets the pulse width (actually, the duty cycle) to `value` in `coeffs`.
 *
 *    Valid range: [`0.f` (0% duty cycle), `1.f` (100% duty cycle)].
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_osc_pulse_coeffs_is_valid()
 *  ```>>> */
static inline char bw_osc_pulse_coeffs_is_valid(
	const bw_osc_pulse_coeffs * BW_RESTRICT coeffs);
/*! <<<```
 *    Tries to determine whether `coeffs` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `coeffs` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_osc_pulse_coeffs`.
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
enum bw_osc_pulse_coeffs_state {
	bw_osc_pulse_coeffs_state_invalid,
	bw_osc_pulse_coeffs_state_init,
	bw_osc_pulse_coeffs_state_set_sample_rate,
	bw_osc_pulse_coeffs_state_reset_coeffs
};
#endif

struct bw_osc_pulse_coeffs {
#ifdef BW_DEBUG_DEEP
	uint32_t			hash;
	enum bw_osc_pulse_coeffs_state	state;
#endif

	// Sub-components
	bw_one_pole_coeffs		smooth_coeffs;
	bw_one_pole_state		smooth_state;

	// Parameters
	char				antialiasing;
	float				pulse_width;
};

static inline void bw_osc_pulse_init(
		bw_osc_pulse_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	coeffs->antialiasing = 0;
	coeffs->pulse_width = 0.5f;

#ifdef BW_DEBUG_DEEP
	coeffs->hash = bw_hash_sdbm("bw_osc_pulse_coeffs");
	coeffs->state = bw_osc_pulse_coeffs_state_init;
#endif
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_osc_pulse_coeffs_state_init);
}

static inline void bw_osc_pulse_set_sample_rate(
		bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
		float                             sample_rate) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_init);
	BW_ASSERT(bw_is_finite(sample_rate) && sample_rate > 0.f);

	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_osc_pulse_coeffs_state_set_sample_rate;
#endif
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_osc_pulse_coeffs_state_set_sample_rate);
}

static inline void bw_osc_pulse_reset_coeffs(
		bw_osc_pulse_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_set_sample_rate);

	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->pulse_width);

#ifdef BW_DEBUG_DEEP
	coeffs->state = bw_osc_pulse_coeffs_state_reset_coeffs;
#endif
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state == bw_osc_pulse_coeffs_state_reset_coeffs);
}

static inline void bw_osc_pulse_update_coeffs_ctrl(
		bw_osc_pulse_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);

	(void)coeffs;
}

static inline void bw_osc_pulse_update_coeffs_audio(
		bw_osc_pulse_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);

	bw_one_pole_process1(&coeffs->smooth_coeffs, &coeffs->smooth_state, coeffs->pulse_width);

	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);
}

static inline float bw_osc_pulse_process1(
		const bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
		float                                   x) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 0.f && x < 1.f);

	const float pw = bw_one_pole_get_y_z1(&coeffs->smooth_state);
	const float y = bw_signf(pw - x);

	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(y));

	return y;
}

// PolyBLEP residual based on Parzen window (4th-order B-spline), one-sided (x in [0, 2])
static inline float bw_osc_pulse_blep_diff(
		float x) {
	return x < 1.f
		? x * ((0.25f * x - 0.6666666666666666f) * x * x + 1.333333333333333f) - 1.f
		: x * (x * ((0.6666666666666666f - 0.08333333333333333f * x) * x - 2.f) + 2.666666666666667f) - 1.333333333333333f;
}

static inline float bw_osc_pulse_process1_antialias(
		const bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
		float                                   x,
		float                                   x_inc) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(x >= 0.f && x < 1.f);
	BW_ASSERT(bw_is_finite(x_inc));
	BW_ASSERT(x_inc >= -0.5f && x_inc <= 0.5f);

	const float pw = bw_one_pole_get_y_z1(&coeffs->smooth_state);
	const float pw_m_phase = pw - x;
	float v = bw_copysignf(1.f, pw_m_phase); // pw = phase case should be properly compensated by the AA
	const float a_inc = bw_absf(x_inc);
	if (a_inc > 1e-6f) {
		const float phase_inc_2 = a_inc + a_inc;
		const float phase_inc_rcp = bw_rcpf(a_inc);
		const float phase_2 = 0.5f * v + 0.5f - pw_m_phase;
		const float s_1_m_phase = 1.f - x;
		const float s_1_m_phase_2 = 1.f - phase_2;
		if (s_1_m_phase < phase_inc_2)
			v -= bw_osc_pulse_blep_diff(s_1_m_phase * phase_inc_rcp);
		if (s_1_m_phase_2 < phase_inc_2)
			v += bw_osc_pulse_blep_diff(s_1_m_phase_2 * phase_inc_rcp);
		if (x < phase_inc_2)
			v += bw_osc_pulse_blep_diff(x * phase_inc_rcp);
		if (phase_2 < phase_inc_2)
			v -= bw_osc_pulse_blep_diff(phase_2 * phase_inc_rcp);
	}

	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);
	BW_ASSERT(bw_is_finite(v));

	return v;
}

static inline void bw_osc_pulse_process(
		bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
		const float *                     x,
		const float *                     x_inc,
		float *                           y,
		size_t                            n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(coeffs->antialiasing ? x_inc != BW_NULL : 1);
	BW_ASSERT_DEEP(coeffs->antialiasing ? bw_has_only_finite(x_inc, n_samples) : 1);
	BW_ASSERT(y != BW_NULL);

	if (coeffs->antialiasing)
		for (size_t i = 0; i < n_samples; i++) {
			bw_osc_pulse_update_coeffs_audio(coeffs);
			y[i] = bw_osc_pulse_process1_antialias(coeffs, x[i], x_inc[i]);
		}
	else
		for (size_t i = 0; i < n_samples; i++) {
			bw_osc_pulse_update_coeffs_audio(coeffs);
			y[i] = bw_osc_pulse_process1(coeffs, x[i]);
		}

	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);
	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
}

static inline void bw_osc_pulse_process_multi(
		bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
		const float * const *             x,
		const float * const *             x_inc,
		float * const *                   y,
		size_t                            n_channels,
		size_t                            n_samples) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT(coeffs->antialiasing ? x_inc != BW_NULL : 1);
	BW_ASSERT(y != BW_NULL);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(y[i] != y[j]);
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = 0; j < n_channels; j++)
			BW_ASSERT(i == j || x[i] != y[j]);
	if (coeffs->antialiasing)
		for (size_t i = 0; i < n_channels; i++)
			for (size_t j = 0; j < n_channels; j++)
				BW_ASSERT(i == j || x_inc[i] != y[j]);
#endif

	if (coeffs->antialiasing)
		for (size_t i = 0; i < n_samples; i++) {
			bw_osc_pulse_update_coeffs_audio(coeffs);
			for (size_t j = 0; j < n_channels; j++)
				y[j][i] = bw_osc_pulse_process1_antialias(coeffs, x[j][i], x_inc[j][i]);
		}
	else
		for (size_t i = 0; i < n_samples; i++) {
			bw_osc_pulse_update_coeffs_audio(coeffs);
			for (size_t j = 0; j < n_channels; j++)
				y[j][i] = bw_osc_pulse_process1(coeffs, x[j][i]);
		}

	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_reset_coeffs);
}

static inline void bw_osc_pulse_set_antialiasing(
		bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
		char                              value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_init);

	coeffs->antialiasing = value;

	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_init);
}

static inline void bw_osc_pulse_set_pulse_width(
		bw_osc_pulse_coeffs * BW_RESTRICT coeffs,
		float                             value) {
	BW_ASSERT(coeffs != BW_NULL);
	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_init);
	BW_ASSERT(bw_is_finite(value));
	BW_ASSERT(value >= 0.f && value <= 1.f);

	coeffs->pulse_width = value;

	BW_ASSERT_DEEP(bw_osc_pulse_coeffs_is_valid(coeffs));
	BW_ASSERT_DEEP(coeffs->state >= bw_osc_pulse_coeffs_state_init);
}

static inline char bw_osc_pulse_coeffs_is_valid(
		const bw_osc_pulse_coeffs * BW_RESTRICT coeffs) {
	BW_ASSERT(coeffs != BW_NULL);

#ifdef BW_DEBUG_DEEP
	if (coeffs->hash != bw_hash_sdbm("bw_osc_pulse_coeffs"))
		return 0;
	if (coeffs->state < bw_osc_pulse_coeffs_state_init || coeffs->state > bw_osc_pulse_coeffs_state_reset_coeffs)
		return 0;
#endif

	return bw_is_finite(coeffs->pulse_width) && coeffs->pulse_width >= 0.f && coeffs->pulse_width <= 1.f;
}

#ifdef __cplusplus
}

#ifndef BW_CXX_NO_ARRAY
# include <array>
#endif

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::OscPulse
 *  ```>>> */
template<size_t N_CHANNELS>
class OscPulse {
public:
	OscPulse();
	
	void setSampleRate(
		float sampleRate);

	void reset();

	void process(
		const float * const * x,
		const float * const * x_inc,
		float * const *       y,
		size_t                nSamples);

#ifndef BW_CXX_NO_ARRAY
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> x_inc,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples);
#endif
	
	void setAntialiasing(
		bool value);

	void setPulseWidth(
		float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_osc_pulse_coeffs	coeffs;
};

template<size_t N_CHANNELS>
inline OscPulse<N_CHANNELS>::OscPulse() {
	bw_osc_pulse_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void OscPulse<N_CHANNELS>::setSampleRate(
		float sampleRate) {
	bw_osc_pulse_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void OscPulse<N_CHANNELS>::reset() {
	bw_osc_pulse_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void OscPulse<N_CHANNELS>::process(
		const float * const * x,
		const float * const * x_inc,
		float * const *       y,
		size_t                nSamples) {
	bw_osc_pulse_process_multi(&coeffs, x, x_inc, y, N_CHANNELS, nSamples);
}

#ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void OscPulse<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> x_inc,
		std::array<float *, N_CHANNELS>       y,
		size_t                                nSamples) {
	process(x.data(), x_inc.data(), y.data(), nSamples);
}
#endif

template<size_t N_CHANNELS>
inline void OscPulse<N_CHANNELS>::setAntialiasing(
		bool value) {
	bw_osc_pulse_set_antialiasing(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void OscPulse<N_CHANNELS>::setPulseWidth(
		float value) {
	bw_osc_pulse_set_pulse_width(&coeffs, value);
}

}
#endif

#endif
