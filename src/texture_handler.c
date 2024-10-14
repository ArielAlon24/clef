#include "texture_handler.h"
#include "raylib.h"
#include <stdio.h>

/* `id != 0` is used as it is the first attribute of the Texture struct
and the value `0` denotes cannot be given to a loaded texture.  */
#define IS_TEXTURE_LOADED(texture) (texture.id != 0)

const char *texture_kind_path(TextureKind kind) {
    switch (kind) {
        case TEXTURE_EMPTY_CELL:
            return TEXTURES_FOLDER "empty-cell.png";
        case TEXTURE_CURSOR:
            return TEXTURES_FOLDER "cursor.png";
        case TEXTURE_OSCILLOSCOPE:
            return TEXTURES_FOLDER "oscilloscope.png";
        case TEXTURE_OSCILLATOR:
            return TEXTURES_FOLDER "oscillator.png";
        default:
            return "";
    }
}

Texture2D texture_load(TextureKind kind) {
    Texture2D texture = textures[kind];

    if (IS_TEXTURE_LOADED(texture)) return texture;

    const char *path = texture_kind_path(kind);

    texture = LoadTexture(path);
    textures[kind] = texture;
    return texture;
}

void texture_unload(TextureKind kind) {
    Texture2D texture = textures[kind];
    if (!IS_TEXTURE_LOADED(texture)) return;

    UnloadTexture(texture);
    /* Remove from cache */
    textures[kind].id = 0;
}

void texture_unload_all() {
    for (TextureKind kind = 0; kind < _TEXTURES_AMOUNT; ++kind) {
        texture_unload(kind);
    }
}
