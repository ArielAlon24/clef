#include "panels/settings_panel.h"
#include "constants/size.h"
#include "texture_handler.h"
#include "app.h"

void settings_panel_render(App *application, Vector2 position, Vector2 size) {
    Texture2D settings_panel = texture_load(TEXTURE_SETTINGS_PANEL);
    DrawTextureV(settings_panel, position, WHITE);
    Component *component = component_current(application->current_rack);
    if (component) {
        component_settings_render(component, position, size);
    }
}
