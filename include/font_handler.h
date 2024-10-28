#ifndef FONT_HANDLER_H
#define FONT_HANDLER_H

#include "raylib.h"
#include <unistd.h>

#define FONT_FILE_PATH "./resources/fonts/clef-font.png"
#define FONT_WIDTH_S  5
#define FONT_HEIGHT_S 9
#define FONT_WIDTH_M  (FONT_WIDTH_S  * 2)
#define FONT_HEIGHT_M (FONT_HEIGHT_S * 2)
#define FONT_WIDTH_L  (FONT_WIDTH_S *  3)
#define FONT_HEIGHT_L (FONT_HEIGHT_S * 3)

static Font font;

Font font_get();

Font font_load();
void font_unload();


/* Write text with `font` in different sizes:
s - Small  (5  * 9  pixels)
m - Medium (10 * 18 pixels)
l - Large  (15 * 27 pixels)
*/
void font_write_s(const char *text, Vector2 position, Color color);
void font_write_m(const char *text, Vector2 position, Color color);
void font_write_l(const char *text, Vector2 position, Color color);
#endif
