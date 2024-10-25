#ifndef LAPCOUNTER_H
#define LAPCOUNTER_H

#include "Timer.h"

class LapCounter {
    public:
        LapCounter(size_t history_length);
        ~LapCounter();

        void Reset();
        void StartLap();
        void CallLap();
        double GetAverageLapTime(); // YES sliding window this time, we're keeping it INTERESTING
    private:
        int total_laps;
        int* lap_times;
        size_t history_length;

        Timer timer;
};

#endif // LAPCOUNTER_H
