#include "Timer.h"

Timer::Timer() {
    this->active = false;
    this->paused = false;
    this->start_tick = 0;
    this->checkpoint_stored_time = 0;
}

void Timer::Reset() {
    this->active = true;
    this->paused = false;
    this->start_tick = SDL_GetTicks();
    this->checkpoint_stored_time = 0;
}

void Timer::Stop() {
    if (!this->paused && this->active) {
        this->checkpoint_stored_time = SDL_GetTicks() - this->start_tick;
    }
    this->active = false;
    this->paused = false;
    this->start_tick = 0;
}

void Timer::Pause() {
    if (!this->paused && this->active) {
        this->paused = true;
        this->checkpoint_stored_time = SDL_GetTicks() - this->start_tick;
    }
}

void Timer::Unpause() {
    if (this->paused && this->active) {
        this->paused = false;
        this->start_tick = SDL_GetTicks() - this->checkpoint_stored_time;
    }
}

Uint32 Timer::QueryTicks() {
    if (this->paused || !this->active) {
        return this->checkpoint_stored_time;
    }
    return (SDL_GetTicks() - this->start_tick);
}

bool Timer::IsActive() {
    return this->active;
}

bool Timer::IsPaused() {
    return this->paused;
}
