#include "font_handler.h"
#include "raylib.h"

#define IS_FONT_LOADED(f) (f.baseSize != 0)

Font font_get() {
    return IS_FONT_LOADED(font) ? font : font_load();
}

Font font_load() {
    font = LoadFont(FONT_FILE_PATH);
    return font;
}

void font_unload() { UnloadFont(font); }

void font_write_s(const char *text, Vector2 position, Color color) {
    DrawTextEx(font_get(), text, position, FONT_HEIGHT_S, 1, color);
}

void font_write_m(const char *text, Vector2 position, Color color) {
    DrawTextEx(font_get(), text, position, FONT_HEIGHT_M, 1, color);
}
void font_write_l(const char *text, Vector2 position, Color color) {
    DrawTextEx(font_get(), text, position, FONT_HEIGHT_L, 1, color);
}
