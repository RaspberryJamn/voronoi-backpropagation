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

    this->source_texture->Render(&stretch_rect);

    SDL_Color text_color = {0,0,0,255};
    std::stringstream time_text("");
    time_text << "Last frametime: " << this->last_frametime << "ms";
//    std::cout << time_text.str();
    this->text_texture->LoadInRenderedText(time_text.str().c_str(), text_color);
    this->text_texture->Render(0,0);


    SDL_RenderPresent(this->main_renderer);

//    if ((g_jitter_offset < 100) || (g_jitter_offset > 300)) {
//        SDL_BlitScaled(this->Source_Surface, nullptr, this->Display_Surface, &stretchRect);
//    }

    //SDL_BlitSurface(this->Source_Surface, nullptr, this->Display_Surface, nullptr);

    //SDL_UpdateWindowSurface(this->Main_Window);
}
