#include "common.h"
#include <bw_ap1.h>

typedef struct plugin {
	bw_ap1_coeffs	ap1_coeffs;
	bw_ap1_state	ap1_state;
} plugin;

static void plugin_init(plugin *instance) {
	bw_ap1_init(&instance->ap1_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_ap1_set_sample_rate(&instance->ap1_coeffs, sample_rate);
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
	bw_ap1_reset_coeffs(&instance->ap1_coeffs);
	bw_ap1_reset_state(&instance->ap1_coeffs, &instance->ap1_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	(void)index;

	bw_ap1_set_cutoff(&instance->ap1_coeffs, value);
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_ap1_process(&instance->ap1_coeffs, &instance->ap1_state, inputs[0], outputs[0], n_samples);
}