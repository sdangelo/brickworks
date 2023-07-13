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

#ifndef BWPP_SRC_INT_H
#define BWPP_SRC_INT_H

#include <bw_src_int.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::SRCInt
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class SRCInt {
public:
	SRCInt(int ratio);
	
	void reset(float x0 = 0.f);
	std::array<int, N_CHANNELS> process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nInSamples);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_src_int_coeffs	 coeffs;
	bw_src_int_state	 states[N_CHANNELS];
	bw_src_int_state	*statesP[N_CHANNELS];
};

template<BW_SIZE_T N_CHANNELS>
inline SRCInt<N_CHANNELS>::SRCInt(int ratio) {
	bw_src_int_init(&coeffs, ratio);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<BW_SIZE_T N_CHANNELS>
inline void SRCInt<N_CHANNELS>::reset(float x0) {
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_src_int_reset_state(&coeffs, states + i, x0);
}

template<BW_SIZE_T N_CHANNELS>
inline std::array<int, N_CHANNELS> SRCInt<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nInSamples) {
	std::array<int, N_CHANNELS> ret;
	bw_src_int_process_multi(&coeffs, statesP, x.data(), y.data(), ret.data(), N_CHANNELS, nInSamples);
	return ret;
}

}

#endif
