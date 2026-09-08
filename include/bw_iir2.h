/*
 * Brickworks
 *
 * Copyright (C) 2025, 2026 Orastron Srl unipersonale
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
 *  module_type {{{ utility }}}
 *  version {{{ 1.0.2 }}}
 *  requires {{{ bw_common bw_math }}}
 *  description {{{
 *    Lightweight and fast second-order IIR filter (biquad) in TDF-II form.
 *
 *    This is not a regular DSP module, as it exposes state and coefficients,
 *    and it's not appropriate for time-varying operation. If you need that,
 *    check out [bw_ap2](bw_ap2), [bw_hs2](bw_hs2), [bw_ls2](bw_ls2),
 *    [bw_mm2](bw_mm2), [bw_notch](bw_notch), [bw_peak](bw_peak), and
 *    [bw_svf](bw_svf).
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.2</strong>:
 *        <ul>
 *          <li>Fixed coefficients' validity checks in
 *              <code>bw_iir2_coeffs_is_valid()</code> and elsewhere.</li>
 *          <li>Rationalized debugging checks for coefficient validity.</li>
 *          <li>Updated dependencies.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.1</strong>:
 *        <ul>
 *          <li>Added <code>bw_iir2_coeffs_is_valid()</code>.</li>
 *          <li>Added default value for <code>N_CHANNELS</code> in C++ API.</li>
 *          <li>Fixed the documenation of <code>bw_iir2_coeffs_*()</code> w.r.t.
 *              <code>prewarp_at_cutoff</code> and typos.</li>
 *          <li>Updated dependencies.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef BW_IIR2_H
#define BW_IIR2_H

#ifdef BW_INCLUDE_WITH_QUOTES
# include "bw_common.h"
#else
# include <bw_common.h>
#endif

#if !defined(BW_CXX_NO_EXTERN_C) && defined(__cplusplus)
extern "C" {
#endif

/*** Public API ***/

/*! api {{{
 *    #### bw_iir2_reset()
 *  ```>>> */
static inline void bw_iir2_reset(
	float               x_0,
	float * BW_RESTRICT y_0,
	float * BW_RESTRICT s1_0,
	float * BW_RESTRICT s2_0,
	float               b0,
	float               b1,
	float               b2,
	float               a1,
	float               a2);
/*! <<<```
 *    Computes and puts the initial output in `y_0` and the initial states in
 *    `s1_0` and `s2_0`, given the initial input `x_0` and coefficients `b0`,
 *    `b1`, `b2`, `a1`, and `a2`.
 *
 *    The given coefficients must describe a stable filter.
 *
 *    #### bw_iir2_reset_multi()
 *  ```>>> */
static inline void bw_iir2_reset_multi(
	const float *       x_0,
	float *             y_0,
	float * BW_RESTRICT s1_0,
	float *             s2_0,
	float               b0,
	float               b1,
	float               b2,
	float               a1,
	float               a2,
	size_t              n_channels);
/*! <<<```
 *    Computes and puts each of the `n_channels` initial outputs in `y_0` and
 *    initial states in `s1_0` and `s2_0`, given the corresponding initial
 *    inputs `x_0` and coefficients `b0`, `b1`, `b2`, `a1`, and `a2`.
 *
 *    `y_0` and/or `s_0` may be `BW_NULL`, in which case the corresponding
 *    values are not written anywhere.
 *
 *    The given coefficients must describe a stable filter.
 *
 *    #### bw_iir2_process1()
 *  ```>>> */
static inline void bw_iir2_process1(
	float               x,
	float * BW_RESTRICT y,
	float * BW_RESTRICT s1,
	float * BW_RESTRICT s2,
	float               b0,
	float               b1,
	float               b2,
	float               a1,
	float               a2);
/*! <<<```
 *    Processes one input sample `x` using coefficients `b0`, `b1`, `b2`, `a1`,
 *    and `a2`. The output sample and next states value are put in `y` and
 *    `s1`/`s2` respectively.
 *
 *    The given coefficients must describe a stable filter.
 *
 *    #### bw_iir2_process()
 *  ```>>> */
