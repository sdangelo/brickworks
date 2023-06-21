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

#ifndef BWPP_MM1_H
#define BWPP_MM1_H

#include <bw_mm1.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class MM1 {
	public:
		MM1();

		void setSampleRate(float sampleRate);
		void reset(float x0 = 0.f);
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setCutoff(float value);
		void setPrewarpAtCutoff(bool value);
		void setPrewarpFreq(float value);
		void setCoeffX(float value);
		void setCoeffLp(float value);

	private:
		bw_mm1_coeffs	 coeffs;
		bw_mm1_state	 states[N_CHANNELS];
		bw_mm1_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	MM1<N_CHANNELS>::MM1() {
		bw_mm1_init(&coeffs);
		for (unsigned int i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void MM1<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_mm1_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void MM1<N_CHANNELS>::reset(float x0) {
		bw_mm1_reset_coeffs(&coeffs);
		for (unsigned int i = 0; i < N_CHANNELS; i++)
			bw_mm1_reset_state(&coeffs, states + i, x0);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void MM1<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_mm1_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void MM1<N_CHANNELS>::setCutoff(float value) {
		bw_mm1_set_cutoff(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void MM1<N_CHANNELS>::setPrewarpAtCutoff(bool value) {
		bw_mm1_set_prewarp_at_cutoff(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void MM1<N_CHANNELS>::setPrewarpFreq(float value) {
		bw_mm1_set_prewarp_freq(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void MM1<N_CHANNELS>::setCoeffX(float value) {
		bw_mm1_set_coeff_x(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void MM1<N_CHANNELS>::setCoeffLp(float value) {
		bw_mm1_set_coeff_lp(&coeffs, value);
	}
}

#endif
