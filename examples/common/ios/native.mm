/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
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
 */

#include <algorithm>
#include <mutex>
#include <vector>
#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_RUNTIME_LINKING
#include <miniaudio.h>
#include "config.h"

#define BLOCK_SIZE	32
#define NUM_BUFS	(NUM_CHANNELS_IN > NUM_CHANNELS_OUT ? NUM_CHANNELS_IN : NUM_CHANNELS_OUT)

ma_device device;
#ifdef P_NOTE_ON
CFStringRef midiClientName = NULL;
MIDIClientRef midiClient = NULL;
CFStringRef midiInputName = NULL;
MIDIPortRef midiPort = NULL;
#endif
P_TYPE instance;
float paramValues[NUM_PARAMETERS];
float bufs[NUM_BUFS][BLOCK_SIZE];
#if NUM_CHANNELS_IN != 0
const float *inBufs[NUM_CHANNELS_IN];
#endif
float *outBufs[NUM_CHANNELS_OUT];
std::mutex mutex;
#ifdef P_MEM_REQ
void *mem;
#endif

static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	(void)pDevice;
#if NUM_CHANNELS_IN == 0
	(void)pInput;
#else
	const float *x = reinterpret_cast<const float *>(pInput);
#endif
	float *y = reinterpret_cast<float *>(pOutput);

	if (mutex.try_lock()) {
		for (int i = 0; i < NUM_PARAMETERS; i++)
		if (config_parameters[i].out)
			paramValues[i] = P_GET_PARAMETER(&instance, i);
		else
			P_SET_PARAMETER(&instance, i, paramValues[i]);
		mutex.unlock();
	}

	ma_uint32 i = 0;
	while (i < frameCount) {
		ma_uint32 n = std::min(frameCount - i, static_cast<ma_uint32>(BLOCK_SIZE));

		int l;
#if NUM_CHANNELS_IN != 0
		l = NUM_CHANNELS_IN * i;
		for (ma_uint32 j = 0; j < n; j++)
			for (int k = 0;  k < NUM_CHANNELS_IN; k++, l++)
				bufs[k][j] = x[l];
#endif

#if NUM_CHANNELS_IN != 0
		P_PROCESS(&instance, inBufs, outBufs, n);
#else
		P_PROCESS(&instance, NULL, outBufs, n);
#endif

		l = NUM_CHANNELS_OUT * i;
		for (ma_uint32 j = 0; j < n; j++)
			for (int k = 0;  k < NUM_CHANNELS_OUT; k++, l++)
				y[l] = bufs[k][j];

		i += n;
	}
}

#ifdef P_NOTE_ON
void (^midiNotifyBlock)(const MIDINotification *message) = ^(const MIDINotification *message) {
	if (message->messageID != kMIDIMsgObjectAdded)
		return;
	const MIDIObjectAddRemoveNotification *n = reinterpret_cast<const MIDIObjectAddRemoveNotification *>(message);
	MIDIEndpointRef endPoint = n->child;
	MIDIPortConnectSource(midiPort, endPoint, NULL);
};

void (^midiReceiveBlock)(const MIDIEventList *evtlist, void *srcConnRefCon) = ^(const MIDIEventList *evtlist, void *srcConnRefCon) {
	const MIDIEventPacket *p = evtlist->packet;
	for (UInt32 i = 0; i < evtlist->numPackets; i++) {
		for (UInt32 j = 0; j < p->wordCount; j++) {
			UInt32 w = p->words[j];
			if ((w & 0xf0000000) != 0x20000000)
				continue;
			switch (w & 0x00f00000) {
			case 0x00900000:
				mutex.lock();
				P_NOTE_ON(&instance, (w & 0x0000ff00) >> 8, w & 0x000000ff);
				mutex.unlock();
				break;
			case 0x00800000:
				mutex.lock();
				P_NOTE_OFF(&instance, (w & 0x0000ff00) >> 8);
				mutex.unlock();
				break;
#ifdef P_PITCH_BEND
			case 0x00e00000:
				mutex.lock();
				P_PITCH_BEND(&instance, ((w & 0x0000007f) << 7) | ((w & 0x00007f00) >> 8));
				mutex.unlock();
				break;
#endif
#ifdef P_MOD_WHEEL
			case 0x00b00000:
				if ((w & 0x0000ff00) == 0x00000100) {
					mutex.lock();
					P_MOD_WHEEL(&instance, w & 0x000000ff);
					mutex.unlock();
				}
				break;
#endif
			}
		}
		p = MIDIEventPacketNext(p);
	}
};
#endif

