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

#ifndef BWPP_DELAY_H
#define BWPP_DELAY_H

#include <bw_delay.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Delay
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class Delay {
public:
	Delay(float maxDelay = 1.f);
	~Delay();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);
		
	float read(BW_SIZE_T channel, BW_SIZE_T di, float df);
	void write(BW_SIZE_T channel, float x);

	void setDelay(float value);
	
	BW_SIZE_T getLength();
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_delay_coeffs	 coeffs;
	bw_delay_state	 states[N_CHANNELS];
	bw_delay_state	*statesP[N_CHANNELS];
	void		*mem;
};

template<BW_SIZE_T N_CHANNELS>
inline Delay<N_CHANNELS>::Delay(float maxDelay) {
	bw_delay_init(&coeffs, maxDelay);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
	mem = nullptr;
}

template<BW_SIZE_T N_CHANNELS>
inline Delay<N_CHANNELS>::~Delay() {
	if (mem != nullptr)
		operator delete(mem);
}

template<BW_SIZE_T N_CHANNELS>
inline void Delay<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_delay_set_sample_rate(&coeffs, sampleRate);
	BW_SIZE_T req = bw_delay_mem_req(&coeffs);
	if (mem != nullptr)
		operator delete(mem);
	mem = operator new(req * N_CHANNELS);
	void *m = mem;
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++, m = static_cast<char *>(m) + req)
		bw_delay_mem_set(&coeffs, states + i, m);
}

template<BW_SIZE_T N_CHANNELS>
inline void Delay<N_CHANNELS>::reset() {
	bw_delay_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_delay_reset_state(&coeffs, states + i);
}

template<BW_SIZE_T N_CHANNELS>
inline void Delay<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_delay_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline float Delay<N_CHANNELS>::read(BW_SIZE_T channel, BW_SIZE_T di, float df) {
	return bw_delay_read(&coeffs, states + channel, di, df);
}

template<BW_SIZE_T N_CHANNELS>
inline void Delay<N_CHANNELS>::write(BW_SIZE_T channel, float x) {
	bw_delay_write(&coeffs, states + channel, x);
}

template<BW_SIZE_T N_CHANNELS>
inline void Delay<N_CHANNELS>::setDelay(float value) {
	bw_delay_set_delay(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline BW_SIZE_T Delay<N_CHANNELS>::getLength() {
	return bw_delay_get_length(&coeffs);
}

}

#endif
