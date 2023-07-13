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

#ifndef BWPP_SVF_H
#define BWPP_SVF_H

#include <bw_svf.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::SVF
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class SVF {
public:
	SVF();

	void setSampleRate(float sampleRate);
	void reset(float x0 = 0.f);
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y_lp,
		std::array<float *, N_CHANNELS> y_bp,
		std::array<float *, N_CHANNELS> y_hp,
		int nSamples);

	void setCutoff(float value);
	void setQ(float value);
	void setPrewarpAtCutoff(bool value);
	void setPrewarpFreq(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_svf_coeffs		 coeffs;
	bw_svf_state		 states[N_CHANNELS];
	bw_svf_state		*statesP[N_CHANNELS];
};

template<BW_SIZE_T N_CHANNELS>
inline SVF<N_CHANNELS>::SVF() {
	bw_svf_init(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<BW_SIZE_T N_CHANNELS>
inline void SVF<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_svf_set_sample_rate(&coeffs, sampleRate);
}

template<BW_SIZE_T N_CHANNELS>
inline void SVF<N_CHANNELS>::reset(float x0) {
	bw_svf_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_svf_reset_state(&coeffs, states + i, x0);
}

template<BW_SIZE_T N_CHANNELS>
inline void SVF<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y_lp,
		std::array<float *, N_CHANNELS> y_bp,
		std::array<float *, N_CHANNELS> y_hp,
		int nSamples) {
	bw_svf_process_multi(&coeffs, statesP, x.data(), y_lp.data(), y_bp.data(), y_hp.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void SVF<N_CHANNELS>::setCutoff(float value) {
	bw_svf_set_cutoff(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void SVF<N_CHANNELS>::setQ(float value) {
	bw_svf_set_Q(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void SVF<N_CHANNELS>::setPrewarpAtCutoff(bool value) {
	bw_svf_set_prewarp_at_cutoff(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void SVF<N_CHANNELS>::setPrewarpFreq(float value) {
	bw_svf_set_prewarp_freq(&coeffs, value);
}

}

#endif
