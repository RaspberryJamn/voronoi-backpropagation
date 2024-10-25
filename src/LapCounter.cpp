#include "LapCounter.h"

LapCounter::LapCounter(size_t history_length) {
    this->timer.Stop();
    this->lap_times = new int[history_length];
    this->history_length = history_length;
    this->total_laps = 0;
//    for (size_t i = 0; i < this->history_length; i++) {
//        this->lap_times[i] = 0; // whatevs
//    }
}

LapCounter::~LapCounter() {
    delete[] this->lap_times;
    this->history_length = 0;
    this->total_laps = 0;
}

void LapCounter::Reset() {
    this->timer.Stop();
    this->total_laps = 0;
    for (size_t i = 0; i < this->history_length; i++) {
        this->lap_times[i] = 0; // whatevs
    }
}

void LapCounter::StartLap() {
    this->timer.Reset();
}

void LapCounter::CallLap() {
    this->timer.Stop();
    this->lap_times[this->total_laps%this->history_length] = this->timer.QueryTicks();
    this->total_laps++;
}

double LapCounter::GetAverageLapTime() {
    if (this->total_laps == 0) {
        return 0.0;
    }
    int samples = this->total_laps;
    if ((size_t)this->total_laps > this->history_length) {samples = this->history_length;}

    int cumm = 0;
    for (size_t i = 0; i < (size_t)samples; i++) {
        cumm += this->lap_times[i];
    }
    return ((double)cumm)/samples;
}
