#include "dB_meter.h"
#include "raylib.h"
#include "sample_buffer.h"
#include "math.h"
#include "macros.h"
#include "color.h"
#include <stddef.h>
#include <stdio.h>

#define MIN_dB -100.0f
#define MAX_dB 0.0f

/* range used for searching the max value in the sample buffers */
#define SEARCH_RANGE 100

void dB_meter_render(SampleBuffer *sample_buffer, Vector2 position, Vector2 size) {
    DrawRectangleV(position, size, COLOR_DARK_GRAY);

    float right, left;

    sample_buffer_lock(sample_buffer);
    right = sample_buffer_max_right(sample_buffer, SEARCH_RANGE);
    left  = sample_buffer_max_left(sample_buffer, SEARCH_RANGE);
    sample_buffer_unlock(sample_buffer);

    float right_dB = 20 * log10f(fmaxf(fabs(right), 1e-10));
    float left_dB  = 20 * log10f(fmaxf(fabs(left), 1e-10));

    float right_height = size.y * (right_dB - MIN_dB) / (MAX_dB - MIN_dB);
    float left_height  = size.y * (left_dB  - MIN_dB) / (MAX_dB - MIN_dB);

    /* Draw left channel */
    DrawRectangle(position.x, position.y + size.y - left_height, size.x / 2, left_height, COLOR_GRAY);

    /* Draw right channel */
    DrawRectangle(position.x + size.x / 2, position.y + size.y - right_height, size.x / 2, right_height, COLOR_WHITE);
}
