#include "impl.h"

#include "common.h"
#include <bw_slew_lim.h>

using namespace Brickworks;

extern "C" {

impl impl_new(void) {
	SlewLim<1> *instance = new SlewLim<1>();
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	SlewLim<1> *instance = reinterpret_cast<SlewLim<1> *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	SlewLim<1> *instance = reinterpret_cast<SlewLim<1> *>(handle);
	instance->setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	SlewLim<1> *instance = reinterpret_cast<SlewLim<1> *>(handle);
	instance->reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	(void)index;
	SlewLim<1> *instance = reinterpret_cast<SlewLim<1> *>(handle);
	instance->setMaxRate(value);
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	SlewLim<1> *instance = reinterpret_cast<SlewLim<1> *>(handle);
	instance->process(inputs, outputs, n_samples);
}

}
