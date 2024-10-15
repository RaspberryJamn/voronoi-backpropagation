#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

// measurements are in milliseconds
// stop halts the recording and the timer must be reset to be used again, though the time can still be queried. timer is stopped on initialization
// reset erases the state and sets it recording from zero
// pausing puts the time recorded in stasis, but does not clear the data. has no effect while stopped
// unpausing resumes measurement from where it left off. has no effect while stopped

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
