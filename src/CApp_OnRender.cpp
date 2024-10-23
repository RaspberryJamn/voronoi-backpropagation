#include "CApp.h"
#include <sstream>

int g_jitter_offset = 20;

void CApp::OnRender() {

    g_jitter_offset += 1;
    if (g_jitter_offset > 400) {
        g_jitter_offset = 0;
    }

    SDL_Rect stretch_rect;
    stretch_rect.x = g_jitter_offset;
    stretch_rect.y = g_jitter_offset;
    stretch_rect.w = 400;
    stretch_rect.h = 400;
//    SDL_BlitScaled(this->Source_Surface, nullptr, this->Display_Surface, &stretchRect);

    SDL_RenderClear(this->main_renderer);

    this->source_texture->Render(0,0);

    SDL_Rect string_bounds = {5,5,0,0};
    this->text_textures[0]->RenderRTL(&string_bounds); // "Last frametime: "
    this->number_renderer.DrawRTL(std::to_string(this->last_frametime), &string_bounds);
    this->text_textures[1]->RenderRTL(&string_bounds); // "ms"

    SDL_RenderPresent(this->main_renderer);

}
