#define MINIAUDIO_IMPLEMENTATION

#include <audio_engine.h>
#include <miniaudio.h>
#include <stdio.h>

void audio_engine_init(AudioEngineCallback callback) {
    audio_engine = malloc(sizeof(AudioEngine));
    audio_engine->callback = callback;

    /* Initialize AudioEngine's config */
    audio_engine->config = ma_device_config_init(ma_device_type_playback);
    audio_engine->config.playback.format = ma_format_f32;
    audio_engine->config.playback.channels = 2;
    audio_engine->config.sampleRate = SAMPLE_RATE;
    audio_engine->config.dataCallback = _audio_engine_callback;

    /* TODO: Decide on erro handling */
    ma_result result = ma_device_init(NULL, &audio_engine->config, &audio_engine->device);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to initialize playback device.\n");
        exit(0);
    }
}

/* AudioEngine callback used under the hood, call the AudioEngineCallback type provided by the user */
void _audio_engine_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count) {
    audio_engine->callback(output, input, frame_count);
}

void audio_engine_free() {
    ma_device_uninit(&audio_engine->device);
    free(audio_engine);
}

AudioEngineResult audio_engine_play() {
    return ma_device_start(&audio_engine->device);
}
