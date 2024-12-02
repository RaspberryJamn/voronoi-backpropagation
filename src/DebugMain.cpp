#include <iostream>
#include "NodeLinkedList.h"
#include "VoronoiNode.h"
#include "VoronoiGraph.h"

int main(int argc, char* argv[]) {
    int menu_option;
    while (true) {
        std::cout << "Please enter a choice (1-4)" << std::endl;
        std::cout << "1:Enqueue values" << std::endl;
        std::cout << "2:Dequeue minumum value" << std::endl;
        std::cout << "3:Display array representation" << std::endl;
        std::cout << "4:Display as tree" << std::endl;
        std::cout << "5:Exit" << std::endl;

        std::cout << ">";
        std::cin >> menu_option;

        if(menu_option == 5) {
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
            case 4:
                std::cout << "Min-tree: ";
//                queue.TerminalLogTree();
            break;
            default:
                std::cout << "Enter a valid choice" << std::endl;
            break;
        }
        std::cout << std::endl << std::endl;
    }

}
