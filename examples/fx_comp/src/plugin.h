#include "common.h"
#include <bw_comp.h>

typedef struct plugin {
	bw_comp_coeffs	comp_coeffs;
	bw_comp_state	comp_state;
	char		ext_sidechain;
} plugin;

static void plugin_init(plugin *instance) {
	bw_comp_init(&instance->comp_coeffs);
	instance->ext_sidechain = 0;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_comp_set_sample_rate(&instance->comp_coeffs, sample_rate);
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
	bw_comp_reset_coeffs(&instance->comp_coeffs);
	bw_comp_reset_state(&instance->comp_coeffs, &instance->comp_state, 0.f, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_comp_set_thresh_dBFS(&instance->comp_coeffs, value);
		break;
	case 1:
		bw_comp_set_ratio(&instance->comp_coeffs, bw_rcpf(value));
		break;
	case 2:
		// using rise time 10% -> 90%: tau = rise time / log(9)
		bw_comp_set_attack_tau(&instance->comp_coeffs, (0.001f * 0.4551196133134186f) * value);
		break;
	case 3:
		// as before
		bw_comp_set_release_tau(&instance->comp_coeffs, (0.001f * 0.4551196133134186f) * value);
		break;
	case 4:
		bw_comp_set_gain_dB(&instance->comp_coeffs, value);
		break;
	case 5:
		instance->ext_sidechain = (char)value;
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_comp_process(&instance->comp_coeffs, &instance->comp_state, inputs[0], instance->ext_sidechain ? inputs[1] : inputs[0], outputs[0], n_samples);
}
