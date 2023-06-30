/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can reenv_genribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is env_genributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef BWPP_ENV_GEN_H
#define BWPP_ENV_GEN_H

#include <bw_env_gen.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class EnvGen {
	public:
		EnvGen();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<char, N_CHANNELS> gate,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setAttack(float value);
		void setDecay(float value);
		void setSustain(float value);
		void setRelease(float value);
		
		bw_env_gen_phase getPhase(BW_SIZE_T channel);
		float getYZ1(BW_SIZE_T channel);

	private:
		bw_env_gen_coeffs	 coeffs;
		bw_env_gen_state	 states[N_CHANNELS];
		bw_env_gen_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline EnvGen<N_CHANNELS>::EnvGen() {
		bw_env_gen_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void EnvGen<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_env_gen_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void EnvGen<N_CHANNELS>::reset() {
		bw_env_gen_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_env_gen_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void EnvGen<N_CHANNELS>::process(
			std::array<char, N_CHANNELS> gate,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_env_gen_process_multi(&coeffs, statesP, gate.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void EnvGen<N_CHANNELS>::setAttack(float value) {
		bw_env_gen_set_attack(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void EnvGen<N_CHANNELS>::setDecay(float value) {
		bw_env_gen_set_decay(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void EnvGen<N_CHANNELS>::setSustain(float value) {
		bw_env_gen_set_sustain(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void EnvGen<N_CHANNELS>::setRelease(float value) {
		bw_env_gen_set_release(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline bw_env_gen_phase EnvGen<N_CHANNELS>::getPhase(BW_SIZE_T channel) {
		return bw_env_gen_get_phase(states + channel);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline float EnvGen<N_CHANNELS>::getYZ1(BW_SIZE_T channel) {
		return bw_env_gen_get_y_z1(states + channel);
	}
}

#endif
