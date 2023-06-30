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

#ifndef BWPP_NOISE_GATE_H
#define BWPP_NOISE_GATE_H

#include <bw_noise_gate.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class NoiseGate {
	public:
		NoiseGate();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<const float *, N_CHANNELS> xSC,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setTreshLin(float value);
		void setTreshDBFS(float value);
		void setRatio(float value);
		void setAttackTau(float value);
		void setReleaseTau(float value);

	private:
		bw_noise_gate_coeffs	 coeffs;
		bw_noise_gate_state	 states[N_CHANNELS];
		bw_noise_gate_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline NoiseGate<N_CHANNELS>::NoiseGate() {
		bw_noise_gate_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void NoiseGate<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_noise_gate_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void NoiseGate<N_CHANNELS>::reset() {
		bw_noise_gate_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_noise_gate_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void NoiseGate<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<const float *, N_CHANNELS> xSC,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_noise_gate_process_multi(&coeffs, statesP, x.data(), xSC.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void NoiseGate<N_CHANNELS>::setTreshLin(float value) {
		bw_noise_gate_set_thresh_lin(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void NoiseGate<N_CHANNELS>::setTreshDBFS(float value) {
		bw_noise_gate_set_thresh_dBFS(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void NoiseGate<N_CHANNELS>::setRatio(float value) {
		bw_noise_gate_set_ratio(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void NoiseGate<N_CHANNELS>::setAttackTau(float value) {
		bw_noise_gate_set_attack_tau(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void NoiseGate<N_CHANNELS>::setReleaseTau(float value) {
		bw_noise_gate_set_release_tau(&coeffs, value);
	}
}

#endif
