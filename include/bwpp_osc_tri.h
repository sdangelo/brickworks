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

#ifndef BWPP_OSC_TRI_H
#define BWPP_OSC_TRI_H

#include <bw_osc_tri.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::OscTri
 *  ```>>> */
template<size_t N_CHANNELS>
class OscTri {
public:
	OscTri();
	
	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> x_phase_inc,
		std::array<float *, N_CHANNELS> y,
		int nSamples);
	
	void setAntialiasing(bool value);
	void setSlope(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_osc_tri_coeffs	 coeffs;
};

template<size_t N_CHANNELS>
inline OscTri<N_CHANNELS>::OscTri() {
	bw_osc_tri_init(&coeffs);
}

template<size_t N_CHANNELS>
inline void OscTri<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_osc_tri_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void OscTri<N_CHANNELS>::reset() {
	bw_osc_tri_reset_coeffs(&coeffs);
}

template<size_t N_CHANNELS>
inline void OscTri<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> x_phase_inc,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_osc_tri_process_multi(&coeffs, x.data(), x_phase_inc.data(), y.data(), N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void OscTri<N_CHANNELS>::setAntialiasing(bool value) {
	bw_osc_tri_set_antialiasing(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void OscTri<N_CHANNELS>::setSlope(float value) {
	bw_osc_tri_set_slope(&coeffs, value);
}

}

#endif
