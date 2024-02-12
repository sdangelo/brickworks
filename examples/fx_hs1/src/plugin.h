#include "common.h"
#include <bw_hs1.h>

typedef struct plugin {
	bw_hs1_coeffs	hs1_coeffs;
	bw_hs1_state	hs1_state;
} plugin;

static void plugin_init(plugin *instance) {
	bw_hs1_init(&instance->hs1_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_hs1_set_sample_rate(&instance->hs1_coeffs, sample_rate);
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
	bw_hs1_reset_coeffs(&instance->hs1_coeffs);
	bw_hs1_reset_state(&instance->hs1_coeffs, &instance->hs1_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_hs1_set_cutoff(&instance->hs1_coeffs, value);
		break;
	case 1:
		bw_hs1_set_high_gain_dB(&instance->hs1_coeffs, value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_hs1_process(&instance->hs1_coeffs, &instance->hs1_state, inputs[0], outputs[0], n_samples);
}
