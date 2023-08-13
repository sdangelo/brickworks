/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can refuzzribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is fuzzributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef _BW_EXAMPLE_FXPP_FUZZ_H
#define _BW_EXAMPLE_FXPP_FUZZ_H

#include "platform.h"

#include <bw_fuzz.h>
#include <bw_src_int.h>

using namespace Brickworks;

extern "C" {

enum {
	p_fuzz,
	p_volume,
	p_n
};

#define BUF_SIZE	32

struct _bw_example_fxpp_fuzz {
	// Sub-components
	Fuzz<1>		fuzz;
	SRCInt<1>	srcUp;
	SRCInt<1>	srcDown;

	// Parameters
	float		params[p_n];

	// Buffers
	float		buf[BUF_SIZE];

	_bw_example_fxpp_fuzz() : srcUp(2), srcDown(-2) {}
};
typedef struct _bw_example_fxpp_fuzz bw_example_fxpp_fuzz;

void bw_example_fxpp_fuzz_init(bw_example_fxpp_fuzz *instance);
void bw_example_fxpp_fuzz_set_sample_rate(bw_example_fxpp_fuzz *instance, float sample_rate);
void bw_example_fxpp_fuzz_reset(bw_example_fxpp_fuzz *instance);
void bw_example_fxpp_fuzz_process(bw_example_fxpp_fuzz *instance, const float** x, float** y, int n_samples);
void bw_example_fxpp_fuzz_set_parameter(bw_example_fxpp_fuzz *instance, int index, float value);
float bw_example_fxpp_fuzz_get_parameter(bw_example_fxpp_fuzz *instance, int index);

}

#endif
