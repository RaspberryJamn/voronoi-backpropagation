#include "CApp.h"

int g_index = 0;
int g_offset = 0;
void CApp::OnLoop() {

//    for (int i = 0; i < 10; i++) {
//        int x = g_index%source_texture->GetWidth();
//        int y = g_index/source_texture->GetHeight();
//
//        SDL_SetRenderDrawColor(this->main_renderer, (Uint8)(x+g_offset), (Uint8)y, (Uint8)(x+y-g_offset), 0xFF);
//        int error = SDL_RenderDrawPoint(this->main_renderer, x, y);
//        if (error != 0) {
//            std::cout<<error;
//        }
//
//        g_index++;
//        if (g_index >= (source_texture->GetWidth()*source_texture->GetHeight())) {
//            g_offset++;
//            g_index = 0;
//            SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//            SDL_RenderClear(this->main_renderer);
//            break;
//        }
//    }
}
