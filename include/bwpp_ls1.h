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

#ifndef BWPP_LS1_H
#define BWPP_LS1_H

#include <bw_ls1.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class LS1 {
	public:
		LS1();

		void setSampleRate(float sampleRate);
		void reset(float x0 = 0.f);
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setCutoff(float value);
		void setDcGainLin(float value);
		void setDcGainDB(float value);

	private:
		bw_ls1_coeffs	 coeffs;
		bw_ls1_state	 states[N_CHANNELS];
		bw_ls1_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline LS1<N_CHANNELS>::LS1() {
		bw_ls1_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LS1<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_ls1_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LS1<N_CHANNELS>::reset(float x0) {
		bw_ls1_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_ls1_reset_state(&coeffs, states + i, x0);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LS1<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_ls1_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LS1<N_CHANNELS>::setCutoff(float value) {
		bw_ls1_set_cutoff(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LS1<N_CHANNELS>::setDcGainLin(float value) {
		bw_ls1_set_dc_gain_lin(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LS1<N_CHANNELS>::setDcGainDB(float value) {
		bw_ls1_set_dc_gain_dB(&coeffs, value);
	}
}

#endif
