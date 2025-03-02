#include "CApp.h"
#include "IdkFunctions.h"
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "RGBColor.h"


void CApp::OnRender() {

    this->root_screen_element->Tick();
    this->root_screen_element->Draw();

    SDL_RenderPresent(this->main_renderer);

}
