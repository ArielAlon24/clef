#ifndef OSCILLOSCOPE
#define OSCILLOSCOPE

#include "raylib.h"
#include "sample_buffer.h"

/* Draw an oscilloscope between the top right corner `start` and the bottom left corner `end` */
void render_oscilloscope(SampleBuffer *sample_buffer, Vector2 start, Vector2 end);

#endif
