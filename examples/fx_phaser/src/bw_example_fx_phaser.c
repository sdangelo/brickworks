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

#include "bw_example_fx_phaser.h"

void bw_example_fx_phaser_init(bw_example_fx_phaser *instance) {
	bw_phaser_init(&instance->phaser_coeffs);
}

void bw_example_fx_phaser_set_sample_rate(bw_example_fx_phaser *instance, float sample_rate) {
	bw_phaser_set_sample_rate(&instance->phaser_coeffs, sample_rate);
}

void bw_example_fx_phaser_reset(bw_example_fx_phaser *instance) {
	bw_phaser_reset_coeffs(&instance->phaser_coeffs);
	bw_phaser_reset_state(&instance->phaser_coeffs, &instance->phaser_state, 0.f);
}

void bw_example_fx_phaser_process(bw_example_fx_phaser *instance, const float** x, float** y, int n_samples) {
	bw_phaser_process(&instance->phaser_coeffs, &instance->phaser_state, x[0], y[0], n_samples);
}

void bw_example_fx_phaser_set_parameter(bw_example_fx_phaser *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_rate:
		bw_phaser_set_rate(&instance->phaser_coeffs, (5.f - 0.1f) * value * value * value + 0.1f);
		break;
	case p_center:
		bw_phaser_set_center(&instance->phaser_coeffs, (10e3f - 100.f) * value * value * value + 100.f);
		break;
	case p_amount:
		bw_phaser_set_amount(&instance->phaser_coeffs, 4.f * value);
		break;
	}
}

float bw_example_fx_phaser_get_parameter(bw_example_fx_phaser *instance, int index) {
	return instance->params[index];
}
