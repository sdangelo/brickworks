#include "impl.h"

#include "common.h"
#include <bw_mm1.h>

using namespace Brickworks;

extern "C" {

impl impl_new(void) {
	MM1<1> *instance = new MM1<1>();
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	MM1<1> *instance = reinterpret_cast<MM1<1> *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	MM1<1> *instance = reinterpret_cast<MM1<1> *>(handle);
	instance->setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	MM1<1> *instance = reinterpret_cast<MM1<1> *>(handle);
	instance->reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	MM1<1> *instance = reinterpret_cast<MM1<1> *>(handle);
	switch (index) {
	case 0:
		instance->setCutoff(value);
		break;
	case 1:
		instance->setCoeffX(value);
		break;
	case 2:
		instance->setCoeffLp(value);
		break;
	}
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	MM1<1> *instance = reinterpret_cast<MM1<1> *>(handle);
	instance->process(inputs, outputs, n_samples);
}

}
