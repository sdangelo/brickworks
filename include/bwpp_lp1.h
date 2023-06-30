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

#ifndef BWPP_LP1_H
#define BWPP_LP1_H

#include <bw_lp1.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class LP1 {
	public:
		LP1();

		void setSampleRate(float sampleRate);
		void reset(float x0 = 0.f);
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setCutoff(float value);
		void setPrewarpAtCutoff(bool value);
		void setPrewarpFreq(float value);

	private:
		bw_lp1_coeffs	 coeffs;
		bw_lp1_state	 states[N_CHANNELS];
		bw_lp1_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline LP1<N_CHANNELS>::LP1() {
		bw_lp1_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LP1<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_lp1_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LP1<N_CHANNELS>::reset(float x0) {
		bw_lp1_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_lp1_reset_state(&coeffs, states + i, x0);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LP1<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_lp1_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LP1<N_CHANNELS>::setCutoff(float value) {
		bw_lp1_set_cutoff(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LP1<N_CHANNELS>::setPrewarpAtCutoff(bool value) {
		bw_lp1_set_prewarp_at_cutoff(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void LP1<N_CHANNELS>::setPrewarpFreq(float value) {
		bw_lp1_set_prewarp_freq(&coeffs, value);
	}
}

#endif
