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

#ifndef BWPP_NOISE_GEN_H
#define BWPP_NOISE_GEN_H

#include <bw_noise_gen.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class NoiseGen {
	public:
		NoiseGen(uint64_t *BW_RESTRICT state);

		void setSampleRate(float sampleRate);
		void process(
			std::array<float *, N_CHANNELS> y,
			int nSamples);

		void setSampleRateScaling(bool value);
		
		float getScalingK();

	private:
		bw_noise_gen_coeffs	 coeffs;
	};
	
	template<BW_SIZE_T N_CHANNELS>
	NoiseGen<N_CHANNELS>::NoiseGen(uint64_t *BW_RESTRICT state) {
		bw_noise_gen_init(&coeffs, state);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void NoiseGen<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_noise_gen_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void NoiseGen<N_CHANNELS>::process(
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		for (int i = 0; i < N_CHANNELS; i++)
			bw_noise_gen_process(&coeffs, y.data()[i], nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void NoiseGen<N_CHANNELS>::setSampleRateScaling(bool value) {
		bw_noise_gen_set_sample_rate_scaling(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	float NoiseGen<N_CHANNELS>::getScalingK(float value) {
		return bw_noise_gen_get_scaling_k(&coeffs);
	}
}

#endif
