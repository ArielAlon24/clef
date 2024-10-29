#include "panels/play_pause_panel.h"
#include "raylib.h"
#include "app.h"
#include "texture_handler.h"
#include "constants/size.h"
#include "audio_engine.h"
#include "font_handler.h"
#include "stdio.h"
#include "constants/color.h"

void play_pause_panel_render(App *application, Vector2 position, Vector2 size) {
    Texture2D play_pause_panel = texture_load(TEXTURE_PLAY_PAUSE_PANEL);
    DrawTextureV(play_pause_panel, position, WHITE);

    /* TODO: Recreate the play / pause texture */
    if (audio_engine_is_playing()) {
        Texture2D play = texture_load(TEXTURE_PLAY);
        DrawTextureV(play, position, WHITE);
    } else {
        Texture2D pause = texture_load(TEXTURE_PAUSE);
        DrawTextureV(pause, position, WHITE);
    }

    char position_text[25];
    Vector2 position_text_position = {position.x + size.x - FONT_WIDTH_S * 4 - 1, position.y + 4 };
    Rack *current_rack = (Rack *)application->current_rack;
    sprintf(position_text, "%d,%d", (int)current_rack->cursor.x + 1, (int)current_rack->cursor.y + 1);
    font_write_s(position_text, position_text_position, COLOR_GRAY);
}
