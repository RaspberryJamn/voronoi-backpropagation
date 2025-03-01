#include "CApp.h"
#include "IdkFunctions.h"
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "RGBColor.h"


void CApp::OnRender() {
    SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->main_renderer);

    this->root_screen_element->Draw();

    SDL_RenderPresent(this->main_renderer);

}
