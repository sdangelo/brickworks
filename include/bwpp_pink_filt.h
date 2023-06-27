/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can repink_filtribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is pink_filtributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef BWPP_PINK_FILT_H
#define BWPP_PINK_FILT_H

#include <bw_pink_filt.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class PinkFilt {
	public:
		PinkFilt();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setSampleRateScaling(bool value);
		
		float getScalingK();

	private:
		bw_pink_filt_coeffs	 coeffs;
		bw_pink_filt_state	 states[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	PinkFilt<N_CHANNELS>::PinkFilt() {
		bw_pink_filt_init(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PinkFilt<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_pink_filt_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PinkFilt<N_CHANNELS>::reset() {
		bw_pink_filt_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_pink_filt_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PinkFilt<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_pink_filt_process(&coeffs, x.data()[i], y.data()[i], nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PinkFilt<N_CHANNELS>::setSampleRateScaling(bool value) {
		bw_pink_filt_set_sample_rate_scaling(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	float PinkFilt<N_CHANNELS>::getScalingK(float value) {
		return bw_pink_filt_get_scaling_k(&coeffs);
	}
}

#endif
