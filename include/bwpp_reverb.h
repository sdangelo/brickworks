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

#ifndef BWPP_REVERB_H
#define BWPP_REVERB_H

#include <bw_reverb.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Reverb
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class Reverb {
public:
	Reverb();
	~Reverb();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> xl,
		std::array<const float *, N_CHANNELS> xr,
		std::array<float *, N_CHANNELS> yl,
		std::array<float *, N_CHANNELS> yr,
		int nSamples);

	void setPredelay(float value);
	void setBandwidth(float value);
	void setDamping(float value);
	void setDecay(float value);
	void setWet(float value);
/*! <<<... }```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_reverb_coeffs	 coeffs;
	bw_reverb_state		 states[N_CHANNELS];
	bw_reverb_state		*statesP[N_CHANNELS];
	void			*mem;
};

template<BW_SIZE_T N_CHANNELS>
inline Reverb<N_CHANNELS>::Reverb() {
	bw_reverb_init(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
	mem = nullptr;
}

template<BW_SIZE_T N_CHANNELS>
inline Reverb<N_CHANNELS>::~Reverb() {
	if (mem != nullptr)
		operator delete(mem);
}

template<BW_SIZE_T N_CHANNELS>
inline void Reverb<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_reverb_set_sample_rate(&coeffs, sampleRate);
	BW_SIZE_T req = bw_reverb_mem_req(&coeffs);
	if (mem != nullptr)
		operator delete(mem);
	mem = operator new(req * N_CHANNELS);
	void *m = mem;
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++, m = static_cast<char *>(m) + req)
		bw_reverb_mem_set(&coeffs, states + i, m);
}

template<BW_SIZE_T N_CHANNELS>
inline void Reverb<N_CHANNELS>::reset() {
	bw_reverb_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_reverb_reset_state(&coeffs, states + i);
}

template<BW_SIZE_T N_CHANNELS>
inline void Reverb<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> xl,
		std::array<const float *, N_CHANNELS> xr,
		std::array<float *, N_CHANNELS> yl,
		std::array<float *, N_CHANNELS> yr,
		int nSamples) {
	bw_reverb_process_multi(&coeffs, statesP, xl.data(), xr.data(), yl.data(), yr.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void Reverb<N_CHANNELS>::setPredelay(float value) {
	bw_reverb_set_predelay(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Reverb<N_CHANNELS>::setBandwidth(float value) {
	bw_reverb_set_bandwidth(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Reverb<N_CHANNELS>::setDamping(float value) {
	bw_reverb_set_damping(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Reverb<N_CHANNELS>::setDecay(float value) {
	bw_reverb_set_decay(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void Reverb<N_CHANNELS>::setWet(float value) {
	bw_reverb_set_wet(&coeffs, value);
}

}

#endif
