#ifndef SIZE_H
#define SIZE_H

#include "raylib.h"

/* WIDTH and HEIGHT are the sizes of the internal pixel renderer */
#define WIDTH 528
#define HEIGHT 528

/* WINDOW_[WIDTH | HEIGHT] are the dimensions of the GUI window */
#define WINDOW_WIDTH (1.5 * WIDTH)
#define WINDOW_HEIGHT (1.5 * HEIGHT)
#define WINDOW_DIMENSIONS (Vector2) { WINDOW_WIDTH, WINDOW_HEIGHT }

/* The (minimum) distance between the window borders and actual GUI */
#define BORDER_PADDING (WIDTH / 12)

/* Each GUI component has dimensions of n * CUBIC_SIZE by m * CUBIC_SIZE */
#define CUBIC (WIDTH / 24)

/* The entire screen is splitted into 12 by 12 cubes which also
represent the size of a single component */
#define COMPONENT_WIDTH (WIDTH / 12)
#define COMPONENT_HEIGHT (HEIGHT / 12)
#define COMPONENT_DIMENSIONS (Vector2) { COMPONENT_WIDTH, COMPONENT_HEIGHT }

/* The distance between components (same for horizontal and vertical axis) */
#define COMPONENT_PADDING (COMPONENT_WIDTH / 2)

#define OSCILLOSCOPE_POSITION (Vector2) { CUBIC * 15, CUBIC * 2 }
#define OSCILLOSCOPE_DIMENSIONS (Vector2) { CUBIC * 7, CUBIC * 2 }

/* Number of elements in each rack row / column */
#define RACK_SIZE 5
#define RACK_POSITION (Vector2) { CUBIC * 2, CUBIC * 10 }
#define RACK_DIMENSIONS (Vector2) { COMPONENT_WIDTH * (RACK_SIZE + 1), COMPONENT_WIDTH * (RACK_SIZE + 1)}

#define SETTINGS_PANEL_POSITION (Vector2){ BORDER_PADDING, BORDER_PADDING + 3 * CUBIC }
#define SETTINGS_PANEL_DIMENSIONS (Vector2){ CUBIC * 12, CUBIC * 4 }

#define DB_METER_POSITION (Vector2){ BORDER_PADDING + 18 * CUBIC, BORDER_PADDING + 3 * CUBIC}
#define DB_METER_DIMENSIONS (Vector2){ CUBIC * 2, CUBIC * 4 }

#define PLAY_PAUSE_PANEL_POSITION (Vector2){ BORDER_PADDING, BORDER_PADDING }
#define PLAY_PAUSE_PANEL_DIMENSIONS (Vector2){ CUBIC * 12, CUBIC * 2 }

#define COMPONENT_PREVIEW_PANEL_POSITION (Vector2){ BORDER_PADDING + 13 * CUBIC, BORDER_PADDING + 3 * CUBIC }
#define COMPONENT_PREVIEW_PANEL_DIMENSIONS (Vector2){ CUBIC * 4, CUBIC * 4 }

#define COMPONENT_PREVIEW_COMPONENT_POSITION (Vector2){ BORDER_PADDING + 14 * CUBIC, BORDER_PADDING + 4 * CUBIC }

#endif
