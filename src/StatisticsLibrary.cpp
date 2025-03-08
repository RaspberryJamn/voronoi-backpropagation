#include "StatisticsLibrary.h"
#include <string>
#include <vector>
#include <iostream>

StatisticsLibrary::StatisticsLibrary() {
    AccessorData invalid;
    invalid.pointer = nullptr;
    invalid.method = INVALID;
    invalid.name = "";
    this->stats.push_back(invalid);
}

StatisticsLibrary::~StatisticsLibrary() {
    this->stats.clear();
}

statid_t StatisticsLibrary::GenerateNewIdFromName(std::string name) {
//    std::for_each(this->stats.begin(), this->stats.end(), [&](AccessorData stat_data) {
    for (AccessorData stat_data : this->stats) {
        if (stat_data.name == name) {
            std::cout << "statistic \"" << name << "\"has already been assigned" << std::endl;
            return (statid_t)(INVALID); // you cant generate the same name twice
        }
    }
//    });
    AccessorData new_entry;
    new_entry.pointer = nullptr;
    new_entry.method = UNPOPULATED;
    new_entry.name = name;
    this->stats.push_back(new_entry);
    return (this->stats.size()-1);
}
statid_t StatisticsLibrary::FindIdFromName(std::string name) {
    int index = 0;
    for (AccessorData stat_data : this->stats) {
        if (stat_data.name == name) {
            return (statid_t)(index);
        }
        index++;
    }
    return (statid_t)(INVALID);
}

void StatisticsLibrary::Publish(statid_t id, int* target) {
    if (this->stats[id].method != UNPOPULATED) {// erm dont give a bad id pls
        return;
    }
    this->stats[id].pointer = target;
    this->stats[id].method = INT_POINTER;
}
void StatisticsLibrary::Publish(statid_t id, double* target) {
    if (this->stats[id].method != UNPOPULATED) {// erm dont give a bad id pls
        return;
    }
    this->stats[id].pointer = target;
    this->stats[id].method = DOUBLE_POINTER;
}
void StatisticsLibrary::Publish(statid_t id, int (*getter)()) {
    if (this->stats[id].method != UNPOPULATED) {// erm dont give a bad id pls
        return;
    }
    this->stats[id].pointer = (void*)getter;
    this->stats[id].method = INT_GETTER;
}
void StatisticsLibrary::Publish(statid_t id, double (*getter)()) {
    if (this->stats[id].method != UNPOPULATED) {// erm dont give a bad id pls
        return;
    }
    this->stats[id].pointer = (void*)getter;
    this->stats[id].method = DOUBLE_GETTER;
}

void StatisticsLibrary::Unpublish(statid_t id) {
    this->stats[id].pointer = nullptr;
    this->stats[id].method = INVALID;
}

int StatisticsLibrary::ReadIntFromId(statid_t id) {
    AccessorData selected_entry = this->stats.at(id);
    switch (selected_entry.method) {
        case INVALID: {
    //        SDL_assert((selected_entry.method == INVALID)&&false);
            return 0;
        }
        case UNPOPULATED: {
    //        SDL_assert((selected_entry.method == UNPOPULATED)&&false);
            return 0;
        }
        case INT_POINTER: {
            return *((int*)selected_entry.pointer);
        }
        case DOUBLE_POINTER: {
            return (int)(*((double*)selected_entry.pointer));
        }
        case INT_GETTER: {
            int (*int_getter)() = reinterpret_cast<int(*)()>(selected_entry.pointer);
            return int_getter();
        }
        case DOUBLE_GETTER: {
            double (*double_getter)() = reinterpret_cast<double(*)()>(selected_entry.pointer);
            return (int)double_getter();
        }
    }
    return 0;
}

double StatisticsLibrary::ReadDoubleFromId(statid_t id) {
    AccessorData selected_entry = this->stats.at(id);
    switch (selected_entry.method) {
        case INVALID: {
    //        SDL_assert((selected_entry.method == INVALID)&&false);
            return 0;
        }
        case UNPOPULATED: {
    //        SDL_assert((selected_entry.method == UNPOPULATED)&&false);
            return 0;
        }
        case INT_POINTER: {
            return (double)(*((int*)selected_entry.pointer));
        }
        case DOUBLE_POINTER: {
            return *((double*)selected_entry.pointer);
        }
        case INT_GETTER: {
            double (*int_getter)() = reinterpret_cast<double(*)()>(selected_entry.pointer);
            return (double)int_getter();
        }
        case DOUBLE_GETTER: {
            int (*double_getter)() = reinterpret_cast<int(*)()>(selected_entry.pointer);
            return double_getter();
        }
    }
    return 0;
}
