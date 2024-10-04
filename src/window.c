#include "raylib.h"
#include "window.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void window_init(int width, int height) {
    window = malloc(sizeof(Window));
    assert(window != NULL);

    window->width = width;
    window->height = height;
    window->should_close = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    SetTargetFPS(FPS);
    InitWindow(width, height, "clef");
}

void window_free() {
    free(window);
    CloseWindow();
}

void window_update() {
    if (IsWindowResized()) {
        window->height = GetScreenHeight();
        window->width = GetScreenWidth();
    }
}

int window_width() { return window->width; }
int window_height() { return window->height; }

bool window_should_close() { return WindowShouldClose() || window->should_close; }
void window_close() { window->should_close = true; }
