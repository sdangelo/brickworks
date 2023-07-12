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

#ifndef BWPP_SLEW_LIM_H
#define BWPP_SLEW_LIM_H

#include <bw_slew_lim.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::SlewLim
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class SlewLim {
public:
	SlewLim();

	void setSampleRate(float sampleRate);
	void reset(float y_z1 = 0.f);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setMaxRate(float value);
	void setMaxRateUp(float value);
	void setMaxRateDown(float value);
	
	float getYZ1(BW_SIZE_T channel);
/*! <<<... }```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_slew_lim_coeffs	 coeffs;
	bw_slew_lim_state	 states[N_CHANNELS];
	bw_slew_lim_state	*statesP[N_CHANNELS];
};

template<BW_SIZE_T N_CHANNELS>
inline SlewLim<N_CHANNELS>::SlewLim() {
	bw_slew_lim_init(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<BW_SIZE_T N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_slew_lim_set_sample_rate(&coeffs, sampleRate);
}

template<BW_SIZE_T N_CHANNELS>
inline void SlewLim<N_CHANNELS>::reset(float y_z1) {
	bw_slew_lim_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_slew_lim_reset_state(&coeffs, states + i, y_z1);
}

template<BW_SIZE_T N_CHANNELS>
inline void SlewLim<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_slew_lim_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setMaxRate(float value) {
	bw_slew_lim_set_max_rate(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setMaxRateUp(float value) {
	bw_slew_lim_set_max_rate_up(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void SlewLim<N_CHANNELS>::setMaxRateDown(float value) {
	bw_slew_lim_set_max_rate_down(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline float SlewLim<N_CHANNELS>::getYZ1(BW_SIZE_T channel) {
	return bw_slew_lim_get_y_z1(states + channel);
}

}

#endif
