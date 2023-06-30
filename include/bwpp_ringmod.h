/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can reringmodribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is ringmodributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef BWPP_RINGMOD_H
#define BWPP_RINGMOD_H

#include <bw_ringmod.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class RingMod {
	public:
		RingMod();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x_mod,
			std::array<const float *, N_CHANNELS> x_car,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setAmount(float value);

	private:
		bw_ringmod_coeffs	 coeffs;
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline RingMod<N_CHANNELS>::RingMod() {
		bw_ringmod_init(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void RingMod<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_ringmod_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void RingMod<N_CHANNELS>::reset() {
		bw_ringmod_reset_coeffs(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void RingMod<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x_mod,
			std::array<const float *, N_CHANNELS> x_car,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_ringmod_process_multi(&coeffs, statesP, x_mod.data(), x_car.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void RingMod<N_CHANNELS>::setAmount(float value) {
		bw_ringmod_set_amount(&coeffs, value);
	}
}

#endif
