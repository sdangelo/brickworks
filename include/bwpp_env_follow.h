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

#ifndef BWPP_ENV_FOLLOW_H
#define BWPP_ENV_FOLLOW_H

#include <bw_env_follow.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::EnvFollow
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class EnvFollow {
public:
	EnvFollow();

	void setSampleRate(float sampleRate);
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setAttackTau(float value);
	void setReleaseTau(float value);
	
	float getYZ1(BW_SIZE_T channel);
/*! <<<... }```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_env_follow_coeffs	 coeffs;
	bw_env_follow_state	 states[N_CHANNELS];
	bw_env_follow_state	*statesP[N_CHANNELS];
};

template<BW_SIZE_T N_CHANNELS>
inline EnvFollow<N_CHANNELS>::EnvFollow() {
	bw_env_follow_init(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		statesP[i] = states + i;
}

template<BW_SIZE_T N_CHANNELS>
inline void EnvFollow<N_CHANNELS>::setSampleRate(float sampleRate) {
	bw_env_follow_set_sample_rate(&coeffs, sampleRate);
}

template<BW_SIZE_T N_CHANNELS>
inline void EnvFollow<N_CHANNELS>::reset() {
	bw_env_follow_reset_coeffs(&coeffs);
	for (BW_SIZE_T i = 0; i < N_CHANNELS; i++)
		bw_env_follow_reset_state(&coeffs, states + i);
}

template<BW_SIZE_T N_CHANNELS>
inline void EnvFollow<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_env_follow_process_multi(&coeffs, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void EnvFollow<N_CHANNELS>::setAttackTau(float value) {
	bw_env_follow_set_attack_tau(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline void EnvFollow<N_CHANNELS>::setReleaseTau(float value) {
	bw_env_follow_set_release_tau(&coeffs, value);
}

template<BW_SIZE_T N_CHANNELS>
inline float EnvFollow<N_CHANNELS>::getYZ1(BW_SIZE_T channel) {
	return bw_env_follow_get_y_z1(states + channel);
}

}

#endif
