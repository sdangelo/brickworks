#include "impl.h"

#include "common.h"
#include <bw_sr_reduce.h>
#include <bw_bd_reduce.h>

using namespace Brickworks;

class Engine {
public:
	SRReduce<1>	sr;
	BDReduce<1>	bd;
};

extern "C" {

impl impl_new(void) {
	Engine *instance = new Engine();
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->sr.setSampleRate(sample_rate);
	instance->bd.setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->sr.reset();
	instance->bd.reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (index) {
	case 0:
		instance->sr.setRatio(0.01f * value);
		break;
	case 1:
		instance->bd.setBitDepth((char)value);
		break;
	}
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->sr.process(inputs, outputs, n_samples);
	instance->bd.process(inputs, outputs, n_samples);
}

}
