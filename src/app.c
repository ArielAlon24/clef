#include "app.h"
#include "window.h"
#include "audio_engine.h"
#include "midi.h"
#include "oscillator.h"
#include <raylib.h>
#include <stdlib.h>
#include <assert.h>
#include "macros.h"
#include "oscilloscope.h"
#include <string.h>

void callback(float *buffer, unsigned int frame_count) {
    MidiMessage message;
    while (midi_stream_read(app->midi_stream, &message)) {
        switch (message.type) {
            case MIDI_MESSAGE_NOTE_ON:
                app->oscillator->frequency = note_number_to_frequency(message.data.two[0]);
                app->oscillator->amplitude = (float)message.data.two[1] / 100;
                break;
            default:
                NOT_IMPLEMENTED
        }
    }

    oscillator_next(app->oscillator, buffer, frame_count);
}

void analyzer(const float *buffer, unsigned int frame_count) {
    sample_buffer_push(app->sample_buffer, buffer, frame_count);
}

void app_init() {
    app = malloc(sizeof(App));
    assert(app != NULL);

    app->sample_buffer = sample_buffer_init(SAMPLE_RATE / 10);
    app->oscillator = oscillator_init(OSCILLATOR_SINE, 440.0, 0.5);

    app->midi_stream = midi_stream_init();

    audio_engine_init(callback, analyzer);
    window_init(500, 500);
}

void app_free() {
    window_free();
    audio_engine_free();

    sample_buffer_free(app->sample_buffer);
    oscillator_free(app->oscillator);
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
            char text[20];
            sprintf(text, "Playing (%.1f Hz)", app->oscillator->frequency);
            DrawText(text, 10, 10, 20, BLACK);
        } else {
            DrawText("Paused", 10, 10, 20, BLACK);
        }

        Vector2 position = {10, 50};
        Vector2 size = {window_width() - 20, 200};
        render_oscilloscope(app->sample_buffer, position, size);

    EndDrawing();
}
