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
 *  version {{{ 0.5.0 }}}
 *  requires {{{ bw_common bw_config bw_math bw_one_pole }}}
 *  description {{{
 *    Phase generator with portamento and exponential frequency modulation.
 *
 *    It outputs a normalized phase signal (range [`0.f`, `1.f`]).
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>Added <code>bw_phase_gen_process_multi()</code>.</li>
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
 *          <li>Added <code>BW_RESTRICT</code> to
 *              <code>bw_phase_gen_process1*()</code>.</li>
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

#ifndef _BW_PHASE_GEN_H
#define _BW_PHASE_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_phase_gen_coeffs
 *  ```>>> */
typedef struct _bw_phase_gen_coeffs bw_phase_gen_coeffs;
/*! <<<```
 *    Coefficients and related.
 *
 *    #### bw_phase_gen_state
 *  ```>>> */
typedef struct _bw_phase_gen_state bw_phase_gen_state;
/*! <<<```
 *    Internal state and related.
 *
 *    #### bw_phase_gen_init()
 *  ```>>> */
static inline void bw_phase_gen_init(bw_phase_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Initializes input parameter values in `coeffs`.
 *
 *    #### bw_phase_gen_set_sample_rate()
 *  ```>>> */
static inline void bw_phase_gen_set_sample_rate(bw_phase_gen_coeffs *BW_RESTRICT coeffs, float sample_rate);
/*! <<<```
 *    Sets the `sample_rate` (Hz) value in `coeffs`.
 *
 *    #### bw_phase_gen_reset_coeffs()
 *  ```>>> */
static inline void bw_phase_gen_reset_coeffs(bw_phase_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Resets coefficients in `coeffs` to assume their target values.
 *
 *    #### bw_phase_gen_reset_state()
 *  ```>>> */
static inline void bw_phase_gen_reset_state(const bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state *BW_RESTRICT state, float phase_0);
/*! <<<```
 *    Resets the given `state` to its initial values using the given `coeffs`.
 *
 *    #### bw_phase_gen_update_coeffs_ctrl()
 *  ```>>> */
static inline void bw_phase_gen_update_coeffs_ctrl(bw_phase_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers control-rate update of coefficients in `coeffs`.
 *
 *    #### bw_phase_gen_update_coeffs_audio()
 *  ```>>> */
static inline void bw_phase_gen_update_coeffs_audio(bw_phase_gen_coeffs *BW_RESTRICT coeffs);
/*! <<<```
 *    Triggers audio-rate update of coefficients in `coeffs`.
 *
 *    #### bw_phase_gen_process1\*()
 *  ```>>> */
static inline void bw_phase_gen_process1(const bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state *BW_RESTRICT state, float *BW_RESTRICT y, float *BW_RESTRICT y_phase_inc);
static inline void bw_phase_gen_process1_mod(const bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state *BW_RESTRICT state, float x_mod, float *BW_RESTRICT y, float *BW_RESTRICT y_phase_inc);
/*! <<<```
 *    These functions generate and return one sample using `coeffs`, while using
 *    and updating `state`, and put the corresponding phase increment value in
 *    `y_phase_inc`.
 *
 *    In particular:
 *     * `bw_phase_gen_process1()` does not apply exponential frequency
 *       modulation;
 *     * `bw_phase_gen_process1_mod()` applies exponential frequency modulation
 *       using `x_mod` as modulation input (scale `1.f`/octave).
 *
 *    #### bw_phase_gen_process()
 *  ```>>> */
static inline void bw_phase_gen_process(bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state *BW_RESTRICT state, const float *x_mod, float *y, float *y_phase_inc, int n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the output buffer `y`, while
 *    using and updating both `coeffs` and `state` (control and audio rate).
 *
 *    If `x_mod` is not `NULL`, it is used as a source of exponential frequency
 *    modulation (scale `1.f`/octave).
 *
 *    If `y_inc` is not `NULL`, it is filled with phase increment values.
 *
 *    #### bw_phase_gen_process_multi()
 *  ```>>> */
static inline void bw_phase_gen_process_multi(bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state **BW_RESTRICT state, const float **x_mod, float **y, float **y_phase_inc, int n_channels, int n_samples);
/*! <<<```
 *    Generates and fills the first `n_samples` of the `n_channels` output
 *    buffers `y`, while using and updating both the common `coeffs` and each of
 *    the `n_channels` `state`s (control and audio rate).
 *
 *    If `x_mod` and the channel-specific element are not `NULL`, this is used
 *    as a source of exponential frequency modulation (scale `1.f`/octave) for
 *    that channel.
 *
 *    If `y_inc` and the channel-specific element are not `NULL`, this is filled
 *    with phase increment values for that channel.
 *
 *    #### bw_phase_gen_set_frequency()
 *  ```>>> */
static inline void bw_phase_gen_set_frequency(bw_phase_gen_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the base frequency to `value` (Hz) in `coeffs`.
 *    
 *    Default value: `1.f`.
 *
 *    #### bw_phase_gen_set_portamento_tau()
 *  ```>>> */
static inline void bw_phase_gen_set_portamento_tau(bw_phase_gen_coeffs *BW_RESTRICT coeffs, float value);
/*! <<<```
 *    Sets the portamento time constant `value` (s) in `coeffs`.
 *
 *    Default value: `0.f`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#include <bw_math.h>
#include <bw_one_pole.h>

struct _bw_phase_gen_coeffs {
	// Sub-components
	bw_one_pole_coeffs	portamento_coeffs;
	bw_one_pole_state	portamento_state;

	// Coefficients
	float			T;

	float			portamento_target;

	// Parameters
	float			frequency;
	float			frequency_prev;
};

struct _bw_phase_gen_state {
	float	phase;
};

static inline void bw_phase_gen_init(bw_phase_gen_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_init(&coeffs->portamento_coeffs);
	coeffs->frequency = 1.f;
}

static inline void bw_phase_gen_set_sample_rate(bw_phase_gen_coeffs *BW_RESTRICT coeffs, float sample_rate) {
	bw_one_pole_set_sample_rate(&coeffs->portamento_coeffs, sample_rate);
	coeffs->T = 1.f / sample_rate;
}

static inline void _bw_phase_gen_do_update_coeffs_ctrl(bw_phase_gen_coeffs *BW_RESTRICT coeffs, char force) {
	bw_one_pole_update_coeffs_ctrl(&coeffs->portamento_coeffs);
	if (force || coeffs->frequency != coeffs->frequency_prev) {
		coeffs->portamento_target = coeffs->T * coeffs->frequency;
		coeffs->frequency_prev = coeffs->frequency;
	}
}

static inline void bw_phase_gen_reset_coeffs(bw_phase_gen_coeffs *BW_RESTRICT coeffs) {
	_bw_phase_gen_do_update_coeffs_ctrl(coeffs, 1);
	bw_one_pole_reset_coeffs(&coeffs->portamento_coeffs);
	bw_one_pole_reset_state(&coeffs->portamento_coeffs, &coeffs->portamento_state, coeffs->portamento_target);
}

static inline void bw_phase_gen_reset_state(const bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state *BW_RESTRICT state, float phase_0) {
	(void)coeffs;
	state->phase = phase_0;
}

static inline void bw_phase_gen_update_coeffs_ctrl(bw_phase_gen_coeffs *BW_RESTRICT coeffs) {
	_bw_phase_gen_do_update_coeffs_ctrl(coeffs, 0);
}

static inline void bw_phase_gen_update_coeffs_audio(bw_phase_gen_coeffs *BW_RESTRICT coeffs) {
	bw_one_pole_process1(&coeffs->portamento_coeffs, &coeffs->portamento_state, coeffs->portamento_target);
}

static inline float _bw_phase_gen_update_phase(bw_phase_gen_state *BW_RESTRICT state, float phase_inc) {
	state->phase += phase_inc;
	state->phase -= bw_floorf(state->phase);
	return state->phase;
}

static inline void bw_phase_gen_process1(const bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state *BW_RESTRICT state, float *BW_RESTRICT y, float *BW_RESTRICT y_phase_inc) {
	*y_phase_inc = bw_one_pole_get_y_z1(&coeffs->portamento_state);
	*y = _bw_phase_gen_update_phase(state, *y_phase_inc);
}

static inline void bw_phase_gen_process1_mod(const bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state *BW_RESTRICT state, float x_mod, float *BW_RESTRICT y, float *BW_RESTRICT y_phase_inc) {
	*y_phase_inc = bw_one_pole_get_y_z1(&coeffs->portamento_state) * bw_pow2f_3(x_mod);
	*y = _bw_phase_gen_update_phase(state, *y_phase_inc);
}

static inline void bw_phase_gen_process(bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state *BW_RESTRICT state, const float *x_mod, float* y, float *y_phase_inc, int n_samples) {
	bw_phase_gen_update_coeffs_ctrl(coeffs);
	if (y != NULL) {
		if (x_mod != NULL) {
			if (y_phase_inc != NULL)
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					bw_phase_gen_process1_mod(coeffs, state, x_mod[i], y + i, y_phase_inc + i);
				}
			else
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v_phase_inc;
					bw_phase_gen_process1_mod(coeffs, state, x_mod[i], y + i, &v_phase_inc);
				}
		} else {
			if (y_phase_inc != NULL)
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					bw_phase_gen_process1(coeffs, state, y + i, y_phase_inc + i);
				}
			else
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v_phase_inc;
					bw_phase_gen_process1(coeffs, state, y + i, &v_phase_inc);
				}
		}
	} else {
		if (x_mod != NULL) {
			if (y_phase_inc != NULL)
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v;
					bw_phase_gen_process1_mod(coeffs, state, x_mod[i], &v, y_phase_inc + i);
				}
			else
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v, v_phase_inc;
					bw_phase_gen_process1_mod(coeffs, state, x_mod[i], &v, &v_phase_inc);
				}
		} else {
			if (y_phase_inc != NULL)
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v;
					bw_phase_gen_process1(coeffs, state, &v, y_phase_inc + i);
				}
			else
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					float v, v_phase_inc;
					bw_phase_gen_process1(coeffs, state, &v, &v_phase_inc);
				}
		}
	}
}

static inline void bw_phase_gen_process_multi(bw_phase_gen_coeffs *BW_RESTRICT coeffs, bw_phase_gen_state **BW_RESTRICT state, const float **x_mod, float **y, float **y_phase_inc, int n_channels, int n_samples) {
	bw_phase_gen_update_coeffs_ctrl(coeffs);
	if (y != NULL) {
		if (x_mod != NULL) {
			if (y_phase_inc != NULL)
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (int j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						if (x_mod[j])
							bw_phase_gen_process1_mod(coeffs, state[j], x_mod[j][i], &v, &v_phase_inc);
						else
							bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y[j])
							y[j][i] = v;
						if (y_phase_inc[j])
							y_phase_inc[j][i] = v_phase_inc;
					}
				}
			else
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (int j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						if (x_mod[j])
							bw_phase_gen_process1_mod(coeffs, state[j], x_mod[j][i], &v, &v_phase_inc);
						else
							bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y[j])
							y[j][i] = v;
					}
				}
		} else {
			if (y_phase_inc != NULL)
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (int j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y[j])
							y[j][i] = v;
						if (y_phase_inc[j])
							y_phase_inc[j][i] = v_phase_inc;
					}
				}
			else
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (int j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y[j])
							y[j][i] = v;
					}
				}
		}
	} else {
		if (x_mod != NULL) {
			if (y_phase_inc != NULL)
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (int j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						if (x_mod[j])
							bw_phase_gen_process1_mod(coeffs, state[j], x_mod[j][i], &v, &v_phase_inc);
						else
							bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y_phase_inc[j])
							y_phase_inc[j][i] = v_phase_inc;
					}
				}
			else
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (int j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						if (x_mod[j])
							bw_phase_gen_process1_mod(coeffs, state[j], x_mod[j][i], &v, &v_phase_inc);
						else
							bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
					}
				}
		} else {
			if (y_phase_inc != NULL)
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (int j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
						if (y_phase_inc[j])
							y_phase_inc[j][i] = v_phase_inc;
					}
				}
			else
				for (int i = 0; i < n_samples; i++) {
					bw_phase_gen_update_coeffs_audio(coeffs);
					for (int j = 0; j < n_channels; j++) {
						float v, v_phase_inc;
						bw_phase_gen_process1(coeffs, state[j], &v, &v_phase_inc);
					}
				}
		}
	}
}

static inline void bw_phase_gen_set_frequency(bw_phase_gen_coeffs *BW_RESTRICT coeffs, float value) {
	coeffs->frequency = value;
}

static inline void bw_phase_gen_set_portamento_tau(bw_phase_gen_coeffs *BW_RESTRICT coeffs, float value) {
	bw_one_pole_set_tau(&coeffs->portamento_coeffs, value);
}

#ifdef __cplusplus
}
#endif

#endif
