#include "common.h"
#include <bw_trem.h>

typedef struct plugin {
	bw_trem_coeffs	trem_coeffs;
	bw_trem_state	trem_state;
} plugin;

static void plugin_init(plugin*instance) {
	bw_trem_init(&instance->trem_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_trem_set_sample_rate(&instance->trem_coeffs, sample_rate);
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
	bw_trem_reset_coeffs(&instance->trem_coeffs);
	bw_trem_reset_state(&instance->trem_coeffs, &instance->trem_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_trem_set_rate(&instance->trem_coeffs, value);
		break;
	case 1:
		bw_trem_set_amount(&instance->trem_coeffs, 0.01f * value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_trem_process(&instance->trem_coeffs, &instance->trem_state, inputs[0], outputs[0], n_samples);
}
