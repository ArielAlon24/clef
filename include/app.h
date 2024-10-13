#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "sample_buffer.h"
#include "midi/midi_stream.h"
#include "rack/rack.h"
#include "pixel_renderer.h"

typedef struct {
    PixelRenderer *pixel_renderer;

    SampleBuffer *sample_buffer;
    MidiStream *midi_stream;

    /* The root rack of all other racks. */
    Rack *root_rack;
    /* The current used rack */
    Rack *current_rack;
} App ;

static App *app = NULL;

void app_init();
void app_free();

void app_run();

void app_update();
void app_render();

#endif
