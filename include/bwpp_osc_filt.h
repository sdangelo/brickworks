/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can reosc_filtribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is osc_filtributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef BWPP_OSC_FILT_H
#define BWPP_OSC_FILT_H

#include <bw_osc_filt.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class OscFilt {
	public:
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples);

	private:
		bw_osc_filt_state	 states[N_CHANNELS];
	};
	
	template<BW_SIZE_T N_CHANNELS>
	inline void OscFilt<N_CHANNELS>::reset() {
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_osc_filt_reset_state(states + i);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void OscFilt<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
			bw_osc_filt_process(states + i, x.data()[i], y.data()[i], nSamples);
	}
}

#endif
