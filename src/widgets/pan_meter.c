#include "widgets/pan_meter.h"
#include "raylib.h"
#include "constants/color.h"
#include "macros.h"

void pan_meter_render(float pan, Vector2 position, Vector2 size) {
    Rectangle rectangle = {.x = position.x, .y = position.y, .height = size.y, .width = size.x};
    DrawRectangleLinesEx(rectangle, 1, COLOR_GRAY);
    Vector2 rec_position, rec_size;
    if (IS_BETWEEN(pan, -0.05f, 0.05f)) {
        rec_position.x = position.x + size.x / 2 - 1;
        rec_position.y = position.y + 1;
        rec_size.x = (int)rectangle.width % 2 == 0 ? 2 : 1;
        rec_size.y = size.y - 2;
    } else if (pan > 0.0f) {
        rec_position.x = position.x + size.x / 2;
        rec_position.y = position.y + 1;
        rec_size.x = pan * (rectangle.width - 2) / 2;
        rec_size.y = size.y - 2;
    } else {
        rec_position.x = position.x + size.x / 2 + pan * (rectangle.width - 2) / 2;
        rec_position.y = position.y + 1;
        rec_size.x = -pan * (rectangle.width - 2) / 2;
        rec_size.y = size.y - 2;
    }
    DrawRectangleV(rec_position, rec_size, COLOR_WHITE);
}
