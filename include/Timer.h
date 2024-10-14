#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

// reset clears previous data and starts recording from zero
// stop halts the recording and the timer must be reset to be used again
// pausing puts the time recorded in stasis, but does not clear the data

class Timer {
    public:
        Timer();

        void Reset();
        void Stop();
        void Pause();
        void Unpause();

        Uint32 QueryTicks();

        bool IsActive();
        bool IsPaused();

    private:
        Uint32 start_tick;
        Uint32 checkpoint_stored_time;

        bool paused;
        bool active;
};

#endif // TIMER_H
