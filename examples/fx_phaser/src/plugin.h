#include "common.h"
#include <bw_phaser.h>

typedef struct plugin {
	bw_phaser_coeffs	phaser_coeffs;
	bw_phaser_state		phaser_state;
} plugin;

static void plugin_init(plugin*instance) {
	bw_phaser_init(&instance->phaser_coeffs);
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_phaser_set_sample_rate(&instance->phaser_coeffs, sample_rate);
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
	bw_phaser_reset_coeffs(&instance->phaser_coeffs);
	bw_phaser_reset_state(&instance->phaser_coeffs, &instance->phaser_state, 0.f);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
		bw_phaser_set_rate(&instance->phaser_coeffs, value);
		break;
	case 1:
		bw_phaser_set_amount(&instance->phaser_coeffs, value);
		break;
	case 2:
		bw_phaser_set_center(&instance->phaser_coeffs, value);
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	bw_phaser_process(&instance->phaser_coeffs, &instance->phaser_state, inputs[0], outputs[0], n_samples);
}