static inline void bw_iir2_process(
	const float *       x,
	float *             y,
	float * BW_RESTRICT s1,
	float * BW_RESTRICT s2,
	float               b0,
	float               b1,
	float               b2,
	float               a1,
	float               a2,
	size_t              n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using coefficients `b0`,
 *    `b1`, `b2`, `a1`, and `a2`. The next state values are put in `s1` and
 *    `s2`.
 *
 *    The given coefficients must describe a stable filter.
 *
 *    #### bw_iir2_process_multi()
 *  ```>>> */
static inline void bw_iir2_process_multi(
	const float * const * x,
	float * const *       y,
	float * BW_RESTRICT   s1,
	float * BW_RESTRICT   s2,
	float                 b0,
	float                 b1,
	float                 b2,
	float                 a1,
	float                 a2,
	size_t                n_channels,
	size_t                n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using coefficients `b0`, `b1`, `b2`, `a1`, and `a2`. The next `n_channels`
 *    state values are put in `s1` and `s2`.
 *
 *    The given coefficients must describe a stable filter.
 *
 *    #### bw_iir2_coeffs_ap2()
 *  ```>>> */
static inline void bw_iir2_coeffs_ap2(
	float               sample_rate,
	float               cutoff,
	float               Q,
	char                prewarp_at_cutoff,
	float               prewarp_freq,
	float * BW_RESTRICT b0,
	float * BW_RESTRICT b1,
	float * BW_RESTRICT b2,
	float * BW_RESTRICT a1,
	float * BW_RESTRICT a2);
/*! <<<```
 *    Computes and puts coefficient values in `b0`, `b1`, `b2`, `a1`, and `a2`
 *    resulting in a second-order allpass filter (180° shift at cutoff,
 *    approaching 360° shift at high frequencies) with unitary gain, using the
 *    bilinear transform with prewarping.
 *
 *    It takes the `sample_rate` (Hz, must be positive), the `cutoff` frequency
 *    (Hz, in [`1e-6f`, `1e12f`]), and the quality factor `Q` (in [`1e-6f`,
 *    `1e6f`]). If `prewarp_at_cutoff` is non-`0`, then the prewarping frequency
 *    matches `cutoff`, otherwise the value specified by `prewarp_freq` (Hz, in
 *    [`1e-6f`, `1e12f`], however internally limited to avoid instability) is
 *    used.
 *
 *    #### bw_iir2_coeffs_bp2()
 *  ```>>> */
static inline void bw_iir2_coeffs_bp2(
	float               sample_rate,
	float               cutoff,
	float               Q,
	char                prewarp_at_cutoff,
	float               prewarp_freq,
	float * BW_RESTRICT b0,
	float * BW_RESTRICT b1,
	float * BW_RESTRICT b2,
	float * BW_RESTRICT a1,
	float * BW_RESTRICT a2);
/*! <<<```
 *    Computes and puts coefficient values in `b0`, `b1`, `b2`, `a1`, and `a2`
 *    resulting in a second-order bandpass filter (6 dB/oct) with peak gain
 *    `Q` (linear gain), using the bilinear transform with prewarping.
 *
 *    It takes the `sample_rate` (Hz, must be finite and positive), the `cutoff`
 *    frequency (Hz, in [`1e-6f`, `1e12f`]), and the quality factor `Q` (in
 *    [`1e-6f`, `1e6f`]). If `prewarp_at_cutoff` is non-`0`, then the prewarping
 *    frequency matches `cutoff`, otherwise the value specified by
 *    `prewarp_freq` (Hz, in [`1e-6f`, `1e12f`], however internally limited to
 *    avoid instability) is used.
 *
 *    #### bw_iir2_coeffs_hp2()
 *  ```>>> */
static inline void bw_iir2_coeffs_hp2(
	float               sample_rate,
	float               cutoff,
	float               Q,
	char                prewarp_at_cutoff,
	float               prewarp_freq,
	float * BW_RESTRICT b0,
	float * BW_RESTRICT b1,
	float * BW_RESTRICT b2,
	float * BW_RESTRICT a1,
	float * BW_RESTRICT a2);
/*! <<<```
 *    Computes and puts coefficient values in `b0`, `b1`, `b2`, `a1`, and `a2`
 *    resulting in a second-order highpass filter (12 dB/oct) with gain
 *    asymptotically approaching unity as frequency increases, using the
 *    bilinear transform with prewarping.
 *
 *    It takes the `sample_rate` (Hz, must be finite and positive), the `cutoff`
 *    frequency (Hz, in [`1e-6f`, `1e12f`]), and the quality factor `Q` (in
 *    [`1e-6f`, `1e6f`]). If `prewarp_at_cutoff` is non-`0`, then the prewarping
 *    frequency matches `cutoff`, otherwise the value specified by
 *    `prewarp_freq` (Hz, in [`1e-6f`, `1e12f`], however internally limited to
 *    avoid instability) is used.
 *
 *    #### bw_iir2_coeffs_hs2()
 *  ```>>> */
static inline void bw_iir2_coeffs_hs2(
	float               sample_rate,
	float               cutoff,
	float               Q,
	char                prewarp_at_cutoff,
	float               prewarp_freq,
	char                high_gain_dB,
	float               high_gain,
	float * BW_RESTRICT b0,
	float * BW_RESTRICT b1,
	float * BW_RESTRICT b2,
	float * BW_RESTRICT a1,
	float * BW_RESTRICT a2);
/*! <<<```
 *    Computes and puts coefficient values in `b0`, `b1`, `b2`, `a1,` and `a2`
 *    resulting in a second-order high shelf filter (12 dB/oct) with unitary DC
 *    gain, using the bilinear transform with prewarping.
 *
 *    It takes the `sample_rate` (Hz, must be finite and positive), the `cutoff`
 *    frequency (Hz, must be finite and positive), the quality factor `Q` (in
 *    [`1e-6f`, `1e6f`]), and the high-frequency gain `high_gain`, either as
 *    linear gain (in [`1e-30f`, `1e30f`]) if `high_gain_dB` is `0`, or
 *    otherwise in dB (in [`-600.f`, `600.f`]). If `prewarp_at_cutoff` is
 *    non-`0`, then the prewarpingfrequency matches `cutoff`, otherwise the
 *    value specified by `prewarp_freq` (Hz, in [`1e-6f`, `1e12f`], however
 *    internally limited to avoid instability) is used.
 *
 *    `cutoff * bw_sqrtf(bw_sqrtf(high_gain))` must be in [`1e-6f`, `1e12f`],
 *    where `high_gain` is expressed as linear gain.
 *
 *    #### bw_iir2_coeffs_lp2()
 *  ```>>> */
static inline void bw_iir2_coeffs_lp2(
	float               sample_rate,
	float               cutoff,
	float               Q,
	char                prewarp_at_cutoff,
	float               prewarp_freq,
	float * BW_RESTRICT b0,
	float * BW_RESTRICT b1,
	float * BW_RESTRICT b2,
	float * BW_RESTRICT a1,
	float * BW_RESTRICT a2);
/*! <<<```
 *    Computes and puts coefficient values in `b0`, `b1`, `b2`, `a1`, and `a2`
 *    resulting in a second-order lowpass filter (12 dB/oct) with unitary DC
 *    gain, using the bilinear transform with prewarping.
 *
 *    It takes the `sample_rate` (Hz, must be finite and positive), the `cutoff`
 *    frequency (Hz, in [`1e-6f`, `1e12f`]), and the quality factor `Q` (in
 *    [`1e-6f`, `1e6f`]). If `prewarp_at_cutoff` is non-`0`, then the prewarping
 *    frequency matches `cutoff`, otherwise the value specified by
 *    `prewarp_freq` (Hz, in [`1e-6f`, `1e12f`], however internally limited to
 *    avoid instability) is used.
 *
 *    #### bw_iir2_coeffs_ls2()
 *  ```>>> */
static inline void bw_iir2_coeffs_ls2(
	float               sample_rate,
	float               cutoff,
	float               Q,
	char                prewarp_at_cutoff,
	float               prewarp_freq,
	char                dc_gain_dB,
	float               dc_gain,
	float * BW_RESTRICT b0,
	float * BW_RESTRICT b1,
	float * BW_RESTRICT b2,
	float * BW_RESTRICT a1,
	float * BW_RESTRICT a2);
/*! <<<```
 *    Computes and puts coefficient values in `b0`, `b1`, `b2`, `a1,` and `a2`
 *    resulting in a second-order low shelf filter (12 dB/oct) with gain
 *    asymptotically approaching unity as frequency increases, using the
 *    bilinear transform with prewarping.
 *
 *    It takes the `sample_rate` (Hz, must be finite and positive), the `cutoff`
 *    frequency (Hz, must be finite and positive), the quality factor `Q` (in
 *    [`1e-6f`, `1e6f`]), and the `dc_gain`, either as linear gain (in
 *    [`1e-30f`, `1e30f`]) if `high_gain_dB` is `0`, or otherwise in dB (in
 *    [`-600.f`, `600.f`]). If `prewarp_at_cutoff` is non-`0`, then the
 *    prewarping frequency matches `cutoff`, otherwise the value specified by
 *    `prewarp_freq` (Hz, in [`1e-6f`, `1e12f`], however internally limited to
 *    avoid instability) is used.
 *
 *    `cutoff * bw_rpcf(bw_sqrtf(bw_sqrtf(dc_gain)))` must be in [`1e-6f`,
 *    `1e12f`], where `dc_gain` is expressed as linear gain.
 *
 *    #### bw_iir2_coeffs_mm2()
 *  ```>>> */
static inline void bw_iir2_coeffs_mm2(
	float               sample_rate,
	float               cutoff,
	float               Q,
	char                prewarp_at_cutoff,
	float               prewarp_freq,
	float               coeff_x,
	float               coeff_lp,
	float               coeff_bp,
	float               coeff_hp,
	float * BW_RESTRICT b0,
	float * BW_RESTRICT b1,
	float * BW_RESTRICT b2,
	float * BW_RESTRICT a1,
	float * BW_RESTRICT a2);
/*! <<<```
 *    Computes and puts coefficient values in `b0`, `b1`, `b2`, `a1`, and `a2`
 *    resulting in a second-order filter implementing an approximation of the
 *    Laplace-domain transfer function
 *
 *    > H(s) = coeff\_x + (coeff\_hp s^2 + 2 pi fc s coeff\_bp
 *    >        + (2 pi fc)^2 coeff\_lp) / (s^2 + 2 pi fc / Q s + (2 pi fc)^2)
 *
 *    where fc is the cutoff frequency and Q is the quality factor, using the
 *    bilinear transform with prewarping.
 *
 *    It takes the `sample_rate` (Hz, must be finite and positive), the `cutoff`
 *    frequency (Hz, in [`1e-6f`, `1e12f`]), the quality factor `Q` (in
 *    [`1e-6f`, `1e6f`]), and output coefficients `coeff_x`, `coeff_lp`,
 *    `coeff_bp`, `coeff_hp` (all must be finite). If `prewarp_at_cutoff` is
 *    non-`0`, then the prewarping frequency matches `cutoff`, otherwise the
 *    value specified by `prewarp_freq` (Hz, in [`1e-6f`, `1e12f`], however
 *    internally limited to avoid instability) is used.
 *
 *    #### bw_iir2_coeffs_notch()
 *  ```>>> */
static inline void bw_iir2_coeffs_notch(
	float               sample_rate,
	float               cutoff,
	float               Q,
	char                prewarp_at_cutoff,
	float               prewarp_freq,
	float * BW_RESTRICT b0,
	float * BW_RESTRICT b1,
	float * BW_RESTRICT b2,
	float * BW_RESTRICT a1,
	float * BW_RESTRICT a2);
/*! <<<```
 *    Computes and puts coefficient values in `b0`, `b1`, `b2`, `a1`, and `a2`
 *    resulting in a second-order notch filter with unitary gain at DC and
 *    asymptotically as frequency increases, and null gain at cutoff frequency,
 *    using the bilinear transform with prewarping.
 *
 *    It takes the `sample_rate` (Hz, must be finite and positive), the `cutoff`
 *    frequency (Hz, in [`1e-6f`, `1e12f`]), and the quality factor `Q` (in
 *    [`1e-6f`, `1e6f`]). If `prewarp_at_cutoff` is non-`0`, then the prewarping
 *    frequency matches `cutoff`, otherwise the value specified by
 *    `prewarp_freq` (Hz, in [`1e-6f`, `1e12f`], however internally limited to
 *    avoid instability) is used.
 *
 *    #### bw_iir2_coeffs_peak()
 *  ```>>> */
static inline void bw_iir2_coeffs_peak(
	float               sample_rate,
	float               cutoff,
	char                use_bandwidth,
	float               Q_bandwidth,
	char                prewarp_at_cutoff,
	float               prewarp_freq,
	char                peak_gain_dB,
	float               peak_gain,
	float * BW_RESTRICT b0,
	float * BW_RESTRICT b1,
	float * BW_RESTRICT b2,
	float * BW_RESTRICT a1,
	float * BW_RESTRICT a2);
/*! <<<```
 *    Computes and puts coefficient values in `b0`, `b1`, and `a1` resulting in
 *    a second-order peak filter with unitary gain at DC and asymptotically as
 *    frequency increases, using the bilinear transform with prewarping.
 *
 *    It takes the `sample_rate` (Hz, must be finite and  positive), the
 *    `cutoff` frequency (Hz, in [`1e-6f`, `1e12f`]), and the `peak_gain`,
 *    either as linear gain (in [`1e-30f`, `1e30f`]) if `peak_gain_dB` is `0`,
 *    or otherwise in dB (in [`-600.f`, `600.f`]). `Q_bandwidth` indicates
 *    either the quality factor (in [`1e-6f`, `1e6f`]) if `use_bandwidth` is
 *    `0`, or otherwise the bandwidth (octaves, in [`1e-6f`, `90.f`]),
 *    designating the distance between midpoint gain frequencies (i.e.,
 *    frequencies with gain = peak gain / 2 in dB terms). If `prewarp_at_cutoff`
 *    is non-`0`, then the prewarping frequency matches `cutoff`, otherwise the
 *    value specified by `prewarp_freq` (Hz, in [`1e-6f`, `1e12f`], however
 *    internally limited to avoid instability) is used.
 *
 *    If `use_bandwidth` is non-`0`, then `bw_sqrtf(bw_pow2f(Q_bandwidth) *
 *    peak_gain) * bw_rcpf(bw_pow2f(Q_bandwidth) - 1.f)` must be in [`1e-6f`,
 *    `1e6f`], where `peak_gain` is expressed as linear gain.
 *
 *    #### bw_iir2_coeffs_is_valid()
 *  ```>>> */
static inline char bw_iir2_coeffs_is_valid(
	float b0,
	float b1,
	float b2,
	float a1,
	float a2);
/*! <<<```
 *    Determines whether `b0`, `b1`, `b2`, `a1`, and `a2` are valid and
 *    describe a stable or marginally stable filter.
 *
 *    It returns non-`0` if it is the case and `0` otherwise.
 *  }}} */

#if !defined(BW_CXX_NO_EXTERN_C) && defined(__cplusplus)
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#ifdef BW_INCLUDE_WITH_QUOTES
# include "bw_math.h"
#else
# include <bw_math.h>
#endif

#if !defined(BW_CXX_NO_EXTERN_C) && defined(__cplusplus)
extern "C" {
#endif

static inline void bw_iir2_reset(
		float               x_0,
		float * BW_RESTRICT y_0,
		float * BW_RESTRICT s1_0,
		float * BW_RESTRICT s2_0,
		float               b0,
		float               b1,
		float               b2,
		float               a1,
		float               a2) {
	BW_ASSERT(bw_is_finite(x_0));
	BW_ASSERT(y_0 != BW_NULL);
	BW_ASSERT(s1_0 != BW_NULL);
	BW_ASSERT(s2_0 != BW_NULL);
	BW_ASSERT(y_0 != s1_0);
	BW_ASSERT(y_0 != s2_0);
	BW_ASSERT(s1_0 != s2_0);
	BW_ASSERT(bw_iir2_coeffs_is_valid(b0, b1, b2, a1, a2));

	if (a1 + a2 == -1.f) {
		*y_0 = 0.f;
		*s1_0 = 0.f;
		*s2_0 = 0.f;
	} else {
		const float d = bw_rcpf(1.f + a1 + a2);
		const float k = d * x_0;
		*y_0 = k * (b0 + b1 + b2);
		*s1_0 = k * (b1 + b2 - b0 * (a1 + a2));
		*s2_0 = k * (b2 + b2 * a1 - a2 * (b0 + b1));
	}

	BW_ASSERT(bw_is_finite(*y_0));
	BW_ASSERT(bw_is_finite(*s1_0));
	BW_ASSERT(bw_is_finite(*s2_0));
}

static inline void bw_iir2_reset_multi(
		const float *       x_0,
		float *             y_0,
		float * BW_RESTRICT s1_0,
		float *             s2_0,
		float               b0,
		float               b1,
		float               b2,
		float               a1,
		float               a2,
		size_t              n_channels) {
	BW_ASSERT(x_0 != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x_0, n_channels));
	BW_ASSERT(s1_0 == BW_NULL || x_0 != s1_0);
	BW_ASSERT(y_0 == BW_NULL || s1_0 == BW_NULL || y_0 != s1_0);
	BW_ASSERT(s2_0 == BW_NULL || x_0 != s2_0);
	BW_ASSERT(y_0 == BW_NULL || s2_0 == BW_NULL || y_0 != s2_0);
	BW_ASSERT(s1_0 == BW_NULL || s2_0 == BW_NULL || s1_0 != s2_0);
	BW_ASSERT(bw_iir2_coeffs_is_valid(b0, b1, b2, a1, a2));

	if (y_0 != BW_NULL) {
		if (s1_0 != BW_NULL) {
			if (s2_0 != BW_NULL)
				for (size_t i = 0; i < n_channels; i++)
					bw_iir2_reset(x_0[i], y_0 + i, s1_0 + i, s2_0 + i, b0, b1, b2, a1, a2);
			else
				for (size_t i = 0; i < n_channels; i++) {
					float v_s2;
					bw_iir2_reset(x_0[i], y_0 + i, s1_0 + i, &v_s2, b0, b1, b2, a1, a2);
				}
		} else {
			if (s2_0 != BW_NULL)
				for (size_t i = 0; i < n_channels; i++) {
					float v_s1;
					bw_iir2_reset(x_0[i], y_0 + i, &v_s1, s2_0 + i, b0, b1, b2, a1, a2);
				}
			else
				for (size_t i = 0; i < n_channels; i++) {
					float v_s1, v_s2;
					bw_iir2_reset(x_0[i], y_0 + i, &v_s1, &v_s2, b0, b1, b2, a1, a2);
				}
		}
	} else {
		if (s1_0 != BW_NULL) {
			if (s2_0 != BW_NULL)
				for (size_t i = 0; i < n_channels; i++) {
					float v_y;
					bw_iir2_reset(x_0[i], &v_y, s1_0 + i, s2_0 + i, b0, b1, b2, a1, a2);
				}
			else
				for (size_t i = 0; i < n_channels; i++) {
					float v_y, v_s2;
					bw_iir2_reset(x_0[i], &v_y, s1_0 + i, &v_s2, b0, b1, b2, a1, a2);
				}
		} else {
			if (s2_0 != BW_NULL)
				for (size_t i = 0; i < n_channels; i++) {
					float v_y, v_s1;
					bw_iir2_reset(x_0[i], &v_y, &v_s1, s2_0 + i, b0, b1, b2, a1, a2);
				}
			else
				{} // no need to do anything
		}
	}

	BW_ASSERT_DEEP(y_0 != BW_NULL ? bw_has_only_finite(y_0, n_channels) : 1);
	BW_ASSERT_DEEP(s1_0 != BW_NULL ? bw_has_only_finite(s1_0, n_channels) : 1);
	BW_ASSERT_DEEP(s2_0 != BW_NULL ? bw_has_only_finite(s2_0, n_channels) : 1);
}

static inline void bw_iir2_process1(
		float               x,
		float * BW_RESTRICT y,
		float * BW_RESTRICT s1,
		float * BW_RESTRICT s2,
		float               b0,
		float               b1,
		float               b2,
		float               a1,
		float               a2) {
	BW_ASSERT(bw_is_finite(x));
	BW_ASSERT(y != BW_NULL);
	BW_ASSERT(s1 != BW_NULL);
	BW_ASSERT(s2 != BW_NULL);
	BW_ASSERT(y != s1);
	BW_ASSERT(y != s2);
	BW_ASSERT(s1 != s2);
	BW_ASSERT(bw_is_finite(*s1));
	BW_ASSERT(bw_is_finite(*s2));
	BW_ASSERT(bw_iir2_coeffs_is_valid(b0, b1, b2, a1, a2));

	*y = b0 * x + *s1;
	*s1 = b1 * x - a1 * *y + *s2;
	*s2 = b2 * x - a2 * *y;

	BW_ASSERT(bw_is_finite(*y));
	BW_ASSERT(bw_is_finite(*s1));
	BW_ASSERT(bw_is_finite(*s2));
}

static inline void bw_iir2_process(
		const float *       x,
		float *             y,
		float * BW_RESTRICT s1,
		float * BW_RESTRICT s2,
		float               b0,
		float               b1,
		float               b2,
		float               a1,
		float               a2,
		size_t              n_samples) {
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT_DEEP(bw_has_only_finite(x, n_samples));
	BW_ASSERT(y != BW_NULL);
	BW_ASSERT(s1 != BW_NULL);
	BW_ASSERT(s2 != BW_NULL);
	BW_ASSERT(x != s1);
	BW_ASSERT(y != s1);
	BW_ASSERT(x != s2);
	BW_ASSERT(y != s2);
	BW_ASSERT(s1 != s2);
	BW_ASSERT(bw_is_finite(*s1));
	BW_ASSERT(bw_is_finite(*s2));
	BW_ASSERT(bw_iir2_coeffs_is_valid(b0, b1, b2, a1, a2));

	for (size_t i = 0; i < n_samples; i++)
		bw_iir2_process1(x[i], y + i, s1, s2, b0, b1, b2, a1, a2);

	BW_ASSERT_DEEP(bw_has_only_finite(y, n_samples));
	BW_ASSERT(bw_is_finite(*s1));
	BW_ASSERT(bw_is_finite(*s2));
}

static inline void bw_iir2_process_multi(
		const float * const * x,
		float * const *       y,
		float * BW_RESTRICT   s1,
		float * BW_RESTRICT   s2,
		float                 b0,
		float                 b1,
		float                 b2,
		float                 a1,
		float                 a2,
		size_t                n_channels,
		size_t                n_samples) {
	BW_ASSERT(x != BW_NULL);
	BW_ASSERT(y != BW_NULL);
	BW_ASSERT(s1 != BW_NULL);
	BW_ASSERT(s2 != BW_NULL);
	BW_ASSERT(s1 != s2);
#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++) {
		BW_ASSERT(x[i] != BW_NULL);
		BW_ASSERT_DEEP(bw_has_only_finite(x[i], n_samples));
		BW_ASSERT(y[i] != BW_NULL);
	}
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = i + 1; j < n_channels; j++)
			BW_ASSERT(y[i] != y[j]);
	for (size_t i = 0; i < n_channels; i++)
		for (size_t j = 0; j < n_channels; j++)
			BW_ASSERT(i == j || x[i] != y[j]);
	for (size_t i = 0; i < n_channels; i++) {
		BW_ASSERT(bw_is_finite(s1[i]));
		BW_ASSERT(bw_is_finite(s2[i]));
	}
