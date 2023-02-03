#include "daisy_seed.h"
#include "config.h"
#include "config_daisy_seed.h"

#define BLOCK_SIZE 32

using namespace daisy;

DaisySeed hardware;
CpuLoadMeter loadMeter;

P_TYPE instance;

float buf[2][BLOCK_SIZE];
float *bufs[2] = { buf[0], buf[1] };

static void setParams() {
	for (int i = 0; i < NUM_PINS; i++)
		P_SET_PARAMETER(&instance, config_pins[i].param_index, hardware.adc.GetFloat(i));
}

static void AudioCallback(
		AudioHandle::InterleavingInputBuffer in,
		AudioHandle::InterleavingOutputBuffer out,
		size_t size) {
	loadMeter.OnBlockStart();
	setParams();
	const size_t n = size >> 1;
#if NUM_CHANNELS_IN != 0
	for (size_t i = 0; i < n; i++) {
		const size_t j = i << 1;
		buf[0][i] = in[j];
# if NUM_CHANNELS_IN > 1
		buf[1][i] = in[j + 1];
# endif
	}
#endif
	P_PROCESS(&instance, (const float **)bufs, bufs, n);
	for (size_t i = 0; i < n; i++) {
		const size_t j = i << 1;
#if NUM_CHANNELS_OUT > 0
		out[j] = buf[0][i];
#else
		out[j] = 0.f;
#endif
#if NUM_CHANNELS_OUT > 1
		out[j + 1] = buf[1][i];
#else
		out[j + 1] = 0.f;
#endif
	}
	loadMeter.OnBlockEnd();
}

int main() {
	hardware.Configure();
	hardware.Init();

	AdcChannelConfig adcConfig[NUM_PINS];
	for (int i = 0; i < NUM_PINS; i++)
		adcConfig[i].InitSingle(hardware.GetPin(config_pins[i].pin));

	hardware.adc.Init(adcConfig, NUM_PINS);
	hardware.adc.Start();

	hardware.SetAudioBlockSize(BLOCK_SIZE);
	float sample_rate = hardware.AudioSampleRate();

	P_INIT(&instance);
	P_SET_SAMPLE_RATE(&instance, sample_rate);
	
	hardware.StartLog();

	loadMeter.Init(sample_rate, BLOCK_SIZE);

	setParams();
	P_RESET(&instance);
	
	hardware.StartAudio(AudioCallback);
	
	while (1) {
		const float avgLoad = loadMeter.GetAvgCpuLoad();
		const float maxLoad = loadMeter.GetMaxCpuLoad();
		const float minLoad = loadMeter.GetMinCpuLoad();
		hardware.PrintLine("---");
		for (int i = 0; i < NUM_PARAMETERS; i++)
			if (config_parameters[i].out)
				hardware.PrintLine("%s: %f", config_parameters[i].name, P_GET_PARAMETER(&instance, i));
		hardware.PrintLine("---");
		hardware.PrintLine("Processing Load %:");
		hardware.PrintLine("Max: " FLT_FMT3, FLT_VAR3(maxLoad * 100.0f));
		hardware.PrintLine("Avg: " FLT_FMT3, FLT_VAR3(avgLoad * 100.0f));
		hardware.PrintLine("Min: " FLT_FMT3, FLT_VAR3(minLoad * 100.0f));
		System::Delay(500);
	}
}
