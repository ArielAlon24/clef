#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include "raylib.h"

#define TEXTURES_FOLDER "resources/textures/"

typedef enum {
    TEXTURE_EMPTY_CELL,
    TEXTURE_CURSOR,
    TEXTURE_OSCILLOSCOPE,
    TEXTURE_OSCILLATOR,
    TEXTURE_RACK_PREVIEW,
    TEXTURE_ENVELOPE,
    _TEXTURES_AMOUNT,
} TextureKind;

static Texture2D textures[_TEXTURES_AMOUNT];

const char *texture_kind_path(TextureKind kind);

Texture2D texture_load(TextureKind kind);

void texture_unload(TextureKind kind);
void texture_unload_all();

#endif
