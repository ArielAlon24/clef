#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <unistd.h>
#include "raylib.h"

#define TEXTURES_FOLDER "/resources/textures/"

/* TODO: This becoming a bit hard to maintain, maybe a directory based loader? */
/* TODO: Currently breaking when executing now from the build directory */

typedef enum {
    TEXTURE_CELL,
    TEXTURE_CURSOR,
    TEXTURE_OSCILLOSCOPE,
    TEXTURE_OSCILLATOR,
    TEXTURE_RACK_PREVIEW,
    TEXTURE_ENVELOPE,
    TEXTURE_SETTINGS_PANEL,
    TEXTURE_PLAY_PAUSE_PANEL,
    TEXTURE_PLAY,
    TEXTURE_PAUSE,
    _TEXTURES_AMOUNT,
} TextureKind;

static Texture2D textures[_TEXTURES_AMOUNT];

const char *texture_kind_path(TextureKind kind);

Texture2D texture_load(TextureKind kind);

void texture_unload(TextureKind kind);
void texture_unload_all();

#endif