#endif
	BW_ASSERT(bw_iir2_coeffs_is_valid(b0, b1, b2, a1, a2));

	for (size_t j = 0; j < n_channels; j++)
		for (size_t i = 0; i < n_samples; i++)
			bw_iir2_process1(x[j][i], y[j] + i, s1 + j, s2 + j, b0, b1, b2, a1, a2);

#ifndef BW_NO_DEBUG
	for (size_t i = 0; i < n_channels; i++) {
		BW_ASSERT_DEEP(bw_has_only_finite(y[i], n_samples));
		BW_ASSERT(bw_is_finite(s1[i]));
		BW_ASSERT(bw_is_finite(s2[i]));
	}
#endif
}

#define BW_IIR2_COEFFS_COMMON \
	prewarp_freq = prewarp_at_cutoff ? cutoff : prewarp_freq; \
	prewarp_freq = bw_minf(prewarp_freq, 0.499f * sample_rate); \
	const float t = bw_tanf(3.141592653589793f * prewarp_freq * bw_rcpf(sample_rate)); \
	const float k1 = prewarp_freq * prewarp_freq; \
	const float k2 = t * cutoff; \
	const float k3 = k2 * k2; \
	const float k4 = k2 * prewarp_freq; \
	const float k5 = Q * (k1 + k3); \
	const float d = bw_rcpf(k5 + k4); \
	*a1 = d * (Q + Q) * (k3 - k1); \
	*a2 = d * (k5 - k4);

