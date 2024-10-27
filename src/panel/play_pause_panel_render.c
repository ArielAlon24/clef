#include "panels/play_pause_panel.h"
#include "raylib.h"
#include "app.h"
#include "texture_handler.h"
#include "constants/size.h"
#include "audio_engine.h"

void play_pause_panel_render(App *application, Vector2 position, Vector2 size) {
    Texture2D play_pause_panel = texture_load(TEXTURE_PLAY_PAUSE_PANEL);
    DrawTextureV(play_pause_panel, position, WHITE);
    if (audio_engine_is_playing()) {
        Texture2D play = texture_load(TEXTURE_PLAY);
        DrawTextureV(play, position, WHITE);
    } else {
        Texture2D pause = texture_load(TEXTURE_PAUSE);
        DrawTextureV(pause, position, WHITE);
    }
}
