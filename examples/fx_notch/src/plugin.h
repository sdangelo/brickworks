#include "common.h"
#include <bw_notch.h>

typedef struct plugin {
	bw_notch_coeffs	notch_coeffs;
	bw_notch_state	notch_state;
} plugin;

static void plugin_init(plugin *instance) {
	bw_notch_init(&instance->notch_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_notch_set_sample_rate(&instance->notch_coeffs, sample_rate);
}

static size_t plugin_mem_req(plugin *instance) {
	(void)instance;
	return 0;
}

static void plugin_mem_set(plugin *instance, void *mem) {
	(void)instance;
	(void)mem;
}

static void plugin_reset(plugin *instance) {
	bw_notch_reset_coeffs(&instance->notch_coeffs);
	bw_notch_reset_state(&instance->notch_coeffs, &instance->notch_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_notch_set_cutoff(&instance->notch_coeffs, value);
		break;
	case 1:
		bw_notch_set_Q(&instance->notch_coeffs, value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_notch_process(&instance->notch_coeffs, &instance->notch_state, inputs[0], outputs[0], n_samples);
}
