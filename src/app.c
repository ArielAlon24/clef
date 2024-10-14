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

    if (IsKeyPressed(KEY_ONE)) {
        Component *oscillator = component_create(COMPONENT_OSCILLATOR);
        rack_mount(app->current_rack, oscillator);
    }

    if (IsKeyPressed(KEY_TWO)) {
        Rack *new_rack = rack_init(RACK_SIZE, app->current_rack);
        Component *new_rack_comp = rack_component_init(new_rack);
        rack_mount(app->current_rack, new_rack_comp);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        Rack *parent = rack_get_parent(app->current_rack);
        if (parent) {
            app->current_rack = parent;
        }
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

        if (audio_engine_is_playing()) {
            DrawText("Playing", WINDOW_PADDING, WINDOW_PADDING, 10, COLOR_WHITE);
        } else {
            DrawText("Paused", WINDOW_PADDING, WINDOW_PADDING, 10, COLOR_WHITE);
        }

        DrawRectangleV((Vector2){WINDOW_PADDING, WINDOW_PADDING}, (Vector2){WINDOW_PADDING * 6, WINDOW_PADDING * 3.5}, COLOR_DARK_GRAY);
        DrawRectangleV((Vector2){7.5 * WINDOW_PADDING, 2.5 * WINDOW_PADDING}, (Vector2){WINDOW_PADDING * 3.5, WINDOW_PADDING * 8.5}, COLOR_DARK_GRAY);

        oscilloscope_render(app->sample_buffer, OSCILLOSCOPE_POSITION, OSCILLOSCOPE_DIMENSIONS);
    pixel_renderer_end(app->pixel_renderer);
}
