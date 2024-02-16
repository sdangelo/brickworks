#include "impl.h"

#include "common.h"
#include <bw_chorus.h>

using namespace Brickworks;

class Engine {
public:
	Engine() : chorus(0.04f) {} // = 2 semitones * 2 @ 1hz, rounded up

	Chorus<1>	chorus;
	float		rateK;
	float		amountK;
};

extern "C" {

impl impl_new(void) {
	Engine *instance = new Engine();
	instance->chorus.setCoeffX(0.f);
	instance->chorus.setCoeffMod(1.f);
	// for the first plugin_set_parameter()
	instance->rateK = 1.f / 6.283185307179586f;
	instance->amountK = 0.f;
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->chorus.setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->chorus.reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (index) {
	case 0:
		instance->chorus.setRate(value);
		instance->rateK = (1.f / 6.283185307179586f) * bw_rcpf(value);
		break;
	case 1:
		instance->amountK = bw_pow2f((1.f / 12.f) * value) - 1.f;
		break;
	}
	float v = instance->rateK * instance->amountK;
	instance->chorus.setDelay(v);
	instance->chorus.setAmount(v);
}

float impl_get_parameter(impl handle, size_t index) {
	(void)handle;
	(void)index;
	return 0.f;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->chorus.process(inputs, outputs, n_samples);
}

}