static inline void bw_iir2_assert_valid_params(
		float sample_rate,
		float cutoff,
		float Q,
		char prewarp_at_cutoff,
		float prewarp_freq) {
#ifndef BW_NO_DEBUG
	BW_ASSERT(bw_is_finite(sample_rate));
	BW_ASSERT(sample_rate > 0.f);
	BW_ASSERT(bw_is_finite(cutoff));
	BW_ASSERT(cutoff >= 1e-6f && cutoff <= 1e12f);
	BW_ASSERT(bw_is_finite(Q));
	BW_ASSERT(Q >= 1e-6f && Q <= 1e6f);
	BW_ASSERT(prewarp_at_cutoff ? bw_is_finite(prewarp_freq) : 1);
	BW_ASSERT(prewarp_at_cutoff ? prewarp_freq >= 1e-6f && prewarp_freq <= 1e12f : 1);
#else
	(void)sample_rate;
	(void)cutoff;
	(void)Q;
	(void)prewarp_at_cutoff;
	(void)prewarp_freq;
#endif
}

#define BW_IIR2_ASSERT_VALID_COEFF_PTRS \
	BW_ASSERT(b0 != BW_NULL); \
	BW_ASSERT(b1 != BW_NULL); \
	BW_ASSERT(b2 != BW_NULL); \
	BW_ASSERT(a1 != BW_NULL); \
	BW_ASSERT(a2 != BW_NULL); \
	BW_ASSERT(b0 != b1); \
	BW_ASSERT(b0 != b2); \
	BW_ASSERT(b1 != b2); \
	BW_ASSERT(b0 != a1); \
	BW_ASSERT(b1 != a1); \
	BW_ASSERT(b2 != a1); \
	BW_ASSERT(b0 != a2); \
	BW_ASSERT(b1 != a2); \
	BW_ASSERT(b2 != a2); \
	BW_ASSERT(a1 != a2);