extern "C"
char audioStart() {
#if NUM_CHANNELS_IN == 0
	ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
#else
	ma_device_config deviceConfig = ma_device_config_init(ma_device_type_duplex);
#endif
	deviceConfig.periodSizeInFrames		= BLOCK_SIZE;
	deviceConfig.periods			= 1;
	deviceConfig.performanceProfile		= ma_performance_profile_low_latency;
	deviceConfig.noPreSilencedOutputBuffer	= 1;
	deviceConfig.noClip			= 0;
	deviceConfig.noDisableDenormals		= 0;
	deviceConfig.noFixedSizedCallback	= 1;
	deviceConfig.dataCallback		= data_callback;
	deviceConfig.capture.pDeviceID		= NULL;
	deviceConfig.capture.format		= ma_format_f32;
	deviceConfig.capture.channels		= NUM_CHANNELS_IN;
	deviceConfig.capture.shareMode		= ma_share_mode_shared;
	deviceConfig.playback.pDeviceID		= NULL;
	deviceConfig.playback.format		= ma_format_f32;
	deviceConfig.playback.channels		= NUM_CHANNELS_OUT;
	deviceConfig.playback.shareMode		= ma_share_mode_shared;

	size_t req = 0; // make stupid compiler happy

	if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
		goto err_device_init;

#ifdef P_NOTE_ON
	if (midiClientName == NULL) {
		midiClientName = CFSTR("template");
		if (midiClientName == NULL)
			goto err_midiClientName;
	}
	if (midiClient == (MIDIClientRef)NULL) {
		if (MIDIClientCreateWithBlock(midiClientName, &midiClient, midiNotifyBlock) != 0)
			goto err_midiClientCreate;
	}

	if (midiInputName == NULL) {
		midiInputName = CFSTR("Input");
		if (midiInputName == NULL)
			goto err_midiInputName;
	}
	if (midiPort == (MIDIPortRef)NULL) {
		if (MIDIInputPortCreateWithProtocol(midiClient, midiInputName, kMIDIProtocol_1_0, &midiPort, midiReceiveBlock) != 0)
			goto err_midiInputPort;

		ItemCount n = MIDIGetNumberOfSources();
		for (ItemCount i = 0; i < n; i++) {
			MIDIEndpointRef endPoint = MIDIGetSource(i);
			MIDIPortConnectSource(midiPort, endPoint, NULL);
		}
	}
#endif
    
	P_INIT(&instance);
	P_SET_SAMPLE_RATE(&instance, (float)device.sampleRate);
#ifdef P_MEM_REQ
	req = P_MEM_REQ(&instance);
	if (req) {
		mem = malloc(req);
		if (mem == NULL)
			goto err_mem_req;
		P_MEM_SET(&instance, mem);
	} else
		mem = NULL;
#endif

	for (int i = 0; i < NUM_PARAMETERS; i++) {
		paramValues[i] = config_parameters[i].defaultValueUnmapped;
		if (!config_parameters[i].out)
			P_SET_PARAMETER(&instance, i, paramValues[i]);
	}

	P_RESET(&instance);

#if NUM_CHANNELS_IN != 0
	for (int i = 0; i < NUM_CHANNELS_IN; i++)
		inBufs[i] = bufs[i];
#endif
	for (int i = 0; i < NUM_CHANNELS_OUT; i++)
		outBufs[i] = bufs[i];

	if (ma_device_start(&device) != MA_SUCCESS)
		goto err_device_start;

	return 1;

err_device_start:
#ifdef P_MEM_REQ
	free(mem);
err_mem_req:
#endif
#ifdef P_FINI
	P_FINI(&instance);
#endif
#ifdef P_NOTE_ON
err_midiInputPort:
err_midiInputName:
err_midiClientCreate:
err_midiClientName:
#endif
	ma_device_uninit(&device);
err_device_init:
	return 0;
}

extern "C"
void audioStop() {
#ifdef P_MEM_REQ
	free(mem);
#endif
#ifdef P_FINI
	P_FINI(&instance);
#endif
	ma_device_stop(&device);
	ma_device_uninit(&device);
}

extern "C"
void setParameter(int i, float v) {
	mutex.lock();
	paramValues[i] = v;
	mutex.unlock();
}

extern "C"
float getParameter(int i) {
	mutex.lock();
	float v = paramValues[i];
	mutex.unlock();
	return v;
}
