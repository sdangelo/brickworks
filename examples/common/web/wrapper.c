/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
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
 * File authors: Stefano D'Angelo
 */

#include "config.h"
#include "walloc.h"

struct _wrapper {
	P_TYPE		 instance;
	// wasting a little bit of memory if buses are mono, but let's KISS
#if NUM_BUSES_IN != 0
	float		 ins[NUM_BUSES_IN * 2 * 128];
	const float	*x[NUM_BUSES_IN * 2];
#endif
#if NUM_BUSES_OUT != 0
	float		 outs[NUM_BUSES_OUT * 2 * 128];
	float		*y[NUM_BUSES_OUT * 2];
#endif
#if NUM_PARAMETERS != 0
	float		 param_values[NUM_PARAMETERS];
#endif
#ifdef P_MEM_REQ
	void		*mem;
#endif
};

typedef struct _wrapper *wrapper;

void wrapper_set_parameter(wrapper w, int index, float value);

wrapper wrapper_new(float sample_rate) {
	wrapper ret = malloc(sizeof(struct _wrapper));
	if (ret == NULL)
		return NULL;

	P_INIT(&ret->instance);

#if NUM_BUSES_IN != 0
	int dx = 0;
	int jx = 0;
	for (int i = 0; i < NUM_BUSES_IN; i++) {
		ret->x[jx] = ret->ins + dx;
		dx += 128;
		jx++;
		if (config_buses_in[i].configs & IO_STEREO) {
			ret->x[jx] = ret->ins + dx;
			dx += 128;
			jx++;
		}
	}
#endif
#if NUM_BUSES_OUT != 0
	int dy = 0;
	int jy = 0;
	for (int i = 0; i < NUM_BUSES_OUT; i++) {
		ret->y[jy] = ret->outs + dy;
		dy += 128;
		jy++;
		if (config_buses_out[i].configs & IO_STEREO) {
			ret->y[jy] = ret->outs + dy;
			dy += 128;
			jy++;
		}
	}
#endif

#if NUM_PARAMETERS != 0
	for (int i = 0; i < NUM_PARAMETERS; i++)
		wrapper_set_parameter(ret, i, config_parameters[i].defaultValueUnmapped);
#endif

	P_SET_SAMPLE_RATE(&ret->instance, sample_rate);
#ifdef P_MEM_REQ
	size_t req = P_MEM_REQ(&ret->instance);
	if (req) {
		ret->mem = malloc(req);
		if (ret->mem == NULL) {
			free(ret);
			return NULL;
		}
		P_MEM_SET(&ret->instance, ret->mem);
	} else
		ret->mem = NULL;
#endif
	P_RESET(&ret->instance);

	return ret;
}

void wrapper_free(wrapper w) {
#ifdef P_MEM_REQ
	if (w->mem)
		free(w->mem);
#endif
#ifdef P_FINI
	P_FINI(&w->instance);
#endif
	free(w);
}

float *wrapper_get_ins(wrapper w) {
#if NUM_BUSES_IN != 0
	return w->ins;
#else
	(void)w;
	return NULL;
#endif
}

float *wrapper_get_outs(wrapper w) {
#if NUM_BUSES_OUT != 0
	return w->outs;
#else
	(void)w;
	return NULL;
#endif
}

float *wrapper_get_param_values(wrapper w) {
#if NUM_PARAMETERS != 0
	return w->param_values;
#else
	(void)w;
	return NULL;
#endif
}

void wrapper_process(wrapper w, int n_samples) {
#if NUM_BUSES_IN != 0
	const float **x = w->x;
#else
	const float **x = NULL;
#endif
#if NUM_BUSES_OUT != 0
	float **y = w->y;
#else
	float **y = NULL;
#endif
	P_PROCESS(&w->instance, x, y, n_samples);

#if NUM_PARAMETERS != 0
	for (int i = 0; i < NUM_PARAMETERS; i++)
		w->param_values[i] = P_GET_PARAMETER(&w->instance, i);
#endif
}

void wrapper_set_parameter(wrapper w, int index, float value) {
#if NUM_PARAMETERS != 0
	P_SET_PARAMETER(&w->instance, index, value);
	w->param_values[index] = value;
#else
	(void)w;
	(void)index;
	(void)value;
#endif
}

void wrapper_note_on(wrapper w, int note, int velocity) {
#ifdef P_NOTE_ON
	P_NOTE_ON(&w->instance, note, velocity);
#else
	(void)w;
	(void)note;
	(void)velocity;
#endif
}

void wrapper_note_off(wrapper w, int note) {
#ifdef P_NOTE_OFF
	P_NOTE_OFF(&w->instance, note);
#else
	(void)w;
	(void)note;
#endif
}

void wrapper_pitch_bend(wrapper w, int bend) {
#ifdef P_PITCH_BEND
	P_PITCH_BEND(&w->instance, bend);
#else
	(void)w;
	(void)bend;
#endif
}

void wrapper_mod_wheel(wrapper w, int wheel) {
#ifdef P_MOD_WHEEL
	P_MOD_WHEEL(&w->instance, wheel);
#else
	(void)w;
	(void)wheel;
#endif
}
