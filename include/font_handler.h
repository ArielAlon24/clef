#ifndef FONT_HANDLER_H
#define FONT_HANDLER_H

#include "raylib.h"
#include <unistd.h>

#define FONT_FILE_PATH "./resources/fonts/clef-font.png"

static Font font;

Font font_get();

Font font_load();
void font_unload();

#endif
