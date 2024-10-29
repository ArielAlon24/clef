#include "panels/component_preview_panel.h"
#include "app.h"
#include "raylib.h"
#include "constants/size.h"
#include "constants/color.h"
#include "component_system/component_system.h"
#include "component_system/component.h"
#include "stdio.h"
#include "texture_handler.h"

void component_preview_panel_render(App *application, Vector2 position, Vector2 size, Vector2 component_position, Vector2 component_size) {
    Texture2D component_preview_texture = texture_load(TEXTURE_COMPONENT_PREVIEW);
    DrawTextureV(component_preview_texture, position, WHITE);
    component_preview(application->component_selector, component_position, component_size);
}
