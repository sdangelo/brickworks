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

#ifndef BWPP_PPM_H
#define BWPP_PPM_H

#include <bw_ppm.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class PPM {
	public:
		PPM();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setIntegrationTau(float value);
		
		float getYZ1(BW_SIZE_T channel);

	private:
		bw_ppm_coeffs	 coeffs;
		bw_ppm_state	 states[N_CHANNELS];
		bw_ppm_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	PPM<N_CHANNELS>::PPM() {
		bw_ppm_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PPM<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_ppm_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PPM<N_CHANNELS>::reset() {
		bw_ppm_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_ppm_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PPM<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_ppm_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PPM<N_CHANNELS>::setIntegrationTau(float value) {
		bw_ppm_set_integration_tau(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	float PPM<N_CHANNELS>::getYZ1(BW_SIZE_T channel) {
		return bw_ppm_get_y_z1(states + channel);
	}
}

#endif
