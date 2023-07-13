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

#ifndef BWPP_GAIN_H
#define BWPP_GAIN_H

#include <bw_gain.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Gain
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class Gain {
public:
	Gain();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setGainLin(float value);
	void setGainDB(float value);
	void setSmoothTau(float value);
	
	float getGain();
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_gain_coeffs	 coeffs;
};

template<BW_SIZE_T N_CHANNELS>
inline Gain<N_CHANNELS>::Gain() {
	bw_gain_init(&coeffs);
}

template<BW_SIZE_T N_CHANNELS>
inline void Gain<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_gain_set_sample_rate(&coeffs, sampleRate);
}

template<BW_SIZE_T N_CHANNELS>
inline void Gain<N_CHANNELS>::reset() {
	bw_gain_reset_coeffs(&coeffs);
}

template<BW_SIZE_T N_CHANNELS>
inline void Gain<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_gain_process_multi(&coeffs, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void Gain<N_CHANNELS>::setGainLin(float value) {
	bw_gain_set_gain_lin(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Gain<N_CHANNELS>::setGainDB(float value) {
	bw_gain_set_gain_dB(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Gain<N_CHANNELS>::setSmoothTau(float value) {
	bw_gain_set_smooth_tau(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline float Gain<N_CHANNELS>::getGain() {
	return bw_gain_get_gain(&coeffs);
}

}

#endif
