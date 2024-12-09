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

    VoronoiNode* real_list[10] = {};
    VoronoiGraph graph = VoronoiGraph();
    graph.RespecTree(0, 0, 100, 100, 4, 0);

    int menu_option;
    int value;
    while (true) {
        std::cout << "Please enter a choice (1-4)" << std::endl;
        std::cout << "1:Append node pointers" << std::endl;
        std::cout << "2:Remove given node pointer" << std::endl;
        std::cout << "3:Print list" << std::endl;
        std::cout << "4:Add and remove 1-9 randomly 100000 times" << std::endl;
        std::cout << "5:Add to a branch then merge that 100000 times" << std::endl;
        std::cout << "6:Print graph" << std::endl;
        std::cout << "7:Create/regenerate 10 random nodes" << std::endl;
        std::cout << "8:Add all real nodes to graph" << std::endl;
        std::cout << "9:Add and remove random real node 100000 times" << std::endl;
        std::cout << "10:Add and remove newcomer node 100000 times" << std::endl;
        std::cout << "0:Exit" << std::endl;

        std::cout << ">";
        std::cin >> menu_option;

        if(menu_option == 0) {
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
            case 5:
                std::srand(std::time(0));
                for (int i = 0; i < 100000; i++) {
                    VQuadTree* root = nullptr;
                    root = new VQuadTree(0, 0, 100, 100, 0);
                    root->tree_children[0] = new VQuadTree(0 , 0 , 50 , 50 , 1);
                    root->tree_children[1] = new VQuadTree(50, 0 , 100, 50 , 1);
                    root->tree_children[2] = new VQuadTree(0 , 50, 50 , 100, 1);
                    root->tree_children[3] = new VQuadTree(50, 50, 100, 100, 1);

                    NODELINKEDLIST_FOREACH(list, {
                        int random = std::rand() % 4;
                        NodeLinkedList::Append(current_node, &(root->tree_children[random]->node_children));
                    });
//                    VQuadTree::Print(root, 0);
                    graph.ConsolidateChildLists(root);
//                    VQuadTree::Print(root, 0);
                    NODELINKEDLIST_FOREACH(list, {
                        if (!NodeLinkedList::Contains(root->node_children, current_node)) {
                            std::cout << "ITS MISSING SOMETHING WENT WRONG. (i = " << i << " btw)" << std::endl;
                        }
                    });
                    delete root;
                }
            break;
            case 6:
                graph.PrintTree();
            break;
            case 7:
                for (int i = 0; i < 10; i++) {
                    if (real_list[i] != nullptr) {
                        delete real_list[i];
                        real_list[i] = nullptr;
                    }
                }
                std::srand(std::time(0));
                for (int i = 0; i < 10; i++) {
                    real_list[i] = new VoronoiNode(std::rand()%100, std::rand()%100, i, i, i);
                }
            break;
            case 8:
                for (int i = 0; i < 10; i++) {
                    graph.AddNode(real_list[i]);
                }
            break;
            case 9:
                {
                    std::srand(std::time(0));
    //                for (int i = 0; i < 10; i++) {
    //                    real_list[i] = new VoronoiNode(std::rand()%100, std::rand()%100, i, i, i);
    //                }
                    for (int i = 0; i < 100000; i++) {
                        int random = std::rand() % 10;
                        graph.RemoveNode(real_list[random]);
                        graph.AddNode(real_list[random]);
                    }
                }
            break;
            case 10:
                {
                    VoronoiNode* newcomer = new VoronoiNode(15, 15, 1, 2, 3);
                    for (int i = 0; i < 100000; i++) {
                        graph.AddNode(newcomer);
                        graph.RemoveNode(newcomer);
                    }
                    delete newcomer;
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
