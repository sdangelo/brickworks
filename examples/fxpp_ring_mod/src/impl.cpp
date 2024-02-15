#include "impl.h"

#include "common.h"
#include <bw_ring_mod.h>
#include <bw_phase_gen.h>
#include <bw_osc_sin.h>

using namespace Brickworks;

class Engine {
public:
	PhaseGen<1>	phase_gen;
	RingMod<1>	ring_mod;
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
	instance->phase_gen.setSampleRate(sample_rate);
	instance->ring_mod.setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->phase_gen.reset();
	instance->ring_mod.reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (index) {
	case 0:
		instance->phase_gen.setFrequency(value);
		break;
	case 1:
		instance->ring_mod.setAmount(0.01f * value);
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
	instance->phase_gen.process(nullptr, outputs, nullptr, n_samples);
	oscSinProcess<1>(outputs, outputs, n_samples);
	instance->ring_mod.process(inputs, outputs, outputs, n_samples);
}

}
