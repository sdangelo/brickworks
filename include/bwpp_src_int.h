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
	template<BW_SIZE_T N_CHANNELS>
	class SRCInt {
	public:
		SRCInt(int ratio);
		
		void reset(float x0 = 0.f);
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nInSamples);

	private:
		bw_src_int_coeffs	 coeffs;
		bw_src_int_state	 states[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	SRCInt<N_CHANNELS>::SRCInt(int ratio) {
		bw_src_int_init(&coeffs, ratio);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void SRCInt<N_CHANNELS>::reset(float x0) {
		for (unsigned int i = 0; i < N_CHANNELS; i++)
			bw_src_int_reset_state(&coeffs, states + i, x0);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void SRCInt<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nInSamples) {
		for (unsigned int i = 0; i < N_CHANNELS; i++)
			bw_src_int_process(&coeffs, states + i, x.data()[i], y.data()[i], nInSamples);
	}
}

#endif
