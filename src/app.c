#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "color.h"
#include "app.h"
#include "window.h"
#include "macros.h"
#include "audio_engine.h"
#include "oscilloscope.h"
#include "midi/midi.h"
#include "components/oscillator.h"
#include "rack/rack.h"
#include "rack/component_handler.h"
#include "texture_handler.h"
#include "size.h"

void callback(float *buffer, unsigned int frame_count) {
    rack_next(app->root_rack, app->midi_stream, buffer, frame_count);
}

void analyzer(const float *buffer, unsigned int frame_count) {
    sample_buffer_push(app->sample_buffer, buffer, frame_count);
}

void app_init() {
    app = malloc(sizeof(App));
    assert(app != NULL);

    app->sample_buffer = sample_buffer_init(SAMPLE_RATE / 10);
    app->midi_stream = midi_stream_init();
    app->root_rack = rack_init(RACK_SIZE, NULL);
    app->current_rack = app->root_rack;
    app->component_selector = (ComponentType) 0; /* The first component type */

    audio_engine_init(callback, analyzer);
    window_init(WINDOW_WIDTH, WINDOW_HEIGHT);
    component_handler_init();

    app->pixel_renderer = pixel_renderer_init(WIDTH, HEIGHT);
}

void app_free() {
    audio_engine_free();

    pixel_renderer_free(app->pixel_renderer);
    sample_buffer_free(app->sample_buffer);
    rack_free(app->root_rack);
    midi_stream_free(app->midi_stream);

    texture_unload_all();
    window_free();

    free(app);
}

void app_run() {
    while (!window_should_close()) {
        app_update();
        app_render();
    }
}

void app_update() {
    window_update();
    MidiMessage message;

    if (IsKeyPressed(KEY_SPACE) && !IsKeyPressedRepeat(KEY_SPACE)) {
        if (audio_engine_is_playing()) {
            MIDI_MESSAGE(message, MIDI_MESSAGE_STOP);
            audio_engine_pause();
        } else {
            MIDI_MESSAGE(message, MIDI_MESSAGE_START);
            audio_engine_play();
        }
        midi_stream_write(app->midi_stream, message);
    }

    if (IsKeyPressed(KEY_D)) rack_cursor_right(app->current_rack);
    if (IsKeyPressed(KEY_A)) rack_cursor_left(app->current_rack);
    if (IsKeyPressed(KEY_W)) rack_cursor_up(app->current_rack);
    if (IsKeyPressed(KEY_S)) rack_cursor_down(app->current_rack);

    if (IsKeyPressed(KEY_RIGHT)) app->component_selector = (app->component_selector + 1) % _COMPONENT_TYPE_SIZE;
    if (IsKeyPressed(KEY_LEFT)) app->component_selector = (app->component_selector - 1) % _COMPONENT_TYPE_SIZE;

    if (IsKeyPressed(KEY_ENTER)) {
        Component *component;
        if (app->component_selector == COMPONENT_RACK) {
            Rack *new_rack = rack_init(RACK_SIZE, app->current_rack);
            component = rack_component_init(new_rack);
        } else {
            component = component_create(app->component_selector);
        }
        rack_mount(app->current_rack, component);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        Rack *parent = rack_get_parent(app->current_rack);
        if (parent) app->current_rack = parent;
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !IsKeyPressedRepeat(KEY_BACKSPACE)) {
        rack_unmount(app->current_rack);
    }

    if (IsKeyPressed(KEY_TAB) && !IsKeyPressedRepeat(KEY_TAB)) {
        Component *component = rack_get_component(app->current_rack);

        if (component && component_is_enterable(component)) {
            app->current_rack = component->state;
        }
    }
}

void app_render() {
    pixel_renderer_begin(app->pixel_renderer);
        ClearBackground(COLOR_BLACK);
        rack_render(app->current_rack, RACK_POSITION, RACK_DIMENSIONS);

        /* Play-Pause panel */
        DrawRectangleV((Vector2){ BORDER_PADDING, BORDER_PADDING }, (Vector2){ CUBIC * 12, CUBIC * 2 }, COLOR_DARK_GRAY);

        /* Component-Settings Panel */
        DrawRectangleV((Vector2){ BORDER_PADDING, BORDER_PADDING + 3 * CUBIC }, (Vector2){ CUBIC * 12, CUBIC * 4 }, COLOR_DARK_GRAY);

        /* Component-Preview Panel */
        DrawRectangleV((Vector2){ BORDER_PADDING + 13 * CUBIC, BORDER_PADDING + 3 * CUBIC }, (Vector2){ CUBIC * 4, CUBIC * 4 }, COLOR_DARK_GRAY);

        /* dB Meter */
        DrawRectangleV((Vector2){ BORDER_PADDING + 18 * CUBIC, BORDER_PADDING + 3 * CUBIC}, (Vector2){ CUBIC * 2, CUBIC * 4 }, COLOR_DARK_GRAY);

        /* Rack Tree Panel */
        DrawRectangleV((Vector2){ BORDER_PADDING + 13 * CUBIC, BORDER_PADDING + 8 * CUBIC}, (Vector2){ CUBIC * 7, CUBIC * 12}, COLOR_DARK_GRAY);

        component_preview(app->component_selector, (Vector2){ BORDER_PADDING + 14 * CUBIC, BORDER_PADDING + 4 * CUBIC });

        oscilloscope_render(app->sample_buffer, OSCILLOSCOPE_POSITION, OSCILLOSCOPE_DIMENSIONS);

        if (audio_engine_is_playing()) {
            DrawText("Playing", BORDER_PADDING, BORDER_PADDING, 10, COLOR_WHITE);
        } else {
            DrawText("Paused", BORDER_PADDING, BORDER_PADDING, 10, COLOR_WHITE);
        }

    pixel_renderer_end(app->pixel_renderer);
}
