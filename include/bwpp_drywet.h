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

#ifndef BWPP_DRYWET_H
#define BWPP_DRYWET_H

#include <bw_drywet.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class DryWet {
	public:
		DryWet();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x_dry,
			std::array<const float *, N_CHANNELS> x_wet,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setWet(float value);
		void setSmoothTau(float value);

	private:
		bw_drywet_coeffs	 coeffs;
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline DryWet<N_CHANNELS>::DryWet() {
		bw_drywet_init(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void DryWet<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_drywet_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void DryWet<N_CHANNELS>::reset() {
		bw_drywet_reset_coeffs(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void DryWet<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x_dry,
			std::array<const float *, N_CHANNELS> x_wet,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_drywet_process_multi(&coeffs, x_dry.data(), x_wet.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void DryWet<N_CHANNELS>::setWet(float value) {
		bw_drywet_set_wet(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void DryWet<N_CHANNELS>::setSmoothTau(float value) {
		bw_drywet_set_smooth_tau(&coeffs, value);
	}
}

#endif
