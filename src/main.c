#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

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

void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count) {
    float* buffer = (float*)output;

    generate_sine_wave(buffer, frame_count, FREQUENCY, AMPLITUDE, SAMPLE_RATE);

    (void)input;  // Unused input buffer
}

int main(void) {
    ma_result result;
    ma_device_config deviceConfig;
    ma_device device;

    // Configure the playback device
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = ma_format_f32;      // 32-bit floating point
    deviceConfig.playback.channels = 2;                // Mono audio
    deviceConfig.sampleRate = SAMPLE_RATE;             // 44.1 kHz sample rate
    deviceConfig.dataCallback = data_callback;         // Callback function to provide audio data

    // Initialize the playback device
    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize playback device.\n");
        return -1;
    }

    // Start playback
    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        return -1;
    }

    printf("Playing sine wave... Press Enter to stop.\n");
    getchar();  // Wait for user input

    // Stop playback and clean up
    ma_device_uninit(&device);
    return 0;
}

