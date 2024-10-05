#ifndef WINDOW_H
#define WINDOW_H

#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#define FPS 60

typedef struct {
    int width;
    int height;
    bool should_close;
} Window;

static Window *window = NULL;

void window_init(int height, int width);
void window_free();

int window_width();
int window_height();

void window_close();
bool window_should_close();

void window_update();

#endif
