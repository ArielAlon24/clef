#ifndef COMPONENT_HANDLER_H
#define COMPONENT_HANDLER_H

#include "rack/component.h"

typedef Component *(*ComponentFactory)(void);
typedef void (*ComponentPreviewCallback)(Vector2 position);

typedef struct {
    ComponentFactory factory;
    ComponentPreviewCallback preview_callback;
} LibraryComponent;

LibraryComponent library_component_table[_COMPONENT_TYPE_SIZE];

void component_handler_init();

Component *component_create(ComponentType type);
void component_preview(ComponentType type, Vector2 position);

#endif