#include "impl.h"

#include "common.h"
#include <bw_balance.h>
#include <bw_ppm.h>

using namespace Brickworks;

class Engine {
public:
	Balance<1>	balance;
	PPM<2>		ppm;
};

extern "C" {

impl impl_new(void) {
	return reinterpret_cast<impl>(new Engine());
}

void impl_free(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->balance.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->balance.reset();
	instance->ppm.reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	(void)index;

	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->balance.setBalance(0.01f * value);
}

float impl_get_parameter(impl handle, size_t index) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	float v = instance->ppm.getYZ1(index - 1);
	return v < -60.f ? -60.f : (v > 0.f ? 0.f : v);
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
#ifdef WASM
	const float *xL[1] = {inputs[0]};
	const float *xR[1] = {inputs[1]};
	float *yL[1] = {outputs[0]};
	float *yR[1] = {outputs[1]};
	instance->balance.process(xL, xR, yL, yR, n_samples);
	instance->ppm.process(outputs, nullptr, n_samples);
#else
	instance->balance.process({inputs[0]}, {inputs[1]}, {outputs[0]}, {outputs[1]}, n_samples);
	instance->ppm.process({outputs[0], outputs[1]}, {nullptr, nullptr}, n_samples);
#endif
}

}