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

#ifndef BWPP_ONE_POLE_H
#define BWPP_ONE_POLE_H

#include <bw_one_pole.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class OnePole {
	public:
		OnePole();

		void setSampleRate(float sampleRate);
		void reset(float y_z1 = 0.f);
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setCutoff(float value);
		void setCutoffUp(float value);
		void setCutoffDown(float value);
		void setTau(float value);
		void setTauUp(float value);
		void setTauDown(float value);
		void setStickyThresh(float value);
		void setStickyMode(bw_one_pole_sticky_mode value);
		
		float getYZ1(BW_SIZE_T channel);

	private:
		bw_one_pole_coeffs	 coeffs;
		bw_one_pole_state	 states[N_CHANNELS];
		bw_one_pole_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	OnePole<N_CHANNELS>::OnePole() {
		bw_one_pole_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_one_pole_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::reset(float y_z1) {
		bw_one_pole_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_one_pole_reset_state(&coeffs, states + i, y_z1);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_one_pole_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::setCutoff(float value) {
		bw_one_pole_set_cutoff(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::setCutoffUp(float value) {
		bw_one_pole_set_cutoff_up(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::setCutoffDown(float value) {
		bw_one_pole_set_cutoff_down(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::setTau(float value) {
		bw_one_pole_set_tau(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::setTauUp(float value) {
		bw_one_pole_set_tau_up(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::setTauDown(float value) {
		bw_one_pole_set_tau_down(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::setStickyThresh(float value) {
		bw_one_pole_set_sticky_thresh(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OnePole<N_CHANNELS>::setStickyMode(bw_one_pole_sticky_mode value) {
		bw_one_pole_set_sticky_mode(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	float OnePole<N_CHANNELS>::getYZ1(BW_SIZE_T channel) {
		return bw_one_pole_get_y_z1(states + channel);
	}
}

#endif
