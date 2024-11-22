#ifndef LAPCOUNTER_H
#define LAPCOUNTER_H

#include "Timer.h"

const size_t LAP_COUNTER_BUCKETS = 10;
const int LAP_COUNTER_MIN_BUCKET_LENGTH = 25;
const int LAP_COUNTER_MIN_SLOW_FILL_LEVEL = 2;

class LapCounter {
    public:
        LapCounter();
        ~LapCounter();

        void Reset();
        void CallLap();

        double GetAverageLapTime(); // YES sliding window this time, we're keeping it INTERESTING
    private:

        int ms_numerators[LAP_COUNTER_BUCKETS];
        int lap_denominators[LAP_COUNTER_BUCKETS];
        int running_ms;
        int running_laps;
        size_t write_head;

        Timer timer;
};

#endif // LAPCOUNTER_H
