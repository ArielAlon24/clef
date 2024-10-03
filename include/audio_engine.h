#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "miniaudio.h"
#include <pthread.h>

#define SAMPLE_RATE     44100
#define FADE_SPEED_RATE 0.5f

typedef void (* AudioEngineCallback)(float* buffer, unsigned int frame_count);
typedef ma_result AudioEngineResult;

/*
AudioEngineState_t defines a state machine for the AudioEngines's thread. It uses both states and transitions.
  STATE_PLAY     - Make the callback function the audio source and transition to STATE_FADE_IN.
  STATE_PLAYING  - The callback function is used as the audio source.
  STATE_PAUSE    - Stop using the callback function as the audio source and transition to STATE_FADE_OUT.
  STATE_PAUSED   - The callback function is not being used as the audio source.
  STATE_FADE_IN  - Slowly increase the amplitude until 100% and transition to STATE_PLAYING.
  STATE_FADE_OUT - Slowly decrease the amplitude until 0% and transition to STATE_PAUSED
*/
typedef enum {
    STATE_PLAY,
    STATE_PLAYING,
    STATE_PAUSE,
    STATE_PAUSED,
    STATE_FADE_IN,
    STATE_FADE_OUT,
} AudioEngineState;

typedef struct {
    ma_device device;
    ma_device_config config;
    AudioEngineCallback callback;
    pthread_mutex_t lock; /* TODO: Use a write only lock */
    AudioEngineState state;
    float amplitude; /* An inner amplitude value (between 0 and 1) used for fade-in and fade-out */
} AudioEngine;

static AudioEngine *audio_engine = NULL;

void _audio_engine_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count);

void audio_engine_init(AudioEngineCallback callback);
void audio_engine_free();

void audio_engine_play();
void audio_engine_pause();

void fade_in(float *buffer, unsigned int frame_count);
void fade_out(float *buffer, unsigned int frame_count);

void _audio_engine_set_state(AudioEngineState state);
AudioEngineState _audio_engine_get_state(void);

bool audio_engine_is_playing();

#endif
