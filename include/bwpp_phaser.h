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

#ifndef BWPP_PHASER_H
#define BWPP_PHASER_H

#include <bw_phaser.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class Phaser {
	public:
		Phaser();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setRate(float value);
		void setCenter(float value);
		void setAmount(float value);

	private:
		bw_phaser_coeffs	 coeffs;
		bw_phaser_state		 states[N_CHANNELS];
		bw_phaser_state		*statesP[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline Phaser<N_CHANNELS>::Phaser() {
		bw_phaser_init(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			statesP[i] = states + i;
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Phaser<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_phaser_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Phaser<N_CHANNELS>::reset() {
		bw_phaser_reset_coeffs(&coeffs);
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_phaser_reset_state(&coeffs, states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Phaser<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_phaser_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Phaser<N_CHANNELS>::setRate(float value) {
		bw_phaser_set_rate(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Phaser<N_CHANNELS>::setCenter(float value) {
		bw_phaser_set_center(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void Phaser<N_CHANNELS>::setAmount(float value) {
		bw_phaser_set_amount(&coeffs, value);
	}
}

#endif
