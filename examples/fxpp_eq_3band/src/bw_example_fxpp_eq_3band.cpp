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

#include "bw_example_fxpp_eq_3band.h"

void bw_example_fxpp_eq_3band_init(bw_example_fxpp_eq_3band *instance) {
	(void)instance;
}

void bw_example_fxpp_eq_3band_set_sample_rate(bw_example_fxpp_eq_3band *instance, float sample_rate) {
	instance->ls2.setSampleRate(sample_rate);
	instance->peak.setSampleRate(sample_rate);
	instance->hs2.setSampleRate(sample_rate);
}

void bw_example_fxpp_eq_3band_reset(bw_example_fxpp_eq_3band *instance) {
	instance->ls2.reset();
	instance->peak.reset();
	instance->hs2.reset();
}

void bw_example_fxpp_eq_3band_process(bw_example_fxpp_eq_3band *instance, const float** x, float** y, int n_samples) {
	instance->ls2.process({x[0]}, {y[0]}, n_samples);
	instance->peak.process({y[0]}, {y[0]}, n_samples);
	instance->hs2.process({y[0]}, {y[0]}, n_samples);
}

void bw_example_fxpp_eq_3band_set_parameter(bw_example_fxpp_eq_3band *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_ls_cutoff:
		instance->ls2.setCutoff(20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_ls_gain:
		instance->ls2.setDcGainDB(-20.f + 40.f * value);
		break;
	case p_ls_Q:
		instance->ls2.setQ(0.5f + 4.5f * value);
		break;
	case p_peak_cutoff:
		instance->peak.setCutoff(20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_peak_gain:
		instance->peak.setPeakGainDB(-20.f + 40.f * value);
		break;
	case p_peak_bw:
		instance->peak.setBandwidth(0.01f + 1.99f * value);
		break;
	case p_hs_cutoff:
		instance->hs2.setCutoff(20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_hs_gain:
		instance->hs2.setHighGainDB(-20.f + 40.f * value);
		break;
	case p_hs_Q:
		instance->hs2.setQ(0.5f + 4.5f * value);
		break;
	}
}

float bw_example_fxpp_eq_3band_get_parameter(bw_example_fxpp_eq_3band *instance, int index) {
	return instance->params[index];
}
