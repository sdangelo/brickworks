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

#ifndef BWPP_DIST_H
#define BWPP_DIST_H

#include <bw_dist.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class Dist {
	public:
		Dist();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setDistortion(float value);
		void setTone(float value);
		void setVolume(float value);

	private:
		bw_dist_coeffs	 coeffs;
		bw_dist_state	 states[N_CHANNELS];
		bw_dist_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	Dist<N_CHANNELS>::Dist() {
		bw_dist_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Dist<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_dist_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Dist<N_CHANNELS>::reset() {
		bw_dist_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_dist_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Dist<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_dist_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Dist<N_CHANNELS>::setDistortion(float value) {
		bw_dist_set_distortion(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Dist<N_CHANNELS>::setTone(float value) {
		bw_dist_set_tone(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Dist<N_CHANNELS>::setVolume(float value) {
		bw_dist_set_volume(&coeffs, value);
	}
}

#endif
