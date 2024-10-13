#include <stdlib.h>
#include <assert.h>
#include "pixel_renderer.h"
#include "raylib.h"
#include "window.h"


PixelRenderer *pixel_renderer_init(int width, int height) {
    PixelRenderer *renderer = malloc(sizeof(PixelRenderer));
    assert(renderer != NULL);

    renderer->render_texture = LoadRenderTexture(width, height);
    renderer->width = width;
    renderer->height = height;

    return renderer;
}

void pixel_renderer_begin(PixelRenderer *renderer) {
    BeginTextureMode(renderer->render_texture);
}

void pixel_renderer_end(PixelRenderer *renderer) {
    EndTextureMode();

    Rectangle source = {0, 0, renderer->width, -renderer->height};
    float scale = window_width() / source.width;
    Rectangle dest = {0, 0, window_width(), (int)source.height * scale};
    Vector2 origin = {0, 0};

    BeginDrawing();
        DrawTexturePro(renderer->render_texture.texture, source, dest, origin, 0, WHITE);
    EndDrawing();
}

void pixel_renderer_free(PixelRenderer *renderer) {
    UnloadTexture(renderer->render_texture.texture);
    free(renderer);
}
