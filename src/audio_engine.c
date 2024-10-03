#define MINIAUDIO_IMPLEMENTATION

#include "audio_engine.h"
#include <miniaudio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void audio_engine_init(AudioEngineCallback callback, AudioEngineAnalyzer analyzer) {
    audio_engine = malloc(sizeof(AudioEngine));
    audio_engine->amplitude = 0.0f;
    audio_engine->callback = callback;
    audio_engine->analyzer = analyzer;

    /* Initialize AudioEngine's config */
    audio_engine->config = ma_device_config_init(ma_device_type_playback);
    audio_engine->config.playback.format = ma_format_f32;
    audio_engine->config.playback.channels = 2;
    audio_engine->config.sampleRate = SAMPLE_RATE;
    audio_engine->config.dataCallback = _audio_engine_callback;

    /* TODO: Decide on error handling */
    ma_result result = ma_device_init(NULL, &audio_engine->config, &audio_engine->device);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to initialize playback device.\n");
        exit(1);
    }

    /* Start the audio device (non-blocking) */
    pthread_rwlock_init(&audio_engine->lock, NULL);
    audio_engine->state = STATE_PAUSED;
    result = ma_device_start(&audio_engine->device);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to start device.\n");
        exit(1);
    }
}

void _audio_engine_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count) {
    float *buffer = (float *)output;
    AudioEngineState state = _audio_engine_get_state();

    switch (state) {
        case STATE_PLAY:
            audio_engine->state = STATE_FADE_IN;
        case STATE_FADE_IN:
            audio_engine->callback(buffer, frame_count);
            fade_in(buffer, frame_count);
            if (audio_engine->amplitude == 1.0f) { audio_engine->state = STATE_PLAYING; }
            break;
        case STATE_PAUSE:
            audio_engine->state = STATE_FADE_OUT;
        case STATE_FADE_OUT:
            audio_engine->callback(buffer, frame_count);
            fade_out(buffer, frame_count);
            if (audio_engine->amplitude == 0.0f) { audio_engine->state = STATE_PAUSED; }
            break;
        case STATE_PLAYING:
            audio_engine->callback(buffer, frame_count);
            break;
        case STATE_PAUSED:
            memset(buffer, 0, frame_count * 2 * sizeof(float));
            break;
    }

    audio_engine->analyzer(buffer, frame_count);
}

void fade_in(float *buffer, unsigned int frame_count) {
    float amplitude = audio_engine->amplitude;
    float goal = MIN(1.0f, amplitude + FADE_SPEED_RATE);
    float interval = (goal - amplitude) / frame_count;

    for (size_t i = 0; i < frame_count * 2; i += 2) {
        amplitude += interval;
        buffer[i] *= amplitude;     /* Left Channel */
        buffer[i + 1] *= amplitude; /* Right Channel */
    }

    audio_engine->amplitude = goal;
}

void fade_out(float *buffer, unsigned int frame_count) {
    float amplitude = audio_engine->amplitude;
    float goal = MAX(0.0f, amplitude - FADE_SPEED_RATE);

    float interval = (amplitude - goal) / frame_count;

    for (size_t i = 0; i < frame_count * 2; i += 2) {
        amplitude -= interval;
        buffer[i] *= amplitude;     /* Left Channel */
        buffer[i + 1] *= amplitude; /* Right Channel */
    }

    audio_engine->amplitude = goal;
}

void audio_engine_free() {
    audio_engine_pause();
    while (_audio_engine_get_state() != STATE_PAUSED) {}
    ma_device_uninit(&audio_engine->device);
    free(audio_engine);
}

void audio_engine_play() { _audio_engine_set_state(STATE_PLAY); }
void audio_engine_pause() { _audio_engine_set_state(STATE_PAUSE); }

void _audio_engine_set_state(AudioEngineState state) {
    pthread_rwlock_wrlock(&audio_engine->lock);
    audio_engine->state = state;
    pthread_rwlock_unlock(&audio_engine->lock);
}

AudioEngineState _audio_engine_get_state(void) {
    return audio_engine->state;
}

bool audio_engine_is_playing() {
    return _audio_engine_get_state() != STATE_PAUSED;
}
