#include "common.h"
#include <bw_slew_lim.h>

typedef struct plugin {
	bw_slew_lim_coeffs	slew_lim_coeffs;
	bw_slew_lim_state	slew_lim_state;
} plugin;

static void plugin_init(plugin *instance) {
	bw_slew_lim_init(&instance->slew_lim_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_slew_lim_set_sample_rate(&instance->slew_lim_coeffs, sample_rate);
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
	bw_slew_lim_reset_coeffs(&instance->slew_lim_coeffs);
	bw_slew_lim_reset_state(&instance->slew_lim_coeffs, &instance->slew_lim_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	(void)index;
	bw_slew_lim_set_max_rate(&instance->slew_lim_coeffs, value);
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_slew_lim_process(&instance->slew_lim_coeffs, &instance->slew_lim_state, inputs[0], outputs[0], n_samples);
}
