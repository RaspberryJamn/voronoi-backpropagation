#include "CApp.h"
#include <iostream>

bool CApp::OnInit() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL video not initialized, SDL error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)){
        std::cout << "SDL image not initialized, SDL_image error: " << IMG_GetError() << std::endl;
        return false;
    }

    if(TTF_Init() == -1){
        std::cout << "SDL ttf not initialized, SDL_ttf error: " << TTF_GetError() << std::endl;
        return false;
    }

    this->main_window = new CanvasWindow();
    this->main_window->Init("sdl2 window",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            800,
                            600,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (this->main_window == nullptr) {
        std::cout << "Window not created, SDL error: " << SDL_GetError() << std::endl;
        return false;
    }

    this->main_renderer = this->main_window->CreateRenderer();
    if (this->main_renderer == nullptr) {
        std::cout << "Main window renderer not created, SDL error: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->main_renderer);
//    SDL_RendererInfo info;
//    SDL_GetRendererInfo(this->main_renderer, &info);
//    std::cout << "renderer info: " << info.flags << std::endl;
//    return false;

    if (this->ImportAssets() == false) {
        std::cout << "Failed to load assets" << std::endl;
        return false;
    }

    this->media_texture = new Texture(this->main_renderer);
    this->media_texture->NewBlankFromDims(this->source_texture->GetWidth(), this->source_texture->GetHeight());

    this->voronoi_graph = new VoronoiGraph();
    this->voronoi_graph->RespecTree(0, 0, this->media_texture->GetWidth(), this->media_texture->GetHeight(), 4, 1);
    this->voronoi_graph->AddNode(new VoronoiNode(300,540,100,100,100));
    this->voronoi_graph->AddNode(new VoronoiNode(270,540,255,0  ,0  ));
    this->voronoi_graph->AddNode(new VoronoiNode(370,160,0  ,255,0  ));
    this->voronoi_graph->AddNode(new VoronoiNode(420,580,255,255,0  ));
    this->voronoi_graph->AddNode(new VoronoiNode(060,110,0  ,0  ,255));
    this->voronoi_graph->AddNode(new VoronoiNode(110,520,255,0  ,255));
    this->voronoi_graph->AddNode(new VoronoiNode(200,180,0  ,255,255));
    this->voronoi_graph->AddNode(new VoronoiNode(180,120,255,255,255));
    this->refresh_period = 100;
    this->refresh_counter = 0;
//    this->voronoi_graph->PrintTree();
    return true;
}
