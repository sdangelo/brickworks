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

#ifndef BWPP_SRC_H
#define BWPP_SRC_H

#include <bw_src.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::SRC
 *  ```>>> */
template<size_t N_CHANNELS>
class SRC {
public:
	SRC(float ratio);
	
	void reset(float x0 = 0.f);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		std::array<int *, N_CHANNELS> nInSamples,
		std::array<int *, N_CHANNELS> nOutSamples);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_src_coeffs	 coeffs;
	bw_src_state	 states[N_CHANNELS];
	bw_src_state	*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline SRC<N_CHANNELS>::SRC(float ratio) {
	bw_src_init(&coeffs, ratio);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void SRC<N_CHANNELS>::reset(float x0) {
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_src_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void SRC<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		std::array<int *, N_CHANNELS> nInSamples,
		std::array<int *, N_CHANNELS> nOutSamples) {
	bw_src_process_multi(coeffs, statesP, x.data(), y.data(), N_CHANNELS, nInSamples.data(), nOutSamples.data());
}

}

#endif
