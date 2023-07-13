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

#ifndef BWPP_PAN_H
#define BWPP_PAN_H

#include <bw_pan.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Pan
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class Pan {
public:
	Pan();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y_l,
		std::array<float *, N_CHANNELS> y_r,
		int nSamples);

	void setPan(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_pan_coeffs	 coeffs;
};

template<BW_SIZE_T N_CHANNELS>
inline Pan<N_CHANNELS>::Pan() {
	bw_pan_init(&coeffs);
}

template<BW_SIZE_T N_CHANNELS>
inline void Pan<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_pan_set_sample_rate(&coeffs, sampleRate);
}

template<BW_SIZE_T N_CHANNELS>
inline void Pan<N_CHANNELS>::reset() {
	bw_pan_reset_coeffs(&coeffs);
}

template<BW_SIZE_T N_CHANNELS>
inline void Pan<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y_l,
		std::array<float *, N_CHANNELS> y_r,
		int nSamples) {
	bw_pan_process_multi(&coeffs, x.data(), y_l.data(), y_r.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void Pan<N_CHANNELS>::setPan(float value) {
	bw_pan_set_pan(&coeffs, value);
}

}

#endif
