/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can redriveribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is driveributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef BWPP_DRIVE_H
#define BWPP_DRIVE_H

#include <bw_drive.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::Drive
 *  ```>>> */
template<size_t N_CHANNELS>
class Drive {
public:
	Drive();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setDrive(float value);
	void setTone(float value);
	void setVolume(float value);
/*! <<<...
 *  }
 *  ```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_drive_coeffs	 coeffs;
	bw_drive_state	 states[N_CHANNELS];
	bw_drive_state	*statesP[N_CHANNELS];
};

template<size_t N_CHANNELS>
inline Drive<N_CHANNELS>::Drive() {
	bw_drive_init(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_drive_set_sample_rate(&coeffs, sampleRate);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::reset() {
	bw_drive_reset_coeffs(&coeffs);
	for (size_t i = 0; i < N_CHANNELS; i++)
		bw_drive_reset_state(&coeffs, states + i);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_drive_process_multi(&coeffs, statesP, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::setDrive(float value) {
	bw_drive_set_drive(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::setTone(float value) {
	bw_drive_set_tone(&coeffs, value);
}

template<size_t N_CHANNELS>
inline void Drive<N_CHANNELS>::setVolume(float value) {
	bw_drive_set_volume(&coeffs, value);
}

}

#endif
