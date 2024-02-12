#include "common.h"
#include <bw_lp1.h>

typedef struct plugin {
	bw_lp1_coeffs	lp1_coeffs;
	bw_lp1_state	lp1_state;
} plugin;

static void plugin_init(plugin *instance) {
	bw_lp1_init(&instance->lp1_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_lp1_set_sample_rate(&instance->lp1_coeffs, sample_rate);
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
	bw_lp1_reset_coeffs(&instance->lp1_coeffs);
	bw_lp1_reset_state(&instance->lp1_coeffs, &instance->lp1_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	(void)index;
	bw_lp1_set_cutoff(&instance->lp1_coeffs, value);
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_lp1_process(&instance->lp1_coeffs, &instance->lp1_state, inputs[0], outputs[0], n_samples);
}
