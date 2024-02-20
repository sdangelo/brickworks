#include "impl.h"

#include "common.h"
#include <bw_phase_gen.h>
#include <bw_osc_pulse.h>
#include <bw_osc_filt.h>
#include <bw_svf.h>
#include <bw_env_gen.h>
#include <bw_gain.h>
#include <bw_ppm.h>
#include <bw_buf.h>

#define BUFFER_SIZE	128

using namespace Brickworks;

class Engine {
public:
	PhaseGen<1>	phaseGen;
	OscPulse<1>	oscPulse;
	OscFilt<1>	oscFilt;
	SVF<1>		svf;
	EnvGen<1>	envGen;
	Gain<1>		gain;
	PPM<1>		ppm;

	float		masterTune;
	int		note;

	float		buf[BUFFER_SIZE];
};

extern "C" {

impl impl_new(void) {
	Engine *instance = new Engine();
	instance->oscPulse.setAntialiasing(true);
	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->phaseGen.setSampleRate(sample_rate);
	instance->oscPulse.setSampleRate(sample_rate);
	instance->svf.setSampleRate(sample_rate);
	instance->envGen.setSampleRate(sample_rate);
	instance->gain.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	instance->phaseGen.reset();
	instance->oscPulse.reset();
	instance->oscFilt.reset();
	instance->svf.reset();
	instance->envGen.reset();
	instance->gain.reset();
	instance->ppm.reset();
	instance->note = -1;
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (index) {
	case 0:
	{
		float v = 0.01f * value;
		instance->gain.setGainLin(v * v * v);
	}
		break;
	case 1:
		instance->masterTune = value;
		break;
	case 2:
		// using portamento time 0% -> 90%: tau = portamento time / log(10)
		instance->phaseGen.setPortamentoTau((0.001f * 0.4342944819032517f) * value);
		break;
	case 3:
		instance->oscPulse.setPulseWidth(0.01f * value);
		break;
	case 4:
		instance->svf.setCutoff(value);
		break;
	case 5:
		instance->svf.setQ(0.5f + (9.5f * 0.01f) * value);
		break;
	case 6:
		instance->envGen.setAttack(0.001f * value);
		break;
	case 7:
		instance->envGen.setDecay(0.001f * value);
		break;
	case 8:
		instance->envGen.setSustain(0.01f * value);
		break;
	case 9:
		instance->envGen.setRelease(0.001f * value);
		break;
	}
}

float impl_get_parameter(impl handle, size_t index) {
	(void)index;
	Engine *instance = reinterpret_cast<Engine *>(handle);
	return bw_clipf(instance->ppm.getYZ1(0), -60.f, 0.f);
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	(void)inputs;

	Engine *instance = reinterpret_cast<Engine *>(handle);

	if (instance->note >= 0)
		instance->phaseGen.setFrequency(instance->masterTune * bw_pow2f(8.333333333333333e-2f * (instance->note - 69)));

	for (size_t i = 0; i < n_samples; i += BUFFER_SIZE) {
		float *out = outputs[0] + i;
		int n = bw_minf(n_samples - i, BUFFER_SIZE);

#ifdef WASM
		float *y[1] = {out};
		float *b[1] = {instance->buf};
		char gate[1] = {instance->note >= 0};
		instance->phaseGen.process(nullptr, y, b, n);
		instance->oscPulse.process(y, b, y, n);
		instance->oscFilt.process(y, y, n);
		instance->svf.process(y, y, nullptr, nullptr, n);
		instance->envGen.process(gate, b, n);
		bufMul<1>(y, b, y, n);
		instance->gain.process(y, y, n);
		instance->ppm.process(y, nullptr, n);
#else
		instance->phaseGen.process({nullptr}, {out}, {instance->buf}, n);
		instance->oscPulse.process({out}, {instance->buf}, {out}, n);
		instance->oscFilt.process({out}, {out}, n);
		instance->svf.process({out}, {out}, {nullptr}, {nullptr}, n);
		instance->envGen.process({instance->note >= 0}, {instance->buf}, n);
		bufMul<1>({out}, {instance->buf}, {out}, n);
		instance->gain.process({out}, {out}, n);
		instance->ppm.process({out}, {nullptr}, n);
#endif
	}
}

void impl_midi_msg_in(impl handle, size_t index, const uint8_t * data) {
	(void)index;
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (data[0] & 0xf0) {
	case 0x90: // note on
		if (data[2] == 0) { // no, note off actually
			if (data[1] == instance->note)
				instance->note = -1;
		} else
			instance->note = data[1];
		break;
	case 0x80: // note off
		if (data[1] == instance->note)
			instance->note = -1;
		break;
	}
}

}
