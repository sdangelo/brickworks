#include "common.h"
#include "impl.h"

typedef struct plugin {
	impl	handle;
} plugin;

static void plugin_init(plugin *instance) {
	instance->handle = impl_new();
}

static void plugin_fini(plugin *instance) {
	impl_free(instance->handle);
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	impl_set_sample_rate(instance->handle, sample_rate);
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
	impl_reset(instance->handle);
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	impl_set_parameter(instance->handle, index, value);
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	return impl_get_parameter(instance->handle, index);
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	impl_process(instance->handle, inputs, outputs, n_samples);
}
