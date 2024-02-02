#include "impl.h"

#include "common.h"
#include <bw_ap1.h>

using namespace Brickworks;

extern "C" {

impl impl_new() {
	return reinterpret_cast<impl>(new AP1<1>());
}

void impl_free(impl instance) {
	AP1<1> *ap1 = reinterpret_cast<AP1<1> *>(instance);
	delete ap1;
}

void impl_set_sample_rate(impl instance, float sample_rate) {
	AP1<1> *ap1 = reinterpret_cast<AP1<1> *>(instance);
	ap1->setSampleRate(sample_rate);
}

void impl_reset(impl instance) {
	AP1<1> *ap1 = reinterpret_cast<AP1<1> *>(instance);
	ap1->reset();
}

void impl_set_parameter(impl instance, size_t index, float value) {
	(void)index;
	AP1<1> *ap1 = reinterpret_cast<AP1<1> *>(instance);
	ap1->setCutoff(value);
}

float impl_get_parameter(impl instance, size_t index) {
	(void)instance;
	(void)index;
	return 0.f;
}

void impl_process(impl instance, const float **inputs, float **outputs, size_t n_samples) {
	AP1<1> *ap1 = reinterpret_cast<AP1<1> *>(instance);
	ap1->process(inputs, outputs, n_samples);
}

}
