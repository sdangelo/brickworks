#include "impl.h"

#include "common.h"
#include <bw_ls2.h>
#include <bw_hs2.h>
#include <bw_peak.h>

using namespace Brickworks;

class Engine {
public:
	LS2<1>	ls2;
	Peak<1>	peak;
	HS2<1>	hs2;
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
	instance->ls2.setSampleRate(sample_rate);
	instance->peak.setSampleRate(sample_rate);
	instance->hs2.setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->ls2.reset();
	instance->peak.reset();
	instance->hs2.reset();
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (index) {
	case 0:
		instance->ls2.setCutoff(value);
		break;
	case 1:
		instance->ls2.setDcGainDB(value);
		break;
	case 2:
		instance->ls2.setQ(value);
		break;
	case 3:
		instance->peak.setCutoff(value);
		break;
	case 4:
		instance->peak.setPeakGainDB(value);
		break;
	case 5:
		instance->peak.setBandwidth(value);
		break;
	case 6:
		instance->hs2.setCutoff(value);
		break;
	case 7:
		instance->hs2.setHighGainDB(value);
		break;
	case 8:
		instance->hs2.setQ(value);
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
	instance->ls2.process(inputs, outputs, n_samples);
	instance->peak.process(outputs, outputs, n_samples);
	instance->hs2.process(outputs, outputs, n_samples);
}

}
