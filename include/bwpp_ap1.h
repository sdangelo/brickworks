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

#ifndef BWPP_AP1_H
#define BWPP_AP1_H

#include <bw_ap1.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class AP1 {
	public:
		AP1();

		void setSampleRate(float sampleRate);
		void reset(float x0 = 0.f);
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setCutoff(float value);

	private:
		bw_ap1_coeffs	 coeffs;
		bw_ap1_state	 states[N_CHANNELS];
		bw_ap1_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline AP1<N_CHANNELS>::AP1() {
		bw_ap1_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void AP1<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_ap1_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void AP1<N_CHANNELS>::reset(float x0) {
		bw_ap1_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_ap1_reset_state(&coeffs, states + i, x0);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void AP1<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_ap1_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void AP1<N_CHANNELS>::setCutoff(float value) {
		bw_ap1_set_cutoff(&coeffs, value);
	}
}

#endif
