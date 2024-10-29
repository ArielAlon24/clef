#ifndef MACROS_H
#define MACROS_H

#include <assert.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define CLAMP(value, min, max) ((value) < (min) ? (min) : (value) > (max) ? (max) : (value))

#define IS_BETWEEN(value, min, max) (value > min && value < max)

#define NOT_IMPLEMENTED assert(0 && "Not Implemented");
#define UNREACHABLE     assert(0 && "Unreachable");

#define LEN(a) (sizeof(a) / sizeof(a[0]))

/* Macros to compute the scale (0 to 1) of the amplitude for each channel based on the `pan` value*/
#define PAN_R_SCALE(pan) MIN(1.0f, (pan + 1.0f));
#define PAN_L_SCALE(pan) MIN(1.0f, 2.0f - (pan + 1.0f))

#endif
