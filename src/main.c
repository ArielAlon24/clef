#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "audio_engine.h"

#define SAMPLE_RATE 44100      // 44.1 kHz sample rate
#define FREQUENCY 440.0        // Frequency of sine wave (A4)
#define AMPLITUDE 0.5          // Amplitude of the sine wave (volume)
#define TWO_PI 6.28318530718   // 2 * Pi

void generate_sine_wave(float* buffer, size_t frame_count, float frequency, float amplitude, float sample_rate) {
    static float phase = 0.0f;  // Phase will keep track of the sine wave phase across calls

    for (size_t i = 0; i < frame_count * 2; i += 2) {
        buffer[i] = amplitude * sinf(phase);
        buffer[i + 1] = buffer[i];
        phase += (TWO_PI * frequency) / sample_rate;

        if (phase > TWO_PI) {
            phase -= TWO_PI;
        }
    }
}

void callback(void* output, const void* input, unsigned int frame_count) {
    float* buffer = (float*)output;

    generate_sine_wave(buffer, frame_count, FREQUENCY, AMPLITUDE, SAMPLE_RATE);

    (void)input;
}

int main(void) {
    audio_engine_init(callback);

    audio_engine_play();

    printf("Playing sine wave... Press Enter to stop.\n");
    getchar();

    audio_engine_free();
    return 0;
}

