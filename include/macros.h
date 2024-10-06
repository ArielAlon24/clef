#ifndef MACROS_H
#define MACROS_H

#include <assert.h>

/* TODO: Move constants to a different place */
#define SAMPLE_RATE 44100
#define TWO_PI 6.283185307179586

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define NOT_IMPLEMENTED assert(0 && "Not Implemented");

#endif
