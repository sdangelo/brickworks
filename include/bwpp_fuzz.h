/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can refuzzribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is fuzzributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef BWPP_FUZZ_H
#define BWPP_FUZZ_H

#include <bw_fuzz.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class Fuzz {
	public:
		Fuzz();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setFuzz(float value);
		void setVolume(float value);

	private:
		bw_fuzz_coeffs	 coeffs;
		bw_fuzz_state	 states[N_CHANNELS];
		bw_fuzz_state	*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	Fuzz<N_CHANNELS>::Fuzz() {
		bw_fuzz_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Fuzz<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_fuzz_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Fuzz<N_CHANNELS>::reset() {
		bw_fuzz_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_fuzz_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Fuzz<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_fuzz_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Fuzz<N_CHANNELS>::setFuzz(float value) {
		bw_fuzz_set_fuzz(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Fuzz<N_CHANNELS>::setVolume(float value) {
		bw_fuzz_set_volume(&coeffs, value);
	}
}

#endif
