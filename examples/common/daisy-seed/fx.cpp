#include "daisy_seed.h"
#include "config.h"

using namespace daisy;

DaisySeed hardware;

P_TYPE instance;

static void AudioCallback(
		AudioHandle::InterleavingInputBuffer in,
		AudioHandle::InterleavingOutputBuffer out,
		size_t size) {
	// set params
	// update coeffs ctrl
	for (size_t i = 0; i < size; i++)
		// update coeffs audio
		out[i] = in[i];
}

int main() {
	hardware.Configure();
	hardware.Init();

	// ...
	AdcChannelConfig adcConfig;
	adcConfig.InitSingle(hardware.GetPin(21));

	hardware.adc.Init(&adcConfig, 1);
	hardware.adc.Start();
	// ...

	hardware.SetAudioBlockSize(32);
	float sample_rate = hardware.AudioSampleRate();

	// init
	// set sample rate
	
	hardware.StartLog();

	// set params
	// reset coeffs
	// reset state
	
	hardware.StartAudio(AudioCallback);
	
	while (1) {
		//hardware.adc.GetFloat(0);
		//hardware.PrintLine("%f",x);
		//System::Delay(x);
	}
}
