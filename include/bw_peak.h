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
 *  requires {{{ bw_common bw_gain bw_math bw_mm2 bw_one_pole bw_svf }}}
 *  description {{{
 *    Second-order peak filter with unitary gain at DC and asymptotically
 *    as frequency increases.
 *
 *    The quality factor of the underlying bandpass filter can be either
 *    directly controlled via the Q parameter or indirectly through the
 *    bandwidth parameter, which designates the distance in octaves between
 *    midpoint gain frequencies, i.e., frequencies with gain = peak gain / 2 in
 *    dB terms. The use_bandiwdth parameter allows you to choose which
 *    parameterization to use.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_peak_process_multi()</code>.</li>
 *          <li>Fixed documentation for <code>bw_peak_set_peak_gain_lin()</code>
 *              and <code>bw_peak_set_gain_dB()</code>.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_peak_reset_state()</code>.</li>
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

#ifndef _BW_PEAK_H
#define _BW_PEAK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_peak_coeffs
 *  ```>>> */
typedef struct _bw_peak_coeffs bw_peak_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_peak_state
 *  ```>>> */
typedef struct _bw_peak_state bw_peak_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_peak_init()
 *  ```>>> */
static inline void bw_peak_init(bw_peak_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_peak_set_sample_rate()
 *  ```>>> */
static inline void bw_peak_set_sample_rate(bw_peak_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_peak_reset_coeffs()
 *  ```>>> */
static inline void bw_peak_reset_coeffs(bw_peak_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_peak_reset_state()
 *  ```>>> */
static inline void bw_peak_reset_state(const bw_peak_coeffs *BW_RESTRICT coeffs, bw_peak_state *BW_RESTRICT state, float x0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x0`.
 *
 *    #### bw_peak_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_peak_update_coeffs_ctrl(bw_peak_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_peak_update_coeffs_audio()
 *  ```>>> */
static inline void bw_peak_update_coeffs_audio(bw_peak_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_peak_process1()
 *  ```>>> */
static inline float bw_peak_process1(const bw_peak_coeffs *BW_RESTRICT coeffs, bw_peak_state *BW_RESTRICT state, float x);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. Returns the corresponding output sample.
 *
 *    #### bw_peak_process()
 *  ```>>> */
static inline void bw_peak_process(bw_peak_coeffs *BW_RESTRICT coeffs, bw_peak_state *BW_RESTRICT state, const float *x, float *y, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffer `y`, while using and updating both
 *    `coeffs` and `state` (control and audio rate).
 *
 *    #### bw_peak_process_multi()
 *  ```>>> */
static inline void bw_peak_process_multi(bw_peak_coeffs *BW_RESTRICT coeffs, bw_peak_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y`, while
 *    using and updating both the common `coeffs` and each of the `n_channels`
 *    `state`s (control and audio rate).
 *
 *    #### bw_peak_set_cutoff()
 *  ```>>> */
static inline void bw_peak_set_cutoff(bw_peak_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency `value` (Hz) in `coeffs`.
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_peak_set_Q()
 *  ```>>> */
static inline void bw_peak_set_Q(bw_peak_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the quality factor to the given `value` in `coeffs`.
 *
 *    `value` must be equal or bigger than `0.5f`.
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_peak_set_peak_gain_lin()
 *  ```>>> */
static inline void bw_peak_set_peak_gain_lin(bw_peak_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the peak gain parameter to the given `value` (linear gain) in
 *    `coeffs`.
 *
 *    Default value: `0.f`.
 *
 *    #### bw_peak_set_peak_gain_dB()
 *  ```>>> */
static inline void bw_peak_set_peak_gain_dB(bw_peak_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the peak gain parameter to the given `value` (dB) in `coeffs`.
 *
 *    Default value: `-INFINITY`.
 *
 *    #### bw_peak_set_bandiwdth()
 *  ```>>> */
static inline void bw_peak_set_bandwidth(bw_peak_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the bandwidth `value` (octaves) in `coeffs`.
 *
 *    Default value: `0.5f`.
 *
 *    #### bw_peak_set_use_bandwidth()
 *  ```>>> */
static inline void bw_peak_set_use_bandwidth(bw_peak_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether the quality factor should be controlled via the bandwidth
 *    parameter (`value` non-`0`) or via the Q parameter (`0`).
 *
 *    Default value: non-`0` (use bandwidth parameter).
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_mm2.h>
#include <bw_math.h>

struct _bw_peak_coeffs {
	// Sub-components
	bw_mm2_coeffs	mm2_coeffs;

	// Coefficients
	float		bw_k;

	// Parameters
	float		peak_gain;
	float		Q;
	float		bandwidth;
	char		use_bandwidth;
	int		param_changed;
};

struct _bw_peak_state {
	bw_mm2_state	mm2_state;
};

#define _BW_PEAK_PARAM_PEAK_GAIN	1
#define _BW_PEAK_PARAM_Q		(1<<1)
#define _BW_PEAK_PARAM_BANDWIDTH	(1<<2)

static inline void bw_peak_init(bw_peak_coeffs *BW_RESTRICT coeffs) {
	bw_mm2_init(&coeffs->mm2_coeffs);
	coeffs->peak_gain = 1.f;
	coeffs->Q = 0.5f;
	coeffs->bandwidth = 2.543106606327224f;
	coeffs->use_bandwidth = 1;
}

static inline void bw_peak_set_sample_rate(bw_peak_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_mm2_set_sample_rate(&coeffs->mm2_coeffs, sample_rate);
}

static inline void _bw_peak_update_mm2_params(bw_peak_coeffs *BW_RESTRICT coeffs) {
	if (coeffs->param_changed) {
		if (coeffs->use_bandwidth) {
			if (coeffs->param_changed & (_BW_PEAK_PARAM_PEAK_GAIN | _BW_PEAK_PARAM_BANDWIDTH)) {
				if (coeffs->param_changed & _BW_PEAK_PARAM_BANDWIDTH)
					coeffs->bw_k = bw_pow2f(coeffs->bandwidth);
				const float Q = bw_sqrtf(coeffs->bw_k * coeffs->peak_gain) * bw_rcpf(coeffs->bw_k - 1.f);
				bw_mm2_set_Q(&coeffs->mm2_coeffs, Q);
				bw_mm2_set_coeff_bp(&coeffs->mm2_coeffs, (coeffs->peak_gain - 1.f) * bw_rcpf(Q));
			}
		} else {
			if (coeffs->param_changed & (_BW_PEAK_PARAM_PEAK_GAIN | _BW_PEAK_PARAM_Q)) {
				if (coeffs->param_changed & _BW_PEAK_PARAM_Q)
					bw_mm2_set_Q(&coeffs->mm2_coeffs, coeffs->Q);
				bw_mm2_set_coeff_bp(&coeffs->mm2_coeffs, (coeffs->peak_gain - 1.f) * bw_rcpf(coeffs->Q));
			}
		}
		coeffs->param_changed = 0;
	}
}

static inline void bw_peak_reset_coeffs(bw_peak_coeffs *BW_RESTRICT coeffs) {
	coeffs->param_changed = ~0;
	_bw_peak_update_mm2_params(coeffs);
	bw_mm2_reset_coeffs(&coeffs->mm2_coeffs);
}

static inline void bw_peak_reset_state(const bw_peak_coeffs *BW_RESTRICT coeffs, bw_peak_state *BW_RESTRICT state, float x0) {
	bw_mm2_reset_state(&coeffs->mm2_coeffs, &state->mm2_state, x0);
}

static inline void bw_peak_update_coeffs_ctrl(bw_peak_coeffs *BW_RESTRICT coeffs) {
	_bw_peak_update_mm2_params(coeffs);
	bw_mm2_update_coeffs_ctrl(&coeffs->mm2_coeffs);
}

static inline void bw_peak_update_coeffs_audio(bw_peak_coeffs *BW_RESTRICT coeffs) {
	bw_mm2_update_coeffs_audio(&coeffs->mm2_coeffs);
}

static inline float bw_peak_process1(const bw_peak_coeffs *BW_RESTRICT coeffs, bw_peak_state *BW_RESTRICT state, float x) {
	return bw_mm2_process1(&coeffs->mm2_coeffs, &state->mm2_state, x);
}

static inline void bw_peak_process(bw_peak_coeffs *BW_RESTRICT coeffs, bw_peak_state *BW_RESTRICT state, const float *x, float *y, int n_samples) {
	bw_peak_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_peak_update_coeffs_audio(coeffs);
		y[i] = bw_peak_process1(coeffs, state, x[i]);
	}
}

static inline void bw_peak_process_multi(bw_peak_coeffs *BW_RESTRICT coeffs, bw_peak_state **BW_RESTRICT state, const float **x, float **y, int n_channels, int n_samples) {
	bw_peak_update_coeffs_ctrl(coeffs);
	for (int i = 0; i < n_samples; i++) {
		bw_peak_update_coeffs_audio(coeffs);
		for (int j = 0; j < n_channels; j++)
			y[j][i] = bw_peak_process1(coeffs, state[j], x[j][i]);
	}
}

static inline void bw_peak_set_cutoff(bw_peak_coeffs *BW_RESTRICT coeffs, float value) {
	bw_mm2_set_cutoff(&coeffs->mm2_coeffs, value);
}

static inline void bw_peak_set_Q(bw_peak_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->Q != value) {
		coeffs->Q = value;
		coeffs->param_changed |= _BW_PEAK_PARAM_Q;
	}
}

static inline void bw_peak_set_peak_gain_lin(bw_peak_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->peak_gain != value) {
		coeffs->peak_gain = value;
		coeffs->param_changed |= _BW_PEAK_PARAM_PEAK_GAIN;
	}
}

static inline void bw_peak_set_peak_gain_dB(bw_peak_coeffs *BW_RESTRICT coeffs, float value) {
	bw_peak_set_peak_gain_lin(coeffs, bw_dB2linf(value));
}

static inline void bw_peak_set_bandwidth(bw_peak_coeffs *BW_RESTRICT coeffs, float value) {
	if (coeffs->bandwidth != value) {
		coeffs->bandwidth = value;
		coeffs->param_changed |= _BW_PEAK_PARAM_BANDWIDTH;
	}
}

static inline void bw_peak_set_use_bandwidth(bw_peak_coeffs *BW_RESTRICT coeffs, char value) {
	if ((coeffs->use_bandwidth && !value) || (!coeffs->use_bandwidth && value)) {
		coeffs->use_bandwidth = value;
		coeffs->param_changed |= _BW_PEAK_PARAM_Q | _BW_PEAK_PARAM_BANDWIDTH;
	}
}

#undef _BW_PEAK_PARAM_PEAK_GAIN
#undef _BW_PEAK_PARAM_Q
#undef _BW_PEAK_PARAM_BANDWIDTH

#ifdef __cplusplus
}
#endif

#endif
