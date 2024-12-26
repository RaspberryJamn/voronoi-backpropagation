#include "CApp.h"
#include <iostream>
#include <cstdlib>

bool CApp::OnInit() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL video not initialized, SDL error: " << SDL_GetError() << std::endl;
        return false;
    }

    int img_flags = IMG_INIT_PNG;
    if(!(IMG_Init(img_flags) & img_flags)){
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
                            800,
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

    this->voronoi_graph = new VoronoiGraph();
//    this->voronoi_graph->SetErrorLogger(this->error_logger);
    this->voronoi_graph->Reshape(0, 0, this->media_texture->GetWidth(), this->media_texture->GetHeight(), 5, 3);

//    const int node_count = 3;
//    for (int i = 0; i < node_count; i++) {
//        this->voronoi_graph->AddNode(new VoronoiNode(std::fmod((i*1.618034+0.5),1.0)*this->media_texture->GetWidth(),
//                                                     ((double)i/node_count)*this->media_texture->GetHeight(),
//                                                     127+i%6,127+i%12,127+i%18));
//    }
    this->voronoi_graph->AddNode(new VoronoiNode(390,80 ,255,255,255));
    this->voronoi_graph->AddNode(new VoronoiNode(160,290,0  ,160,230));
    this->voronoi_graph->AddNode(new VoronoiNode(380,390,170,60 ,190));
//    this->voronoi_graph->AddNode(new VoronoiNode(300,540,100,100,100));
//    this->voronoi_graph->AddNode(new VoronoiNode(270,540,255,0  ,0  ));
//    this->voronoi_graph->AddNode(new VoronoiNode(370,160,0  ,255,0  ));
//    this->voronoi_graph->AddNode(new VoronoiNode(420,580,255,255,0  ));
//    this->voronoi_graph->AddNode(new VoronoiNode(060,110,0  ,0  ,255));
//    this->voronoi_graph->AddNode(new VoronoiNode(110,520,255,0  ,255));
//    this->voronoi_graph->AddNode(new VoronoiNode(200,180,0  ,255,255));
//    this->voronoi_graph->AddNode(new VoronoiNode(180,120,255,255,255));
    double band_width = 6.0;
    double gain = 0.00005; // higher gain => higher sharpness / smaller gain => higher smoothness
    this->voronoi_graph->SetGain(gain);
    this->voronoi_graph->SetBandWidth(band_width);

    this->refresh_period = 100;
    this->loop_advantage_factor = 3.0;

//    this->voronoi_graph->PrintTree();
    return true;
}
