#include "CApp.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

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
    int value;
    while (true) {
        std::cout << "Please enter a choice (1-4)" << std::endl;
        std::cout << "1:Append node pointers" << std::endl;
        std::cout << "2:Remove given node" << std::endl;
        std::cout << "3:Print list" << std::endl;
        std::cout << "4:Add and remove 1-9 randomly 100000 times" << std::endl;
        std::cout << "5:Exit" << std::endl;

        std::cout << ">";
        std::cin >> menu_option;

        if(menu_option == 5) {
            break;
        }

        std::cout << std::endl;
        switch(menu_option) {
            case 1:
                std::cout << "Append node pointers, enter -1 to end" << std::endl;
                std::cout << ">";
                while (true) {
                    std::cin >> value;
                    if(value == -1) { break; }
                    NodeLinkedList::Append((VoronoiNode*)(value), &list);
                }
            break;
            case 2:
                std::cout << "Enter node to remove" << std::endl;
                std::cout << ">";
                std::cin >> value;
                NodeLinkedList::RemoveGeneric((VoronoiNode*)(value), &list);
            break;
            case 3:
                NodeLinkedList::Print("List: ", list, 0);
            break;
            case 4:
                std::srand(std::time(0));
                for (int i = 0; i < 100000; i++) {
                    int random = std::rand() % 9 + 1; // 1-9
                    NodeLinkedList::RemoveGeneric((VoronoiNode*)(random), &list);
                    NodeLinkedList::Append((VoronoiNode*)(random), &list);
                }
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
