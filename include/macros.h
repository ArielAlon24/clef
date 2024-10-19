#ifndef MACROS_H
#define MACROS_H

#include <assert.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define CLAMP(value, min, max) ((value) < (min) ? (min) : (value) > (max) ? (max) : (value))

#define NOT_IMPLEMENTED assert(0 && "Not Implemented");

#endif
