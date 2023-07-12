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

#ifndef BWPP_BD_REDUCE_H
#define BWPP_BD_REDUCE_H

#include <bw_bd_reduce.h>
#include <array>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::BDReduce
 *  ```>>> */
template<BW_SIZE_T N_CHANNELS>
class BDReduce {
public:
	BDReduce();
	
	void reset();
	void process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples);

	void setBitDepth(char value);
/*! <<<... }```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

private:
	bw_bd_reduce_coeffs	 coeffs;
};

template<BW_SIZE_T N_CHANNELS>
inline BDReduce<N_CHANNELS>::BDReduce() {
	bw_bd_reduce_init(&coeffs);
}

template<BW_SIZE_T N_CHANNELS>
inline void BDReduce<N_CHANNELS>::reset() {
	bw_bd_reduce_reset_coeffs(&coeffs);
}

template<BW_SIZE_T N_CHANNELS>
inline void BDReduce<N_CHANNELS>::process(
		std::array<const float *, N_CHANNELS> x,
		std::array<float *, N_CHANNELS> y,
		int nSamples) {
	bw_bd_reduce_process_multi(&coeffs, x.data(), y.data(), N_CHANNELS, nSamples);
}

template<BW_SIZE_T N_CHANNELS>
inline void BDReduce<N_CHANNELS>::setBitDepth(char value) {
	bw_bd_reduce_set_bit_depth(&coeffs, value);
}

}

#endif
