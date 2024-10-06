#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "ring_buffer.h"
#include "midi_stream.h"
#include "oscillator.h"

typedef struct {
    RingBuffer *right_samples;
    RingBuffer *left_samples;
    pthread_mutex_t samples_lock;

    Oscillator *oscillator;

    MidiStream *midi_stream;
} App ;

static App *app = NULL;

void app_init();
void app_free();

void app_run();

void app_update();
void app_render();

#endif
