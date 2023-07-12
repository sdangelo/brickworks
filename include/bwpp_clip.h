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

#ifndef BWPP_CLIP_H
#define BWPP_CLIP_H

#include <bw_clip.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Clip
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class Clip {
public:
	Clip();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setBias(float value);
	void setGain(float value);
	void setGainCompensation(bool value);
/*! <<<... }```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_clip_coeffs	 coeffs;
	bw_clip_state	 states[N_CHANNELS];
	bw_clip_state	*statesP[N_CHANNELS];
};

template<BW_SIZE_T N_CHANNELS>
inline Clip<N_CHANNELS>::Clip() {
	bw_clip_init(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<BW_SIZE_T N_CHANNELS>
inline void Clip<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_clip_set_sample_rate(&coeffs, sampleRate);
}

template<BW_SIZE_T N_CHANNELS>
inline void Clip<N_CHANNELS>::reset() {
	bw_clip_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_clip_reset_state(&coeffs, states + i);
}

template<BW_SIZE_T N_CHANNELS>
inline void Clip<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_clip_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void Clip<N_CHANNELS>::setBias(float value) {
	bw_clip_set_bias(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Clip<N_CHANNELS>::setGain(float value) {
	bw_clip_set_gain(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Clip<N_CHANNELS>::setGainCompensation(bool value) {
	bw_clip_set_gain_compensation(&coeffs, value);
}

}

#endif
