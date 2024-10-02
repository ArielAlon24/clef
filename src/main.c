#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "audio_engine.h"

#define SAMPLE_RATE 44100      // 44.1 kHz sample rate
#define FREQUENCY 440.0        // Frequency of sine wave (A4)
#define AMPLITUDE 0.2          // Amplitude of the sine wave (volume)
#define TWO_PI 6.28318530718   // 2 * Pi

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

int main(void) {
    audio_engine_init(callback);
    audio_engine_play();

    printf("Playing sine wave... Press Enter to stop.\n");
    getchar();
    audio_engine_pause();

    printf("Stopped sine wave... Press Enter to play.\n");
    getchar();
    audio_engine_play();

    printf("Playing sine wave... Press Enter to stop.\n");
    getchar();

    audio_engine_pause();

    audio_engine_free();
    return 0;
}

