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

#ifndef BWPP_COMB_H
#define BWPP_COMB_H

#include <bw_comb.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Comb
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class Comb {
public:
	Comb(float maxDelay = 1.f);
	~Comb();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setDelayFF(float value);
	void setDelayFB(float value);
	void setCoeffBlend(float value);
	void setCoeffFF(float value);
	void setCoeffFB(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_comb_coeffs	 coeffs;
	bw_comb_state	 states[N_CHANNELS];
	bw_comb_state	*statesP[N_CHANNELS];
	void		*mem;
};

template<BW_SIZE_T N_CHANNELS>
inline Comb<N_CHANNELS>::Comb(float maxDelay) {
	bw_comb_init(&coeffs, maxDelay);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
	mem = nullptr;
}

template<BW_SIZE_T N_CHANNELS>
inline Comb<N_CHANNELS>::~Comb() {
	if (mem != nullptr)
		operator delete(mem);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comb<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_comb_set_sample_rate(&coeffs, sampleRate);
	BW_SIZE_T req = bw_comb_mem_req(&coeffs);
	if (mem != nullptr)
		operator delete(mem);
	mem = operator new(req * N_CHANNELS);
	void *m = mem;
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++, m = static_cast<char *>(m) + req)
		bw_comb_mem_set(&coeffs, states + i, m);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comb<N_CHANNELS>::reset() {
	bw_comb_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_comb_reset_state(&coeffs, states + i);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comb<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_comb_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comb<N_CHANNELS>::setDelayFF(float value) {
	bw_comb_set_delay_ff(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comb<N_CHANNELS>::setDelayFB(float value) {
	bw_comb_set_delay_fb(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comb<N_CHANNELS>::setCoeffBlend(float value) {
	bw_comb_set_coeff_blend(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comb<N_CHANNELS>::setCoeffFF(float value) {
	bw_comb_set_coeff_ff(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Comb<N_CHANNELS>::setCoeffFB(float value) {
	bw_comb_set_coeff_fb(&coeffs, value);
}

}

#endif
