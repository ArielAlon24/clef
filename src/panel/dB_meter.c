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

/* TODO: Make this work in dB (currently just percentage) */
void dB_meter_render(SampleBuffer *sample_buffer, Vector2 position, Vector2 size) {
    Texture2D dB_meter_texture = texture_load(TEXTURE_DB_METER);
    DrawTextureV(dB_meter_texture, position, WHITE);

    float right, left;

    sample_buffer_lock(sample_buffer);
    right = sample_buffer_max_right(sample_buffer, SEARCH_RANGE);
    left  = sample_buffer_max_left(sample_buffer, SEARCH_RANGE);
    sample_buffer_unlock(sample_buffer);

    /* To prevent stutter every value 0.05f shy of full is converted into full height */
    int max_height = size.y - 2;
    int left_height =  MIN(left * max_height + 0.05f, max_height);
    int right_height = MIN(right * max_height + 0.05f, max_height);

    Vector2 left_pos = {position.x + 1, position.y + size.y - left_height - 1};
    Vector2 right_pos = {position.x + size.x / 2, position.y + size.y - right_height - 1};
    Vector2 left_dim = {size.x / 2 - 1, left_height};
    Vector2 right_dim = {left_dim.x, right_height};

    DrawRectangleV(left_pos, left_dim, COLOR_WHITE);
    DrawRectangleV(right_pos, right_dim, COLOR_WHITE);
}
