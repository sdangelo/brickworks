#include "impl.h"

#include "common.h"
#include <bw_lp1.h>

using namespace Brickworks;

extern "C" {

impl impl_new(void) {
	LP1<1> *instance = new LP1<1>();
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	LP1<1> *instance = reinterpret_cast<LP1<1> *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	LP1<1> *instance = reinterpret_cast<LP1<1> *>(handle);
	instance->setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	LP1<1> *instance = reinterpret_cast<LP1<1> *>(handle);
	instance->reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	(void)index;
	LP1<1> *instance = reinterpret_cast<LP1<1> *>(handle);
	instance->setCutoff(value);
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	LP1<1> *instance = reinterpret_cast<LP1<1> *>(handle);
	instance->process(inputs, outputs, n_samples);
}

}
