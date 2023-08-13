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
 *  requires {{{ bw_common bw_math bw_one_pole }}}
 *  description {{{
 *    State variable filter (2nd order, 12 dB/oct) model with separated lowpass,
 *    bandpass, and highpass outputs.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li><code>bw_svf_process()</code> and
 *              <code>bw_svf_process_multi()</code> now use <code>size_t</code>
 *              to count samples and channels.</li>
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
 *          <li>Added <code>bw_svf_process_multi()</code>.</li>
 *          <li>Fixed bug in <code>bw_svf_process()</code> when only
 *              <code>y_hp</code> is <code>NULL</code>.</li>
 *          <li>Fixed prewarping-related stability bug.</li>
 *          <li>Added C++ wrapper.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.4.0</strong>:
 *        <ul>
 *          <li>Added initial input value to
 *              <code>bw_svf_reset_state()</code>.</li>
 *          <li>Fixed unused parameter warnings.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.3.0</strong>:
 *        <ul>
 *          <li>Strenghtened algorithm for modulation.</li>
 *          <li>Added prewarping control parameters (prewarp_at_cutoff and
 *              prewarp_freq).</li>
 *          <li>Added <code>BW_RESTRICT</code> to
 *              <code>bw_svf_process1()</code>.</li>
 *          <li>Fixed typo in <code>bw_svf_set_Q()</code> documentation.</li>
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

#ifndef BW_SVF_H
#define BW_SVF_H

#include <bw_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! api {{{
 *    #### bw_svf_coeffs
 *  ```>>> */
typedef struct bw_svf_coeffs bw_svf_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_svf_state
 *  ```>>> */
typedef struct bw_svf_state bw_svf_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_svf_init()
 *  ```>>> */
static inline void bw_svf_init(bw_svf_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_svf_set_sample_rate()
 *  ```>>> */
static inline void bw_svf_set_sample_rate(bw_svf_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_svf_reset_coeffs()
 *  ```>>> */
static inline void bw_svf_reset_coeffs(bw_svf_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_svf_reset_state()
 *  ```>>> */
static inline void bw_svf_reset_state(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`
 *    and the quiescent/initial input value `x_0`.
 *
 *    #### bw_svf_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_svf_update_coeffs_ctrl(bw_svf_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_svf_update_coeffs_audio()
 *  ```>>> */
static inline void bw_svf_update_coeffs_audio(bw_svf_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_svf_process1()
 *  ```>>> */
static inline void bw_svf_process1(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x, float *BW_RESTRICT y_lp, float *BW_RESTRICT y_bp, float *BW_RESTRICT y_hp);
/*! <<<```
 *    Processes one input sample `x` using `coeffs`, while using and updating
 *    `state`. The lowpass, bandpass, and highpass output samples are put into
 *    `y_lp`, `y_bp`, and `y_hp` respectively.
 *
 *    #### bw_svf_process()
 *  ```>>> */
static inline void bw_svf_process(bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, const float *x, float *y_lp, float *y_bp, float *y_hp, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the input buffer `x` and fills the
 *    first `n_samples` of the output buffers `y_lp` (lowpass), `y_bp`
 *    (bandpass), and `y_hp` (highpass), if they are not `NULL`, while using and
 *    updating both `coeffs` and `state` (control and audio rate).
 * 
 *    #### bw_svf_process_multi()
 *  ```>>> */
static inline void bw_svf_process_multi(bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state * const *BW_RESTRICT state, const float * const *x, float * const *y_lp, float * const *y_bp, float * const *y_hp, size_t n_channels, size_t n_samples);
/*! <<<```
 *    Processes the first `n_samples` of the `n_channels` input buffers `x` and
 *    fills the first `n_samples` of the `n_channels` output buffers `y_lp`
 *    (lowpass), `y_bp` (bandpass), and `y_hp` (highpass), while using and
 *    updating both the common `coeffs` and each of the `n_channels` `state`s
 *    (control and audio rate).
 * 
 *    `y_lp`, `y_bp`, and `y_hp`, or any of their elements may be `NULL`.
 *
 *    #### bw_svf_set_cutoff()
 *  ```>>> */
static inline void bw_svf_set_cutoff(bw_svf_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the cutoff frequency to the given `value` (Hz) in `coeffs`.
 *
 *    `value` must be positive and smaller than the Nyquist frequency (half the
 *    sample rate).
 *
 *    Default value: `1e3f`.
 *
 *    #### bw_svf_set_Q()
 *  ```>>> */
static inline void bw_svf_set_Q(bw_svf_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the quality factor to the given `value` in `coeffs`.
 *
 *    `value` must be equal or bigger than `0.5f`.
 *
 *    Default value: `0.5f`.
 * 
 *    #### bw_svf_set_prewarp_at_cutoff()
 *  ```>>> */
static inline void bw_svf_set_prewarp_at_cutoff(bw_svf_coeffs *BW_RESTRICT coeffs, char value);
/*! <<<```
 *    Sets whether bilinear transform prewarping frequency should match the
 *    cutoff frequency (non-`0`) or not (`0`).
 *
 *    Default value: non-`0` (on).
 *
 *    #### bw_svf_set_prewarp_freq()
 *  ```>>> */
static inline void bw_svf_set_prewarp_freq(bw_svf_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the prewarping frequency `value` (Hz) in `coeffs`.
 *
 *    Only used when the prewarp\_at\_cutoff parameter is off.
 *
 *    Default value: `1e3f`.
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

struct bw_svf_coeffs {
	// Sub-components
	bw_one_pole_coeffs	smooth_coeffs;
	bw_one_pole_state	smooth_cutoff_state;
	bw_one_pole_state	smooth_Q_state;
	bw_one_pole_state	smooth_prewarp_freq_state;
	
	// Coefficients
	float			t_k;

	float			prewarp_k;
	float			t;
	float			kf;
	float			kbl;
	float			k;
	float			hp_hb;
	float			hp_x;

	// Parameters
	float			cutoff;
	float			Q;
	float			prewarp_freq;
};

struct bw_svf_state {
	float	hp_z1;
	float	lp_z1;
	float	bp_z1;
	float	cutoff_z1;
};

static inline void bw_svf_init(bw_svf_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->smooth_coeffs);
	bw_one_pole_set_tau(&coeffs->smooth_coeffs, 0.005f);
	bw_one_pole_set_sticky_thresh(&coeffs->smooth_coeffs, 1e-3f);
	coeffs->cutoff = 1e3f;
	coeffs->Q = 0.5f;
	coeffs->prewarp_freq = 1e3f;
	coeffs->prewarp_k = 1.f;
}

static inline void bw_svf_set_sample_rate(bw_svf_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->smooth_coeffs, sample_rate);
	bw_one_pole_reset_coeffs(&coeffs->smooth_coeffs);
	coeffs->t_k = 3.141592653589793f / sample_rate;
}

static inline void bw_svf_do_update_coeffs(bw_svf_coeffs *BW_RESTRICT coeffs, char force) {
	const float prewarp_freq = coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq);
	float cutoff_cur = bw_one_pole_get_y_z1(&coeffs->smooth_cutoff_state);
	float prewarp_freq_cur = bw_one_pole_get_y_z1(&coeffs->smooth_prewarp_freq_state);
	float Q_cur = bw_one_pole_get_y_z1(&coeffs->smooth_Q_state);
	const char cutoff_changed = force || coeffs->cutoff != cutoff_cur;
	const char prewarp_freq_changed = force || prewarp_freq != prewarp_freq_cur;
	const char Q_changed = force || coeffs->Q != Q_cur;
	if (cutoff_changed || prewarp_freq_changed || Q_changed) {
		if (cutoff_changed || prewarp_freq_changed) {
			if (cutoff_changed)
				cutoff_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
			if (prewarp_freq_changed) {
				prewarp_freq_cur = bw_one_pole_process1_sticky_rel(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, prewarp_freq);
				coeffs->t = bw_tanf(coeffs->t_k * prewarp_freq_cur);
				coeffs->kf = coeffs->t * bw_rcpf(prewarp_freq_cur);
			}
			coeffs->kbl = coeffs->kf * cutoff_cur;
		}
		if (Q_changed) {
			Q_cur = bw_one_pole_process1_sticky_abs(&coeffs->smooth_coeffs, &coeffs->smooth_Q_state, coeffs->Q);
			coeffs->k = bw_rcpf(Q_cur);
		}
		coeffs->hp_hb = coeffs->k + coeffs->kbl;
		coeffs->hp_x = bw_rcpf(1.f + coeffs->kbl * coeffs->hp_hb);
	}
}

static inline void bw_svf_reset_coeffs(bw_svf_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_cutoff_state, coeffs->cutoff);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_Q_state, coeffs->Q);
	bw_one_pole_reset_state(&coeffs->smooth_coeffs, &coeffs->smooth_prewarp_freq_state, coeffs->prewarp_freq + coeffs->prewarp_k * (coeffs->cutoff - coeffs->prewarp_freq));
	bw_svf_do_update_coeffs(coeffs, 1);
}

static inline void bw_svf_reset_state(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x_0) {
	state->hp_z1 = 0.f;
	state->lp_z1 = x_0;
	state->bp_z1 = 0.f;
	state->cutoff_z1 = coeffs->cutoff;
}

static inline void bw_svf_update_coeffs_ctrl(bw_svf_coeffs *BW_RESTRICT coeffs) {
	(void)coeffs;
}

static inline void bw_svf_update_coeffs_audio(bw_svf_coeffs *BW_RESTRICT coeffs) {
	bw_svf_do_update_coeffs(coeffs, 0);
}

static inline void bw_svf_process1(const bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, float x, float *BW_RESTRICT y_lp, float *BW_RESTRICT y_bp, float *BW_RESTRICT y_hp) {
	const float kk = coeffs->kf * state->cutoff_z1;
	const float lp_xz1 = state->lp_z1 - kk * state->bp_z1;
	const float bp_xz1 = state->bp_z1 - kk * state->hp_z1;
	*y_hp = coeffs->hp_x * (x + coeffs->hp_hb * bp_xz1 - lp_xz1);
	*y_bp = bp_xz1 - coeffs->kbl * *y_hp;
	*y_lp = lp_xz1 - coeffs->kbl * *y_bp;
	state->hp_z1 = *y_hp;
	state->lp_z1 = *y_lp;
	state->bp_z1 = *y_bp;
	state->cutoff_z1 = bw_one_pole_get_y_z1(&coeffs->smooth_cutoff_state);
}

static inline void bw_svf_process(bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state *BW_RESTRICT state, const float *x, float *y_lp, float *y_bp, float *y_hp, size_t n_samples) {
	if (y_lp != NULL) {
		if (y_bp != NULL) {
			if (y_hp != NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					bw_svf_process1(coeffs, state, x[i], y_lp + i, y_bp + i, y_hp + i);
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_hp;
					bw_svf_process1(coeffs, state, x[i], y_lp + i, y_bp + i, &v_hp);
				}
			}
		} else {
			if (y_hp != NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_bp;
					bw_svf_process1(coeffs, state, x[i], y_lp + i, &v_bp, y_hp + i);
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_bp, v_hp;
					bw_svf_process1(coeffs, state, x[i], y_lp + i, &v_bp, &v_hp);
				}
			}
		}
	} else {
		if (y_bp != NULL) {
			if (y_hp != NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, y_bp + i, y_hp + i);
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp, v_hp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, y_bp + i, &v_hp);
				}
			}
		} else {
			if (y_hp != NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp, v_bp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, &v_bp, y_hp + i);
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					float v_lp, v_bp, v_hp;
					bw_svf_process1(coeffs, state, x[i], &v_lp, &v_bp, &v_hp);
				}
			}
		}
	}
}

