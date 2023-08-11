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

#include "bw_example_synthpp_simple.h"

#include <bw_buf.h>

void bw_example_synthpp_simple_init(bw_example_synthpp_simple *instance) {
	instance->oscPulse.setAntialiasing(true);
}

void bw_example_synthpp_simple_set_sample_rate(bw_example_synthpp_simple *instance, float sample_rate) {
	instance->phaseGen.setSampleRate(sample_rate);
	instance->oscPulse.setSampleRate(sample_rate);
	instance->svf.setSampleRate(sample_rate);
	instance->envGen.setSampleRate(sample_rate);
	instance->gain.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);
}

void bw_example_synthpp_simple_reset(bw_example_synthpp_simple *instance) {
	instance->phaseGen.reset();
	instance->oscPulse.reset();
	instance->oscFilt.reset();
	instance->svf.reset();
	instance->envGen.reset();
	instance->gain.reset();
	instance->ppm.reset();
	instance->note = -1;
}

void bw_example_synthpp_simple_process(bw_example_synthpp_simple *instance, const float** x, float** y, int n_samples) {
	(void)x;

	if (instance->note >= 0)
		instance->phaseGen.setFrequency(
			440.f * bw_pow2f(8.333333333333333e-2f * ((instance->note - 69) + 2.f * instance->params[p_master_tune] - 1.f)));

	for (int i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = y[0] + i;
		int n = bw_minf(n_samples - i, BUFFER_SIZE);
		
		instance->phaseGen.process({nullptr}, {out}, {instance->buf}, n);
		instance->oscPulse.process({out}, {instance->buf}, {out}, n);
		instance->oscFilt.process({out}, {out}, n);
		instance->svf.process({out}, {out}, {nullptr}, {nullptr}, n);
		instance->envGen.process({instance->note >= 0}, {instance->buf}, n);
		bufMul<1>({out}, {instance->buf}, {out}, n);
		instance->gain.process({out}, {out}, n);
		instance->ppm.process({out}, {nullptr}, n);
	}
}

void bw_example_synthpp_simple_set_parameter(bw_example_synthpp_simple *instance, int index, float value) {
	instance->params[index] = value;
	switch (index) {
	case p_volume:
		instance->gain.setGainLin(value * value * value);
		break;
	case p_portamento:
		instance->phaseGen.setPortamentoTau(value);
		break;
	case p_pulse_width:
		instance->oscPulse.setPulseWidth(value);
		break;
	case p_cutoff:
		instance->svf.setCutoff(20.f + (20e3f - 20.f) * value * value * value);
		break;
	case p_Q:
		instance->svf.setQ(0.5f + 9.5f * value);
		break;
	case p_attack:
		instance->envGen.setAttack(bw_maxf(0.002f, value));
		break;
	case p_decay:
		instance->envGen.setDecay(value);
		break;
	case p_sustain:
		instance->envGen.setSustain(value);
		break;
	case p_release:
		instance->envGen.setRelease(bw_maxf(0.002f, value));
		break;
	}
}

float bw_example_synthpp_simple_get_parameter(bw_example_synthpp_simple *instance, int index) {
	if (index < p_n)
		return instance->params[index];
	const float v = instance->ppm.getYZ1(0);
	return v < -200.f ? 0.f : bw_clipf(0.01666666666666666f * v + 1.f, 0.f, 1.f);
}

void bw_example_synthpp_simple_note_on(bw_example_synthpp_simple *instance, char note, char velocity) {
	if (velocity == 0) 
		bw_example_synthpp_simple_note_off(instance, note);
	else
		instance->note = note;
}

void bw_example_synthpp_simple_note_off(bw_example_synthpp_simple *instance, char note) {
	if (note == instance->note)
		instance->note = -1;
}
