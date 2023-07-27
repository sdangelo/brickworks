#include <jni.h>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "config.h"

extern "C" {

ma_device device;

static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
}

JNIEXPORT jboolean JNICALL
Java_com_orastron_@JNI_NAME@_MainActivity_nativeAudioStart(JNIEnv* env, jobject thiz) {
#if NUM_CHANNELS_IN == 0
	ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
#else
	ma_device_config deviceConfig = ma_device_config_init(ma_device_type_duplex);
#endif
	deviceConfig.sampleRate			= 0;
	deviceConfig.periodSizeInFrames		= 64;
	deviceConfig.periods			= 1;
	deviceConfig.noPreSilencedOutputBuffer	= 1;
	deviceConfig.noDisableDenormals		= 0;
	deviceConfig.performanceProfile		= ma_performance_profile_low_latency;
	deviceConfig.capture.pDeviceID		= NULL;
	deviceConfig.capture.format		= ma_format_f32;
	deviceConfig.capture.channels		= NUM_CHANNELS_IN;
	deviceConfig.capture.shareMode		= ma_share_mode_shared;
	deviceConfig.playback.pDeviceID		= NULL;
	deviceConfig.playback.format		= ma_format_f32;
	deviceConfig.playback.channels		= NUM_CHANNELS_OUT;
	deviceConfig.dataCallback		= data_callback;
	
	if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
		return false;

	if (ma_device_start(&device) != MA_SUCCESS) {
		ma_device_uninit(&device);
		return false;
	}

	return true;
}

JNIEXPORT void JNICALL
Java_com_orastron_@JNI_NAME@_MainActivity_nativeAudioStop(JNIEnv* env, jobject thiz) {
	ma_device_stop(&device);
	ma_device_uninit(&device);
}

JNIEXPORT jfloat JNICALL
Java_com_orastron_@JNI_NAME@_MainActivity_nativeGetParameter(JNIEnv* env, jobject thiz, jint i) {
	return 0.3f;
}

JNIEXPORT void JNICALL
Java_com_orastron_@JNI_NAME@_MainActivity_nativeSetParameter(JNIEnv* env, jobject thiz, jint i, jfloat v) {
}

}
