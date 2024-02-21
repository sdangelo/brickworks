#include "impl.h"

#include "common.h"
#include <bw_note_queue.h>
#include <bw_phase_gen.h>
#include <bw_osc_saw.h>
#include <bw_osc_pulse.h>
#include <bw_osc_tri.h>
#include <bw_osc_sin.h>
#include <bw_osc_filt.h>
#include <bw_noise_gen.h>
#include <bw_pink_filt.h>
#include <bw_svf.h>
#include <bw_env_gen.h>
#include <bw_gain.h>
#include <bw_ppm.h>
#include <bw_buf.h>
#include <bw_voice_alloc.h>

#define BUFFER_SIZE	128
#define SYNC_RATE	1e-3f	// synchronous control rate, seconds
#define N_VOICES	8

using namespace Brickworks;

class Engine;

class Voice {
public:
	PhaseGen<1>		vco1PhaseGen;
	PhaseGen<1>		vco2PhaseGen;
	PhaseGen<1>		vco3PhaseGen;
	SVF<1>			vcf;

	unsigned char		note;
	char			gate;
	float			modK;
	float			vcfEnvK;
	
	float			buf[5][BUFFER_SIZE];

	Engine *		instance;
	int			index;
};

class Engine {
public:
	Engine() : noiseGen(&randState) {}
	
	NoteQueue		noteQueue;
	OscSaw<N_VOICES>	vco1OscSaw;
	OscPulse<N_VOICES>	vco1OscPulse;
	OscTri<N_VOICES>	vco1OscTri;
	Gain<N_VOICES>		vco1Gain;
	OscSaw<N_VOICES>	vco2OscSaw;
	OscPulse<N_VOICES>	vco2OscPulse;
	OscTri<N_VOICES>	vco2OscTri;
	Gain<N_VOICES>		vco2Gain;
	OscSaw<N_VOICES>	vco3OscSaw;
	OscPulse<N_VOICES>	vco3OscPulse;
	OscTri<N_VOICES>	vco3OscTri;
	Gain<N_VOICES>		vco3Gain;
	OscFilt<N_VOICES>	oscFilt;
	NoiseGen<N_VOICES>	noiseGen;
	PinkFilt<N_VOICES>	pinkFilt;
	Gain<N_VOICES>		noiseGain;
	EnvGen<N_VOICES>	vcfEnvGen;
	EnvGen<N_VOICES>	vcaEnvGen;
	PhaseGen<1>		a440PhaseGen;
	Gain<1>			gain;
	PPM<1>			ppm;

	Voice			voices[N_VOICES];

	size_t			syncCount;

	uint64_t		randState;
	float			masterTune;
	float			modulationMix;
	float			vco1Modulation;
	float			vco1Coarse;
	float			vco1Fine;
	char			vco1Waveform;
	float			vco2Modulation;
	float			vco2Coarse;
	float			vco2Fine;
	char			vco2Waveform;
	char			vco3KbdCtrl;
	float			vco3Coarse;
	float			vco3Fine;
	char			vco3Waveform;
	char			noiseColor;
	float			vcfModulation;
	char			vcfKbdCtrl;
	float			vcfCutoff;
	float			vcfContour;
	char			a440;

	float			pitchBend;
	float			modWheel;
	size_t			syncLeft;
	char			vco3WaveformCur;
	char			noiseColorCur;
	char			vco1WaveformCur;
	char			vco2WaveformCur;

	float			buf[BUFFER_SIZE];
};

