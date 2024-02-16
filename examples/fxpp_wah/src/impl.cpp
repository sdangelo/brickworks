#include "impl.h"

#include "common.h"
#include <bw_wah.h>

using namespace Brickworks;

extern "C" {

impl impl_new(void) {
	Wah<1> *instance = new Wah<1>();
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	Wah<1> *instance = reinterpret_cast<Wah<1> *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Wah<1> *instance = reinterpret_cast<Wah<1> *>(handle);
	instance->setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Wah<1> *instance = reinterpret_cast<Wah<1> *>(handle);
	instance->reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	(void)index;
	Wah<1> *instance = reinterpret_cast<Wah<1> *>(handle);
	instance->setWah(0.01f * value);
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	Wah<1> *instance = reinterpret_cast<Wah<1> *>(handle);
	instance->process(inputs, outputs, n_samples);
}

}
