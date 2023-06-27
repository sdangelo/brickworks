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

#ifndef BWPP_SR_REDUCE_H
#define BWPP_SR_REDUCE_H

#include <bw_sr_reduce.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class SRReduce {
	public:
		SRReduce();
		
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setRatio(float value);

	private:
		bw_sr_reduce_coeffs	 coeffs;
		bw_sr_reduce_state	 states[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	SRReduce<N_CHANNELS>::SRReduce() {
		bw_sr_reduce_init(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void SRReduce<N_CHANNELS>::reset() {
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_sr_reduce_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void SRReduce<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_sr_reduce_process(&coeffs, states + i, x.data()[i], y.data()[i], nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void SRReduce<N_CHANNELS>::setRatio(float value) {
		bw_sr_reduce_set_ratio(&coeffs, value);
	}
}

#endif
