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
//    SDL_RenderClear(this->main_renderer);

    this->source_texture->Render(&stretch_rect);

    SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Rect string_bounds = {5,5,0,0};
    this->text_textures[0]->RenderRTL(&string_bounds); // "Last frametime: "
    this->number_renderer.DrawRTL(std::to_string(this->last_frametime), &string_bounds);
    this->text_textures[1]->RenderRTL(&string_bounds); // "ms"

    string_bounds = {5,25,0,0};
    this->text_textures[2]->RenderRTL(&string_bounds); // "Last frametime: "
    this->number_renderer.DrawRTL(std::to_string((int)(1000.0/this->average_framerate)), &string_bounds);
    this->text_textures[3]->RenderRTL(&string_bounds); // "ms"

    SDL_RenderPresent(this->main_renderer);

}
