/*
 * Brickworks
 *
 * Copyright (C) 2024 Orastron Srl unipersonale
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
 * File author: Stefano D'Angelo
 */

#include "common.h"
#include "impl.h"

typedef struct plugin {
	impl	handle;
} plugin;

static void plugin_init(plugin *instance, plugin_callbacks *cbs) {
	(void)cbs;
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
