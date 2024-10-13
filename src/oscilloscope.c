#include <raylib.h>
#include "oscilloscope.h"
#include "sample_buffer.h"
#include "color.h"

void oscilloscope_render(SampleBuffer *sample_buffer, Vector2 position, Vector2 size) {
    DrawRectangleV(position, size, COLOR_BLACK);
    sample_buffer_lock(sample_buffer);
    float half_height = size.y / 2;

    size_t trigger_index = sample_buffer_find_left(sample_buffer, 0.0f);

    size_t buffer_size = sample_buffer_size(sample_buffer);
    float* buffer = sample_buffer_left(sample_buffer);

    Vector2 v0 = {position.x, position.y + half_height}, v1;
    for (int i = 0; i < size.x; ++i) {
        v1.x = position.x + i;
        v1.y = position.y + half_height + half_height * buffer[(trigger_index + i) % buffer_size];
        DrawLineEx(v0, v1, 3, COLOR_WHITE);
        v0 = v1;
    }

    trigger_index = sample_buffer_find_left(sample_buffer, 0.0f);
    buffer = sample_buffer_left(sample_buffer);

    v0.x = position.x;
    v0.y = position.y + half_height;
    for (int i = 0; i < size.x; ++i) {
        v1.x = position.x + i;
        v1.y = position.y + half_height + half_height * buffer[(trigger_index + i) % buffer_size];
        DrawLineEx(v0, v1, 3, COLOR_GRAY);
        v0 = v1;
    }

    DrawText("Oscilloscope", position.x + 10, position.y + 10, 20, COLOR_WHITE);

    sample_buffer_unlock(sample_buffer);
}
