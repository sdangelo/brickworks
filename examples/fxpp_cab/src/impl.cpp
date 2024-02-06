#include "impl.h"

#include "common.h"
#include <bw_cab.h>

using namespace Brickworks;

extern "C" {

impl impl_new(void) {
	return reinterpret_cast<impl>(new Cab<1>());
}

void impl_free(impl handle) {
	Cab<1> *instance = reinterpret_cast<Cab<1> *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Cab<1> *instance = reinterpret_cast<Cab<1> *>(handle);
	instance->setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Cab<1> *instance = reinterpret_cast<Cab<1> *>(handle);
	instance->reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Cab<1> *instance = reinterpret_cast<Cab<1> *>(handle);
	switch (index) {
	case 0:
		instance->setCutoffLow(0.01f * value);
		break;
	case 1:
		instance->setCutoffHigh(0.01f * value);
		break;
	case 2:
		instance->setTone(0.01f * value);
		break;
	}
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	Cab<1> *instance = reinterpret_cast<Cab<1> *>(handle);
	instance->process(inputs, outputs, n_samples);
}

}
