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

#ifndef BWPP_BALANCE_H
#define BWPP_BALANCE_H

#include <bw_balance.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	class Balance {
	public:
		Balance();

		void setSampleRate(float sampleRate);
		void reset();
		void process(
			std::array<const float *, N_CHANNELS> x_l,
			std::array<const float *, N_CHANNELS> x_r,
			std::array<float *, N_CHANNELS> y_l,
			std::array<float *, N_CHANNELS> y_r,
			int nSamples);

		void setBalance(float value);

	private:
		bw_balance_coeffs	 coeffs;
	};
	
	template<BW_SIZE_T N_CHANNELS>
	Balance<N_CHANNELS>::Balance() {
		bw_balance_init(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Balance<N_CHANNELS>::setSampleRate(float sampleRate) {
		bw_balance_set_sample_rate(&coeffs, sampleRate);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Balance<N_CHANNELS>::reset() {
		bw_balance_reset_coeffs(&coeffs);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Balance<N_CHANNELS>::process(
			std::array<const float *, N_CHANNELS> x_l,
			std::array<const float *, N_CHANNELS> x_r,
			std::array<float *, N_CHANNELS> y_l,
			std::array<float *, N_CHANNELS> y_r,
			int nSamples) {
		bw_balance_process_multi(&coeffs, x_l.data(), x_r.data(), y_l.data(), y_r.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	void Balance<N_CHANNELS>::setBalance(float value) {
		bw_balance_set_balance(&coeffs, value);
	}
}

#endif
