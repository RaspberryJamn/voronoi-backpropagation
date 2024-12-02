#include "CApp.h"
#include <iostream>

CApp::CApp() {
    this->running = true;

    this->main_window = nullptr;
    this->main_renderer = nullptr;

    this->last_frametime = 0;
    this->average_frametime = 0;

    this->source_texture = nullptr;
    this->source_pixels = nullptr;

    this->media_texture = nullptr;
    this->voronoi_graph = nullptr;

    this->text_textures[0] = nullptr;
    this->text_textures[1] = nullptr;
    this->text_textures[2] = nullptr;
    this->text_textures[3] = nullptr;
//    AtlasNumberDrawer this->number_renderer;
    this->main_font = nullptr;
    this->refresh_period = 0;
    this->refresh_counter = 0;
    this->error_logger = new ErrorLogger();
    this->error_logger->DisableLogging();

}

int CApp::OnExecute() {
    std::cout << "this is the DEBUG VERSION, not the primary" << std::endl;

//    if(OnInit() == false) {
//        return -1;
//    }

//    SDL_Event Event;
//    Timer frametime_timer;
//    LapCounter frametime_counter;
//    frametime_counter.Reset();
//    while(this->running) {
//        frametime_timer.Reset();
//
//        while(SDL_PollEvent(&Event)) {
//            OnEvent(Event);
//        }
//        OnLoop();
//        OnRender();
//
//        this->last_frametime = frametime_timer.QueryTicks();
//        frametime_counter.CallLap();
//        this->average_frametime = frametime_counter.GetAverageLapTime();
//    }

//    OnCleanup();

    NodeLinkedList* list = nullptr;
    int menu_option;
    while (true) {
        std::cout << "Please enter a choice (1-4)" << std::endl;
        std::cout << "1:Append node pointers" << std::endl;
        std::cout << "2:Remove given node" << std::endl;
        std::cout << "3:Print list" << std::endl;
        std::cout << "4:Exit" << std::endl;

        std::cout << ">";
        std::cin >> menu_option;

        if(menu_option == 4) {
            break;
        }

        std::cout << std::endl;
        switch(menu_option) {
            case 1:
                std::cout << "Enqueue values, enter -1 to end" << std::endl;
                std::cout << ">";
                int value;
                while (true) {
                    std::cin >> value;
                    if(value == -1) { break; }
//                    queue.Insert(value);
                }
            break;
            case 2:
                std::cout << "Dequeued minimum value: ";// << queue.DeleteMin();
            break;
            case 3:
                std::cout << "Queue array: ";// << queue;
            break;
            default:
                std::cout << "Enter a valid choice" << std::endl;
            break;
        }
        std::cout << std::endl << std::endl;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    CApp the_app;

    return the_app.OnExecute();
}
