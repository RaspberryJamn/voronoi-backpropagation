#ifndef LAPCOUNTER_H
#define LAPCOUNTER_H

#include "Timer.h"

class LapCounter {
    public:
        LapCounter();
        ~LapCounter();

        void Reset();
        void Stop();
        void AddLaps(int laps); // marks lap, doesnt stop timer
        double GetAverageLapTime(); // no sliding window this time, we're keeping it simple
    private:
        int laps;
        Timer timer;
};

#endif // LAPCOUNTER_H
