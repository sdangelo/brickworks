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

#include <bw_svf.h>

#include <bw_math.h>
#include <bw_inline_one_pole.h>

struct _bw_svf {
	// Coefficients
	float	t_k;
	float	smooth_mA1;

	float	t;
	float	k;
	float	hp_hp_z1;
	float	hp_bp_z1;
	float	hp_x;

	// Parameters
	float	cutoff;
	float	Q;

	float	cutoff_cur;
	float	Q_cur;

	// State
	char	first_run;
	float	hp_z1;
	float	lp_z1;
	float	bp_z1;
};

bw_svf bw_svf_new() {
	bw_svf instance = (bw_svf)BW_MALLOC(sizeof(struct _bw_svf));
	if (instance == NULL)
		return NULL;

	instance->cutoff = 1e3f;
	instance->Q = 0.5f;

	return instance;
}

void bw_svf_free(bw_svf instance) {
	BW_FREE(instance);
}

void bw_svf_set_sample_rate(bw_svf instance, float sample_rate) {
	instance->t_k = 3.141592653589793f / sample_rate;
	instance->smooth_mA1 = bw_inline_one_pole_get_mA1(sample_rate, 0.05f);
}

void bw_svf_reset(bw_svf instance) {
	instance->first_run = 1;
}

#define PARAM_CUTOFF	1
#define PARAM_Q		(1<<1)

static inline void update_coefficients(bw_svf instance) {
	const char cutoff_changed = instance->cutoff != instance->cutoff_cur || instance->first_run;
	const char Q_changed = instance->Q != instance->Q_cur || instance->first_run;
	if (cutoff_changed || Q_changed) {
		if (cutoff_changed) {
			instance->cutoff_cur = bw_inline_one_pole_sticky_rel(instance->cutoff, instance->cutoff_cur, instance->smooth_mA1, 1e-6f);
			instance->t = bw_tanf_3(instance->t_k * instance->cutoff_cur);
		}
		if (Q_changed) {
			instance->Q_cur = bw_inline_one_pole_sticky_abs(instance->Q, instance->Q_cur, instance->smooth_mA1, 1e-6f);
			instance->k = bw_rcpf_2(instance->Q_cur);
		}
		const float kpt = instance->k + instance->t;
		instance->hp_hp_z1 = instance->t * kpt;
		instance->hp_bp_z1 = instance->t + kpt;
		instance->hp_x = bw_rcpf_2(1.f + instance->hp_hp_z1);
	}
}

void bw_svf_process(bw_svf instance, const float *x, float *y_lp, float *y_bp, float *y_hp, int n_samples) {
	if (instance->first_run) {
		instance->cutoff_cur = instance->cutoff;
		instance->Q_cur = instance->Q;
		update_coefficients(instance);
		instance->hp_z1 = 0.f;
		instance->lp_z1 = 0.f;
		instance->bp_z1 = 0.f;
		instance->first_run = 0;
	}

	for (int i = 0; i < n_samples; i++) {
		update_coefficients(instance);

		const float hp = instance->hp_x * (x[i] - instance->lp_z1 + instance->hp_bp_z1 * instance->bp_z1 - instance->hp_hp_z1 * instance->hp_z1);
		const float bp = instance->bp_z1 - instance->t * (hp + instance->hp_z1);
		const float lp = instance->lp_z1 - instance->t * (bp + instance->bp_z1);
		instance->hp_z1 = hp;
		instance->lp_z1 = lp;
		instance->bp_z1 = bp;
		if (y_lp)
			y_lp[i] = lp;
		if (y_bp)
			y_bp[i] = bp;
		if (y_hp)
			y_hp[i] = hp;
	}
}

void bw_svf_set_cutoff(bw_svf instance, float value) {
	instance->cutoff = value;
}

void bw_svf_set_Q(bw_svf instance, float value) {
	instance->Q = value;
}
