/*
 * Brickworks
 *
 * Copyright (C) 2022, 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo, Paolo Marrone
 *
 * This file contains code from sse2neon
 * (https://github.com/DLTcollab/sse2neon/), which is released under the
 * following licensing conditions.
 *
 * sse2neon is freely redistributable under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "plugin.h"

#include "pluginterfaces/base/conststringtable.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "base/source/fstreamer.h"
#include "common.h"

#include <algorithm>

#if defined(__aarch64__)

/* Beginning of sse2neon code */

/* Denormals are zeros mode macros. */
#define _MM_DENORMALS_ZERO_MASK 0x0040
#define _MM_DENORMALS_ZERO_ON 0x0040
#define _MM_DENORMALS_ZERO_OFF 0x0000

#define _MM_GET_DENORMALS_ZERO_MODE _sse2neon_mm_get_denormals_zero_mode
#define _MM_SET_DENORMALS_ZERO_MODE _sse2neon_mm_set_denormals_zero_mode

/* Flush zero mode macros. */
#define _MM_FLUSH_ZERO_MASK 0x8000
#define _MM_FLUSH_ZERO_ON 0x8000
#define _MM_FLUSH_ZERO_OFF 0x0000

#define _MM_GET_FLUSH_ZERO_MODE _sse2neon_mm_get_flush_zero_mode
#define _MM_SET_FLUSH_ZERO_MODE _sse2neon_mm_set_flush_zero_mode

typedef struct {
    uint16_t res0;
    uint8_t res1 : 6;
    uint8_t bit22 : 1;
    uint8_t bit23 : 1;
    uint8_t bit24 : 1;
    uint8_t res2 : 7;
    uint32_t res3;
} fpcr_bitfield;

static inline unsigned int _sse2neon_mm_get_denormals_zero_mode()
{
    union {
        fpcr_bitfield field;
        uint64_t value;
    } r;

    __asm__ __volatile__("mrs %0, FPCR" : "=r"(r.value));

    return r.field.bit24 ? _MM_DENORMALS_ZERO_ON : _MM_DENORMALS_ZERO_OFF;
}

static inline void _sse2neon_mm_set_denormals_zero_mode(unsigned int flag)
{
    union {
        fpcr_bitfield field;
        uint64_t value;
    } r;

    __asm__ __volatile__("mrs %0, FPCR" : "=r"(r.value));

    r.field.bit24 = (flag & _MM_DENORMALS_ZERO_MASK) == _MM_DENORMALS_ZERO_ON;

    __asm__ __volatile__("msr FPCR, %0" ::"r"(r));
}

static inline unsigned int _sse2neon_mm_get_flush_zero_mode()
{
    union {
        fpcr_bitfield field;
        uint64_t value;
    } r;

    __asm__ __volatile__("mrs %0, FPCR" : "=r"(r.value));

    return r.field.bit24 ? _MM_FLUSH_ZERO_ON : _MM_FLUSH_ZERO_OFF;
}

static inline void _sse2neon_mm_set_flush_zero_mode(unsigned int flag)
{
    union {
        fpcr_bitfield field;
        uint64_t value;
    } r;

    __asm__ __volatile__("mrs %0, FPCR" : "=r"(r.value));

    r.field.bit24 = (flag & _MM_FLUSH_ZERO_MASK) == _MM_FLUSH_ZERO_ON;

    __asm__ __volatile__("msr FPCR, %0" ::"r"(r)); 
}

/* End of sse2neon code */

#elif defined(__i386__) || defined(__x86_64__)

#include <xmmintrin.h>
#include <pmmintrin.h>

#else

#define NO_DAZ_FTZ

#endif

Plugin::Plugin() {
	setControllerClass(FUID(CTRL_GUID_1, CTRL_GUID_2, CTRL_GUID_3, CTRL_GUID_4));
}