extern "C" {

impl impl_new(void) {
	Engine *instance = new Engine();

	instance->vco1OscSaw.setAntialiasing(true);
	instance->vco1OscPulse.setAntialiasing(true);
	instance->vco1OscTri.setAntialiasing(true);
	instance->vco2OscSaw.setAntialiasing(true);
	instance->vco2OscPulse.setAntialiasing(true);
	instance->vco2OscTri.setAntialiasing(true);
	instance->vco3OscSaw.setAntialiasing(true);
	instance->vco3OscPulse.setAntialiasing(true);
	instance->vco3OscTri.setAntialiasing(true);
	instance->a440PhaseGen.setFrequency(440.f);

	instance->randState = 0xbaddecaf600dfeed;

	for (int i = 0; i < N_VOICES; i++) {
		instance->voices[i].instance = instance;
		instance->voices[i].index = i;
	}

	return reinterpret_cast<impl>(instance);
}

void impl_free(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	delete instance;
}

void impl_set_sample_rate(impl handle, float sample_rate) {
	Engine *instance = reinterpret_cast<Engine *>(handle);

	instance->vco1OscSaw.setSampleRate(sample_rate);
	instance->vco1OscPulse.setSampleRate(sample_rate);
	instance->vco1OscTri.setSampleRate(sample_rate);
	instance->vco1Gain.setSampleRate(sample_rate);
	instance->vco2OscSaw.setSampleRate(sample_rate);
	instance->vco2OscPulse.setSampleRate(sample_rate);
	instance->vco2OscTri.setSampleRate(sample_rate);
	instance->vco2Gain.setSampleRate(sample_rate);
	instance->vco3OscSaw.setSampleRate(sample_rate);
	instance->vco3OscPulse.setSampleRate(sample_rate);
	instance->vco3OscTri.setSampleRate(sample_rate);
	instance->vco3Gain.setSampleRate(sample_rate);
	instance->noiseGen.setSampleRate(sample_rate);
	instance->pinkFilt.setSampleRate(sample_rate);
	instance->noiseGain.setSampleRate(sample_rate);
	instance->vcfEnvGen.setSampleRate(sample_rate);
	instance->vcaEnvGen.setSampleRate(sample_rate);
	instance->a440PhaseGen.setSampleRate(sample_rate);
	instance->gain.setSampleRate(sample_rate);
	instance->ppm.setSampleRate(sample_rate);

	for (int i = 0; i < N_VOICES; i++) {
		instance->voices[i].vco1PhaseGen.setSampleRate(sample_rate);
		instance->voices[i].vco2PhaseGen.setSampleRate(sample_rate);
		instance->voices[i].vco3PhaseGen.setSampleRate(sample_rate);
		instance->voices[i].vcf.setSampleRate(sample_rate);
	}

	instance->syncCount = (size_t)bw_roundf(sample_rate * SYNC_RATE);
}

void impl_reset(impl handle) {
	Engine *instance = reinterpret_cast<Engine *>(handle);

	for (int i = 0; i < N_VOICES; i++)
		instance->voices[i].vcf.setCutoff(instance->vcfCutoff);

	instance->noteQueue = NoteQueue();
	instance->vco1OscSaw.reset();
	instance->vco1OscPulse.reset();
	instance->vco1OscTri.reset();
	instance->vco1Gain.reset();
	instance->vco2OscSaw.reset();
	instance->vco2OscPulse.reset();
	instance->vco2OscTri.reset();
	instance->vco2Gain.reset();
	instance->vco3OscSaw.reset();
	instance->vco3OscPulse.reset();
	instance->vco3OscTri.reset();
	instance->vco3Gain.reset();
	instance->oscFilt.reset();
	instance->noiseGen.reset();
	instance->pinkFilt.reset();
	instance->noiseGain.reset();
	instance->vcfEnvGen.reset();
	instance->vcaEnvGen.reset();
	instance->a440PhaseGen.reset();
	instance->gain.reset();
	instance->ppm.reset();
	for (int i = 0; i < N_VOICES; i++) {
		instance->voices[i].vco1PhaseGen.reset();
		instance->voices[i].vco2PhaseGen.reset();
		instance->voices[i].vco3PhaseGen.reset();
		instance->voices[i].vcf.reset();

		instance->voices[i].note = 60;
		instance->voices[i].gate = 0;
	}
	instance->pitchBend = 0.f;
	instance->modWheel = 0.f;
	instance->syncLeft = instance->syncCount;
	instance->vco3WaveformCur = instance->vco3Waveform;
	instance->noiseColorCur = instance->noiseColor;
	instance->vco1WaveformCur = instance->vco1Waveform;
	instance->vco2WaveformCur = instance->vco2Waveform;
}

void impl_set_parameter(impl handle, size_t index, float value) {
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (index) {
	case 0:
	{
		const float v = 0.01f * value;
		instance->gain.setGainLin(v * v * v);
	}
		break;
	case 1:
		instance->masterTune = value;
		break;
	case 2:
	{
		// using portamento time 0% -> 90%: tau = portamento time / log(10)
		const float v = (0.001f * 0.4342944819032517f) * value;
		for (int i = 0; i < N_VOICES; i++) {
			instance->voices[i].vco1PhaseGen.setPortamentoTau(v);
			instance->voices[i].vco2PhaseGen.setPortamentoTau(v);
			instance->voices[i].vco3PhaseGen.setPortamentoTau(v);
		}
		break;
	}
	case 3:
		instance->modulationMix = 0.01f * value;
		break;
	case 4:
		instance->vco1Modulation = 0.01f * value;
		break;
	case 5:
		instance->vco1Coarse = value;
		break;
	case 6:
		instance->vco1Fine = value;
		break;
	case 7:
		instance->vco1Waveform = value;
		break;
	case 8:
	{
		const float v = 0.01f * value;
		instance->vco1OscPulse.setPulseWidth(v);
		instance->vco1OscTri.setSlope(bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case 9:
	{
		const float v = 0.01f * value;
		instance->vco1Gain.setGainLin(v * v * v);
		break;
	}
	case 10:
		instance->vco2Modulation = 0.01f * value;
		break;
	case 11:
		instance->vco2Coarse = value;
		break;
	case 12:
		instance->vco2Fine = value;
		break;
	case 13:
		instance->vco2Waveform = value;
		break;
	case 14:
	{
		const float v = 0.01f * value;
		instance->vco2OscPulse.setPulseWidth(v);
		instance->vco2OscTri.setSlope(bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case 15:
	{
		const float v = 0.01f * value;
		instance->vco2Gain.setGainLin(v * v * v);
		break;
	}
	case 16:
		instance->vco3KbdCtrl = value >= 0.5f;
		break;
	case 17:
		instance->vco3Coarse = value;
		break;
	case 18:
		instance->vco3Fine = value;
		break;
	case 19:
		instance->vco3Waveform = value;
		break;
	case 20:
	{
		const float v = 0.01f * value;
		instance->vco3OscPulse.setPulseWidth(v);
		instance->vco3OscTri.setSlope(bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case 21:
	{
		const float v = 0.01f * value;
		instance->vco3Gain.setGainLin(v * v * v);
		break;
	}
	case 22:
		instance->noiseColor = value;
		break;
	case 23:
	{
		const float v = 0.01f * value;
		instance->noiseGain.setGainLin(v * v * v);
		break;
	}
	case 24:
		instance->vcfModulation = 0.01f * value;
		break;
	case 25:
		instance->vcfKbdCtrl = value;
		break;
	case 26:
		instance->vcfCutoff = value;
		break;
	case 27:
	{
		const float v = 0.5f + (0.01f * 9.5f) * value;
		for (int i = 0; i < N_VOICES; i++)
			instance->voices[i].vcf.setQ(v);
		break;
	}
	case 28:
		instance->vcfContour = 0.01f * value;
		break;
	case 29:
		instance->vcfEnvGen.setAttack(0.001f * value);
		break;
	case 30:
		instance->vcfEnvGen.setDecay(0.001f * value);
		break;
	case 31:
		instance->vcfEnvGen.setSustain(0.01f * value);
		break;
	case 32:
		instance->vcfEnvGen.setRelease(0.001f * value);
		break;
	case 33:
		instance->vcaEnvGen.setAttack(0.001f * value);
		break;
	case 34:
		instance->vcaEnvGen.setDecay(0.001f * value);
		break;
	case 35:
		instance->vcaEnvGen.setSustain(0.01f * value);
		break;
	case 36:
		instance->vcaEnvGen.setRelease(0.001f * value);
		break;
	case 37:
		instance->a440 = value >= 0.5f;
		break;
	}
}

float impl_get_parameter(impl handle, size_t index) {
	(void)index;
	Engine *instance = reinterpret_cast<Engine *>(handle);
	return bw_clipf(instance->ppm.getYZ1(0), -60.f, 0.f);
}

static void noteOn(void *BW_RESTRICT handle, unsigned char note, float velocity) {
	(void)velocity;
	Voice *v = (Voice *)handle;
	v->note = note;
	v->gate = 1;
}

static void noteOff(void *BW_RESTRICT handle, float velocity) {
	(void)velocity;
	Voice *v = (Voice *)handle;
	v->gate = 0;
}

static unsigned char getNote(const void *BW_RESTRICT handle) {
	Voice *v = (Voice *)handle;
	return v->note;
}

static char isFree(const void *BW_RESTRICT handle) {
	Voice *v = (Voice *)handle;
	bw_env_gen_phase phase = v->instance->vcaEnvGen.getPhase(v->index);
	return !v->gate && phase == bw_env_gen_phase_off;
}

void impl_process(impl handle, const float **inputs, float **outputs, size_t n_samples) {
	// here is a WASM-compatible version only as it'd be too cumbersome to maintain two versions

	(void)inputs;

	Engine *instance = reinterpret_cast<Engine *>(handle);

	// voice allocation
	
	static bw_voice_alloc_opts alloc_opts = { bw_voice_alloc_priority_low, noteOn, noteOff, getNote, isFree };
	void *voices[N_VOICES];
	for (int i = 0; i < N_VOICES; i++)
		voices[i] = (void *)(instance->voices + i);
	bw_voice_alloc(&alloc_opts, &instance->noteQueue.queue, voices, N_VOICES);
	instance->noteQueue.clear();

	// asynchronous control-rate operations

	const float df1 = instance->vco1Coarse + instance->pitchBend + (8.333333333333333e-2f * 0.01f) * instance->vco1Fine;
	const float df2 = instance->vco2Coarse + instance->pitchBend + (8.333333333333333e-2f * 0.01f) * instance->vco2Fine;
	const float df3 = instance->vco3Coarse + instance->pitchBend + (8.333333333333333e-2f * 0.01f) * instance->vco3Fine;
	for (int i = 0; i < N_VOICES; i++) {
		int n = instance->voices[i].note - 69;
		int n3 = instance->vco3KbdCtrl ? instance->voices[i].note - 69 : -69;
		instance->voices[i].vco1PhaseGen.setFrequency(instance->masterTune * bw_pow2f(df1 + 8.333333333333333e-2f * n));
		instance->voices[i].vco2PhaseGen.setFrequency(instance->masterTune * bw_pow2f(df2 + 8.333333333333333e-2f * n));
		instance->voices[i].vco3PhaseGen.setFrequency(instance->masterTune * bw_pow2f(df3 + 8.333333333333333e-2f * n3));
	}

	if (instance->vco3WaveformCur != instance->vco3Waveform) {
		switch (instance->vco3Waveform) {
		case 2:
			instance->vco3OscPulse.reset();
			break;
		case 3:
			instance->vco3OscTri.reset();
			break;
		}
		instance->vco3WaveformCur = instance->vco3Waveform;
	}

	if (instance->noiseColorCur != instance->noiseColor) {
		if (instance->noiseColor == 2)
			instance->pinkFilt.reset();
		instance->noiseColorCur = instance->noiseColor;
	}

	if (instance->vco1WaveformCur != instance->vco1Waveform) {
		switch (instance->vco1Waveform) {
		case 2:
			instance->vco1OscPulse.reset();
			break;
		case 3:
			instance->vco1OscTri.reset();
			break;
		}
		instance->vco1WaveformCur = instance->vco1Waveform;
	}

	if (instance->vco2WaveformCur != instance->vco2Waveform) {
		switch (instance->vco2Waveform) {
		case 2:
			instance->vco2OscPulse.reset();
			break;
		case 3:
			instance->vco2OscTri.reset();
			break;
		}
		instance->vco2WaveformCur = instance->vco2Waveform;
	}

	// synchronous control-rate and audio-rate operations

	float *b0[N_VOICES], *b1[N_VOICES], *b2[N_VOICES], *b3[N_VOICES], *b4[N_VOICES];
	char g[N_VOICES];
	for (int j = 0; j < N_VOICES; j++) {
		b0[j] = instance->voices[j].buf[0];
		b1[j] = instance->voices[j].buf[1];
		b2[j] = instance->voices[j].buf[2];
		b3[j] = instance->voices[j].buf[3];
		b4[j] = instance->voices[j].buf[4];
		g[j] = instance->voices[j].gate;
	}

	for (size_t i = 0; i < n_samples; ) {
		float *out = outputs[0] + i;
		int n = bw_minf(bw_minf(n_samples - i, BUFFER_SIZE), instance->syncLeft);

		const char sync = instance->syncLeft == instance->syncCount;
		float *y[1] = {out};

		// vco 3
		
		for (int j = 0; j < N_VOICES; j++) {
			float *vb0[1] = {instance->voices[j].buf[0]};
			float *vb1[1] = {instance->voices[j].buf[1]};
			instance->voices[j].vco3PhaseGen.process(nullptr, vb0, vb1, n);
		}
		switch (instance->vco3WaveformCur) {
		case 1:
			instance->vco3OscSaw.process(b0, b1, b0, n);
			break;
		case 2:
			instance->vco3OscPulse.process(b0, b1, b0, n);
			break;
		default:
			instance->vco3OscTri.process(b0, b1, b0, n);
			break;
		}

		// noise generator

		instance->noiseGen.process(b1, n);
		if (instance->noiseColorCur == 2)
			instance->pinkFilt.process(b1, b1, n);
		bufScale<N_VOICES>(b1, 5.f, b1, n);

		// modulation signals

		for (int j = 0; j < N_VOICES; j++) {
			for (int k = 0; k < n; k++)
				b2[j][k] = instance->modWheel * (b0[j][k] + instance->modulationMix * (b1[j][k] - b0[j][k]));
		}
		if (sync)
			for (int j = 0; j < N_VOICES; j++)
				instance->voices[j].modK = b2[j][0];

		// vco 1

		for (int j = 0; j < N_VOICES; j++) {
			float *vb2[1] = {instance->voices[j].buf[2]};
			float *vb3[1] = {instance->voices[j].buf[3]};
			float *vb4[1] = {instance->voices[j].buf[4]};
			bufScale<1>(vb2, instance->vco1Modulation, vb3, n);
			instance->voices[j].vco1PhaseGen.process(vb3, vb3, vb4, n);
		}
		switch (instance->vco1WaveformCur) {
		case 1:
			instance->vco1OscSaw.process(b3, b4, b3, n);
			break;
		case 2:
			instance->vco1OscPulse.process(b3, b4, b3, n);
			break;
		default:
			instance->vco1OscTri.process(b3, b4, b3, n);
			break;
		}

		// vco 2

		for (int j = 0; j < N_VOICES; j++) {
			float *vb2[1] = {instance->voices[j].buf[2]};
			float *vb4[1] = {instance->voices[j].buf[4]};
			bufScale<1>(vb2, instance->vco2Modulation, vb2, n);
			instance->voices[j].vco2PhaseGen.process(vb2, vb2, vb4, n);
		}
		switch (instance->vco2WaveformCur) {
		case 1:
			instance->vco2OscSaw.process(b2, b4, b2, n);
			break;
		case 2:
			instance->vco2OscPulse.process(b2, b4, b2, n);
			break;
		default:
			instance->vco2OscTri.process(b2, b4, b2, n);
			break;
		}

		// mixer

		instance->vco1Gain.process(b3, b3, n);
		instance->vco2Gain.process(b2, b2, n);
		instance->vco3Gain.process(b0, b0, n);
		instance->noiseGain.process(b1, b1, n);
		bufMix<N_VOICES>(b0, b2, b0, n);
		bufMix<N_VOICES>(b0, b3, b0, n);

		instance->oscFilt.process(b0, b0, n);

		const float k = instance->noiseColorCur == 2
			? 6.f * instance->noiseGen.getScalingK() * instance->pinkFilt.getScalingK()
			: 0.1f * instance->noiseGen.getScalingK();
		bufScale<N_VOICES>(b1, k, b1, n);
		bufMix<N_VOICES>(b0, b1, b0, n);

		// vcf

		instance->vcfEnvGen.process(g, nullptr, n);
		if (sync)
			for (int j = 0; j < N_VOICES; j++)
				instance->voices[j].vcfEnvK = instance->vcfEnvGen.getYZ1(j);
		const float cutoffUnmapped = 0.1447648273010839f * bw_logf(0.05f * instance->vcfCutoff);
		for (int j = 0; j < N_VOICES; j++) {
			const float cutoffVpos =
				cutoffUnmapped
				+ instance->vcfContour * instance->voices[j].vcfEnvK
				+ 0.3f * instance->vcfModulation * instance->voices[j].modK;
			float cutoff = 20.f * bw_expf(6.907755278982137 * cutoffVpos);
			switch (instance->vcfKbdCtrl) {
			case 2: // 1/3
				cutoff *= bw_pow2f((0.629960524947437f * 8.333333333333333e-2f) * (instance->voices[j].note - 60));
				break;
			case 3: // 2/3
				cutoff *= bw_pow2f((0.793700525984100f * 8.333333333333333e-2f) * (instance->voices[j].note - 60));
				break;
			case 4: // full
				cutoff *= bw_pow2f(8.333333333333333e-2f * (instance->voices[j].note - 60));
				break;
			default: // off, do nothing
				break;
			}
			instance->voices[j].vcf.setCutoff(bw_clipf(cutoff, 20.f, 20e3f));
			float *vb0[1] = {instance->voices[j].buf[0]};
			instance->voices[j].vcf.process(vb0, vb0, nullptr, nullptr, n);
		}

		// vca

		instance->vcaEnvGen.process(g, b1, n);
		bufMul<N_VOICES>(b0, b1, b0, n);

		// mix voices

		bufFill<1>(0.f, y, n);
		for (int j = 0; j < N_VOICES; j++) {
			float *vb0[1] = {instance->voices[j].buf[0]};
			bufMix<1>(y, vb0, y, n);
		}

		// A 440 Hz osc

		if (instance->a440) { // nobody will notice or care about phase issues here
			float *b[1] = {instance->buf};
			instance->a440PhaseGen.process(nullptr, b, nullptr, n);
			oscSinProcess<1>(b, b, n);
			bufMix<1>(y, b, y, n);
		}

		// output

		instance->gain.process(y, y, n);
		instance->ppm.process(y, nullptr, n);

		instance->syncLeft -= n;
		if (instance->syncLeft == 0)
			instance->syncLeft = instance->syncCount;

		i += n;
	}
}

void impl_midi_msg_in(impl handle, size_t index, const uint8_t * data) {
	(void)index;
	Engine *instance = reinterpret_cast<Engine *>(handle);
	switch (data[0] & 0xf0) {
	case 0x90: // note on
		instance->noteQueue.add(data[1], data[2] != 0, (1.f / 127.f) * data[2], false);
		break;
	case 0x80: // note off
		instance->noteQueue.add(data[1], false, 0, false);
		break;
	case 0xe0: // pitch bend
	{
		const uint16_t v = (data[2] << 7) | data[1];
		instance->pitchBend = 2.f * bw_maxf((1.f / 16383.f) * (v - 0x2000), -1.f) - 1.f;
		break;
	}
	case 0xb0: // control change
		if (data[1] == 1) // mod wheel
			instance->modWheel = (1.f / 127.f) * data[2];
		break;
	}
}

}
