#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdbool.h>

typedef enum {
    COMPONENT_OSCILLATOR,
    COMPONENT_RACK,
    _COMPONENT_TYPE_SIZE,
} ComponentType;

typedef struct {
    ComponentType type;
    bool is_container;
} Component;

#endif