tresult PLUGIN_API Plugin::initialize(FUnknown *context) {
	tresult r = AudioEffect::initialize(context);
	if (r != kResultTrue)
		return r;

	P_INIT(&instance);

#ifdef P_NOTE_ON
	addEventInput(ConstStringTable::instance()->getString("MIDI Input"));
#endif

	// FIXME: vst3 sdk validator always seem to get kDefaultActive even in sdk plugins - it's probably broken, but let's check
#if NUM_BUSES_IN != 0
	for (int i = 0; i < NUM_BUSES_IN; i++)
		addAudioInput(
			ConstStringTable::instance()->getString(config_buses_in[i].name),
			config_buses_in[i].configs & IO_STEREO ? SpeakerArr::kStereo : SpeakerArr::kMono,
			config_buses_in[i].aux ? kAux : kMain,
			(config_buses_in[i].cv ? BusInfo::kIsControlVoltage : 0)
			| (config_buses_in[i].aux ? 0 : BusInfo::kDefaultActive)
		);
#endif

#if NUM_BUSES_OUT != 0
	for (int i = 0; i < NUM_BUSES_OUT; i++)
		addAudioOutput(
			ConstStringTable::instance()->getString(config_buses_out[i].name),
			config_buses_out[i].configs & IO_STEREO ? SpeakerArr::kStereo : SpeakerArr::kMono,
			config_buses_out[i].aux ? kAux : kMain,
			(config_buses_out[i].cv ? BusInfo::kIsControlVoltage : 0)
			| (config_buses_out[i].aux ? 0 : BusInfo::kDefaultActive)
		);
#endif

#if NUM_PARAMETERS != 0
	for (int i = 0; i < NUM_PARAMETERS; i++) {
		parameters[i] = config_parameters[i].defaultValueUnmapped;
		P_SET_PARAMETER(&instance, i, parameters[i]);
	}
#endif

	return kResultTrue;
}

tresult PLUGIN_API Plugin::terminate() {
	return AudioEffect::terminate();
}

tresult PLUGIN_API Plugin::setActive(TBool state) {
	if (state) {
		P_SET_SAMPLE_RATE(&instance, sampleRate);
		P_RESET(&instance);
	}
	return AudioEffect::setActive(state);
}

tresult PLUGIN_API Plugin::setupProcessing(ProcessSetup &setup) {
	sampleRate = static_cast<float>(setup.sampleRate);
	return AudioEffect::setupProcessing(setup);
}

