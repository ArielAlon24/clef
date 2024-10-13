#ifndef PIXEL_RENDERER_H
#define PIXEL_RENDERER_H

#include "raylib.h"

typedef struct {
    RenderTexture2D render_texture;
    int width;
    int height;
} PixelRenderer;

PixelRenderer *pixel_renderer_init(int width, int height);

void pixel_renderer_begin(PixelRenderer *renderer);
void pixel_renderer_end(PixelRenderer *renderer);

void pixel_renderer_free(PixelRenderer *renderer);

#endif
