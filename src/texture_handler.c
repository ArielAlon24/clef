#include "texture_handler.h"
#include "raylib.h"
#include <stdio.h>

/* `id != 0` is used as it is the first attribute of the Texture struct
and the value `0` denotes cannot be given to a loaded texture.  */
#define IS_TEXTURE_LOADED(texture) (texture.id != 0)

const char *texture_kind_path(TextureKind kind) {
    switch (kind) {
        case TEXTURE_CELL:
            return TEXTURES_FOLDER "cell.png";
        case TEXTURE_CURSOR:
            return TEXTURES_FOLDER "cursor.png";
        case TEXTURE_OSCILLOSCOPE:
            return TEXTURES_FOLDER "oscilloscope.png";
        case TEXTURE_OSCILLATOR:
            return TEXTURES_FOLDER "oscillator.png";
        case TEXTURE_RACK_PREVIEW:
            return TEXTURES_FOLDER "rack-preview.png";
        case TEXTURE_ENVELOPE:
            return TEXTURES_FOLDER "envelope.png";
        case TEXTURE_SETTINGS_PANEL:
            return TEXTURES_FOLDER "settings-panel.png";
        case TEXTURE_PLAY_PAUSE_PANEL:
            return TEXTURES_FOLDER "play-pause-panel.png";
        case TEXTURE_PLAY:
            return TEXTURES_FOLDER "play.png";
        case TEXTURE_PAUSE:
            return TEXTURES_FOLDER "pause.png";
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
