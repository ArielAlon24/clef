#ifndef COMPONENT_PREVIEW_PANEL_H
#define COMPONENT_PREVIEW_PANEL_H

#include "app.h"
#include "raylib.h"
#include "component_system/component_system.h"

void component_preview_panel_render(App *application, Vector2 position, Vector2 size, Vector2 component_position, Vector2 component_size);

#endif
