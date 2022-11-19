/*
 * Brickworks
 *
 * Copyright (C) 2022 Orastron Srl unipersonale
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
 *
 * File author: Stefano D'Angelo
 */

#include <bw_env_gen.h>

#include <bw_math.h>
#include <bw_inline_one_pole.h>

void bw_env_gen_init(bw_env_gen *instance) {
	instance->gate = 0;
	instance->attack = 0.f;
	instance->decay = 0.f;
	instance->sustain = 1.f;
	instance->release = 0.f;
}

void bw_env_gen_set_sample_rate(bw_env_gen *instance, float sample_rate) {
	instance->T = 1.f / sample_rate;
	instance->smooth_mA1 = bw_inline_one_pole_get_mA1(sample_rate, 0.05f);
}

void bw_env_gen_reset(bw_env_gen *instance) {
	instance->first_run = 1;
	instance->param_changed = ~0;
}

#define PARAM_ATTACK	1
#define PARAM_DECAY	(1<<1)
#define PARAM_SUSTAIN	(1<<2)
#define PARAM_RELEASE	(1<<3)

void bw_env_gen_process(bw_env_gen *instance, float* y, int n_samples) {
	if (instance->param_changed) {
		// 1 ns considered instantaneous
		if (instance->param_changed & PARAM_ATTACK)
			instance->attack_inc = instance->attack > 1e-9f ? instance->T * bw_rcpf_2(instance->attack) : INFINITY;
		if (instance->param_changed & (PARAM_DECAY | PARAM_SUSTAIN))
			instance->decay_inc = instance->decay > 1e-9f ? (instance->sustain - 1.f) * instance->T * bw_rcpf_2(instance->decay) : -INFINITY;
		if (instance->param_changed & (PARAM_SUSTAIN | PARAM_RELEASE))
			instance->release_inc = instance->release > 1e-9f ? -instance->sustain * instance->T * bw_rcpf_2(instance->release) : -INFINITY;
	}

	if (instance->first_run) {
		instance->state = _bw_env_gen_state_off;
		instance->y_z1 = 0.f;
		instance->first_run = 0;
	}

	if (instance->gate) {
		if (instance->state == _bw_env_gen_state_off || instance->state == _bw_env_gen_state_release)
			instance->state = _bw_env_gen_state_attack;
	} else {
		if (instance->state != _bw_env_gen_state_off)
			instance->state = _bw_env_gen_state_release;
	}

	for (int i = 0; i < n_samples; i++) {
		float v;
		switch (instance->state) {
		case _bw_env_gen_state_attack:
			v = instance->y_z1 + instance->attack_inc;
			if (v >= 1.f) {
				v = 1.f;
				instance->state = _bw_env_gen_state_decay;
			}
			break;
		case _bw_env_gen_state_decay:
			v = instance->y_z1 + instance->decay_inc;
			if (v <= instance->sustain) {
				v = instance->sustain;
				instance->state = _bw_env_gen_state_sustain;
			}
			break;
		case _bw_env_gen_state_sustain:
			v = bw_inline_one_pole(instance->sustain, instance->y_z1, instance->smooth_mA1);
			break;
		case _bw_env_gen_state_release:
			v = instance->y_z1 + instance->release_inc;
			if (v <= 0.f) {
				v = 0.f;
				instance->state = _bw_env_gen_state_off;
			}
			break;
		case _bw_env_gen_state_off:
			v = 0.f;
			break;
		}

		y[i] = v;
		instance->y_z1 = y[i];
	}
}

void bw_env_gen_set_gate(bw_env_gen *instance, char value) {
	instance->gate = value;
}

void bw_env_gen_set_attack(bw_env_gen *instance, float value) {
	if (instance->attack != value) {
		instance->attack = value;
		instance->param_changed |= PARAM_ATTACK;
	}
}

void bw_env_gen_set_decay(bw_env_gen *instance, float value) {
	if (instance->decay != value) {
		instance->decay = value;
		instance->param_changed |= PARAM_DECAY;
	}
}

void bw_env_gen_set_sustain(bw_env_gen *instance, float value) {
	if (instance->sustain != value) {
		instance->sustain = value;
		instance->param_changed |= PARAM_SUSTAIN;
	}
}

void bw_env_gen_set_release(bw_env_gen *instance, float value) {
	if (instance->release != value) {
		instance->release = value;
		instance->param_changed |= PARAM_RELEASE;
	}
}

char bw_env_gen_get_is_off(bw_env_gen *instance) {
	return instance->state == _bw_env_gen_state_off;
}
