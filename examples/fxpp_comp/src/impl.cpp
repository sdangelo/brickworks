#include "impl.h"

#include "common.h"
#include <bw_comp.h>

using namespace Brickworks;

class Engine {
public:
	Comp<1>	comp;
	bool	extSidechain;
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
	instance->comp.setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->comp.reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (index) {
	case 0:
		instance->comp.setThreshDBFS(value);
		break;
	case 1:
		instance->comp.setRatio(bw_rcpf(value));
		break;
	case 2:
		// using rise time 10% -> 90%: tau = rise time / log(9)
		instance->comp.setAttackTau((0.001f * 0.4551196133134186f) * value);
		break;
	case 3:
		// as before
		instance->comp.setReleaseTau((0.001f * 0.4551196133134186f) * value);
		break;
	case 4:
		instance->comp.setGainDB(value);
		break;
	case 5:
		instance->extSidechain = value >= 0.5f;
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
#ifdef WASM
	const float *x[1] = {inputs[0]};
	const float *xSc[1] = {instance->extSidechain ? inputs[1] : inputs[0]};
	float *y[1] = {outputs[0]};
	instance->comp.process(x, xSc, y, n_samples);
#else
	instance->comp.process({inputs[0]}, {instance->extSidechain ? inputs[1] : inputs[0]}, {outputs[0]}, n_samples);
#endif
}

}
