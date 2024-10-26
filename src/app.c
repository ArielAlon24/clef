#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "constants/size.h"
#include "constants/color.h"
#include "widgets/oscilloscope.h"
#include "widgets/dB_meter.h"
#include "widgets/settings_panel.h"
#include "widgets/play_pause_panel.h"
#include "component_system/component.h"
#include "component_system/component_system.h"
#include "midi/midi.h"
#include "app.h"
#include "window.h"
#include "macros.h"
#include "audio_engine.h"
#include "texture_handler.h"

void callback(float *buffer, size_t buffer_size) {
    component_audio_callback(app->root_rack, buffer, buffer_size);
}

void analyzer(const float *buffer, size_t frame_count) {
    sample_buffer_push(app->sample_buffer, buffer, frame_count);
}

void app_init() {
    app = malloc(sizeof(App));
    assert(app != NULL);

    component_system_init();

    app->sample_buffer = sample_buffer_init(SAMPLE_RATE / 10);
    app->global_stream = midi_stream_init();
    app->user_stream = midi_stream_init();
    app->root_rack = component_init(COMPONENT_RACK, NULL);
    app->current_rack = app->root_rack;
    app->component_selector = (ComponentType)0; /* The first component type */

    audio_engine_init(callback, analyzer);
    window_init(WINDOW_WIDTH, WINDOW_HEIGHT);

    app->pixel_renderer = pixel_renderer_init(WIDTH, HEIGHT);
}

void app_free() {
    audio_engine_free();

    pixel_renderer_free(app->pixel_renderer);
    sample_buffer_free(app->sample_buffer);
    component_free(app->root_rack);
    midi_stream_free(app->global_stream);
    midi_stream_free(app->user_stream);

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

KeyboardKey MIDI_KEYS[] = {KEY_LEFT_BRACKET, KEY_RIGHT_BRACKET, KEY_C};

void app_update() {
    window_update();
    if (IsKeyPressed(KEY_SPACE) && !IsKeyPressedRepeat(KEY_SPACE)) {
        if (audio_engine_is_playing()) {
            midi_stream_write(app->global_stream, MIDI_MESSAGE(MIDI_MESSAGE_STOP));
            audio_engine_pause();
        } else {
            midi_stream_write(app->global_stream, MIDI_MESSAGE(MIDI_MESSAGE_START));
            audio_engine_play();
        }
    }

    if (IsKeyPressed(KEY_D)) component_move_cursor(app->current_rack, (Vector2){1, 0});
    if (IsKeyPressed(KEY_A)) component_move_cursor(app->current_rack, (Vector2){-1, 0});
    if (IsKeyPressed(KEY_W)) component_move_cursor(app->current_rack, (Vector2){0, -1});
    if (IsKeyPressed(KEY_S)) component_move_cursor(app->current_rack, (Vector2){0, 1});

    for (int i = 0; i < LEN(MIDI_KEYS); ++i) {
        if (IsKeyPressed(MIDI_KEYS[i])) {
            midi_stream_write(app->user_stream, MIDI_MESSAGE1(MIDI_MESSAGE_SYSEX, MIDI_KEYS[i]));
        }
    }

    if (IsKeyPressed(KEY_RIGHT)) app->component_selector = (app->component_selector + 1) % _COMPONENT_TYPE_SIZE;
    if (IsKeyPressed(KEY_LEFT)) app->component_selector = (app->component_selector + _COMPONENT_TYPE_SIZE - 1) % _COMPONENT_TYPE_SIZE;

    if (IsKeyPressed(KEY_ENTER)) {
        Component *component = component_init(app->component_selector, app->current_rack);
        rack_mount(app->current_rack, component);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        Component *parent = app->current_rack->parent;
        if (parent) app->current_rack = parent;
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !IsKeyPressedRepeat(KEY_BACKSPACE)) {
        rack_unmount(app->current_rack);
    }

    if (IsKeyPressed(KEY_TAB) && !IsKeyPressedRepeat(KEY_TAB)) {
        Component *component = component_current(app->current_rack);

        if (component && component->is_container) {
            app->current_rack = component;
        }
    }

    app_dispatch_midi();
}

void app_render() {
    pixel_renderer_begin(app->pixel_renderer);
        ClearBackground(COLOR_BLACK);
        /* TODO: Turn each element commented here into a widget render function */
        /* Component-Preview Panel */
        DrawRectangleV((Vector2){ BORDER_PADDING + 13 * CUBIC, BORDER_PADDING + 3 * CUBIC }, (Vector2){ CUBIC * 4, CUBIC * 4 }, COLOR_DARK_GRAY);

        /* Rack Tree Panel */
        DrawRectangleV((Vector2){ BORDER_PADDING + 13 * CUBIC, BORDER_PADDING + 8 * CUBIC}, (Vector2){ CUBIC * 7, CUBIC * 12}, COLOR_DARK_GRAY);

        /* Component Preview Panel */
        component_preview(app->component_selector, (Vector2){ BORDER_PADDING + 14 * CUBIC, BORDER_PADDING + 4 * CUBIC }, COMPONENT_DIMENSIONS);

        play_pause_panel_render(app, PLAY_PAUSE_PANEL_POSITION, PLAY_PAUSE_PANEL_DIMENSIONS);
        settings_panel_render(app, SETTINGS_PANEL_POSITION, SETTINGS_PANEL_DIMENSIONS);
        component_rack_render(app->current_rack, RACK_POSITION, RACK_DIMENSIONS);
        dB_meter_render(app->sample_buffer, DB_METER_POSITION, DB_METER_DIMENSIONS);
        oscilloscope_render(app->sample_buffer, OSCILLOSCOPE_POSITION, OSCILLOSCOPE_DIMENSIONS);

    pixel_renderer_end(app->pixel_renderer);
}


void app_dispatch_midi() {
    const MidiMessage *messages;
    size_t messages_size;

    messages = midi_stream_messages(app->global_stream);
    messages_size = midi_stream_size(app->global_stream);
    component_midi_callback(app->root_rack, messages, messages_size, true);
    midi_stream_flush(app->global_stream);

    Component *component = rack_current(app->current_rack);
    if (component) {
        messages = midi_stream_messages(app->user_stream);
        messages_size = midi_stream_size(app->user_stream);
        component_midi_callback(component, messages, messages_size, false);
    }
    midi_stream_flush(app->user_stream);
}