static inline void bw_iir2_coeffs_ap2(
		float               sample_rate,
		float               cutoff,
		float               Q,
		char                prewarp_at_cutoff,
		float               prewarp_freq,
		float * BW_RESTRICT b0,
		float * BW_RESTRICT b1,
		float * BW_RESTRICT b2,
		float * BW_RESTRICT a1,
		float * BW_RESTRICT a2) {
	bw_iir2_assert_valid_params(sample_rate, cutoff, Q, prewarp_at_cutoff, prewarp_freq);
	BW_IIR2_ASSERT_VALID_COEFF_PTRS

	BW_IIR2_COEFFS_COMMON
	*b0 = *a2;
	*b1 = *a1;
	*b2 = 1.f;

	BW_ASSERT(bw_iir2_coeffs_is_valid(*b0, *b1, *b2, *a1, *a2));
}

static inline void bw_iir2_coeffs_bp2(
		float               sample_rate,
		float               cutoff,
		float               Q,
		char                prewarp_at_cutoff,
		float               prewarp_freq,
		float * BW_RESTRICT b0,
		float * BW_RESTRICT b1,
		float * BW_RESTRICT b2,
		float * BW_RESTRICT a1,
		float * BW_RESTRICT a2) {
	bw_iir2_assert_valid_params(sample_rate, cutoff, Q, prewarp_at_cutoff, prewarp_freq);
	BW_IIR2_ASSERT_VALID_COEFF_PTRS

	BW_IIR2_COEFFS_COMMON
	*b0 = Q * k4;
	*b1 = 0.f;
	*b2 = -*b0;

	BW_ASSERT(bw_iir2_coeffs_is_valid(*b0, *b1, *b2, *a1, *a2));
}

