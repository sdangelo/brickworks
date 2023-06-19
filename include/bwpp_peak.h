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

#ifndef BWPP_PEAK_H
#define BWPP_PEAK_H

#include <bw_peak.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class PEAK {
	public:
		PEAK();

		void setSampleRate(float sampleRate);
		void reset(float x0 = 0.f);
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setCutoff(float value);
		void setQ(float value);
		void setDcGainLin(float value);
		void setDcGainDB(float value);
		void setBandwidth(float value);
		void setUseBandwidth(bool value);

	private:
		bw_peak_coeffs	 coeffs;
		bw_peak_state	 states[N_CHANNELS];
		bw_peak_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	PEAK<N_CHANNELS>::PEAK() {
		bw_peak_init(&coeffs);
		for (unsigned int i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PEAK<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_peak_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PEAK<N_CHANNELS>::reset(float x0) {
		bw_peak_reset_coeffs(&coeffs);
		for (unsigned int i = 0; i < N_CHANNELS; i++)
			bw_peak_reset_state(&coeffs, states + i, x0);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PEAK<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_peak_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PEAK<N_CHANNELS>::setCutoff(float value) {
		bw_peak_set_cutoff(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PEAK<N_CHANNELS>::setQ(float value) {
		bw_peak_set_Q(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PEAK<N_CHANNELS>::setPeakGainLin(float value) {
		bw_peak_set_peak_gain_lin(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PEAK<N_CHANNELS>::setPeakGainDB(float value) {
		bw_peak_set_peak_gain_dB(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PEAK<N_CHANNELS>::setBandwidth(float value) {
		bw_peak_set_bandwidth(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PEAK<N_CHANNELS>::setUseBandwidth(bool value) {
		bw_peak_set_use_bandwidth(&coeffs, value);
	}
}

#endif
