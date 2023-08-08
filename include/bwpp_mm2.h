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

#ifndef BWPP_MM2_H
#define BWPP_MM2_H

#include <bw_mm2.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::MM2
 *  ```>>> */
template<size_t N_CHANNELS>
class MM2 {
public:
	MM2();

	void setSampleRate(float sampleRate);
	void reset(float x0 = 0.f);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setCutoff(float value);
	void setQ(float value);
	void setPrewarpAtCutoff(bool value);
	void setPrewarpFreq(float value);
	void setCoeffX(float value);
	void setCoeffLp(float value);
	void setCoeffBp(float value);
	void setCoeffHp(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_mm2_coeffs	 coeffs;
	bw_mm2_state	 states[N_CHANNELS];
	bw_mm2_state	*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline MM2<N_CHANNELS>::MM2() {
	bw_mm2_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_mm2_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::reset(float x0) {
	bw_mm2_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_mm2_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_mm2_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::setCutoff(float value) {
	bw_mm2_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::setQ(float value) {
	bw_mm2_set_Q(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::setPrewarpAtCutoff(bool value) {
	bw_mm2_set_prewarp_at_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::setPrewarpFreq(float value) {
	bw_mm2_set_prewarp_freq(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::setCoeffX(float value) {
	bw_mm2_set_coeff_x(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::setCoeffLp(float value) {
	bw_mm2_set_coeff_lp(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::setCoeffBp(float value) {
	bw_mm2_set_coeff_bp(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void MM2<N_CHANNELS>::setCoeffHp(float value) {
	bw_mm2_set_coeff_hp(&coeffs, value);
}

}

#endif
