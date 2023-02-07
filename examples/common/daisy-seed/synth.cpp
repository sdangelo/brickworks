#include "daisy_seed.h"
#include "config.h"
#include "config_daisy_seed.h"

#define BLOCK_SIZE 32

using namespace daisy;

DaisySeed hardware;
CpuLoadMeter loadMeter;
MidiUsbHandler midi;

P_TYPE instance;

float buf[2][BLOCK_SIZE];
float *bufs[2] = { buf[0], buf[1] };

static void AudioCallback(
		AudioHandle::InterleavingInputBuffer in,
		AudioHandle::InterleavingOutputBuffer out,
		size_t size) {
	loadMeter.OnBlockStart();
	const size_t n = size >> 1;
	P_PROCESS(&instance, nullptr, bufs, n);
	for (size_t i = 0; i < n; i++) {
		const size_t j = i << 1;
		out[j] = buf[0][i];
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

	hardware.SetAudioBlockSize(BLOCK_SIZE);
	float sample_rate = hardware.AudioSampleRate();

	P_INIT(&instance);
	P_SET_SAMPLE_RATE(&instance, sample_rate);
	
//	hardware.StartLog();

	loadMeter.Init(sample_rate, BLOCK_SIZE);

	P_RESET(&instance);

	MidiUsbHandler::Config midi_cfg;
	midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
	midi.Init(midi_cfg);
	
	hardware.StartAudio(AudioCallback);
	
//	int i = 0;
	while (1) {
		midi.Listen();
		while (midi.HasEvents()) {
			MidiEvent ev = midi.PopEvent();
			switch (ev.type) {
			case NoteOn:
			{
				NoteOnEvent v = ev.AsNoteOn();
				P_NOTE_ON(&instance, v.note, v.velocity);
			}
				break;
			case NoteOff:
			{
				NoteOffEvent v = ev.AsNoteOff();
				P_NOTE_OFF(&instance, v.note);
			}
				break;
#ifdef P_PITCH_BEND
			case PitchBend:
			{
				PitchBendEvent v = ev.AsPitchBend();
				P_PITCH_BEND(&instance, v.value);
			}
#endif
				break;
			case ControlChange:
			{
				ControlChangeEvent v = ev.AsControlChange();
#ifdef P_MOD_WHEEL
				if (v.control_number == 1)
					P_MOD_WHEEL(&instance, v.value);
				else {
#endif
					for (int i = 0; i < NUM_CCS; i++)
						if (v.control_number == config_ccs[i].cc)
							P_SET_PARAMETER(&instance, config_ccs[i].param_index, v.value);
#ifdef P_MOD_WHEEL
				}
#endif
			}
				break;
			default:
				break;
			}
		}
		/*
		i++;
		if (i == 500) {
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
			i = 0;
		}
		*/
		System::Delay(1);
	}
}
