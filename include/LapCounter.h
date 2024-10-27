#ifndef LAPCOUNTER_H
#define LAPCOUNTER_H

#include "Timer.h"



class LapCounter {
    public:
        LapCounter();
        ~LapCounter();

        void Reset();
        void CallLap();

        double GetAverageLapTime(); // YES sliding window this time, we're keeping it INTERESTING
    private:

        int ms_numerators[10];
        int lap_denominators[10];
        int running_ms;
        int running_laps;
        size_t write_head;

        Timer timer;
};

#endif // LAPCOUNTER_H
