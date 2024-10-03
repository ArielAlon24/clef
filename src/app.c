#include "app.h"
#include "window.h"
#include "audio_engine.h"
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define SAMPLE_RATE 44100
#define FREQUENCY 110.0
#define AMPLITUDE 0.2
#define TWO_PI 6.28318530718

void generate_sine_wave(float* buffer, size_t frame_count, float frequency, float amplitude, float sample_rate) {
    static float phase = 0.0f;

    for (size_t i = 0; i < frame_count * 2; i += 2) {
        buffer[i] = amplitude * sinf(phase);
        buffer[i + 1] = buffer[i];
        phase += (TWO_PI * frequency) / sample_rate;

        if (phase > TWO_PI) {
            phase -= TWO_PI;
        }
    }
}

void callback(float *buffer, unsigned int frame_count) {
    generate_sine_wave(buffer, frame_count, FREQUENCY, AMPLITUDE, SAMPLE_RATE);
}

void analyzer(const float *buffer, unsigned int frame_count) {
    ring_buffer_insert(app->samples, buffer, frame_count);
}

void app_init() {
    app = malloc(sizeof(App));
    assert(app != NULL);

    app->samples = ring_buffer_init(400);

    audio_engine_init(callback, analyzer);
    window_init(500, 500);
}

void app_free() {
    window_free();
    audio_engine_free();

    ring_buffer_free(app->samples);
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
}

void app_render() {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        if (audio_engine_is_playing()) {
            DrawText("Playing", 10, 10, 20, BLACK);
        } else {
            DrawText("Paused", 10, 10, 20, BLACK);
        }

        for (int i = 0; i < app->samples->size; ++i) {
            DrawCircle(10 + i, 100 + 200 * app->samples->buffer[i], 4, BLACK);
        }

    EndDrawing();
}
