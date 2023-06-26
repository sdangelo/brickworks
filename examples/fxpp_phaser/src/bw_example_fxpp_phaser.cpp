/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can rephaserribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is phaserributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#include "bw_example_fxpp_phaser.h"

void bw_example_fxpp_phaser_init(bw_example_fxpp_phaser *instance) {
	(void)instance;
}

void bw_example_fxpp_phaser_set_sample_rate(bw_example_fxpp_phaser *instance, float sample_rate) {
	instance->phaser.setSampleRate(sample_rate);
}

void bw_example_fxpp_phaser_reset(bw_example_fxpp_phaser *instance) {
	instance->phaser.reset();
}

void bw_example_fxpp_phaser_process(bw_example_fxpp_phaser *instance, const float** x, float** y, int n_samples) {
	instance->phaser.process({x[0]}, {y[0]}, n_samples);
}

void bw_example_fxpp_phaser_set_parameter(bw_example_fxpp_phaser *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_rate:
		instance->phaser.setRate((5.f - 0.1f) * value * value * value + 0.1f);
		break;
	case p_center:
		instance->phaser.setCenter((10e3f - 100.f) * value * value * value + 100.f);
		break;
	case p_amount:
		instance->phaser.setAmount(4.f * value);
		break;
	}
}

float bw_example_fxpp_phaser_get_parameter(bw_example_fxpp_phaser *instance, int index) {
	return instance->params[index];
}
