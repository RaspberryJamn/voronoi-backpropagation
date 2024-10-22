#include "AtlasNumberDrawer.h"

AtlasNumberDrawer::AtlasNumberDrawer() {
    this->atlas = nullptr; // lack of this line was causing the inconsistent behavior?
}

AtlasNumberDrawer::~AtlasNumberDrawer() {
    delete this->atlas;
    this->atlas = nullptr;
}


void AtlasNumberDrawer::BuildAtlas(SDL_Renderer* target_renderer, TTF_Font* font, SDL_Color text_color) {
    if (this->atlas != nullptr) {
        return;
    }
//    this->renderer = target_renderer;
    int horizontal_position = 0;
    int max_vertical_position = 0;
    Texture number_texture;
    number_texture.SetRendererAndFont(target_renderer, font);
    char str[2] = {'0','\0'};
    SDL_Texture* texture_queue[10];
    for (int i = 0; i <= 9; i++) {
        number_texture.LoadInRenderedText(str, text_color);
		str[0]++;

		SDL_Rect shape = {horizontal_position, 0, number_texture.GetWidth(), number_texture.GetHeight()};
        horizontal_position += shape.w;
        if (shape.h > max_vertical_position) {max_vertical_position = shape.h;}
        this->number_locations[i] = shape;

        texture_queue[i] = number_texture.OrphanTexture();
    }
    this->atlas = new Texture(target_renderer);
    this->atlas->NewBlankFromDims(horizontal_position, max_vertical_position);
    this->atlas->SetSelfAsRenderTarget();

    SDL_RenderClear(target_renderer);

    for (int i = 0; i <= 9; i++) {
        SDL_RenderCopy(target_renderer, texture_queue[i], nullptr, &this->number_locations[i]);
        SDL_DestroyTexture(texture_queue[i]);
    }
    SDL_SetRenderTarget(target_renderer, nullptr);

//    std::cout << max_vertical_position << std::endl;
}

void AtlasNumberDrawer::DrawRTL(std::string text, SDL_Rect* inout_bounds) {
    int i = 0;
    SDL_Rect foot = *inout_bounds;
    SDL_Rect paste = {foot.x+foot.w,foot.y,0,0};
    while (text[i]) {
        int c = text[i]-48;
        if ((c >= 0) && (c <= 9)) {
            SDL_Rect cut = this->number_locations[c];
            paste.w = cut.w;
            paste.h = cut.h;
            this->atlas->Render(&cut, &paste);
            paste.x+=paste.w;
        }
        i++;
    }
    inout_bounds->w = paste.x-foot.x;
}
