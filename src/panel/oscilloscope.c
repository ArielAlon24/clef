#include <stdio.h>
#include "raylib.h"
#include "panels/oscilloscope.h"
#include "sample_buffer.h"
#include "constants/color.h"
#include "texture_handler.h"

void oscilloscope_render(SampleBuffer *sample_buffer, Vector2 position, Vector2 size) {
    Texture2D background = texture_load(TEXTURE_OSCILLOSCOPE);
    DrawTextureV(background, position, WHITE);
    sample_buffer_lock(sample_buffer);
    float half_height = size.y / 2;

    /* Draw left channel */
    size_t trigger_index = sample_buffer_find_left(sample_buffer, 0.0f);

    size_t buffer_size = sample_buffer_size(sample_buffer);
    float* buffer = sample_buffer_left(sample_buffer);

    /* Not overlapping the oscilloscope's border */
    float x0 = position.x + 1;

    Vector2 v0 = {x0, position.y + half_height}, v1;
    for (int i = 0; i < size.x - 1; ++i) {
        v1.x = x0 + i;
        v1.y = position.y + half_height + half_height * buffer[(trigger_index + i) % buffer_size];
        DrawLineEx(v0, v1, 1, COLOR_GRAY);
        v0 = v1;
    }

    /* Draw right channel */
    trigger_index = sample_buffer_find_left(sample_buffer, 0.0f);
    buffer = sample_buffer_left(sample_buffer);

    v0.x = x0;
    v0.y = position.y + half_height;
    for (int i = 0; i < size.x - 1; ++i) {
        v1.x = x0 + i;
        v1.y = position.y + half_height + half_height * buffer[(trigger_index + i) % buffer_size];
        DrawLineEx(v0, v1, 1, COLOR_WHITE);
        v0 = v1;
    }

    sample_buffer_unlock(sample_buffer);
}
