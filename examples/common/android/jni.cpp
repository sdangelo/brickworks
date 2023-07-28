#include <algorithm>
#include <mutex>
#include <jni.h>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "config.h"

#define BLOCK_SIZE	32
#define NUM_BUFS	(NUM_CHANNELS_IN > NUM_CHANNELS_OUT ? NUM_CHANNELS_IN : NUM_CHANNELS_OUT)

ma_device device;
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
#endif

	const float *x = reinterpret_cast<const float *>(pInput);
	float *y = reinterpret_cast<float *>(pOutput);

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

		P_PROCESS(&instance, inBufs, outBufs, n);

		l = NUM_CHANNELS_OUT * i;
		for (ma_uint32 j = 0; j < n; j++)
			for (int k = 0;  k < NUM_CHANNELS_OUT; k++, l++)
				y[l] = bufs[k][j];

		i += n;
	}

	if (mutex.try_lock()) {
		for (int i = 0; i < NUM_PARAMETERS; i++)
			if (config_parameters[i].out)
				paramValues[i] = P_GET_PARAMETER(&instance, i);
			else
				P_SET_PARAMETER(&instance, i, paramValues[i]);
		mutex.unlock();
	}
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_orastron_@JNI_NAME@_MainActivity_nativeAudioStart(JNIEnv* env, jobject thiz) {
	(void)env;
	(void)thiz;

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

	if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
		return false;

	P_INIT(&instance);
	P_SET_SAMPLE_RATE(&instance, (float)device.sampleRate);
#ifdef P_MEM_REQ
	size_t req = P_MEM_REQ(&instance);
	if (req) {
		mem = malloc(req);
		if (mem == NULL) {
			ma_device_uninit(&device);
			return false;
		}
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

	if (ma_device_start(&device) != MA_SUCCESS) {
#ifdef P_MEM_REQ
		free(mem);
#endif
		ma_device_uninit(&device);
		return false;
	}

	return true;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_orastron_@JNI_NAME@_MainActivity_nativeAudioStop(JNIEnv* env, jobject thiz) {
	(void)env;
	(void)thiz;

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
JNIEXPORT jfloat JNICALL
Java_com_orastron_@JNI_NAME@_MainActivity_nativeGetParameter(JNIEnv* env, jobject thiz, jint i) {
	(void)env;
	(void)thiz;

	mutex.lock();
	float v = paramValues[i];
	mutex.unlock();
	return v;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_orastron_@JNI_NAME@_MainActivity_nativeSetParameter(JNIEnv* env, jobject thiz, jint i, jfloat v) {
	(void)env;
	(void)thiz;

	mutex.lock();
	paramValues[i] = v;
	mutex.unlock();
}
