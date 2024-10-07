#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "window.h"
#include "audio_engine.h"
#include "midi.h"
#include "components/oscillator.h"
#include <assert.h>
#include "macros.h"
#include "oscilloscope.h"
#include "rack/rack.h"

void callback(float *buffer, unsigned int frame_count) {
    rack_next(app->rack, app->midi_stream, buffer, frame_count);
}

void analyzer(const float *buffer, unsigned int frame_count) {
    sample_buffer_push(app->sample_buffer, buffer, frame_count);
}

void app_init() {
    app = malloc(sizeof(App));
    assert(app != NULL);

    app->sample_buffer = sample_buffer_init(SAMPLE_RATE / 10);
    app->midi_stream = midi_stream_init();
    app->rack = rack_init(5);

    audio_engine_init(callback, analyzer);
    window_init(500, 500);
}

void app_free() {
    window_free();
    audio_engine_free();

    sample_buffer_free(app->sample_buffer);
    rack_free(app->rack);
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

    if (IsKeyPressed(KEY_ENTER) && !IsKeyPressedRepeat(KEY_ENTER)) {
        Component *oscillator = oscillator_init(OSCILLATOR_SINE, 440.0f, 0.5);
        rack_mount(app->rack, oscillator, 0, 0);
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
        ClearBackground(RAYWHITE);
        if (audio_engine_is_playing()) {
            DrawText("Playing", 10, 10, 20, BLACK);
        } else {
            DrawText("Paused", 10, 10, 20, BLACK);
        }

        Vector2 position = {10, 50};
        Vector2 size = {window_width() - 20, 200};
        render_oscilloscope(app->sample_buffer, position, size);

    EndDrawing();
}