static inline void bw_iir2_coeffs_hp2(
		float               sample_rate,
		float               cutoff,
		float               Q,
		char                prewarp_at_cutoff,
		float               prewarp_freq,
		float * BW_RESTRICT b0,
		float * BW_RESTRICT b1,
		float * BW_RESTRICT b2,
		float * BW_RESTRICT a1,
		float * BW_RESTRICT a2) {
	bw_iir2_assert_valid_params(sample_rate, cutoff, Q, prewarp_at_cutoff, prewarp_freq);
	BW_IIR2_ASSERT_VALID_COEFF_PTRS

	BW_IIR2_COEFFS_COMMON
	*b0 = Q * k1;
	*b1 = -(*b0 + *b0);
	*b2 = *b0;

	BW_ASSERT(bw_iir2_coeffs_is_valid(*b0, *b1, *b2, *a1, *a2));
}

static inline void bw_iir2_coeffs_hs2(
		float               sample_rate,
		float               cutoff,
		float               Q,
		char                prewarp_at_cutoff,
		float               prewarp_freq,
		char                high_gain_dB,
		float               high_gain,
		float * BW_RESTRICT b0,
		float * BW_RESTRICT b1,
		float * BW_RESTRICT b2,
		float * BW_RESTRICT a1,
		float * BW_RESTRICT a2) {
	bw_iir2_assert_valid_params(sample_rate, cutoff, Q, prewarp_at_cutoff, prewarp_freq);
	BW_IIR2_ASSERT_VALID_COEFF_PTRS
	BW_ASSERT(bw_is_finite(high_gain));
	BW_ASSERT(high_gain_dB ? high_gain >= -600.f && high_gain <= 600.f : high_gain >= 1e-30f && high_gain <= 1e30f);

	high_gain = high_gain_dB ? bw_dB2linf(high_gain) : high_gain;

	BW_ASSERT_DEEP(cutoff * bw_sqrtf(bw_sqrtf(high_gain)) >= 1e-6f && cutoff * bw_sqrtf(bw_sqrtf(high_gain)) <= 1e12f);

	const float sg = bw_sqrtf(high_gain);
	const float ssg = bw_sqrtf(sg);
	cutoff = cutoff * ssg;
	BW_IIR2_COEFFS_COMMON
	const float k6 = k1 * high_gain;
	const float k7 = k3 - k3 * sg;
	const float k8 = Q * (k7 + k6);
	const float k9 = k4 * sg;
	*b0 = d * (k8 + k9);
	*b1 = d * (Q + Q) * (k7 - k6);
	*b2 = d * (k8 - k9);

	BW_ASSERT(bw_iir2_coeffs_is_valid(*b0, *b1, *b2, *a1, *a2));
}