static inline void bw_svf_process_multi(bw_svf_coeffs *BW_RESTRICT coeffs, bw_svf_state * const *BW_RESTRICT state, const float * const *x, float * const *y_lp, float * const *y_bp, float * const *y_hp, size_t n_channels, size_t n_samples) {
	if (y_lp != NULL) {
		if (y_bp != NULL) {
			if (y_hp != NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_lp[j])
							y_lp[j][i] = v_lp;
						if (y_bp[j])
							y_bp[j][i] = v_bp;
						if (y_hp[j])
							y_hp[j][i] = v_hp;
					}
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_lp[j])
							y_lp[j][i] = v_lp;
						if (y_bp[j])
							y_bp[j][i] = v_bp;
					}
				}
			}
		} else {
			if (y_hp != NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_lp[j])
							y_lp[j][i] = v_lp;
						if (y_hp[j])
							y_hp[j][i] = v_hp;
					}
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_lp[j])
							y_lp[j][i] = v_lp;
					}
				}
			}
		}
	} else {
		if (y_bp != NULL) {
			if (y_hp != NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_bp[j])
							y_bp[j][i] = v_bp;
						if (y_hp[j])
							y_hp[j][i] = v_hp;
					}
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_bp[j])
							y_bp[j][i] = v_bp;
					}
				}
			}
		} else {
			if (y_hp != NULL) {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
						if (y_hp[j])
							y_hp[j][i] = v_hp;
					}
				}
			} else {
				for (size_t i = 0; i < n_samples; i++) {
					bw_svf_update_coeffs_audio(coeffs);
					for (size_t j = 0; j < n_channels; j++) {
						float v_lp, v_bp, v_hp;
						bw_svf_process1(coeffs, state[j], x[j][i], &v_lp, &v_bp, &v_hp);
					}
				}
			}
		}
	}
}

