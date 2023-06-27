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

#ifndef BWPP_OSC_PULSE_H
#define BWPP_OSC_PULSE_H

#include <bw_osc_pulse.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class OscPulse {
	public:
		OscPulse();
		
		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x,
			std::array<const float *, N_CHANNELS> x_phase_inc,
			std::array<float *, N_CHANNELS> y,
			int nSamples);
		
		void setAntialiasing(bool value);
		void setPulseWidth(float value);

	private:
		bw_osc_pulse_coeffs	 coeffs;
	};
	
	template<BW_SIZE_T N_CHANNELS>
	OscPulse<N_CHANNELS>::OscPulse() {
		bw_osc_pulse_init(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OscPulse<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_osc_pulse_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OscPulse<N_CHANNELS>::reset() {
		bw_osc_pulse_reset_coeffs(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OscPulse<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x,
			std::array<const float *, N_CHANNELS> x_phase_inc,
			std::array<float *, N_CHANNELS> y,
			int nSamples) {
		bw_osc_pulse_process_multi(&coeffs, x.data(), x_phase_inc.data(), y.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OscPulse<N_CHANNELS>::setAntialiasing(bool value) {
		bw_osc_pulse_set_antialiasing(&coeffs, value);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void OscPulse<N_CHANNELS>::setPulseWidth(bool value) {
		bw_osc_pulse_set_pulse_width(&coeffs, value);
	}
}

#endif
