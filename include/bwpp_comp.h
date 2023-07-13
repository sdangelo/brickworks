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

#ifndef BWPP_COMP_H
#define BWPP_COMP_H

#include <bw_comp.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Comp
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class Comp {
public:
	Comp();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> xSC,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setTreshLin(float value);
	void setTreshDBFS(float value);
	void setRatio(float value);
	void setAttackTau(float value);
	void setReleaseTau(float value);
	void setGainLin(float value);
	void setGainDB(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_comp_coeffs	 coeffs;
	bw_comp_state	 states[N_CHANNELS];
	bw_comp_state	*statesP[N_CHANNELS];
};

template<BW_SIZE_T N_CHANNELS>
inline Comp<N_CHANNELS>::Comp() {
	bw_comp_init(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_comp_set_sample_rate(&coeffs, sampleRate);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::reset() {
	bw_comp_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_comp_reset_state(&coeffs, states + i);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<const float *, N_CHANNELS> xSC,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_comp_process_multi(&coeffs, statesP, x.data(), xSC.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::setTreshLin(float value) {
	bw_comp_set_thresh_lin(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::setTreshDBFS(float value) {
	bw_comp_set_thresh_dBFS(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::setRatio(float value) {
	bw_comp_set_ratio(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::setAttackTau(float value) {
	bw_comp_set_attack_tau(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::setReleaseTau(float value) {
	bw_comp_set_release_tau(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::setGainLin(float value) {
	bw_comp_set_gain_lin(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comp<N_CHANNELS>::setGainDB(float value) {
	bw_comp_set_gain_dB(&coeffs, value);
}

}

#endif
