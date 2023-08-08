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

#ifndef BWPP_HP1_H
#define BWPP_HP1_H

#include <bw_hp1.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::HP1
 *  ```>>> */
template<size_t N_CHANNELS>
class HP1 {
public:
	HP1();

	void setSampleRate(float sampleRate);
	void reset(float x0 = 0.f);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setCutoff(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_hp1_coeffs	 coeffs;
	bw_hp1_state	 states[N_CHANNELS];
	bw_hp1_state	*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline HP1<N_CHANNELS>::HP1() {
	bw_hp1_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_hp1_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::reset(float x0) {
	bw_hp1_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_hp1_reset_state(&coeffs, states + i, x0);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_hp1_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void HP1<N_CHANNELS>::setCutoff(float value) {
	bw_hp1_set_cutoff(&coeffs, value);
}

}

#endif
