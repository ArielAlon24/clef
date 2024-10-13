#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "pixel_renderer.h"
#include "raylib.h"
#include "window.h"
#include "macros.h"
#include "color.h"

#include <stdio.h>

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
    Rectangle dest = {0};
    float scale;

    int width = window_width();
    int height = window_height();

    if (width < height) {
        scale = width / renderer->width;
        dest.width = width;
        dest.height = renderer->height * scale;
    } else {
        scale = height / renderer->height;
        dest.width = renderer->width * scale;
        dest.height = height;
    }

    Vector2 origin = {-MAX(fabs(width - dest.width) / 2, 0), -MAX(fabs(height - dest.height) / 2, 0)};

    Rectangle source = {0, 0, renderer->width, -renderer->height};
    BeginDrawing();
        ClearBackground(COLOR_BLACK);
        DrawTexturePro(renderer->render_texture.texture, source, dest, origin, 0, WHITE);
    EndDrawing();
}

void pixel_renderer_free(PixelRenderer *renderer) {
    UnloadTexture(renderer->render_texture.texture);
    free(renderer);
}
