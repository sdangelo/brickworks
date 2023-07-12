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

#ifndef BWPP_WAH_H
#define BWPP_WAH_H

#include <bw_wah.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Wah
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class Wah {
public:
	Wah();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setWah(float value);
/*! <<<... }```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_wah_coeffs	 coeffs;
	bw_wah_state	 states[N_CHANNELS];
	bw_wah_state	*statesP[N_CHANNELS];
};

template<BW_SIZE_T N_CHANNELS>
inline Wah<N_CHANNELS>::Wah() {
	bw_wah_init(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<BW_SIZE_T N_CHANNELS>
inline void Wah<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_wah_set_sample_rate(&coeffs, sampleRate);
}

template<BW_SIZE_T N_CHANNELS>
inline void Wah<N_CHANNELS>::reset() {
	bw_wah_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_wah_reset_state(&coeffs, states + i);
}

template<BW_SIZE_T N_CHANNELS>
inline void Wah<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_wah_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void Wah<N_CHANNELS>::setWah(float value) {
	bw_wah_set_wah(&coeffs, value);
}

}

#endif
