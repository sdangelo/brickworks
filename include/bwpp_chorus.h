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

#ifndef BWPP_CHORUS_H
#define BWPP_CHORUS_H

#include <bw_chorus.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class Chorus {
	public:
		Chorus(float maxDelay = 0.01f);
		~Chorus();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setRate(float value);
		void setDelay(float value);
		void setAmount(float value);
		void setCoeffX(float value);
		void setCoeffMod(float value);
		void setCoeffFB(float value);

	private:
		bw_chorus_coeffs	 coeffs;
		bw_chorus_state		 states[N_CHANNELS];
		bw_chorus_state		*statesP[N_CHANNELS];
		void			*mem;
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline Chorus<N_CHANNELS>::Chorus(float maxDelay) {
		bw_chorus_init(&coeffs, maxDelay);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
		mem = nullptr;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline Chorus<N_CHANNELS>::~Chorus() {
		if (mem != nullptr)
			operator delete(mem);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Chorus<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_chorus_set_sample_rate(&coeffs, sampleRate);
		BW_SIZE_T req = bw_chorus_mem_req(&coeffs);
		if (mem != nullptr)
			operator delete(mem);
		mem = operator new(req * N_CHANNELS);
		void *m = mem;
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++, m = static_cast<char *>(m) + req)
			bw_chorus_mem_set(&coeffs, states + i, m);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Chorus<N_CHANNELS>::reset() {
		bw_chorus_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_chorus_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Chorus<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_chorus_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Chorus<N_CHANNELS>::setRate(float value) {
		bw_chorus_set_rate(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Chorus<N_CHANNELS>::setDelay(float value) {
		bw_chorus_set_delay(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Chorus<N_CHANNELS>::setAmount(float value) {
		bw_chorus_set_amount(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Chorus<N_CHANNELS>::setCoeffX(float value) {
		bw_chorus_set_coeff_x(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Chorus<N_CHANNELS>::setCoeffMod(float value) {
		bw_chorus_set_coeff_mod(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Chorus<N_CHANNELS>::setCoeffFB(float value) {
		bw_chorus_set_coeff_fb(&coeffs, value);
	}
}

#endif
