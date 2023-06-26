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

#ifndef BWPP_TREM_H
#define BWPP_TREM_H

#include <bw_trem.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class Trem {
	public:
		Trem();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setRate(float value);
		void setAmount(float value);

	private:
		bw_trem_coeffs	 coeffs;
		bw_trem_state		 states[N_CHANNELS];
		bw_trem_state		*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	Trem<N_CHANNELS>::Trem() {
		bw_trem_init(&coeffs);
		for (unsigned int i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Trem<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_trem_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Trem<N_CHANNELS>::reset() {
		bw_trem_reset_coeffs(&coeffs);
		for (unsigned int i = 0; i < N_CHANNELS; i++)
			bw_trem_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Trem<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_trem_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Trem<N_CHANNELS>::setRate(float value) {
		bw_trem_set_rate(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Trem<N_CHANNELS>::setAmount(float value) {
		bw_trem_set_amount(&coeffs, value);
	}
}

#endif