tresult PLUGIN_API Plugin::process(ProcessData &data) {
	if (data.numInputs != NUM_BUSES_IN || data.numOutputs != NUM_BUSES_OUT)
		return kResultFalse;

#if NUM_PARAMETERS != 0
	if (data.inputParameterChanges) {
		int32 n = data.inputParameterChanges->getParameterCount();
		for (int32 i = 0; i < n; i++) {
			IParamValueQueue *q = data.inputParameterChanges->getParameterData(i);
			if (!q)
				continue;
			ParamValue v;
			int32 o;
			if (q->getPoint(q->getPointCount() - 1, o, v) == kResultTrue) {
				int32 pi = q->getParameterId();
				switch (pi) {
#ifdef P_PITCH_BEND
				case TAG_PITCH_BEND:
					P_PITCH_BEND(&instance, static_cast<int>(16383.f * std::min(std::max(static_cast<float>(v), 0.f), 1.f)));
					break;
#endif
#ifdef P_MOD_WHEEL
				case TAG_MOD_WHEEL:
					P_MOD_WHEEL(&instance, static_cast<char>(127.f * std::min(std::max(static_cast<float>(v), 0.f), 1.f)));
					break;
#endif
				default:
					parameters[pi] = v;
					P_SET_PARAMETER(&instance, pi, std::min(std::max(static_cast<float>(v), 0.f), 1.f));
					break;
				}
			}
		}
	}
#endif

#ifdef P_NOTE_ON
	if (data.inputEvents) {
		int32 n = data.inputEvents->getEventCount();
		for (int i = 0; i < n; i++) {
			Event e;
			if (data.inputEvents->getEvent(i, e) != kResultOk)
				continue;
			switch (e.type) {
				case Event::kNoteOnEvent:
					P_NOTE_ON(&instance, e.noteOn.pitch, 127.f * e.noteOn.velocity);
					break;
				case Event::kNoteOffEvent:
					P_NOTE_OFF(&instance, e.noteOff.pitch);
					break;
			}
		}
	}
#endif

#if NUM_BUSES_IN != 0
	int ki = 0;
	for (int i = 0; i < data.numInputs; i++)
		for (int j = 0; j < data.inputs[i].numChannels; j++, ki++)
			inputs[ki] = (const float *)data.inputs[i].channelBuffers32[j];
#endif

#if NUM_BUSES_OUT != 0	
	int ko = 0;
	for (int i = 0; i < data.numOutputs; i++)
		for (int j = 0; j < data.outputs[i].numChannels; j++, ko++)
			outputs[ko] = data.outputs[i].channelBuffers32[j];
#endif

	const unsigned int flush_zero_mode = _MM_GET_FLUSH_ZERO_MODE();
	const unsigned int denormals_zero_mode = _MM_GET_DENORMALS_ZERO_MODE();

#ifndef NO_DAZ_FTZ
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
	_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif

#if NUM_BUSES_IN == 0
	const float **inputs = nullptr;
#endif
#if NUM_BUSES_OUT == 0
	float **outputs = nullptr;
#endif
	P_PROCESS(&instance, inputs, outputs, data.numSamples);

#ifndef NO_DAZ_FTZ
	_MM_SET_FLUSH_ZERO_MODE(flush_zero_mode);
	_MM_SET_DENORMALS_ZERO_MODE(denormals_zero_mode);
#endif

#if NUM_PARAMETERS != 0
	for (int i = 0; i < NUM_PARAMETERS; i++) {
		if (!config_parameters[i].out)
			continue;
		float v = P_GET_PARAMETER(&instance, i);
		if (parameters[i] == v)
			continue;
		parameters[i] = v;
		if (data.outputParameterChanges) {
			int32 index;
			IParamValueQueue* paramQueue = data.outputParameterChanges->addParameterData(i, index);
			if (paramQueue)
				paramQueue->addPoint(0, v, index);
		}
	}
#endif

	return kResultTrue;
}

tresult PLUGIN_API Plugin::setBusArrangements(SpeakerArrangement *inputs, int32 numIns, SpeakerArrangement *outputs, int32 numOuts) {
	if (numIns != NUM_BUSES_IN || numOuts != NUM_BUSES_OUT)
		return kResultFalse;

#if NUM_BUSES_IN != 0
	for (int32 i = 0; i < numIns; i++)
		if ((config_buses_in[i].configs == IO_MONO && inputs[i] != SpeakerArr::kMono)
		    || (config_buses_in[i].configs == IO_STEREO && inputs[i] != SpeakerArr::kStereo))
			return kResultFalse;
#endif

#if NUM_BUSES_OUT != 0
	for (int32 i = 0; i < numOuts; i++)
		if ((config_buses_out[i].configs == IO_MONO && outputs[i] != SpeakerArr::kMono)
		    || (config_buses_out[i].configs == IO_STEREO && outputs[i] != SpeakerArr::kStereo))
			return kResultFalse;
#endif

	return kResultTrue;
}

tresult PLUGIN_API Plugin::setState(IBStream *state) {
	if (!state)
		return kResultFalse;

#if NUM_PARAMETERS != 0
	IBStreamer streamer(state, kLittleEndian);

	float f;
	for (int i = 0; i < NUM_PARAMETERS; i++) {
		if (config_parameters[i].out)
			continue;
		if (streamer.readFloat(f) == false)
			return kResultFalse;
		parameters[i] = f;
		P_SET_PARAMETER(&instance, i, f);
	}
#endif

	return kResultTrue;
}

tresult PLUGIN_API Plugin::getState(IBStream *state) {
#if NUM_PARAMETERS != 0
	IBStreamer streamer(state, kLittleEndian);

	for (int i = 0; i < NUM_PARAMETERS; i++)
		if (!config_parameters[i].out)
			streamer.writeFloat(parameters[i]);
#endif

	return kResultTrue;
}
