/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can retremribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is tremributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#ifndef _BW_EXAMPLE_FXPP_TREM_H
#define _BW_EXAMPLE_FXPP_TREM_H

#include <bwpp_trem.h>

using namespace Brickworks;

extern "C" {

enum {
	p_rate,
	p_amount,
	p_n
};

struct _bw_example_fxpp_trem {
	// Sub-components
	Trem<1>	trem;

	// Parameters
	float	params[p_n];
};
typedef struct _bw_example_fxpp_trem bw_example_fxpp_trem;

void bw_example_fxpp_trem_init(bw_example_fxpp_trem *instance);
void bw_example_fxpp_trem_set_sample_rate(bw_example_fxpp_trem *instance, float sample_rate);
void bw_example_fxpp_trem_reset(bw_example_fxpp_trem *instance);
void bw_example_fxpp_trem_process(bw_example_fxpp_trem *instance, const float** x, float** y, int n_samples);
void bw_example_fxpp_trem_set_parameter(bw_example_fxpp_trem *instance, int index, float value);
float bw_example_fxpp_trem_get_parameter(bw_example_fxpp_trem *instance, int index);

}

#endif
