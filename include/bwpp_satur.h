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

#ifndef BWPP_SATUR_H
#define BWPP_SATUR_H

#include <bw_satur.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Satur
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class Satur {
public:
	Satur();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setBias(float value);
	void setGain(float value);
	void setGainCompensation(bool value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_satur_coeffs	 coeffs;
	bw_satur_state	 states[N_CHANNELS];
	bw_satur_state	*statesP[N_CHANNELS];
};

template<BW_SIZE_T N_CHANNELS>
inline Satur<N_CHANNELS>::Satur() {
	bw_satur_init(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<BW_SIZE_T N_CHANNELS>
inline void Satur<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_satur_set_sample_rate(&coeffs, sampleRate);
}

template<BW_SIZE_T N_CHANNELS>
inline void Satur<N_CHANNELS>::reset() {
	bw_satur_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_satur_reset_state(&coeffs, states + i);
}

template<BW_SIZE_T N_CHANNELS>
inline void Satur<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_satur_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void Satur<N_CHANNELS>::setBias(float value) {
	bw_satur_set_bias(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Satur<N_CHANNELS>::setGain(float value) {
	bw_satur_set_gain(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Satur<N_CHANNELS>::setGainCompensation(bool value) {
	bw_satur_set_gain_compensation(&coeffs, value);
}

}

#endif
