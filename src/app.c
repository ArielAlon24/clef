#include "app.h"
#include "window.h"
#include "audio_engine.h"
#include "midi.h"
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define SAMPLE_RATE 44100
#define TWO_PI 6.28318530718

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static float frequency = 600.0f;
static float amplitude = 0.2f;
void generate_weird_wave(float* buffer, size_t frame_count, float frequency, float amplitude, float sample_rate) {
    static float phase = 0.0f;
    if (frequency == 0.0) {
        amplitude = 0;
    }
    for (size_t i = 0; i < frame_count * 2; i += 2) {
        // buffer[i] = (sinf(phase) >= 0.0f) ? amplitude : -amplitude;
        // buffer[i + 1] = (sinf(phase) >= 0.0f) ? amplitude : -amplitude;
        buffer[i] = amplitude * sinf(phase);
        buffer[i + 1] = amplitude * sinf(phase);
        phase += (TWO_PI * frequency) / sample_rate;

        if (phase > TWO_PI) { phase -= TWO_PI; }
    }
}

void callback(float *buffer, unsigned int frame_count) {
    MidiMessage message;
    while (midi_stream_read(app->midi_stream, &message)) {
        switch (message.type) {
            case MIDI_MESSAGE_NOTE_ON:
                frequency = note_number_to_frequency(message.data.two[0]);
                amplitude = (float)message.data.two[1] / 100;
                break;
            default:
                printf("Not implemented\n");
                break;
        }
    }

    generate_weird_wave(buffer, frame_count, frequency, amplitude, SAMPLE_RATE);
}

void analyzer(const float *buffer, unsigned int frame_count) {
    pthread_mutex_lock(&app->samples_lock);
    for (int i = 0; i < frame_count * 2; i += 2) {
        ring_buffer_push(app->right_samples, buffer[i]);
        ring_buffer_push(app->left_samples, buffer[i + 1]);
    }
    pthread_mutex_unlock(&app->samples_lock);
}

void app_init() {
    app = malloc(sizeof(App));
    assert(app != NULL);

    app->right_samples = ring_buffer_init(SAMPLE_RATE / 10);
    app->left_samples = ring_buffer_init(SAMPLE_RATE / 10);
    pthread_mutex_init(&app->samples_lock, NULL);

    app->midi_stream = midi_stream_init();

    audio_engine_init(callback, analyzer);
    window_init(500, 500);
}

void app_free() {
    window_free();
    audio_engine_free();

    ring_buffer_free(app->right_samples);
    ring_buffer_free(app->left_samples);

    pthread_mutex_destroy(&app->samples_lock);

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
            sprintf(text, "Playing (%.1f Hz)", frequency);
            DrawText(text, 10, 10, 20, BLACK);
        } else {
            DrawText("Paused", 10, 10, 20, BLACK);
        }

        DrawRectangle(10, 50 - 1, window_width() - 20, 200 + 2, LIGHTGRAY);
        DrawRectangle(10, 250 - 1, window_width() - 20, 200 + 2, LIGHTGRAY);

        pthread_mutex_lock(&app->samples_lock);

        /* Drawing left channel samples */
        size_t trigger_index = ring_buffer_find(app->left_samples, 0.0);
        size_t buffer_size = app->left_samples->size;
        int width = window_width() - 20;
        float* buffer = app->left_samples->buffer;

        Vector2 v0 = {10, 150}, v1;
        for (int i = 0; i < width; ++i) {
            v1.x = 10 + i;
            v1.y = 150 + 100 * buffer[(trigger_index + i) % buffer_size];
            DrawLineEx(v0, v1, 2, BLACK);
            v0 = v1;
        }

        /* Drawing right channel samples */
        trigger_index = ring_buffer_find(app->right_samples, 0.0);
        buffer_size = app->right_samples->size;
        width = window_width() - 20;
        buffer = app->right_samples->buffer;

        v0.y = buffer[trigger_index] * 100 + 350;
        v0.x = 10;

        for (int i = 0; i < width; ++i) {
            v1.x = 10 + i;
            v1.y = 350 + 100 * buffer[(trigger_index + i) % buffer_size];
            DrawLineEx(v0, v1, 2, BLACK);
            v0 = v1;
        }

        pthread_mutex_unlock(&app->samples_lock);

        DrawText("Left", 10 + 10, 50 - 1 + 10, 20, WHITE);
        DrawText("Right", 10 + 10, 250 - 1 + 10, 20, WHITE);
    EndDrawing();
}
