#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "sample_buffer.h"
#include "midi/midi_stream.h"
#include "rack/rack.h"

typedef struct {
    SampleBuffer *sample_buffer;
    Rack *rack;
    MidiStream *midi_stream;
} App ;

static App *app = NULL;

void app_init();
void app_free();

void app_run();

void app_update();
void app_render();

#endif