static inline void bw_iir2_coeffs_lp2(
		float               sample_rate,
		float               cutoff,
		float               Q,
		char                prewarp_at_cutoff,
		float               prewarp_freq,
		float * BW_RESTRICT b0,
		float * BW_RESTRICT b1,
		float * BW_RESTRICT b2,
		float * BW_RESTRICT a1,
		float * BW_RESTRICT a2) {
	bw_iir2_assert_valid_params(sample_rate, cutoff, Q, prewarp_at_cutoff, prewarp_freq);
	BW_IIR2_ASSERT_VALID_COEFF_PTRS

	BW_IIR2_COEFFS_COMMON
	*b0 = Q * k3;
	*b1 = *b0 + *b0;
	*b2 = *b0;

	BW_ASSERT(bw_iir2_coeffs_is_valid(*b0, *b1, *b2, *a1, *a2));
}

static inline void bw_iir2_coeffs_ls2(
		float               sample_rate,
		float               cutoff,
		float               Q,
		char                prewarp_at_cutoff,
		float               prewarp_freq,
		char                dc_gain_dB,
		float               dc_gain,
		float * BW_RESTRICT b0,
		float * BW_RESTRICT b1,
		float * BW_RESTRICT b2,
		float * BW_RESTRICT a1,
		float * BW_RESTRICT a2) {
	bw_iir2_assert_valid_params(sample_rate, cutoff, Q, prewarp_at_cutoff, prewarp_freq);
	BW_IIR2_ASSERT_VALID_COEFF_PTRS
	BW_ASSERT(bw_is_finite(dc_gain));
	BW_ASSERT(dc_gain_dB ? dc_gain >= -600.f && dc_gain <= 600.f : dc_gain >= 1e-30f && dc_gain <= 1e30f);

	dc_gain = dc_gain_dB ? bw_dB2linf(dc_gain) : dc_gain;

	BW_ASSERT_DEEP(cutoff * bw_rcpf(bw_sqrtf(bw_sqrtf(dc_gain))) >= 1e-6f && cutoff * bw_rcpf(bw_sqrtf(bw_sqrtf(dc_gain))) <= 1e12f);

	const float sg = bw_sqrtf(dc_gain);
	const float issg = bw_rcpf(bw_sqrtf(sg));
	cutoff = cutoff * issg;
	BW_IIR2_COEFFS_COMMON
	const float k6 = k3 * (dc_gain - sg);
	const float k7 = Q * (k6 + k1);
	const float k8 = k4 * sg;
	*b0 = d * (k7 + k8);
	*b1 = d * (Q + Q) * (k6 - k1);
	*b2 = d * (k7 - k8);

	BW_ASSERT(bw_iir2_coeffs_is_valid(*b0, *b1, *b2, *a1, *a2));
}

static inline void bw_iir2_coeffs_mm2(
		float               sample_rate,
		float               cutoff,
		float               Q,
		char                prewarp_at_cutoff,
		float               prewarp_freq,
		float               coeff_x,
		float               coeff_lp,
		float               coeff_bp,
		float               coeff_hp,
		float * BW_RESTRICT b0,
		float * BW_RESTRICT b1,
		float * BW_RESTRICT b2,
		float * BW_RESTRICT a1,
		float * BW_RESTRICT a2) {
	bw_iir2_assert_valid_params(sample_rate, cutoff, Q, prewarp_at_cutoff, prewarp_freq);
	BW_IIR2_ASSERT_VALID_COEFF_PTRS

	BW_IIR2_COEFFS_COMMON
	const float k6 = k3 * (coeff_lp + coeff_x);
	const float k7 = k1 * (coeff_hp + coeff_x);
	const float k8 = k4 * (Q * coeff_bp + coeff_x);
	const float k9 = Q * (k6 + k7);
	*b0 = d * (k9 + k8);
	*b1 = d * (Q + Q) * (k6 - k7);
	*b2 = d * (k9 - k8);

	BW_ASSERT(bw_iir2_coeffs_is_valid(*b0, *b1, *b2, *a1, *a2));
}

static inline void bw_iir2_coeffs_notch(
		float               sample_rate,
		float               cutoff,
		float               Q,
		char                prewarp_at_cutoff,
		float               prewarp_freq,
		float * BW_RESTRICT b0,
		float * BW_RESTRICT b1,
		float * BW_RESTRICT b2,
		float * BW_RESTRICT a1,
		float * BW_RESTRICT a2) {
	bw_iir2_assert_valid_params(sample_rate, cutoff, Q, prewarp_at_cutoff, prewarp_freq);
	BW_IIR2_ASSERT_VALID_COEFF_PTRS

	BW_IIR2_COEFFS_COMMON
	*b0 = d * k5;
	*b1 = *a1;
	*b2 = *b0;

	BW_ASSERT(bw_iir2_coeffs_is_valid(*b0, *b1, *b2, *a1, *a2));
}

