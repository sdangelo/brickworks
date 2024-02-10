#include "impl.h"

#include "common.h"
#include <bw_ap2.h>

using namespace Brickworks;

extern "C" {

impl impl_new(void) {
	AP2<1> *instance = new AP2<1>();
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	AP2<1> *instance = reinterpret_cast<AP2<1> *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	AP2<1> *instance = reinterpret_cast<AP2<1> *>(handle);
	instance->setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	AP2<1> *instance = reinterpret_cast<AP2<1> *>(handle);
	instance->reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	AP2<1> *instance = reinterpret_cast<AP2<1> *>(handle);
	switch (index) {
	case 0:
		instance->setCutoff(value);
		break;
	case 1:
		instance->setQ(value);
		break;
	}
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	AP2<1> *instance = reinterpret_cast<AP2<1> *>(handle);
	instance->process(inputs, outputs, n_samples);
}

}
