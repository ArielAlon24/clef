#ifndef SIZE_H
#define SIZE_H

#include "raylib.h"

/* WIDTH and HEIGHT are the sizes of the internal pixel renderer */
#define WIDTH 480
#define HEIGHT 480

/* WINDOW_[WIDTH | HEIGHT] are the dimensions of the GUI window */
#define WINDOW_WIDTH (1.5 * WIDTH)
#define WINDOW_HEIGHT (1.5 * HEIGHT)
#define WINDOW_DIMENSIONS                                                                          \
    (Vector2) { WINDOW_WIDTH, WINDOW_HEIGHT }

/* The (minimum) distance between the window borders and actual GUI */
#define WINDOW_PADDING (WIDTH / 12)

/* The entire screen is splitted into 12 by 12 cubes which also
represent the size of a single component */
#define COMPONENT_WIDTH (WIDTH / 12)
#define COMPONENT_HEIGHT (HEIGHT / 12)
#define COMPONENT_DIMENSIONS                                                                       \
    (Vector2) { COMPONENT_WIDTH, COMPONENT_HEIGHT }

/* The distance between components (same for horizontal and vertical axis)*/
#define COMPONENT_PADDING (COMPONENT_WIDTH / 2)

#define RACK_POSITION                                                                              \
    (Vector2) { WINDOW_PADDING, WINDOW_PADDING * 5 }

#define OSCILLOSCOPE_POSITION                                                                      \
    (Vector2) { WINDOW_PADDING * 7.5, WINDOW_PADDING * 1 }

#endif
