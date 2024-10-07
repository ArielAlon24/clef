#include "oscillator.h"
#include "sample_buffer.h"
#include <raylib.h>

void render_oscilloscope(SampleBuffer *sample_buffer, Vector2 position, Vector2 size) {
    DrawRectangleV(position, size, LIGHTGRAY);
    sample_buffer_lock(sample_buffer);
    float quarter_height = size.y / 4;

    DrawLine(position.x, position.y + quarter_height * 2, position.x + size.x, position.y + quarter_height * 2, RAYWHITE);

    size_t trigger_index = sample_buffer_find_left(sample_buffer, 0.0f);
    size_t buffer_size = sample_buffer_size(sample_buffer);
    float* buffer = sample_buffer_left(sample_buffer);

    Vector2 v0 = {position.x, position.y + quarter_height}, v1;
    for (int i = 0; i < size.x; ++i) {
        v1.x = position.x + i;
        v1.y = position.y + quarter_height + quarter_height * buffer[(trigger_index + i) % buffer_size];
        DrawLineEx(v0, v1, 3, BLACK);
        v0 = v1;
    }

    trigger_index = sample_buffer_find_left(sample_buffer, 0.0f);
    buffer = sample_buffer_left(sample_buffer);

    v0.x = position.x;
    v0.y = position.y + 3 * quarter_height;
    for (int i = 0; i < size.x; ++i) {
        v1.x = position.x + i;
        v1.y = position.y + 3 * quarter_height + quarter_height * buffer[(trigger_index + i) % buffer_size];
        DrawLineEx(v0, v1, 3, BLACK);
        v0 = v1;
    }

    DrawText("Left", position.x + 10, position.y + 10, 20, WHITE);
    DrawText("Right", position.x + 10, position.y + 2 * quarter_height + 10, 20, WHITE);

    sample_buffer_unlock(sample_buffer);
}