static inline void bw_svf_set_cutoff(bw_svf_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->cutoff = value;
}

static inline void bw_svf_set_Q(bw_svf_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->Q = value;
}

static inline void bw_svf_set_prewarp_at_cutoff(bw_svf_coeffs *BW_RESTRICT coeffs, char value) {
	coeffs->prewarp_k = value ? 1.f : 0.f;
}

static inline void bw_svf_set_prewarp_freq(bw_svf_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->prewarp_freq = value;
}

#ifdef __cplusplus
}

#include <array>

namespace Brickworks {

/*** Public C++ API ***/

/*! api_cpp {{{
 *    ##### Brickworks::SVF
 *  ```>>> */
template<size_t N_CHANNELS>
class SVF {
public:
	SVF();

	void setSampleRate(float sampleRate);
	void reset(float x_0 = 0.f);
	void process(
		const float * const *x,
		float * const *y_lp,
		float * const *y_bp,
		float * const *y_hp,
		size_t nSamples);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y_lp,
		std::array<float *, N_CHANNELS> y_bp,
		std::array<float *, N_CHANNELS> y_hp,
		size_t nSamples);

	void setCutoff(float value);
	void setQ(float value);
	void setPrewarpAtCutoff(bool value);
	void setPrewarpFreq(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_svf_coeffs		 coeffs;
	bw_svf_state		 states[N_CHANNELS];
	bw_svf_state		*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline SVF<N_CHANNELS>::SVF() {
	bw_svf_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_svf_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::reset(float x_0) {
	bw_svf_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_svf_reset_state(&coeffs, states + i, x_0);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::process(
		const float * const *x,
		float * const *y_lp,
		float * const *y_bp,
		float * const *y_hp,
		size_t nSamples) {
	bw_svf_process_multi(&coeffs, statesP, x, y_lp, y_bp, y_hp, N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y_lp,
		std::array<float *, N_CHANNELS> y_bp,
		std::array<float *, N_CHANNELS> y_hp,
		size_t nSamples) {
	process(x.data(), y_lp.data(), y_bp.data(), y_hp.data(), nSamples);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setCutoff(float value) {
	bw_svf_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setQ(float value) {
	bw_svf_set_Q(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setPrewarpAtCutoff(bool value) {
	bw_svf_set_prewarp_at_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void SVF<N_CHANNELS>::setPrewarpFreq(float value) {
	bw_svf_set_prewarp_freq(&coeffs, value);
}

}
#endif

#endif
