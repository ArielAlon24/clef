#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "miniaudio.h"

#define SAMPLE_RATE 44100

typedef void (* AudioEngineCallback)(float* buffer, unsigned int frame_count);
typedef ma_result AudioEngineResult;

typedef struct {
    ma_device device;
    ma_device_config config;
    AudioEngineCallback callback;
} AudioEngine;

/* AudioEngine global instance */
static AudioEngine *audio_engine = NULL;

/* Miniaudio callback used by audio_engine */
void _audio_engine_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count);

void audio_engine_init(AudioEngineCallback callback);
void audio_engine_free();

AudioEngineResult audio_engine_start();
AudioEngineResult audio_engine_stop();

#endif
