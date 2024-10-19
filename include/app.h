#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "sample_buffer.h"
#include "midi/midi_stream.h"
#include "pixel_renderer.h"
#include "component_system/component_system.h"
#include "components/rack.h"

typedef struct {
    PixelRenderer *pixel_renderer;

    SampleBuffer *sample_buffer;

    /* Global MIDI stream used for global events like play - pause */
    MidiStream *global_stream;
    /* User created MIDI stream used for interacting with components */
    MidiStream *user_stream;

    /* Currently selected component type */
    ComponentType component_selector;

    /* The root rack of all other racks. */
    Component *root_rack;
    /* The current used rack */
    Component *current_rack;
} App ;

static App *app = NULL;

void app_init();
void app_free();

void app_run();

void app_update();
void app_render();

#endif
