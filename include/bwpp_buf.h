/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can reosc_sinribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is osc_sinributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef BWPP_BUF_H
#define BWPP_BUF_H

#include <bw_buf.h>
#include <array>

namespace Brickworks {
	template<BW_SIZE_T N_CHANNELS>
	void bufFill(
			std::array<float *, N_CHANNELS> dest,
			float k,
			int nSamples);
			
	template<BW_SIZE_T N_CHANNELS>
	void bufNeg(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src,
			int nSamples);
			
	template<BW_SIZE_T N_CHANNELS>
	void bufAdd(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src,
			float k,
			int nSamples);

	template<BW_SIZE_T N_CHANNELS>
	void bufScale(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src,
			float k,
			int nSamples);

	template<BW_SIZE_T N_CHANNELS>
	void bufMix(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src1,
			std::array<const float *, N_CHANNELS> src2,
			int nSamples);

	template<BW_SIZE_T N_CHANNELS>
	void bufMul(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src1,
			std::array<const float *, N_CHANNELS> src2,
			int nSamples);
	
	template<BW_SIZE_T N_CHANNELS>
	inline void bufFill(
			std::array<float *, N_CHANNELS> dest,
			float k,
			int nSamples) {
		bw_buf_fill_multi(dest.data(), k, N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void bufNeg(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src,
			int nSamples) {
		bw_buf_neg_multi(dest.data(), src.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void bufAdd(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src,
			float k,
			int nSamples) {
		bw_buf_add_multi(dest.data(), src.data(), k, N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void bufScale(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src,
			float k,
			int nSamples) {
		bw_buf_scale_multi(dest.data(), src.data(), k, N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void bufMix(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src1,
			std::array<const float *, N_CHANNELS> src2,
			int nSamples) {
		bw_buf_mix_multi(dest.data(), src1.data(), src2.data(), N_CHANNELS, nSamples);
	}
	
	template<BW_SIZE_T N_CHANNELS>
	inline void bufMul(
			std::array<float *, N_CHANNELS> dest,
			std::array<const float *, N_CHANNELS> src1,
			std::array<const float *, N_CHANNELS> src2,
			int nSamples) {
		bw_buf_mul_multi(dest.data(), src1.data(), src2.data(), N_CHANNELS, nSamples);
	}
}

#endif
