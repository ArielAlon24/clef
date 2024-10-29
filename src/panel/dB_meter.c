#include <stddef.h>
#include <stdio.h>
#include "panels/dB_meter.h"
#include "raylib.h"
#include "sample_buffer.h"
#include "math.h"
#include "macros.h"
#include "constants/color.h"
#include "texture_handler.h"

#define MIN_dB -100.0f
#define MAX_dB 0.0f

/* range used for searching the max value in the sample buffers */
#define SEARCH_RANGE 1000

void dB_meter_render(SampleBuffer *sample_buffer, Vector2 position, Vector2 size) {
    Texture2D dB_meter_texture = texture_load(TEXTURE_DB_METER);
    DrawTextureV(dB_meter_texture, position, WHITE);

    float right, left;

    sample_buffer_lock(sample_buffer);
    right = sample_buffer_max_right(sample_buffer, SEARCH_RANGE);
    left  = sample_buffer_max_left(sample_buffer, SEARCH_RANGE);
    sample_buffer_unlock(sample_buffer);

    // float right_dB = 20 * log10f(fmaxf(fabs(right), 1e-10));
    // float left_dB  = 20 * log10f(fmaxf(fabs(left), 1e-10));

    // /* TODO: Make this correct */
    // float right_height = size.y * (right_dB - MIN_dB) / (MAX_dB - MIN_dB);
    // float left_height  = size.y * (left_dB  - MIN_dB) / (MAX_dB - MIN_dB);

    float left_height = left * size.y;
    float right_height = right * size.y;
    /* Draw left channel */
    DrawRectangle(position.x + 1, position.y + size.y - left_height, size.x / 2 - 1, left_height, COLOR_WHITE);

    /* Draw right channel */
    DrawRectangle(position.x + size.x / 2, position.y + size.y - right_height, size.x / 2 - 1, right_height, COLOR_WHITE);
}
