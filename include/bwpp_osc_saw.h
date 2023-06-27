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

#ifndef BWPP_OSC_SAW_H
#define BWPP_OSC_SAW_H

#include <bw_osc_saw.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class OscSaw {
	public:
		OscSaw();
		
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<const float *, N_CHANNELS> x_phase_inc,
			std::array<float *, N_CHANNELS> y,
			int nSamples);
		
		void setAntialiasing(bool value);

	private:
		bw_osc_saw_coeffs	 coeffs;
	};
	
	template<BW_SIZE_T N_CHANNELS>
	OscSaw<N_CHANNELS>::OscSaw() {
		bw_osc_saw_init(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OscSaw<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<const float *, N_CHANNELS> x_phase_inc,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_osc_saw_process(&coeffs, x.data()[i], x_phase_inc.data()[i], y.data()[i], nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OscSaw<N_CHANNELS>::setAntialiasing(bool value) {
		bw_osc_saw_set_antialiasing(&coeffs, value);
	}
}

#endif
