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

#ifndef BWPP_HS1_H
#define BWPP_HS1_H

#include <bw_hs1.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::HS1
 *  ```>>> */
template<size_t N_CHANNELS>
class HS1 {
public:
	HS1();

	void setSampleRate(float sampleRate);
	void reset(float x0 = 0.f);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setCutoff(float value);
	void setHighGainLin(float value);
	void setHighGainDB(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_hs1_coeffs	 coeffs;
	bw_hs1_state	 states[N_CHANNELS];
	bw_hs1_state	*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline HS1<N_CHANNELS>::HS1() {
	bw_hs1_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void HS1<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_hs1_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void HS1<N_CHANNELS>::reset(float x0) {
	bw_hs1_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_hs1_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void HS1<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_hs1_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void HS1<N_CHANNELS>::setCutoff(float value) {
	bw_hs1_set_cutoff(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void HS1<N_CHANNELS>::setHighGainLin(float value) {
	bw_hs1_set_high_gain_lin(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void HS1<N_CHANNELS>::setHighGainDB(float value) {
	bw_hs1_set_high_gain_dB(&coeffs, value);
}

}

#endif
