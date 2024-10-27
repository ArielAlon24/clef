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
