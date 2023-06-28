/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can rephase_genribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is phase_genributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef BWPP_PHASE_GEN_H
#define BWPP_PHASE_GEN_H

#include <bw_phase_gen.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class PhaseGen {
	public:
		PhaseGen();

		void setSampleRate(float sampleRate);
		void reset(float phase_0 = 0.f);
		void process(
			std::array<const float *, N_CHANNELS> x_mod,
			std::array<float *, N_CHANNELS> y,
			std::array<float *, N_CHANNELS> y_phase_inc,
			int nSamples);

		void setFrequency(float value);
		void setPortamentoTau(float value);

	private:
		bw_phase_gen_coeffs	 coeffs;
		bw_phase_gen_state	 states[N_CHANNELS];
		bw_phase_gen_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	PhaseGen<N_CHANNELS>::PhaseGen() {
		bw_phase_gen_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PhaseGen<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_phase_gen_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PhaseGen<N_CHANNELS>::reset(float phase_0) {
		bw_phase_gen_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_phase_gen_reset_state(&coeffs, states + i, phase_0);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PhaseGen<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x_mod,
			std::array<float *, N_CHANNELS> y,
			std::array<float *, N_CHANNELS> y_phase_inc,
			int nSamples) {
		bw_phase_gen_process_multi(&coeffs, statesP, x_mod.data(), y.data(), y_phase_inc.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PhaseGen<N_CHANNELS>::setFrequency(float value) {
		bw_phase_gen_set_frequency(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void PhaseGen<N_CHANNELS>::setPortamentoTau(float value) {
		bw_phase_gen_set_portamento_tau(&coeffs, value);
	}
}

#endif
