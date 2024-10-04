#include "app.h"
#include "window.h"
#include "audio_engine.h"
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define SAMPLE_RATE 44100
float frequency = 600.0f;
#define AMPLITUDE 0.5
#define TWO_PI 6.28318530718

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void generate_weird_wave(float* buffer, size_t frame_count, float frequency, float amplitude, float sample_rate) {
    static float phase = 0.0f;
    if (frequency == 0.0) {
        amplitude = 0;
    }
    for (size_t i = 0; i < frame_count * 2; i += 2) {
        buffer[i] = (sinf(phase) >= 0.0f) ? amplitude : -amplitude; /* Square wave in the right channel */
        buffer[i + 1] = amplitude * sinf(phase);                    /* Sine wave in the left channel */
        phase += (TWO_PI * frequency) / sample_rate;

        if (phase > TWO_PI) { phase -= TWO_PI; }
    }
}

void callback(float *buffer, unsigned int frame_count) {
    generate_weird_wave(buffer, frame_count, frequency, AMPLITUDE, SAMPLE_RATE);
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
    while(!window_should_close()) {
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
    int mouse_x = GetMouseX();
    frequency = MAX((float)mouse_x / (float)GetScreenWidth() * 5000.0, 0);

    if (IsKeyDown(KEY_UP)) frequency = MIN(5000.0, frequency + 50.0);
    if (IsKeyDown(KEY_DOWN)) frequency = MAX(0.0, frequency - 50.0);
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

        DrawRectangle(10, 50 - 1, window_width() - 20, 200 + 2, GRAY);
        DrawRectangle(10, 250 - 1, window_width() - 20, 200 + 2, GRAY);

        pthread_mutex_lock(&app->samples_lock);

        /* Drawing left channel samples */
        size_t trigger_index = ring_buffer_find(app->left_samples, 0.0, 0.1);
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
        trigger_index = ring_buffer_find(app->right_samples, 0.0, 0.1);
        buffer_size = app->right_samples->size;
        width = window_width() - 20;
        buffer = app->right_samples->buffer;

        v0.x = 10;
        v0.y = 350;
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
