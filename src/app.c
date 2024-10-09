#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "app.h"
#include "window.h"
#include "macros.h"
#include "audio_engine.h"
#include "oscilloscope.h"
#include "midi/midi.h"
#include "components/oscillator.h"
#include "rack/rack.h"

void callback(float *buffer, unsigned int frame_count) {
    rack_next(app->current_rack, app->midi_stream, buffer, frame_count);
}

void analyzer(const float *buffer, unsigned int frame_count) {
    sample_buffer_push(app->sample_buffer, buffer, frame_count);
}

void app_init() {
    app = malloc(sizeof(App));
    assert(app != NULL);

    app->sample_buffer = sample_buffer_init(SAMPLE_RATE / 10);
    app->midi_stream = midi_stream_init();
    app->root_rack = rack_init(5);
    app->current_rack = app->root_rack;

    audio_engine_init(callback, analyzer);
    window_init(500, 800);
}

void app_free() {
    window_free();
    audio_engine_free();

    sample_buffer_free(app->sample_buffer);
    rack_free(app->root_rack);
    midi_stream_free(app->midi_stream);

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
    if (IsKeyPressed(KEY_SPACE) && !IsKeyPressedRepeat(KEY_SPACE)) {
        if (audio_engine_is_playing()) {
            audio_engine_pause();
        } else {
            audio_engine_play();
        }
    }

    if (IsKeyPressed(KEY_RIGHT)) rack_cursor_right(app->current_rack);
    if (IsKeyPressed(KEY_LEFT)) rack_cursor_left(app->current_rack);
    if (IsKeyPressed(KEY_UP)) rack_cursor_up(app->current_rack);
    if (IsKeyPressed(KEY_DOWN)) rack_cursor_down(app->current_rack);

    if (IsKeyPressed(KEY_ENTER) && !IsKeyPressedRepeat(KEY_ENTER)) {
        Component *oscillator = oscillator_init(OSCILLATOR_SINE, 440.0f, 0.2);
        rack_mount(app->current_rack, oscillator);
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !IsKeyPressedRepeat(KEY_BACKSPACE)) {
        rack_unmount(app->current_rack);
    }

    if (IsKeyPressed(KEY_TAB) && !IsKeyPressedRepeat(KEY_TAB)) {
        Rack *new_rack = rack_init(5);
        Component *new_rack_comp = rack_component_init(new_rack);
        Component *new_osc = oscillator_init(OSCILLATOR_SINE, 660.0f, 0.2);
        Vector2 pos = {0, 0};
        rack_mount_vec(new_rack, new_osc, pos);
        rack_mount(app->current_rack, new_rack_comp);
    }

    MidiMessage message;
    if (IsKeyPressed(KEY_A)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 60, 80); /* C4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_W)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 61, 80);  /* C#4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_S)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 62, 80);  /* D4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_E)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 63, 80);  /* D#4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_D)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 64, 80);  /* E4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_F)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 65, 80);  /* F4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_T)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 66, 80);  /* F#4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_G)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 67, 80);  /* G4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_Y)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 68, 80);  /* G#4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_H)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 69, 80);  /* A4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_U)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 70, 80);  /* A#4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_J)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 71, 80);  /* B4 */
        midi_stream_write(app->midi_stream, &message);
    }
    if (IsKeyPressed(KEY_K)) {
        MIDI_MESSAGE2(message, MIDI_MESSAGE_NOTE_ON, 72, 80);  /* C5 */
        midi_stream_write(app->midi_stream, &message);
    }
}

void app_render() {
    BeginDrawing();
        ClearBackground(BLACK);
        if (audio_engine_is_playing()) {
            DrawText("Playing", 10, 10, 20, WHITE);
        } else {
            DrawText("Paused", 10, 10, 20, WHITE);
        }

        Vector2 rack_position = {10, 40};
        Vector2 rack_size ={ window_height() * 0.6 - 20, window_height() * 0.6 - 20 };
        rack_render(app->current_rack, rack_position, rack_size);

        Vector2 position = {10, rack_position.y + rack_size.y + 10};
        Vector2 size = {200, 200};
        oscilloscope_render(app->sample_buffer, position, size);
    EndDrawing();
}