static inline void bw_iir2_coeffs_peak(
		float               sample_rate,
		float               cutoff,
		char                use_bandwidth,
		float               Q_bandwidth,
		char                prewarp_at_cutoff,
		float               prewarp_freq,
		char                peak_gain_dB,
		float               peak_gain,
		float * BW_RESTRICT b0,
		float * BW_RESTRICT b1,
		float * BW_RESTRICT b2,
		float * BW_RESTRICT a1,
		float * BW_RESTRICT a2) {
	BW_ASSERT(bw_is_finite(Q_bandwidth));
	BW_ASSERT(use_bandwidth ? Q_bandwidth >= 1e-6f && Q_bandwidth <= 90.f : 1);
	BW_ASSERT(bw_is_finite(peak_gain));
	BW_ASSERT(peak_gain_dB ? peak_gain >= -600.f && peak_gain <= 600.f : peak_gain >= 1e-30f && peak_gain <= 1e30f);

	peak_gain = peak_gain_dB ? bw_dB2linf(peak_gain) : peak_gain;
	float Q;
	if (use_bandwidth) {
		const float k6 = bw_pow2f(Q_bandwidth);
		Q = bw_sqrtf(k6 * peak_gain) * bw_rcpf(k6 - 1.f);
	} else
		Q = Q_bandwidth;

	bw_iir2_assert_valid_params(sample_rate, cutoff, Q, prewarp_at_cutoff, prewarp_freq);
	BW_IIR2_ASSERT_VALID_COEFF_PTRS

	BW_IIR2_COEFFS_COMMON
	const float k6 = Q * (k1 + k3);
	const float k7 = k4 * peak_gain;
	*b0 = d * (k6 + k7);
	*b1 = *a1;
	*b2 = d * (k6 - k7);

	BW_ASSERT(bw_iir2_coeffs_is_valid(*b0, *b1, *b2, *a1, *a2));
}

static inline char bw_iir2_coeffs_is_valid(
		float b0,
		float b1,
		float b2,
		float a1,
		float a2) {
	return bw_is_finite(b0)
		&& bw_is_finite(b1)
		&& bw_is_finite(b2)
		&& bw_is_finite(a1)
		&& bw_is_finite(a2)
		&& (bw_absf(a1) <= 2.f && a2 >= bw_absf(a1) - 1.f && a2 <= 1.f);
}

#undef BW_IIR2_COEFFS_COMMON

#if !defined(BW_CXX_NO_EXTERN_C) && defined(__cplusplus)
}
#endif

#if !defined(BW_NO_CXX) && defined(__cplusplus)

# ifndef BW_CXX_NO_ARRAY
#  include <array>
# endif

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::iir2Reset
 *  ```>>> */
template<size_t N_CHANNELS = 1>
void iir2Reset(
	const float *       x0,
	float *             y0,
	float * BW_RESTRICT s10,
	float * BW_RESTRICT s20,
	float               b0,
	float               b1,
	float               b2,
	float               a1,
	float               a2);

# ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS = 1>
void iir2Reset(
	std::array<float, N_CHANNELS>               x0,
	std::array<float, N_CHANNELS> * BW_RESTRICT y0,
	std::array<float, N_CHANNELS> * BW_RESTRICT s10,
	std::array<float, N_CHANNELS> * BW_RESTRICT s20,
	float                                       b0,
	float                                       b1,
	float                                       b2,
	float                                       a1,
	float                                       a2);
# endif

template<size_t N_CHANNELS = 1>
void iir2Process(
	const float * const * x,
	float * const *       y,
	float * BW_RESTRICT   s1,
	float * BW_RESTRICT   s2,
	float                 b0,
	float                 b1,
	float                 b2,
	float                 a1,
	float                 a2,
	size_t                nSamples);

# ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS = 1>
void iir2Process(
	std::array<const float *, N_CHANNELS>       x,
	std::array<float *, N_CHANNELS>             y,
	std::array<float, N_CHANNELS> * BW_RESTRICT s1,
	std::array<float, N_CHANNELS> * BW_RESTRICT s2,
	float                                       b0,
	float                                       b1,
	float                                       b2,
	float                                       a1,
	float                                       a2,
	size_t                                      nSamples);
# endif
/*! <<<```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

template<size_t N_CHANNELS>
inline void iir2Reset(
		const float *       x0,
		float *             y0,
		float * BW_RESTRICT s10,
		float * BW_RESTRICT s20,
		float               b0,
		float               b1,
		float               b2,
		float               a1,
		float               a2) {
	bw_iir2_reset_multi(x0, y0, s10, s20, b0, b1, b2, a1, a2, N_CHANNELS);
}

# ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void iir2Reset(
		std::array<float, N_CHANNELS>               x0,
		std::array<float, N_CHANNELS> * BW_RESTRICT y0,
		std::array<float, N_CHANNELS> * BW_RESTRICT s10,
		std::array<float, N_CHANNELS> * BW_RESTRICT s20,
		float                                       b0,
		float                                       b1,
		float                                       b2,
		float                                       a1,
		float                                       a2) {
	iir2Reset<N_CHANNELS>(x0.data(), y0 != BW_NULL ? y0->data() : BW_NULL, s10 != BW_NULL ? s10->data() : BW_NULL, s20 != BW_NULL ? s20->data() : BW_NULL, b0, b1, b2, a1, a2);
}
# endif

template<size_t N_CHANNELS>
inline void iir2Process(
		const float * const * x,
		float * const *       y,
		float * BW_RESTRICT   s1,
		float * BW_RESTRICT   s2,
		float                 b0,
		float                 b1,
		float                 b2,
		float                 a1,
		float                 a2,
		size_t                nSamples) {
	bw_iir2_process_multi(x, y, s1, s2, b0, b1, b2, a1, a2, N_CHANNELS, nSamples);
}

# ifndef BW_CXX_NO_ARRAY
template<size_t N_CHANNELS>
inline void iir2Process(
		std::array<const float *, N_CHANNELS>       x,
		std::array<float *, N_CHANNELS>             y,
		std::array<float, N_CHANNELS> * BW_RESTRICT s1,
		std::array<float, N_CHANNELS> * BW_RESTRICT s2,
		float                                       b0,
		float                                       b1,
		float                                       b2,
		float                                       a1,
		float                                       a2,
		size_t                                      nSamples) {
	iir2Process<N_CHANNELS>(x.data(), y.data(), s1->data(), s2->data(), b0, b1, b2, a1, a2, nSamples);
}
# endif

}
#endif

#endif
